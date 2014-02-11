#ifndef LOGIC_COOLING_H
#define LOGIC_COOLING_H

#include "Logic.h"

class LogicCooling : public Logic
{
	private:
		static const char* CONFIRMATION_TEXT;
		static const char* CANCEL_LOG_TEXT;
		static const char* START_LOG_TEXT;
		static const char* FINISH_LOG_TEXT;

		static const char* LOG_FAULT_TEXT;

		virtual void onMessage(Message message);

	public:
		virtual void action();

		LogicCooling(MessageReceiver* _messageReceiver = nullptr);
		~LogicCooling();

		bool start();
		void stop(bool msg = false);

	private:
		enum PHASE
		{
			PHASE_INPUT_CONTROL,
			PHASE_INPUT_WAITING_CONTROL,
			PHASE_INPUT_ACTION,
			PHASE_WAITING_CONFIRMATION,
			PHASE_STOP_PROGRAM,
			PHASE_START_PROGRAM,
			PHASE_START_PROGRAM111,
			PHASE_OPEN_GATE,
			PHASE_WAITING_STOP,
			PHASE_CLOSE_GATE,
			PHASE_STOP_PROGRAM_END,
			PHASE_STOP
		};
		PHASE phase;

		virtual void finish(FINISH_ACTOR _finishActor);

		bool phaseStopProgram_Start();
		bool phaseStopProgram_Execution();
		bool phaseStartProgram_Start();
		bool phaseStartProgram_Execution();
		bool phaseGateOpen_Start();
		bool phaseGateOpen_Execution();
		bool phaseWaitingStop_Start();
		bool phaseWaitingStop_Execution();
		bool phaseGateClose_Start();
		bool phaseGateClose_Execution();
		bool phaseStopProgramEnd_Start();
		bool phaseStopProgramEnd_Execution();

		// M061112
		void startWaitingConf();
		// M061112E
};

#endif