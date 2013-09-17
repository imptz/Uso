#include "ExtensionSystem.h"
#include "../message/Messages.h"
#include "devices\IDevice.h"
#include "../Local.h"

const SERIAL_PORT ExtensionSystem::PORT_NAME = SERIAL_PORT_2;
const SERIAL_PORT_SPEED ExtensionSystem::PORT_SPEED = SERIAL_PORT_SPEED_57600;

ExtensionSystem::ExtensionSystem()
	:	ITimer(TIMER_PERIOD), serialPort(SerialPortManager::getSingleton().getPort(PORT_NAME)), 
	phase(PHASE_READY), 
	length(0), position(0), timeCounter(0), 
	transferState(TRANSFER_STATE_READY), 
	transferTimer(0),
	extDevIndex(FIRST_DEVICE_ADDRESS), 
	preStartPhase(PRE_START_PHASE_STOP),
	pArea(nullptr),
	fifoFrame(new Fifo<unsigned char*>(FIFO_FRAME_SIZE)),
	transferTryCount(0),
	subsytemActionIndex(0),
	pSendData(nullptr),
	recvTimeOutEvent(false),
	afterRecvTimeOut(0),
	isRecvFinish(true)
{
	serialPort->setSpeed(PORT_SPEED);

	pTimer->start();
	serialPort->open();
}

ExtensionSystem::~ExtensionSystem()
{
	serialPort->close();
	pTimer->stop();

	for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
		if (subsystems[i].pISubsystem != nullptr)
			SAFE_DELETE(subsystems[i].pISubsystem)
}

void ExtensionSystem::enableTransfer(unsigned int _dataSize)
{
	transferTimer = 1000 / ((serialPort->getSpeed() / 10) / _dataSize) + MINIMAL_TRANSFER_TIME;  
	transferState = TRANSFER_STATE_ACTIVE;
}

void ExtensionSystem::timerHandler()
{
	if (afterRecvTimeOut > 0)
	{
		afterRecvTimeOut--;
	}
	else
	{
		isRecvFinish = true;
	}

	if (transferState == TRANSFER_STATE_ACTIVE)
	{
		transferTimer--;
		if (transferTimer == 0)
		{
			transferState = TRANSFER_STATE_TIME_OUT;
			if (transferTryCount == MAX_TRANSFER_TRY_COUNT)
			{
				putFrame(pSendData, true);
				SAFE_DELETE_ARRAY(pSendData)
				transferTryCount = 0;
			}
		}
	}

	if (phase != PHASE_READY)
	{
		timeCounter++;
		if (timeCounter == MAX_TIME_COUNTER)
		{
			recvTimeOutEvent = true;
			return;
		}
	}
}

void ExtensionSystem::busAction()
{
	unsigned char temp;

	if (recvTimeOutEvent)
	{
		stopLoadTime(STOP_REASON_TIME_OUT);
		recvTimeOutEvent = false;
	}

	switch (phase)
	{
		case PHASE_READY:
			if (serialPort->getRecvFifo()->get(&temp) == 0)
				break;

			if (temp != ADDRESS)
				break;

			phase = PHASE_1;

		case PHASE_1:
			if (serialPort->getRecvFifo()->getDataSize() < 4)
				break;

			timeCounter = 0;

			unsigned char sAddress;
			unsigned char command;
			serialPort->getRecvFifo()->get(&sAddress);
			serialPort->getRecvFifo()->get(&command);
			serialPort->getRecvFifo()->get(reinterpret_cast<unsigned char*>(&length), 2);
			if (length > MAX_FRAME_LENGTH)
			{
				stopLoadTime(STOP_REASON_NORMAL);
				return;
			}
			pArea = new unsigned char[length + 7];
			pArea[0] = ADDRESS;
			pArea[1] = sAddress;
			pArea[2] = command;
			pArea[3] = length;
			pArea[4] = length >> 8;
			length += 7;
			position = 5;
			phase = PHASE_2;
		case PHASE_2:
			unsigned int aDataSize = serialPort->getRecvFifo()->getDataSize();
			if (aDataSize > 0)
			{
				timeCounter = 0;
				if (position + aDataSize > length)
					aDataSize = length - position;

				serialPort->getRecvFifo()->get(&pArea[position], aDataSize);
				position += aDataSize;
			}

			if (position == length)
			{
//for (unsigned int i = 0; i < length; ++i)
//	DEBUG_PUT("%i ", pArea[i]);
//DEBUG_PUT("   CRC16  - ");
				if (Crc16::calcCRC16(pArea, length - 2) == *reinterpret_cast<unsigned short*>(&pArea[length - 2]))
				{
//DEBUG_PUT("true\n");
					transferState = TRANSFER_STATE_READY;
					switch (preStartPhase)
					{
						case PRE_START_PHASE_GET_ID_RECV:
							preStartReadHandlerGetId();
							break;
						default:
							SAFE_DELETE_ARRAY(pSendData)
							transferTryCount = 0;
//DEBUG_PUT_METHOD("rf = %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", 
//	pArea[0], pArea[1], pArea[2], pArea[3], pArea[4], pArea[5], pArea[6], pArea[7], pArea[8], 
//	pArea[9], pArea[10], pArea[11], pArea[12], pArea[13], pArea[14], pArea[15], pArea[16], pArea[17], pArea[18], pArea[19]);
							putFrame(pArea);							
							SAFE_DELETE_ARRAY(pArea)
							break;
					}
					stopLoadTime(STOP_REASON_NORMAL);
				}
				else{
					stopLoadTime(STOP_REASON_TIME_OUT);
//					DEBUG_PUT("false\n");
				}
			}
			break;
	}
}

