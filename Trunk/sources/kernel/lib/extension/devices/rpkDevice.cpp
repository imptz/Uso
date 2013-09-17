#include "RpkDevice.h"
#include "../ExtensionSystem.h"
#include "../../Local.h"
#include "../../log/Log.h"

bool RpkDevice::registered = false;

RpkDevice::RpkDevice(unsigned char _address, unsigned int _type)
	:	IRpkDevice(_address, _type), items(new Item*[MAX_ITEMS_NUMBER])
{
	for (unsigned int i = 0; i < MAX_ITEMS_NUMBER; i++)
		items[i] = nullptr;

	ExtensionSystem::getSingleton().addReceiver(this);
	addReceiver(ExtensionSystem::getSingletonPtr());
}

RpkDevice::~RpkDevice()
{
}

void RpkDevice::init()
{
	phase = PHASE_CONFIG;
}

bool RpkDevice::isReady()
{
	return (phase == PHASE_START);
}

void RpkDevice::timerHandler()
{
	if (actionTimeOut++ == ACTION_TIME_OUT / 4)
	{
		isActionTimeOut = true;
		actionTimeOut = 0;
	}
}

void RpkDevice::action(){
	if (!disabled){
		if (isActionTimeOut){
			for (unsigned int i = 0; i < MAX_ITEMS_NUMBER; ++i)
				if (items[i] != nullptr){
					if ((items[i]->resultCode == FRAME_RESULT_PROCCESS) && (items[i]->timeLive > 0)){
						items[i]->timeLive--;
					}

					if (items[i]->timeLive == 0){
						SAFE_DELETE(items[i])
					}
				}
		}

		switch (phase){
//11062013
			case PHASE_CONFIG:
				createGetInitializeFrame();
				sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				isActionTimeOut = false;
				actionTimeOut = 0;
				phase = PHASE_CONFIG_0;
//				DEBUG_PUT_METHOD("PHASE_CONFIG\n");
				break;
			case PHASE_CONFIG_0:
				if (isActionTimeOut){
//					DEBUG_PUT_METHOD("PHASE_CONFIG_0\n");
					isActionTimeOut = false;
					phase = PHASE_CONFIG;
				}
				break;
			case PHASE_CONFIG_1:
//11062013_
//			case PHASE_CONFIG:
//				DEBUG_PUT_METHOD("PHASE_CONFIG_1\n");
				createInitFrame();
				sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				phase = PHASE_CONFIG_WAIT;
				break;
			case PHASE_INIT_WAIT:
				if (isActionTimeOut){
//DEBUG_PUT_METHOD("PHASE_INIT_WAIT\n");
					isActionTimeOut = false;
					createGetInitializeFrame();
					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}
				break;
			case PHASE_START:
				if (isActionTimeOut){
//11062013
//					obrivOffTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_OFF_RPK, address, 0);
					if (obrivOffTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_OFF_RPK, address, 0)){
						phase = PHASE_CONFIG;
						return;
					}
//11062013_
					isActionTimeOut = false;
					createGetAnyReadyFrame();
					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}
				break;
		}
	}
}

