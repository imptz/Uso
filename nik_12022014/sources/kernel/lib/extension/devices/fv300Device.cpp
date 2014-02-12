#include "fv300Device.h"
#include "../ExtensionSystem.h"
#include "../../config/ConfigData.h"
#include "../../config/Config.h"
#include "../../Local.h"

bool Fv300Device::registered = false;

Fv300Device::Fv300Device(unsigned char _address, unsigned int _type)
	:	IDetectionDevice(_address, _type)
{
	ConfigData* pConfigData = Config::getSingleton().getConfigData();
	unsigned count = pConfigData->fv300_count;

	devicesCount = count;
	pDevices = new Device[devicesCount];

	for (unsigned int i = 0; i < count; i++)
		pDevices[i].address = pConfigData->fv300[i].address;

	ExtensionSystem::getSingleton().addReceiver(this);
	addReceiver(ExtensionSystem::getSingletonPtr());

	points = nullptr;
	pointsCount = 0;
}

Fv300Device::~Fv300Device()
{
	SAFE_DELETE(pDevices)
}

void Fv300Device::init()
{
	phase = PHASE_CONFIG;
}

bool Fv300Device::isReady()
{
	return (phase == PHASE_START);
}

void Fv300Device::timerHandler()
{
	if (actionTimeOut++ == ACTION_TIME_OUT)
	{
		isActionTimeOut = true;
		actionTimeOut = 0;
	}

	if (fireTimeout > 0)
		fireTimeout--;
}

void Fv300Device::action()
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

bool Fv300Device::putFrame(unsigned char* _pArea, bool isNotTransfer)
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
							{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("...............................................................Fv300Device == INITIALIZE_RESULT_OK\n");
								phase = PHASE_START;
							}
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
							fireTimeout = 20 * 10;
							createGetInitializeFrame();
							sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
							//createGetInitializeFrame();
							//sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
						}
						break;
					case PHASE_SET_GET_FIRE:
						SAFE_DELETE_ARRAY(updateChannels)
						updateChannelsCount = 1;

						SAFE_DELETE_ARRAY(fireFrame);

						fireFrame = new unsigned char[_pArea[3] + _pArea[4] * 256 + 7];

						memcpy(fireFrame,_pArea, _pArea[3] + _pArea[4] * 256 + 7);

						buildFire();

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("......................PHASE_SET_GET_FIRE:                   ........: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(fireFrame, fireFrame[3] + fireFrame[4] * 256 + 7);	
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");
						phase = PHASE_START;
						break;
				}

				return true;
			}
		}
	}

	return false;
}

void Fv300Device::onMessage(Message message)
{
}

#pragma region createFrames

void Fv300Device::createInitFrame()
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

void Fv300Device::createGetInitializeFrame()
{
	unsigned char* initData = new unsigned char[7];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_INITIALIZE_RESULT;
	initData[3] = 0;
	initData[4] = 0;

	fifoFrame->put(&initData);
}

void Fv300Device::createSetUpdateListFrame()
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

void Fv300Device::createGetFireFrame()
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

void Fv300Device::createGetDeviceStateFrame()
{
	unsigned char* initData = new unsigned char[7 + updateChannelsCount];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_STATE;
	initData[3] = 0;
	initData[4] = 0;

	fifoFrame->put(&initData);
}

void Fv300Device::createResetFrame()
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

unsigned int Fv300Device::getId()
{
	return ID;
}

void Fv300Device::updateFire(unsigned char* pAddress, unsigned int count, ActionMoveToPoint** actionMoveToPoint)
{
	if (points != nullptr)
		delete[] points;

	points = new ChannelInfo[count];
	pointsCount = count;
	for (unsigned int i = 0; i < pointsCount; ++i)
	{
		points[i].address = actionMoveToPoint[i]->getDeviceAddress();
		points[i].point1.x = actionMoveToPoint[i]->getPoint().x;
		points[i].point1.y = actionMoveToPoint[i]->getPoint().y;
	}

	SAFE_DELETE_ARRAY(fireFrame);
	updateChannels = pAddress;
	updateChannelsCount = count;
	createSetUpdateListFrame();
	sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
	phase = PHASE_SET_UPDATE_LIST;
}

