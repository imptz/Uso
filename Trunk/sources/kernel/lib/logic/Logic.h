#ifndef LOGIC_H
#define LOGIC_H

#include "../timer/Timer.h"
#include "../extension/subsystems/io/ioSubsystem.h"
#include "../extension/subsystems/detection/detectionSubsystem.h"
#include "../config/Config.h"

#include "../Local.h"
#include "../action/actionStopProgramScan.h"
#include "../action/actionStartProgramScanLine.h"
#include "../action/actionStartProgramScanPoint.h"
#include "../action/actionGateOpen.h"
#include "../action/actionGateClose.h"
#include "../action/actionSendPressureTable.h"
#include "../controls/UI.h"

class Logic : public ITimer, public MessageReceiver, public MessageSender
{
	private:
		virtual void timerHandler();
		static const unsigned int TIMER_PERIOD = 1000;

	public:
		Logic();
		~Logic();
		virtual void action() = 0;
	
	protected:
		virtual bool start() = 0;
		virtual void stop(bool msg = false) = 0;

		enum START_ACTOR
		{
			START_ACTOR_FULL_AUTO = 0,
			START_ACTOR_HALF_AUTO = 1
		};

		enum FINISH_ACTOR
		{
			FINISH_ACTOR_TIMER = 0,
			FINISH_ACTOR_BUTTON = 1,
			FINISH_ACTOR_LOGIC = 2
		};
		FINISH_ACTOR finishActor;
		virtual void finish(FINISH_ACTOR _finishActor) = 0;
		int finishTimer;

		int initSignal;
		int getActiveInitialSignal(LOGIC_FUNCTION function, LOGIC_FUNCTION function1 = LOGIC_FUNCTION_UNDEFINED, LOGIC_FUNCTION function2 = LOGIC_FUNCTION_UNDEFINED);
		bool testInitSignal(int index);
		void setInitSignalIgnorable(int index, bool value = true);

	public:
		enum LOGIC_MESSAGE
		{
			LOGIC_MESSAGE_GET_CONFIRMATION = 1,
			LOGIC_MESSAGE_GET_FINISH = 2
		};

	protected:
		char* cancelLogText;
		char* startLogText;
		char* dialogText;

		unsigned int actionCount;
		Action** actionList;

		unsigned int* listProgramIndex;
		unsigned int listProgramIndexCount;
		bool testTotalError();
		void deleteActionInList(unsigned int index);
		int indexPrInActionList(unsigned int value);

		int timeOutBeforeStart;

		// M061112
		static const int TIME_OUT_WAITING_UNDEFINED = -1;
		int timeOutWaiting;
		int getConfigTimeOutWaiting();
		// M061112E
		bool pumpOutputEnable;
};

#endif