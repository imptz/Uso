#pragma once

#include "action.h"

class ActionValveClose : public Action
{
	private:
		virtual void timerHandler();
		unsigned int testTimeOut;
		static const unsigned int TEST_TIME_OUT = 5;
		ActionValveClose();

	public:
		ActionValveClose(unsigned char _deviceAddress);
		virtual ~ActionValveClose();
		virtual void onMessage(Message message);
		virtual void step();

	private:
		enum PHASE
		{
			PHASE_COMMAND,
			PHASE_COMMAND_WAIT,
			PHASE_TEST,
			PHASE_TEST_WAIT,
			PHASE_STOP
		};
		PHASE phase;

		void error();
		void finish();
};
