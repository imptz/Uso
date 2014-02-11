#pragma once

#include "action.h"
#include "../math/math.h"

class ActionGateOpen : public Action
{
	private:
		virtual void timerHandler();
		static const unsigned int TEST_TIME_OUT = 30;
		ActionGateOpen();

	public:
		ActionGateOpen(unsigned char _deviceAddress);
		virtual ~ActionGateOpen();
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

		int frameId;

		void error();
		void finish();
};
