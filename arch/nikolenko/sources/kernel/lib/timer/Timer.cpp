#include "Timer.h"
#include "../low.h"
#include "../interrupt/interrupt.h"

Timer::Timer(unsigned int _index, long _period, ITimer *_client)
	: index(_index), period(_period), counter(0), enable(false), client(_client)
{
}

Timer::~Timer()
{
	TimerManager::getSingleton().deleteTimer(index);
}

void Timer::start()
{
	enable = true;
}

void Timer::stop()
{
	enable = false;
	counter = 0;
}

bool Timer::isStart()
{
	return enable;
}

//================================================================================

Timer* TimerManager::timers[TimerManager::MAX_TIMERS_COUNT];

const unsigned short TimerManager::PIT_CONTROL_WORD = 0x0043;
const unsigned short TimerManager::PIT_COUNTER_0 = 0x0040;
const unsigned short TimerManager::PIT_DIVIDER = 1193;

TimerManager::TimerManager()
	:	timersCount(0)
{
	for (unsigned int i = 0; i < MAX_TIMERS_COUNT; i++)
		timers[i] = nullptr;

	outPort(PIT_CONTROL_WORD, 0x36); 
#pragma warning (disable : 4305 4309)
	outPort(PIT_COUNTER_0, PIT_DIVIDER);
	outPort(PIT_COUNTER_0, PIT_DIVIDER >> 8);
#pragma warning (default : 4305 4309)

	SET_INT_HANDLER(Interrupts::HARDWARE_VECTOR_OFFSET + 0, TimerManager::staticIrqHandler);
	Interrupts::enablingHardwareInterrupt(0);
}

_declspec(naked) void TimerManager::staticIrqHandler()
{
 	_asm cli
	_asm pushfd
	_asm pushad

	for (unsigned int i = 0; i < MAX_TIMERS_COUNT; i++)
	{
		if (timers[i] != nullptr)
		{
			if (timers[i]->enable)
				if (++timers[i]->counter == timers[i]->period)
				{
					timers[i]->client->timerHandler();
					timers[i]->counter = 0;
				}
		}
	}

	_asm mov	al,0x20
	_asm out	0x20,al
	_asm popad
	_asm popfd
	_asm sti
	_asm iretd
}

Timer* TimerManager::createTimer(long _period, ITimer *_client)
{
	for (unsigned int i = 0; i < MAX_TIMERS_COUNT; i++)
		if (timers[i] == nullptr)
		{
			timersCount++;
			timers[i] = new Timer(i, _period, _client);
			return timers[i];
		}

	return nullptr;
}

void TimerManager::deleteTimer(unsigned int index)
{
	if (index < MAX_TIMERS_COUNT) 
	{
		timersCount--;
		timers[index] = nullptr;
		//TODO объект таймера не удаляется - утечка памяти
	}
}

int TimerManager::getTimersCount()
{
	return timersCount;
}

//================================================================================

ITimer::ITimer(long period)
{
	pTimer = TimerManager::getSingleton().createTimer(period, this);		
}

ITimer::~ITimer()
{
	delete pTimer;
}
