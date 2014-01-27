#pragma once

#include "timer\timer.h"
#include "message\messages.h"

class Application : public ITimer, public Singleton<Application>, public MessageReceiver, public MessageSender{
private:
	static const unsigned int TIMER_PERIOD = 10000;
	virtual void timerHandler();
	void createLogic();
	void up();

public:
	Application();
	~Application();
	void start();
	virtual void onMessage(Message message);
};

