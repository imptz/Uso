#pragma once

#include <Multithreading\Thread.h>
#include "../timer/Timer.h"
#include "../config/Config.h"
#include "../action/action.h"

class Povorotniy : public Fr::Thread<Povorotniy>, public ITimer, public Singleton<Povorotniy>
{
	private:
		virtual void timerHandler();
		static const unsigned int TIMER_PERIOD = 1000;

	public:
		Povorotniy();
		~Povorotniy();

	private:
		Fr::CPointer<Povorotniy> init();
		Fr::CPointer<Povorotniy> wait();
		Fr::CPointer<Povorotniy> go();

		ConfigDataStructPRPosition** sp;
		int actionCount;
		Action** actionList;

		bool fStart;
	public:
		void start();
};
