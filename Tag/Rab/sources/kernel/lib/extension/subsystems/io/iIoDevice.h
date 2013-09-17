#pragma once

#include "../../devices/IDevice.h"
#include "../../../DEBUG/serialDebug.h"

class IIODevice : public IDevice
{
	protected:
		enum PHASE
		{
			PHASE_STOP,
			PHASE_CONFIG,
//11062013
			PHASE_CONFIG_0,
			PHASE_CONFIG_1,
//11062013_
			PHASE_CONFIG_WAIT,
			PHASE_INIT_WAIT,
			PHASE_START,
			PHASE_INPUT_WAIT
		};
		PHASE phase;
	public:
		enum COMMAND
		{
			COMMAND_GET_ID = 1, 
			COMMAND_INIT = 2, 
			COMMAND_GET_INITIALIZE_RESULT = 3,
			COMMAND_SET_OUTPUT = 4,
			COMMAND_GET_INPUTS = 5 
		};

		IIODevice(unsigned char _address, unsigned int _type)
			:	IDevice(_address, _type), phase(PHASE_STOP), pInputs(nullptr), inputsCount(0), pOutputs(nullptr), outputsCount(0)
		{}

	protected:
		virtual void disablingDevice()
		{
			disabled = true;
			phase = PHASE_START;
		}

#pragma region ioSpecific
	public:
		enum INPUT_STATE
		{
			INPUT_STATE_ON = 2,
			INPUT_STATE_OFF = 1,
			INPUT_STATE_UNDEFINED = 0
		};
		struct Input
		{
			unsigned int projectNumber;
			unsigned int onDeviceNumber;
			INPUT_STATE state;
		};
	protected:
		Input* pInputs;
		unsigned int inputsCount;
		Input* getInputByOnDeviceNumber(unsigned int onDeviceNumber)
		{
			for (unsigned int i = 0; i < inputsCount; i++)
				if (pInputs[i].onDeviceNumber == onDeviceNumber)
					return &pInputs[i];

			return nullptr;
		}

	public:
		enum OUTPUT_STATE
		{
			OUTPUT_STATE_ON = 1,
			OUTPUT_STATE_OFF = 0
		};
		struct Output
		{
			unsigned int projectNumber;
			unsigned int onDeviceNumber;
		};
	protected:
		Output* pOutputs;
		unsigned int outputsCount;

	public:
		virtual bool getInput(unsigned int number, INPUT_STATE* value) = 0;
		virtual bool setOutput(unsigned int number, OUTPUT_STATE value) = 0;
		virtual void setOutputs(unsigned int* numbers, unsigned int count, OUTPUT_STATE value) = 0;
#pragma endregion
};
