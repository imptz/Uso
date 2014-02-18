#include "tvDevice.h"
#include "../ExtensionSystem.h"
#include "../../config/ConfigData.h"
#include "../../config/Config.h"
#include "../../Local.h"

bool TvDevice::registered = false;

TvDevice::TvDevice(unsigned char _address, unsigned int _type)
	:	IDetectionDevice(_address, _type)
{
	ConfigData* pConfigData = Config::getSingleton().getConfigData();
	unsigned count = pConfigData->getConfigDataStructPRPositionCount();
	ConfigDataStructPRPosition** fvStruct = pConfigData->getConfigDataStructPRPositions();

	devicesCount = count;
	pDevices = new Device[devicesCount];

	for (unsigned int i = 0; i < count; i++)
		pDevices[i].address = i + 1;

	ExtensionSystem::getSingleton().addReceiver(this);
	addReceiver(ExtensionSystem::getSingletonPtr());
}

TvDevice::~TvDevice()
{
	SAFE_DELETE(pDevices)
}

void TvDevice::init()
{
	phase = PHASE_CONFIG;
}

bool TvDevice::isReady()
{
	return (phase == PHASE_START);
}

void TvDevice::timerHandler()
{
	if (actionTimeOut++ == ACTION_TIME_OUT)
	{
		isActionTimeOut = true;
		actionTimeOut = 0;
	}

	if (fireTimeout > 0)
		fireTimeout--;
}

void TvDevice::action()
{
	if (!disabled)
	{
		switch (phase)
		{
			case PHASE_CONFIG:
				createInitFrame();
				sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				phase = PHASE_CONFIG_WAIT;
				break;
			case PHASE_INIT_WAIT:
				if (isActionTimeOut)
				{
					isActionTimeOut = false;
					createGetInitializeFrame();
					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}
				break;
			case PHASE_START:
				if (isActionTimeOut)
				{
					isActionTimeOut = false;
					createGetDeviceStateFrame();
					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}
				break;
			case PHASE_SET_UPDATE_LIST:
				break;
			case PHASE_SET_GET_UPDATE_FLAG:
				if (fireTimeout == 0)
				{
					updateChannelsCount = 0;
					SAFE_DELETE_ARRAY(updateChannels)
					phase = PHASE_START;
				}
				break;
			case PHASE_SET_GET_FIRE:
				break;
		}
	}
}

bool TvDevice::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	if (!disabled)
	{
		if (isNotTransfer)
		{
			if (_pArea[0] == address)
			{
				switch (phase)
				{
					case PHASE_CONFIG_WAIT:
					case PHASE_INIT_WAIT:
						phase = PHASE_START;
						break;
					case PHASE_SET_UPDATE_LIST:
						updateChannelsCount = 0;
						SAFE_DELETE_ARRAY(updateChannels)
						phase = PHASE_START;
						break;
					case PHASE_SET_GET_UPDATE_FLAG:
						break;
					case PHASE_SET_GET_FIRE:
						updateChannelsCount = 0;
						SAFE_DELETE_ARRAY(updateChannels)
						phase = PHASE_START;
						break;
				}
					
				return true;
			}
		}
		else
		{
			if (_pArea[1] == address)
			{
				switch (phase)
				{
					case PHASE_CONFIG_WAIT:
						if (_pArea[2] == COMMAND_INIT)
							phase = PHASE_INIT_WAIT;
						break;
					case PHASE_INIT_WAIT:
						if (_pArea[2] == COMMAND_GET_INITIALIZE_RESULT)
						{
							if (_pArea[5] == INITIALIZE_RESULT_OK)
								phase = PHASE_START;
						}
						break;
					case PHASE_SET_UPDATE_LIST:
						fireTimeout = 20 * 10;
						createGetInitializeFrame();
						sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
						phase = PHASE_SET_GET_UPDATE_FLAG;
						break;
					case PHASE_SET_GET_UPDATE_FLAG:
						if (_pArea[6] == 1)
						{
							fireTimeout = 0;
							createGetFireFrame();
							sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
							phase = PHASE_SET_GET_FIRE;
						}
						else
						{
							createGetInitializeFrame();
							sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
						}
						break;
					case PHASE_SET_GET_FIRE:
						SAFE_DELETE_ARRAY(updateChannels)						updateChannelsCount = 1;						SAFE_DELETE_ARRAY(fireFrame)						//unsigned int _size = _pArea[3] + _pArea[4] * 256 + 7;						//fireFrame = new unsigned char[_size];						//memcpy(fireFrame, _pArea, _size);						unsigned char* pp = nullptr;						unsigned int _size = _pArea[3] + _pArea[4] * 256 + 7;						pp = new unsigned char[_size];						memcpy(pp, _pArea, _size);DEBUG_PUT_METHOD("qqqqqqqqqqqqqq: ");for(unsigned int i = 0; i < 30; ++i){	DEBUG_PUT("%i ", pp[i]);}DEBUG_PUT_METHOD("\n");						unsigned int paramsCount = pp[3] + pp[4] * 256;						const unsigned int HEADER_SIZE = 5;						const unsigned int FIRE_AREA_SIZE = 4;						unsigned int channelOffset = 6;						unsigned int validAddressCount = 0;						while (channelOffset < (paramsCount + HEADER_SIZE))						{							unsigned int fireCount = pp[channelOffset];							if (fireCount != 0)							{								validAddressCount++;								channelOffset += (2 + FIRE_AREA_SIZE * fireCount);							}							else								channelOffset += 2;						}				DEBUG_PUT_METHOD("validAddressCount = %i ", validAddressCount);						fireFrame = new unsigned char[256];						fireFrame[0] = pp[0];						fireFrame[1] = pp[1];						fireFrame[2] = pp[2];						fireFrame[4] = pp[4];						unsigned int parCount = 0;						unsigned int pf = 5;						unsigned int of = 5;				//		for(unsigned int i = 0; i < validAddressCount; ++i){						while(pf < paramsCount + 5){							fireFrame[of++] = pp[pf++];							fireFrame[of++] = pp[pf++];							unsigned int firec = fireFrame[of - 1];							if(firec == 0)								continue;							for(unsigned int t = 0; t < firec; ++t){								fireFrame[of++] = pp[pf++];								fireFrame[of++] = 0;								fireFrame[of++] = pp[pf++];								fireFrame[of++] = 0;								fireFrame[of++] = pp[pf++];								fireFrame[of++] = 0;								fireFrame[of++] = pp[pf++];								fireFrame[of++] = 0;							}						}						fireFrame[3] = of - 5;				//						fireFrame = _pArea;						phase = PHASE_START;						break;
				}

				return true;
			}
		}
	}

	return false;
}

