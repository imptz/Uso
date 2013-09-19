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
int Logic::getActiveInitialSignal(LOGIC_FUNCTION function, LOGIC_FUNCTION function1, LOGIC_FUNCTION function2)
{
	ConfigDataStructInitSignal** pData = Config::getSingleton().getConfigData()->getConfigDataStructInitSignals();

	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructInitSignalsCount(); i++)
	{
		IIODevice::INPUT_STATE signal0 = IOSubsystem::getSingleton().getInput(pData[i]->firstInputNumber);
		IIODevice::INPUT_STATE signal1, signal2;

		if (pData[i]->secondInputNumber == 0)
			signal1 = IIODevice::INPUT_STATE_ON;
		else
			signal1 = IOSubsystem::getSingleton().getInput(pData[i]->secondInputNumber);

		if (pData[i]->thirdInputNumber == 0)
			signal2 = IIODevice::INPUT_STATE_ON;
		else
			signal2 = IOSubsystem::getSingleton().getInput(pData[i]->thirdInputNumber);
		
		if ((signal0 == IIODevice::INPUT_STATE_ON) && (signal1 == IIODevice::INPUT_STATE_ON) && (signal2 == IIODevice::INPUT_STATE_ON)) 
		{
DEBUG_PUT_METHOD("i = %i, pData[i]->function = %i, function = %i, function1 = %i, function2 = %i\n", 
	i, pData[i]->function, function, function1, function2);
			if (((pData[i]->function == function) || (pData[i]->function == function1) || (pData[i]->function == function2)) && (!pData[i]->ignorable))
			{
				SAFE_DELETE_ARRAY(listProgramIndex)

				listProgramIndexCount = Config::getSingleton().getConfigData()->getConfigDataStructProgramInitSignal(pData[i]->number, pData[i]->function, &listProgramIndex);
DEBUG_PUT_METHOD("listProgramIndexCount = %i\n", listProgramIndexCount);
				if (listProgramIndexCount > 0)
					return i;
				else
					pData[i]->ignorable = true;
			}
			else
				continue;
		}
		else
			pData[i]->ignorable = false;
	}

	return -1;
}

bool Logic::testInitSignal(int index)
{
	ConfigDataStructInitSignal** pData = Config::getSingleton().getConfigData()->getConfigDataStructInitSignals();

	IIODevice::INPUT_STATE signal0 = IOSubsystem::getSingleton().getInput(pData[index]->firstInputNumber);
	IIODevice::INPUT_STATE signal1 = IOSubsystem::getSingleton().getInput(pData[index]->secondInputNumber);
	IIODevice::INPUT_STATE signal2 = IOSubsystem::getSingleton().getInput(pData[index]->thirdInputNumber);
		
	bool result = true;

	if ((pData[index]->firstInputNumber != 0) && (signal0 != IIODevice::INPUT_STATE_ON))
		result = false;
	if ((pData[index]->secondInputNumber != 0) && (signal1 != IIODevice::INPUT_STATE_ON))
		result = false;
	if ((pData[index]->thirdInputNumber != 0) && (signal2 != IIODevice::INPUT_STATE_ON))
		result = false;

	return result;
}
// M13112012E
void Logic::setInitSignalIgnorable(int index, bool value)
{
	Config::getSingleton().getConfigData()->getConfigDataStructInitSignals()[index]->ignorable = value;
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
	int time = Config::getSingleton().getConfigData()->getConfigDataStructConst()->timeControlUserAction;

	if (time == 0)
		time = TIME_OUT_WAITING_UNDEFINED;

	DEBUG_PUT_METHOD("timeOut = %is\n", time);

	return time;
}
// M061112E
