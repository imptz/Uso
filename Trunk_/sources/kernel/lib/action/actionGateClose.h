#pragma once

#include "action.h"
#include "actionGate.h"

class ActionGateClose : public Action
{
	private:
		virtual void timerHandler();
		unsigned int testTimeOut;
		static const unsigned int TEST_TIME_OUT = 30;
		ActionGateClose();
		GATE_TYPE gateType;

	public:
		ActionGateClose(unsigned char _deviceAddress, GATE_TYPE _gateType = GATE_TYPE_WATER);
		virtual ~ActionGateClose();
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
