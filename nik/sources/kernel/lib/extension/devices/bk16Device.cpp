#include "bk16Device.h"
#include "../ExtensionSystem.h"
#include "../../config/ConfigData.h"
#include "../../config/Config.h"
#include "../../Local.h"
#include "../../log/Log.h"

bool Bk16Device::registered = false;

Bk16Device::Bk16Device(unsigned char _address, unsigned int _type)
	:	IIODevice(_address, _type)
{
	ConfigData* pConfigData = Config::getSingleton().getConfigData();
	unsigned count = pConfigData->getConfigDataStructIOBk16Count();
	ConfigDataStructIOBk16** bkStruct = pConfigData->getConfigDataStructIOBk16();

	inputsCount = count;
	pInputs = new Input[inputsCount];

	for (unsigned int i = 0; i < count; i++)
		if (bkStruct[i]->outputFunctionGroup != ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_UNDEFINED)
			outputsCount++;

	pOutputs = new Output[outputsCount];

	unsigned int _ouputCount = 0;
	for (unsigned int i = 0; i < count; i++)
	{
		pInputs[i].projectNumber = bkStruct[i]->projectNumber;
		pInputs[i].onDeviceNumber = (bkStruct[i]->bkAddress - 1) * 8 + bkStruct[i]->numberOnDevice - 1;
		pInputs[i].state = INPUT_STATE_UNDEFINED;

		if (bkStruct[i]->outputFunctionGroup != ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_UNDEFINED)
		{
			pOutputs[_ouputCount].projectNumber = bkStruct[i]->projectNumber;
			pOutputs[_ouputCount++].onDeviceNumber = (bkStruct[i]->bkAddress - 1) * 8 + bkStruct[i]->numberOnDevice - 1;
		}
	}

	ExtensionSystem::getSingleton().addReceiver(this);
	addReceiver(ExtensionSystem::getSingletonPtr());
}

Bk16Device::~Bk16Device()
{
	SAFE_DELETE(pInputs)
	SAFE_DELETE(pOutputs)
}

void Bk16Device::init()
{
	phase = PHASE_CONFIG;
}

bool Bk16Device::isReady()
{
	return (phase == PHASE_START);
}

void Bk16Device::timerHandler()
{
	if (actionTimeOut++ == ACTION_TIME_OUT)
	{
		isActionTimeOut = true;
		actionTimeOut = 0;
	}
}

void Bk16Device::action()
{
	if (!disabled)
	{
		switch (phase)
		{
//11062013
			//case PHASE_CONFIG:
			//	createGetInitializeFrame();
			//	sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
			//	isActionTimeOut = false;
			//	actionTimeOut = 0;
			//	phase = PHASE_CONFIG_0;
			//	break;
			//case PHASE_CONFIG_0:
			//	if (isActionTimeOut)
			//	{
			//		isActionTimeOut = false;
			//		phase = PHASE_CONFIG;
			//	}
			//	break;
			//case PHASE_CONFIG_1:
//11062013_
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
			case PHASE_INPUT_WAIT:
				if (isActionTimeOut){
//11062013
//					if (!obrivOffTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_OFF_BK, address, 0)){
					obrivOffTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_OFF_BK, address, 0);
						isActionTimeOut = false;
						createGetInputsFrame();
						sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
						phase = PHASE_INPUT_WAIT;
					//}
					//else{
					//	phase = PHASE_CONFIG;
					//}
//11062013_
				}
				break;
		}
	}
}

bool Bk16Device::putFrame(unsigned char* _pArea, bool isNotTransfer)
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
					case PHASE_INPUT_WAIT:
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
				switch (phase){
//11062013
					case PHASE_CONFIG_0:
						if (_pArea[2] == COMMAND_GET_INITIALIZE_RESULT){
							if (_pArea[5] == INITIALIZE_RESULT_OK){
								phase = PHASE_START;
							}
							else
								phase = PHASE_CONFIG_1;
						}
						break;
//11062013_
					case PHASE_CONFIG_WAIT:
						if (_pArea[2] == COMMAND_INIT)
						{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device.cpp_120 PHASE_CONFIG_WAIT\n");
							phase = PHASE_INIT_WAIT;
						}
						break;
					case PHASE_INIT_WAIT:
						if (_pArea[2] == COMMAND_GET_INITIALIZE_RESULT)
						{
							if (_pArea[5] == INITIALIZE_RESULT_OK)
							{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("...............................................................Bk16Device == INITIALIZE_RESULT_OK\n");
								phase = PHASE_START;
							}
						}
						break;
					case PHASE_INPUT_WAIT:
						if (_pArea[2] == COMMAND_GET_INPUTS)
						{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device.cpp_137 PHASE_INPUT_WAIT\n");
							obrivOnTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_ON_BK, address, 0);
							analizeInputs(_pArea);
						}
						break;
				}

				return true;
			}
		}
	}

	return false;
}

void Bk16Device::onMessage(Message message)
{
}

#pragma region createFrames