unsigned char* Fv300Device::getFire()
{
	return fireFrame;
}

void Fv300Device::buildFire()
{
	unsigned int paramsCount = fireFrame[3] + fireFrame[4] * 256;
	const unsigned int HEADER_SIZE = 5;
	const unsigned int FIRE_AREA_SIZE = 8;

	unsigned int offset = 6;
	unsigned int newSize = 7;

	while (offset < (paramsCount + HEADER_SIZE))
	{
		unsigned int fireCount = fireFrame[offset];
		if (fireCount != 0)
		{
			offset += (2 + FIRE_AREA_SIZE * fireCount);
			newSize += (2 + FIRE_AREA_SIZE * fireCount);
		}
		else
		{
			offset += 2;
			newSize += 2;
		}
	}		
	
	unsigned char* newFrame = new unsigned char[newSize];
	
	memcpy(newFrame, fireFrame, 5);

	offset = 5;
	unsigned int newOffset = 5;

	while (offset < (paramsCount + HEADER_SIZE))
	{
		unsigned int pr = fireFrame[offset];

		unsigned int index;
		for (index = 0; index < pointsCount; ++index)
			if (points[index].address == pr)
				break;

		Point2<unsigned int> point = points[index].point1;
//		Point2<int> point;

		//point.x = p.x;
		//point.y = p.y;

		//if (point.x > 180)
		//	point.x -= 360;
		//if (point.y > 180)
		//	point.y -= 360;

		//if (pr == 3)
		//{
		//	point.x = 315;
		//	point.y = 0;
		//}
		//else
		//{
		//	point.x = 60;
		//	point.y = 0;
		//}

		unsigned int fireCount = fireFrame[offset + 1];

		if (fireCount != 0)
		{
			newFrame[newOffset] = fireFrame[offset];
			newFrame[newOffset + 1] = 1;//fireFrame[offset + 1];
			offset += 2;
			newOffset += 2;

//			for (unsigned int i = 0; i < fireCount; i++)
			{
				int val = fireFrame[offset];
				if (val > 128)
					val = -1 * (255 - val);

				val += point.x;
				
				if (val < 0)
					val = 32000 + static_cast<int>(abs(static_cast<float>(val)));

				newFrame[newOffset] = val % 256;
				newFrame[newOffset + 1] = val / 256;
				
				offset++; 
				newOffset += 2;

				val = fireFrame[offset];

				if (val > 128)
					val = -1 * (255 - val);

				val += point.y;
				
				if (val < 0)
					val = 32000 + static_cast<int>(abs(static_cast<float>(val)));


				newFrame[newOffset] = val % 256;
				newFrame[newOffset + 1] = val / 256;
				
				offset++; 
				newOffset += 2;

				val = fireFrame[offset];
				newFrame[newOffset] = val % 256;
				newFrame[newOffset + 1] = val / 256;
				
				offset++; 
				newOffset += 2;

				val = fireFrame[offset];
				newFrame[newOffset] = val % 256;
				newFrame[newOffset + 1] = val / 256;
				
				offset++; 
				newOffset += 2;
			}

			offset += ((fireCount - 1) * 4); 
			//newOffset += (fireCount * 4 * 2);

		}
		else
		{
			newFrame[newOffset] = fireFrame[offset];
			newFrame[newOffset + 1] = fireFrame[offset + 1];
			offset += 2;
			newOffset += 2;
		}
	}		

	newFrame[newOffset++] = fireFrame[offset++];
	newFrame[newOffset] = fireFrame[offset];

	SAFE_DELETE_ARRAY(fireFrame);
	fireFrame = newFrame;
}

void Fv300Device::reset()
{
	createResetFrame();
	sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
}

void Fv300Device::updateFire(ChannelInfo* pChannelInfo, unsigned int count)
{

}