void ExtensionSystem::stopLoadTime(STOP_REASON value)
{
	length = 0;
	position = 0;
	timeCounter = 0;
	if (pArea != nullptr)
	{
		if (value == STOP_REASON_TIME_OUT)
			SAFE_DELETE_ARRAY(pArea)

		pArea = nullptr;
	}

	serialPort->getRecvFifo()->clear();
	phase = PHASE_READY;
	initAfterRecvTimeOut();
}

void ExtensionSystem::forceAddDevice(unsigned int _type, unsigned int _id, unsigned int _address)
{
	if (_address - FIRST_DEVICE_ADDRESS < EXTENSION_DEVICE_COUNT)
	{
		int index = getSubsystemIndexByType(_type);
		if (index != -1)
		{
			subsystems[index].pISubsystem->addDevice(_id, _address);
		}
	}
}

void ExtensionSystem::preStartReadHandlerGetId()
{
	if (pArea[1] - FIRST_DEVICE_ADDRESS < EXTENSION_DEVICE_COUNT)
	{
		int index = getSubsystemIndexByType(pArea[5]);
		if (index != -1)
		{
			subsystems[index].pISubsystem->addDevice(pArea[6], pArea[1]);
		}

		preStartPhase = PRE_START_PHASE_GET_ID_SEND;
	}

	SAFE_DELETE_ARRAY(pArea)
}

bool ExtensionSystem::sendGetID(unsigned char deviceAddress)
{
	unsigned char data[7];

	data[0] = deviceAddress;
	data[1] = ADDRESS;
	data[2] = IDevice::COMMAND_GET_ID;
	data[3] = 0;
	data[4] = 0;

	unsigned short crc = Crc16::calcCRC16(data, 5);
	data[5] = static_cast<unsigned char>(crc);
	data[6] = crc >> 8;
	
	serialPort->setNewSendData(data, 7);
	serialPort->startSend();

	enableTransfer(7);
	return true;
}

void ExtensionSystem::init()
{
	preStartPhase = PRE_START_PHASE_GET_ID_SEND;
	extDevIndex = FIRST_DEVICE_ADDRESS; 
}

void ExtensionSystem::printRegSubsystem()
{
	for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
		if (subsystems[i].pISubsystem != nullptr)
		{
		}
}

bool ExtensionSystem::isReady()
{
	return (preStartPhase == PRE_START_PHASE_FINISH);
}

