#pragma once

#include "action.h"
#include "../fire/Fire.h"

class ActionSendPressureTable : public Action
{
	private:
		virtual void timerHandler();
		static const unsigned int TEST_TIME_OUT = 60;

		Fire::FireScanProgram* program;

		ActionSendPressureTable();

	public:
		ActionSendPressureTable(Fire::FireScanProgram* _program);
		virtual ~ActionSendPressureTable();
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

	public:
		Fire::FireScanProgram* getProgram();
};
