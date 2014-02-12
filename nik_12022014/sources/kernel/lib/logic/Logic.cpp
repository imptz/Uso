#include "Logic.h"
#include "../controls/UI.h"
#include "../Application.h"
#include "../log/Log.h"
#include "../DEBUG/serialDebug.h"

Logic::Logic()
	:	ITimer(TIMER_PERIOD), initSignal(-1), actionCount(0), actionList(nullptr), listProgramIndex(nullptr), listProgramIndexCount(0), finishTimer(-1), timeOutBeforeStart(-1), 
		// M061112
		timeOutWaiting(TIME_OUT_WAITING_UNDEFINED),
		// M061112E
		pumpOutputEnable(false)
{
	pTimer->start();
}

Logic::~Logic()
{

}

void Logic::timerHandler()
{
	if (timeOutBeforeStart != -1)
	{
		if (timeOutBeforeStart != 0)
			timeOutBeforeStart--;
	}
	// M061112
	if (timeOutWaiting != TIME_OUT_WAITING_UNDEFINED)
	{
		if (timeOutWaiting != 0)
			timeOutWaiting--;
	}
	// M061112E
	if (finishTimer != -1)
	{
		if (finishTimer == 0)
		{
			finishTimer = -1;
			finish(FINISH_ACTOR_TIMER);
		}
		else
			--finishTimer;
	}
}

// M13112012
int Logic::getActiveInitialSignal(unsigned int function, unsigned int function1)
{
//	ConfigDataStructInitSignal** pData = Config::getSingleton().getConfigData()->getConfigDataStructInitSignals();

	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->initSignals_count; i++)
	{
		IIODevice::INPUT_STATE signal0 = IOSubsystem::getSingleton().getInput(Config::getSingleton().getConfigData()->initSignal[i].firstInputNumber);
		IIODevice::INPUT_STATE signal1, signal2;

		if (Config::getSingleton().getConfigData()->initSignal[i].secondInputNumber == 0)
			signal1 = IIODevice::INPUT_STATE_ON;
		else
			signal1 = IOSubsystem::getSingleton().getInput(Config::getSingleton().getConfigData()->initSignal[i].secondInputNumber);

		if (Config::getSingleton().getConfigData()->initSignal[i].thirdInputNumber == 0)
			signal2 = IIODevice::INPUT_STATE_ON;
		else
			signal2 = IOSubsystem::getSingleton().getInput(Config::getSingleton().getConfigData()->initSignal[i].thirdInputNumber);
		
		if ((signal0 == IIODevice::INPUT_STATE_ON) && (signal1 == IIODevice::INPUT_STATE_ON) && (signal2 == IIODevice::INPUT_STATE_ON)) 
		{
			if (((Config::getSingleton().getConfigData()->initSignal[i].function == function) || (Config::getSingleton().getConfigData()->initSignal[i].function == function1)) && (!Config::getSingleton().getConfigData()->initSignal[i].ignorable))
			{
				SAFE_DELETE_ARRAY(listProgramIndex)

				listProgramIndexCount = Config::getSingleton().getConfigDataStructProgramInitSignal(Config::getSingleton().getConfigData()->initSignal[i].number, Config::getSingleton().getConfigData()->initSignal[i].function, &listProgramIndex);
				if (listProgramIndexCount > 0)
					return i;
				else
					Config::getSingleton().getConfigData()->initSignal[i].ignorable = true;
			}
			else
				continue;
		}
		else
			Config::getSingleton().getConfigData()->initSignal[i].ignorable = false;
	}

	return -1;
}

bool Logic::testInitSignal(int index)
{
//	ConfigDataStructInitSignal** pData = Config::getSingleton().getConfigData()->getConfigDataStructInitSignals();

	IIODevice::INPUT_STATE signal0 = IOSubsystem::getSingleton().getInput(Config::getSingleton().getConfigData()->initSignal[index].firstInputNumber);
	IIODevice::INPUT_STATE signal1 = IOSubsystem::getSingleton().getInput(Config::getSingleton().getConfigData()->initSignal[index].secondInputNumber);
	IIODevice::INPUT_STATE signal2 = IOSubsystem::getSingleton().getInput(Config::getSingleton().getConfigData()->initSignal[index].thirdInputNumber);
		
	bool result = true;

	if ((Config::getSingleton().getConfigData()->initSignal[index].firstInputNumber != 0) && (signal0 != IIODevice::INPUT_STATE_ON))
		result = false;
	if ((Config::getSingleton().getConfigData()->initSignal[index].secondInputNumber != 0) && (signal1 != IIODevice::INPUT_STATE_ON))
		result = false;
	if ((Config::getSingleton().getConfigData()->initSignal[index].thirdInputNumber != 0) && (signal2 != IIODevice::INPUT_STATE_ON))
		result = false;

	return result;
}
// M13112012E
void Logic::setInitSignalIgnorable(int index, bool value)
{
	Config::getSingleton().getConfigData()->initSignal[index].ignorable = value;
}

bool Logic::testTotalError()
{
	bool result = true;
	for (unsigned int i = 0; i < actionCount; i++)
		if (actionList[i] != nullptr)
			result = false;
	return result;
}

void Logic::deleteActionInList(unsigned int index)
{
	if ((actionCount > 0) && (index < actionCount))
	{
		delete actionList[index];
		if (actionCount > 1)
		{
			actionList[index] = actionList[actionCount - 1];
			actionList[actionCount - 1] = nullptr;
		}
		else
			actionList[0] = nullptr;

		actionCount--;
	}
}

int Logic::indexPrInActionList(unsigned int value)
{
	for (unsigned int i = 0; i < actionCount; i++)
		if (actionList[i]->getDeviceAddress() == value)
			return i;

	return -1;
}

// M061112
int Logic::getConfigTimeOutWaiting()
{
	int time = Config::getSingleton().getConfigData()->constants.timeControlUserAction;

	if (time == 0)
		time = TIME_OUT_WAITING_UNDEFINED;

	DEBUG_PUT_METHOD("timeOut = %is\n", time);

	return time;
}
// M061112E