void ExtensionSystem::action()
{
	bool res;

	busAction();

	switch (preStartPhase)
	{
		case PRE_START_PHASE_GET_ID_SEND:
			if (isRecvFinish)
			{
				if (!getIdAction())
				{
					for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
						if (subsystems[i].pISubsystem != nullptr)
							subsystems[i].pISubsystem->init();

					preStartPhase = PRE_START_PHASE_CONFIG_WAIT;
				}
			}
			break;
		case PRE_START_PHASE_GET_ID_RECV:
			if (transferState != TRANSFER_STATE_ACTIVE)
				preStartPhase = PRE_START_PHASE_GET_ID_SEND;
			break;
		default:
			if (transferState != TRANSFER_STATE_ACTIVE)
			{
				if (isRecvFinish)
				{
					if (pSendData == nullptr)
						fifoFrame->get(&pSendData);
					if (pSendData != nullptr)
					{
						transferTryCount++;
//DEBUG_PUT_METHOD("transferTryCount = %u, sendFrame = %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", 
//	transferTryCount, pSendData[0], pSendData[1], pSendData[2], pSendData[3], pSendData[4], pSendData[5], pSendData[6], pSendData[7], pSendData[8], 
//	pSendData[9], pSendData[10], pSendData[11], pSendData[12], pSendData[13], pSendData[14], pSendData[15], pSendData[16], pSendData[17], pSendData[18], pSendData[19]);
						sendFrame(pSendData);
					}
					else
					{
						if (subsystems[subsytemActionIndex].pISubsystem != nullptr)
							subsystems[subsytemActionIndex++].pISubsystem->action();

						if ((subsytemActionIndex == MAX_SUBSYSTEM_COUNT) || (subsystems[subsytemActionIndex].pISubsystem == nullptr))
							subsytemActionIndex = 0;
					}
				}
			}
			break;
	}

	if (preStartPhase == PRE_START_PHASE_CONFIG_WAIT)
	{
		res = true;
		for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
			if (subsystems[i].pISubsystem != nullptr)
				if (!subsystems[i].pISubsystem->isReady())
					res = false;

		if (res)
		{
			preStartPhase = PRE_START_PHASE_FINISH;
			testMissingDevices();
		}
	}
}

bool ExtensionSystem::getIdAction()
{
	if (extDevIndex < EXTENSION_DEVICE_COUNT + 1)
	{
		sendGetID(extDevIndex++);
		preStartPhase = PRE_START_PHASE_GET_ID_RECV;
		return true;
	}
	else
		return false;
}

void ExtensionSystem::onMessage(Message message)
{
	if (message.msg == EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA)
	{
		Fifo<unsigned char*>* pFifo = reinterpret_cast<Fifo<unsigned char*>*>(message.par1);
		if (pFifo->getDataSize() > 0)
		{
//			unsigned char* frame;
//			if (message.par2 == PACKING_OFF)
//			{
				unsigned char* _pData;
				if (pFifo->get(&_pData) > 0){
					//DEBUG_PUT_METHOD(" ");
					//DEBUG_PUT("%i %i %i %i %i", _pData[0], _pData[1], _pData[2], _pData[3], _pData[4]);
					//for (unsigned int i = 5; i < _pData[4] * 256 + _pData[3] + 5; ++i)
					//	DEBUG_PUT(" %i", _pData[i]);

					//DEBUG_PUT("\n");

					fifoFrame->put(&_pData);
				}
//			}
//			else
//			{
//			}
		}
	}
}

void ExtensionSystem::sendFrame(unsigned char* _pData)
{
	_pData[1] = ADDRESS;
	unsigned int length = _pData[4];
	length = (length << 8) + _pData[3];
	unsigned short crc = Crc16::calcCRC16(_pData, length + 5);
	_pData[length + 5] = static_cast<unsigned char>(crc);
	_pData[length + 6] = crc >> 8;
	
	serialPort->setNewSendData(_pData, length + 7);
	serialPort->startSend();

	enableTransfer(length + 7);
}

void ExtensionSystem::testMissingDevices()
{
	for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
		if (subsystems[i].pISubsystem != nullptr)
			subsystems[i].pISubsystem->testMissingDevices();
}

void ExtensionSystem::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
		if (subsystems[i].pISubsystem != nullptr)
		{
			subsystems[i].pISubsystem->putFrame(_pArea, isNotTransfer);
		}
}

void ExtensionSystem::initAfterRecvTimeOut()
{
	isRecvFinish = false;
	afterRecvTimeOut = AFTER_RECV_TIME_OUT;
}