void TvDevice::onMessage(Message message)
{
}

#pragma region createFrames

void TvDevice::createInitFrame()
{
	unsigned int dataLength = devicesCount + 1;
	unsigned char* initData = new unsigned char[dataLength + 7];

	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_INIT;
	initData[3] = dataLength;
	initData[4] = dataLength >> 8;

	const unsigned int DATA_OFFSET = 5;

	initData[DATA_OFFSET + 0] = devicesCount;

	for (unsigned int i = 0; i < devicesCount; i++)
		initData[i + DATA_OFFSET + 1] = pDevices[i].address;

	fifoFrame->put(&initData);
}

void TvDevice::createGetInitializeFrame()
{
	unsigned char* initData = new unsigned char[7];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_INITIALIZE_RESULT;
	initData[3] = 0;
	initData[4] = 0;

	fifoFrame->put(&initData);
}

void TvDevice::createSetUpdateListFrame()
{
	unsigned char* initData = new unsigned char[7 + updateChannelsCount];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_SET_UPDATE_LIST;
	initData[3] = updateChannelsCount;
	initData[4] = updateChannelsCount >> 8;

	for (unsigned int i = 0; i < updateChannelsCount; i++)
		initData[i + 5] = updateChannels[i];

	fifoFrame->put(&initData);
}

void TvDevice::createGetFireFrame()
{
	unsigned char* initData = new unsigned char[7 + updateChannelsCount];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_FIRE;
	initData[3] = updateChannelsCount;
	initData[4] = updateChannelsCount >> 8;

	for (unsigned int i = 0; i < updateChannelsCount; i++)
		initData[i + 5] = updateChannels[i];

	fifoFrame->put(&initData);
}

void TvDevice::createGetDeviceStateFrame()
{
	unsigned char* initData = new unsigned char[7 + updateChannelsCount];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_STATE;
	initData[3] = 0;
	initData[4] = 0;

	fifoFrame->put(&initData);
}

void TvDevice::createResetFrame()
{
	unsigned char* initData = new unsigned char[7 + updateChannelsCount];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_RESET_DEVICES;
	initData[3] = 0;
	initData[4] = 0;

	fifoFrame->put(&initData);
}

#pragma endregion

unsigned int TvDevice::getId()
{
	return ID;
}

void TvDevice::updateFire(unsigned char* pAddress, unsigned int count, ActionMoveToPoint** actionMoveToPoint)
{
	DEBUG_PUT_METHOD("count = %i\n", count);
	SAFE_DELETE_ARRAY(fireFrame);
	updateChannels = pAddress;
	updateChannelsCount = count;
	createSetUpdateListFrame();
	sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
	phase = PHASE_SET_UPDATE_LIST;
}

unsigned char* TvDevice::getFire()
{
	return fireFrame;
}

void TvDevice::reset()
{
	createResetFrame();
	sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
}

void TvDevice::updateFire(ChannelInfo* pChannelInfo, unsigned int count)
{

}
