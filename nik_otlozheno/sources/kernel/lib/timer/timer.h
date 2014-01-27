#pragma once

#include "../singleton.h"

class ITimer;
class TimerManager;

class Timer{
	friend class TimerManager;
private:
	unsigned int index;
	long period;
	long counter;
	bool enable;
	ITimer *client;

	Timer();
	Timer(unsigned int _index, long _period, ITimer *_client);
	Timer(const Timer& copy);
	Timer& operator =(const Timer &a);

public:
	~Timer();
	void start();
	void stop();
	bool isStart();
	unsigned int getIndex();
};

class TimerManager : public Singleton<TimerManager>{
private:
	static const unsigned short PIT_CONTROL_WORD;
	static const unsigned short PIT_COUNTER_0;
	static const unsigned short PIT_DIVIDER;

	static const unsigned int MAX_TIMERS_COUNT = 300;
	static Timer* timers[MAX_TIMERS_COUNT];

	static void staticIrqHandler();

public:
	TimerManager();
	Timer* createTimer(long period, ITimer *_client);
	void deleteTimer(unsigned int index);

private:
	int timersCount;
public:
	int getTimersCount();
};

class ITimer{
public:
	Timer *pTimer;
	ITimer(long period);
	virtual ~ITimer();
	virtual void timerHandler() = 0;
};