bool RpkDevice::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	int index;
	if (isNotTransfer)
	{
		if (_pArea[0] == address)
		{
			switch (phase)
			{
				case PHASE_CONFIG_WAIT:
				case PHASE_INIT_WAIT:
				case PHASE_START:
					switch (_pArea[2])
					{
						case COMMAND_PUT_FRAME: 		
							index = findItemById(_pArea[5] + _pArea[6] * 256);
//							DEBUG_PUT_METHOD("COMMAND_PUT_FRAME: p0 = %i, index = %i, id = %i\n ", _pArea[0], index, _pArea[5] + _pArea[6] * 256);
							if (index != -1)
								removeItem(index);
							break;
						case COMMAND_GET_FRAME_BY_ID: 		
							index = findItemById(_pArea[5] + _pArea[6] * 256);
//							DEBUG_PUT_METHOD("COMMAND_GET_FRAME_BY_ID: p0 = %i, index = %i, id = %i\n ", _pArea[0], index, _pArea[5] + _pArea[6] * 256);
							if (index != -1)
								removeItem(index);
							break;
					}
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
//11062013
					case PHASE_CONFIG_0:
						if (_pArea[2] == COMMAND_GET_INITIALIZE_RESULT){
//							DEBUG_PUT_METHOD("PHASE_CONFIG_0  _pArea[2] == COMMAND_GET_INITIALIZE_RESULT: ");
							if (_pArea[5] == INITIALIZE_RESULT_OK){
//								DEBUG_PUT_METHOD("_pArea[5] == INITIALIZE_RESULT_OK\n");
								phase = PHASE_START;
							}
							else{
//								DEBUG_PUT_METHOD("_pArea[5] != INITIALIZE_RESULT_OK\n");
								phase = PHASE_CONFIG_1;
							}
						}
						break;
//11062013_
				case PHASE_CONFIG_WAIT:
					if (_pArea[INTERNAL_FRAME_DATA_POSITION_COMMAND] == COMMAND_INIT){
//DEBUG_PUT_METHOD("PHASE_CONFIG_WAIT\n");
						phase = PHASE_INIT_WAIT;
					}
					break;
				case PHASE_INIT_WAIT:
					if (_pArea[INTERNAL_FRAME_DATA_POSITION_COMMAND] == COMMAND_GET_INITIALIZE_RESULT)
					{
						if (_pArea[RPK_FRAME_DATA_OFFSET + 1] == INITIALIZE_RESULT_OK)
						{
//DEBUG_PUT_METHOD("PHASE_INIT_WAIT INITIALIZE_RESULT_OK -> phase = PHASE_START\n");
							phase = PHASE_START;
						}

					}
					break;
				case PHASE_START:
					obrivOnTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_ON_RPK, address, 0);
					switch (_pArea[INTERNAL_FRAME_DATA_POSITION_COMMAND])
					{
						case COMMAND_PUT_FRAME:	
							if ((_pArea[INTERNAL_FRAME_DATA_OFFSET] + 2) == PUT_FRAME_RESULT_FALSE)
							{
								index = findItemById(_pArea[INTERNAL_FRAME_DATA_OFFSET] + _pArea[INTERNAL_FRAME_DATA_OFFSET + 1] * 256);
								if (index != -1)
									removeItem(index);
							}
							break;
						case COMMAND_GET_ANY_READY_FRAME:	
							if (((_pArea[INTERNAL_FRAME_DATA_OFFSET + 2]) != FRAME_RESULT_ID_NOT_FOUND) && ((_pArea[INTERNAL_FRAME_DATA_OFFSET + 2]) != FRAME_RESULT_PROCCESS))
								handlerGetAnyReadyFrame(_pArea);
							break;
					}
					break;
			}

			return true;
		}
	}

	return false;
}

void RpkDevice::onMessage(Message message)
{
}

#pragma region createFrames

