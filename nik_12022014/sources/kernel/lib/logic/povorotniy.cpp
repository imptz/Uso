#include "povorotniy.h"

#include "../DEBUG/serialDebug.h"
#include "..\config\Config.h"
#include "..\action\actionPovorotniy.h"

Povorotniy::Povorotniy()
	:	Task(&Povorotniy::init), ITimer(TIMER_PERIOD)
{
	actionCount = 0;
	actionList = nullptr;

	fStart = false;

	Process::getSingleton().addTask(this);

	/*pTimer->start();*/
}

Povorotniy::~Povorotniy()
{
}

void Povorotniy::timerHandler()
{
}

void Povorotniy::start()
{
	fStart = true;
}

CPointer<Povorotniy> Povorotniy::init()
{
	return &Povorotniy::wait;
}

CPointer<Povorotniy> Povorotniy::wait()
{
	if (fStart)
	{
		fStart = false;

//		sp = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();
		actionCount = Config::getSingleton().getConfigData()->prPositions_count;
		actionList = new Action*[actionCount];

		for (unsigned int i = 0; i < static_cast<unsigned int>(actionCount); i++)
			actionList[i] = new ActionPovorotniy(Config::getSingleton().getPRAddressByNumber(Config::getSingleton().getConfigData()->prPositions[i].projectNumber));

		return &Povorotniy::go;
	}
	else
		return &Povorotniy::wait;
}

CPointer<Povorotniy> Povorotniy::go()
{
	bool complete = true; 
	for (unsigned int i = 0; i < static_cast<unsigned int>(actionCount); i++)
	{
		actionList[i]->step();

		if (actionList[i]->getState() == Action::STATE_UNDEFINED)
			complete = false;
	}

	if (complete)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(actionCount); i++)
			SAFE_DELETE(actionList[i]);

		SAFE_DELETE_ARRAY(actionList);

		return &Povorotniy::init;
	}

	return &Povorotniy::go;
}
