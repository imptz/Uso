#pragma once

#include "../process/process.h"
#include "../timer/Timer.h"
#include "../config/config.h"
#include "../action/action.h"

class Povorotniy : public Task<Povorotniy>, public ITimer, public Singleton<Povorotniy>
{
	private:
		virtual void timerHandler();
		static const unsigned int TIMER_PERIOD = 1000;

	public:
		Povorotniy();
		~Povorotniy();

	private:
		CPointer<Povorotniy> init();
		CPointer<Povorotniy> wait();
		CPointer<Povorotniy> go();

		int actionCount;
		Action** actionList;

		bool fStart;
	public:
		void start();
};
