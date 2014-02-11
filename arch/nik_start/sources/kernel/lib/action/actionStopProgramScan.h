#pragma once
#include "action.h"

class ActionStopProgramScan : public Action
{
	private:
		virtual void timerHandler();
		ActionStopProgramScan();

	public:
		ActionStopProgramScan(unsigned char _deviceAddress);
		virtual ~ActionStopProgramScan();
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

		void error();
		void finish();
};
