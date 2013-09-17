#pragma once

#include "../../devices/IDevice.h"

class IRpkDevice : public IDevice
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
			PHASE_FRAME_WAIT
		};
		PHASE phase;
	public:
		enum COMMAND
		{
			COMMAND_GET_ID = 1, 
			COMMAND_INIT = 2, 
			COMMAND_GET_INITIALIZE_RESULT = 3,
			COMMAND_PUT_FRAME = 4,
			COMMAND_GET_FRAME_BY_ID = 5,
			COMMAND_GET_ANY_READY_FRAME = 6
		};

		IRpkDevice(unsigned char _address, unsigned int _type)
			:	IDevice(_address, _type), phase(PHASE_STOP), nextFrameId(FIRST_FRAME_ID)
		{
		}

	public:
		enum PUT_FRAME_RESULT
		{
			PUT_FRAME_RESULT_TRUE = 0,
			PUT_FRAME_RESULT_FALSE = 1
		};

		enum FRAME_RESULT
		{
			FRAME_RESULT_PROCCESS = 1,
			FRAME_RESULT_READY = 2,
			FRAME_RESULT_ERROR = 3,
			FRAME_RESULT_ID_NOT_FOUND = 4
		};

	public:
		static const unsigned int BAD_FRAME_ID = 0;
	protected:
		static const unsigned int FIRST_FRAME_ID = 1;
		static const unsigned int MAX_FRAME_ID = 65535;
		unsigned int nextFrameId;
		unsigned int getNextFrameId()
		{
			if (nextFrameId == MAX_FRAME_ID)
				nextFrameId = FIRST_FRAME_ID;
			else
				nextFrameId++;

			return nextFrameId;
		}

	public:
		virtual unsigned int write(unsigned char* pFrame) = 0;
		virtual IRpkDevice::FRAME_RESULT read(unsigned int id, unsigned char** pFrame) = 0;

	protected:
		virtual void disablingDevice()
		{
			disabled = true;
			phase = PHASE_START;
		}
};