void RpkDevice::createInitFrame()
{
	unsigned char* initData = new unsigned char[7];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_INIT;
	initData[3] = 0;
	initData[4] = 0;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("RpkDevice::createInitFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, 7);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

void RpkDevice::createGetInitializeFrame()
{
	unsigned char* initData = new unsigned char[7];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_INITIALIZE_RESULT;
	initData[3] = 0;
	initData[4] = 0;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("RpkDevice::createGetInitializeFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, 7);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

#pragma endregion

unsigned int RpkDevice::getId()
{
	return ID;
}

bool RpkDevice::addItem(Item* pItem)
{
	for (unsigned int i = 0; i < MAX_ITEMS_NUMBER; i++)
		if (items[i] == nullptr)
		{
			items[i] = pItem;
			return true;
		}
	return false;
}

bool RpkDevice::removeItem(unsigned int index)
{
	if (index < MAX_ITEMS_NUMBER)
	{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("                   RpkDevice::removeItem: ", index, items[index]->id); 

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("IRpkDevice::FRAME_RESULT_READY: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(items[index]->frame, 20);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

SAFE_DELETE(items[index])
		return true;
	}

	return false;
}

int RpkDevice::findItemById(unsigned int id)
{
	for (unsigned int i = 0; i < MAX_ITEMS_NUMBER; i++)
		if (items != nullptr)
			if (items[i]->id == id)
				return i;

	return -1;
}

unsigned char RpkDevice::calcRpcKS(unsigned char* pFrame)
{
	unsigned char res = 0;
	for (unsigned char i = 0; i < pFrame[3] + 4; i++)
		res += pFrame[i];

	return res;
}

unsigned int RpkDevice::write(unsigned char* pFrame)
{
	if (!disabled)
	{
		if (pFrame != nullptr)
		{
			unsigned int newId = getNextFrameId();
			if (addItem(new Item(newId)))
			{
				unsigned int length = pFrame[3] + 5 + 2;
				unsigned char* frame = new unsigned char[length + 7 + 2 + 2];
		
				frame[0] = address;
				frame[1] = ExtensionSystem::ADDRESS;
				frame[2] = COMMAND_PUT_FRAME;
				frame[3] = length;
				frame[4] = length >> 8;
				frame[5] = newId;
				frame[6] = newId >> 8;

				pFrame[pFrame[3] + 4] = calcRpcKS(pFrame);
				memcpy(&frame[7], pFrame, length);

				fifoFrame->put(&frame);
				sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				return newId;
			}
		}
	}

	return BAD_FRAME_ID;
}

IRpkDevice::FRAME_RESULT RpkDevice::read(unsigned int id, unsigned char** pFrame)
{
	*pFrame = nullptr;
	int index = findItemById(id);

	if (index == -1)
		return IRpkDevice::FRAME_RESULT_ID_NOT_FOUND;

	if (items[index]->resultCode == IRpkDevice::FRAME_RESULT_PROCCESS)
		return IRpkDevice::FRAME_RESULT_PROCCESS;

	FRAME_RESULT resultCode = items[index]->resultCode;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RpkDevice::read: ", resultCode);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("   RpkDevice::read: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(items[index]->frame, 20);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	if (resultCode == IRpkDevice::FRAME_RESULT_READY)
	{
		*pFrame = new unsigned char[items[index]->frame[RPK_FRAME_DATA_POSITION_LENGTH] + 5];
		memcpy(*pFrame, items[index]->frame, items[index]->frame[RPK_FRAME_DATA_POSITION_LENGTH] + 5);
	}

	removeItem(index);
	return resultCode;
}

void RpkDevice::createGetAnyReadyFrame()
{
	unsigned char* frame = new unsigned char[7 + 2];

	frame[0] = address;
	frame[1] = ExtensionSystem::ADDRESS;
	frame[2] = COMMAND_GET_ANY_READY_FRAME;
	frame[3] = 2;
	frame[4] = 0;
	frame[5] = 0;
	frame[6] = 0;

	fifoFrame->put(&frame);
}

void RpkDevice::handlerGetAnyReadyFrame(unsigned char* _pArea)
{
	static const unsigned int STATUS_POSITION = 2;
	static const unsigned int ID_POSITION = 0;
	static const unsigned int DATA_POSITION = 3;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("RpkDevice::handlerGetAnyReadyFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(_pArea, 20);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	unsigned int offset = 0;
	while (offset < static_cast<unsigned int>(_pArea[INTERNAL_FRAME_DATA_POSITION_LENGTH_LOW] + _pArea[INTERNAL_FRAME_DATA_POSITION_LENGTH_HIGH] * 256))
	{
		unsigned short id = _pArea[offset + INTERNAL_FRAME_DATA_OFFSET + ID_POSITION] + (_pArea[offset + INTERNAL_FRAME_DATA_OFFSET + ID_POSITION + 1] << 8);
		int index = findItemById(id);
		unsigned char frameResult = _pArea[offset + INTERNAL_FRAME_DATA_OFFSET + STATUS_POSITION];
		if (frameResult == FRAME_RESULT_READY)
		{
			unsigned int length = _pArea[offset + INTERNAL_FRAME_DATA_OFFSET + DATA_POSITION + RPK_FRAME_DATA_POSITION_LENGTH] + 5;
			items[index]->frame = new unsigned char[length];
			memcpy(items[index]->frame, &_pArea[offset + INTERNAL_FRAME_DATA_OFFSET + DATA_POSITION], length);
			items[index]->resultCode = FRAME_RESULT_READY;
			offset += (length + 3);
		}
		else 
		{
			items[index]->resultCode = FRAME_RESULT_ERROR;
			offset += 3;
		}
	}
}
