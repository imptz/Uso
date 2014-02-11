#pragma once

#include "action.h"

class ActionPovorotniy : public Action
{
	private:
		virtual void timerHandler();
		static const unsigned int TEST_TIME_OUT = 120;
		ActionPovorotniy();

	public:
		ActionPovorotniy(unsigned char _deviceAddress);
		virtual ~ActionPovorotniy();
		virtual void onMessage(Message message);
		virtual void step();

	private:
		enum PHASE
		{
			PHASE_COMMAND,
			PHASE_COMMAND_WAIT,
			PHASE_STOP
		};
		PHASE phase;

		int frameId;

		void error();
		void finish();

		enum SUB_COMMAND
		{
			SUB_COMMAND_SVERNUT = 2
		};
};