void Bk16Device::createInitFrame()
{
	ConfigData* pConfigData = Config::getSingleton().getConfigData();
	
	const unsigned int MAX_BOARDS_COUNT = 65;
	bool boards[MAX_BOARDS_COUNT];
	
	for (unsigned int i = 0; i < MAX_BOARDS_COUNT; i++)
		boards[i] = false;

	unsigned count = pConfigData->getConfigDataStructIOBk16Count();
	ConfigDataStructIOBk16** bkStruct = pConfigData->getConfigDataStructIOBk16();

	for (unsigned int i = 0; i < count; i++)
		if (bkStruct[i]->bkAddress < 64)
			boards[bkStruct[i]->bkAddress] = true;

	unsigned char boardsCount = 0;
	for (unsigned int i = 0; i < MAX_BOARDS_COUNT; i++)
		if (boards[i])
			boardsCount++;

	unsigned int dataLength = 1 + boardsCount + boardsCount * 8;
	unsigned char* initData = new unsigned char[dataLength + 7];
	const unsigned int DATA_OFFSET = 5;
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_INIT;
	initData[3] = dataLength;
	initData[4] = dataLength >> 8;

	initData[DATA_OFFSET + 0] = boardsCount;

	unsigned int c = 1;
	for (unsigned int i = 1; i < MAX_BOARDS_COUNT; i++)
		if (boards[i])
			{
				initData[DATA_OFFSET + c] = i;
				c++;
			}

	for (unsigned int i = 0; i < count; i++)
		initData[DATA_OFFSET + 1 + boardsCount + (bkStruct[i]->bkAddress - 1) * 8 + bkStruct[i]->numberOnDevice - 1] = bkStruct[i]->inputFunctionGroup;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device::createInitFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, dataLength + 7);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

void Bk16Device::createGetInitializeFrame()
{
	unsigned char* initData = new unsigned char[7];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_INITIALIZE_RESULT;
	initData[3] = 0;
	initData[4] = 0;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device::createGetInitializeFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, 7);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

void Bk16Device::createSetOutputFrame(unsigned int number, OUTPUT_STATE value)
{
	unsigned char* initData = new unsigned char[7 + 5];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_SET_OUTPUT;
	initData[3] = 5;
	initData[4] = 0;
	initData[5] = 1;
	initData[6] = 0;
	initData[7] = number;
	initData[8] = number >> 8;
	initData[9] = value;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device::createSetOutputFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, 10);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

void Bk16Device::createSetOutputsFrame(unsigned int* number, unsigned int count, OUTPUT_STATE value)
{
	unsigned char* initData = new unsigned char[7 + 2 + 3 * count];
	unsigned int dataLength = count * 3 + 2;
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_SET_OUTPUT;
	initData[3] = dataLength;
	initData[4] = dataLength >> 8;
	initData[5] = count;
	initData[6] = count >> 8;

	for (unsigned int i = 0; i < count; i++)
	{
		initData[7 + i * 3] = number[i];
		initData[8 + i * 3] = number[i] >> 8;
		initData[9 + i * 3] = value;
	}

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device::createSetOutputsFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, 7 + 2 + 3 * count);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

void Bk16Device::createGetInputsFrame()
{
	unsigned char* initData = new unsigned char[7 + 2 + inputsCount * 2];
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_INPUTS;

	unsigned int length = inputsCount * 2 + 2;

	initData[3] = length;
	initData[4] = length >> 8;
	
	initData[5] = inputsCount;
	initData[6] = inputsCount >> 8;

	for (unsigned int i = 0; i < inputsCount; i++)
	{
		initData[7 + i * 2] = pInputs[i].onDeviceNumber;
		initData[8 + i * 2] = pInputs[i].onDeviceNumber >> 8;
	}

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device::createGetInputsFrame ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(initData, 7 + inputsCount * 2);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

	fifoFrame->put(&initData);
}

#pragma endregion

unsigned int Bk16Device::getId()
{
	return ID;
}

bool Bk16Device::getInput(unsigned int number, INPUT_STATE* value)
{
	for (unsigned int i = 0; i < inputsCount; i++)
	{
		if (pInputs[i].projectNumber == number)
		{
			*value = pInputs[i].state;
			return true;
		}
	}

	return false;
}

bool Bk16Device::isOutputExists(unsigned int number)
{
	for (unsigned int i = 0; i < outputsCount; i++)
		if (pOutputs[i].onDeviceNumber == number)
			return true;

	return false;
}

bool Bk16Device::setOutput(unsigned int number, OUTPUT_STATE value)
{
	if (!disabled)
	{
		for (unsigned int i = 0; i < outputsCount; i++)
		{
			if (isOutputExists(number))
			{
				createSetOutputFrame(number, value);
				sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				return true;
			}
		}
	}

	return false;
}

void Bk16Device::setOutputs(unsigned int* numbers, unsigned int count, OUTPUT_STATE value)
{
	if (!disabled)
	{
		unsigned int* _outputs = new unsigned int[count];
		unsigned int _count = 0;

		for (unsigned int i = 0; i < count; i++)
			if (isOutputExists(numbers[i]))
				_outputs[_count++] = numbers[i];


		createSetOutputsFrame(_outputs, _count, value);
		sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
	}
}

void Bk16Device::analizeInputs(unsigned char* resizableArea)
{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Bk16Device::analizeInputs: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(resizableArea, resizableArea[4] * 256 + resizableArea[3]);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");
	unsigned char* _pData = resizableArea;
	unsigned int _inCount = _pData[6];
	_inCount = _inCount * 256 + _pData[5];

	unsigned int _messageOffset = _inCount * 3 + 5;

	if (inputsCount < _inCount)
		_inCount = inputsCount;

	for (unsigned int i = 0; i < _inCount; i++)
	{
		unsigned int _onDeviceNumber = _pData[7 + i * 3 + 1]; 
		_onDeviceNumber = _onDeviceNumber * 256 + _pData[7 + i * 3];
		Input* inp = getInputByOnDeviceNumber(_onDeviceNumber);
		if (inp != nullptr)
		{
			if (_pData[7 + i * 3 + 2] > 2)
				inp->state = INPUT_STATE_UNDEFINED;
			else
				inp->state = static_cast<IIODevice::INPUT_STATE>(_pData[7 + i * 3 + 2]);
		}
	}

	phase = PHASE_START;
}
