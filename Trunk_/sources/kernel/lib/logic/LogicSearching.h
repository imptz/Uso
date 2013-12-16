#ifndef LOGIC_SEARCHING_H
#define LOGIC_SEARCHING_H

#include "Logic.h"
#include "../extension/subsystems/detection/detectionSubsystem.h"
#include "../fire/Fire.h"

class LogicSearching : public Logic
{
	private:
		static const char* CONFIRMATION_TEXT;
		static const char* CANCEL_LOG_TEXT;
		static const char* START_LOG_TEXT;
		static const char* FINISH_LOG_TEXT;

		static const char* LOG_FAULT_TEXT;
		
		static const char* LOG_NOT_FIRE_DETECT_TEXT;
		static const char* LOG_CANCEL_EXTINGUISHING_TEXT;
		static const char* LOG_START_EXTINGUISHING_TEXT;
		static const char* CONFIRMATION_EXTINGUISHING_TEXT;

		virtual void onMessage(Message message);

	public:
		virtual void action();

		LogicSearching(MessageReceiver* _messageReceiver = nullptr);
		~LogicSearching();

		bool start();
		void stop(bool msg = false);

	private:
		enum PHASE
		{
			PHASE_INPUT_CONTROL,
			PHASE_INPUT_WAITING_CONTROL,
			PHASE_INPUT_ACTION,
			// M061112
			PHASE_WAITING_CONFIRMATION_SEARCH,
			PHASE_WAITING_CONFIRMATION_TUSHENIE,
			// M061112E
			PHASE_WAITING_FIRE,
			PHASE_SEND_PRESSURE_TABLE,
			PHASE_START_PROGRAM,
			PHASE_OPEN_GATE,
			PHASE_OPEN_VALVE,
			PHASE_WAITING_STOP,
			PHASE_CLOSE_VALVE,
			PHASE_CLOSE_GATE,
			PHASE_STOP_PROGRAM_END,
			PHASE_COOLING_STOP,
			PHASE_COOLING_START,
			PHASE_COOLING_END,
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
		bool phaseOpenValve_Start();
		bool phaseOpenValve_Execution();
		bool phaseWaitingStop_Start();
		bool phaseWaitingStop_Execution();
		bool phaseCloseValve_Start();
		bool phaseCloseValve_Execution();
		bool phaseGateClose_Start();
		bool phaseGateClose_Execution();
		bool phaseStopProgramEnd_Start();
		bool phaseStopProgramEnd_Execution();
		bool phaseGetFire_Start();
		bool phaseSendPressureTable_Start(unsigned int _fireCount, Fire::FireScanProgram* _programs);
		bool phaseSendPressureTable_Execution();

		Fire::FireObject fire;
		unsigned int fireCount;
		Fire::FireScanProgram* programs;
		bool calcProgram(unsigned int* channelsCount, PreFire* localFires, Fire::FireObject* fire, Fire::FireScanProgram** _programs);

		void coolingAction();
		bool isCoolingStart;
		int coolingSignal;
		bool createCoolingList();
		unsigned int* listCoolingStartIndex;
		unsigned int listCoolingStartCount;
		unsigned int* listOff;
		unsigned int listOffCount;
		Action** actionStartList;
		Action** actionStopList;

		bool phaseCoolingStopProgram_Start();
		bool phaseCoolingStopProgram_Execution();
		bool phaseCoolingStartProgram_Start();
		bool phaseCoolingStartProgram_Execution();
		bool phaseCoolingEnd_Start();
		bool phaseCoolingEnd_Execution();

		PreFire* localFires;
		// M061112
		void startWaitingConf();
		void startWaitingConfSearch();
		void startWaitingConfTushenie();
		// M061112E
};

#endif
