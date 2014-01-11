#include "Application.h"

#include "touchpad\touchpad.h"
#include "message\Messages.h"
#include "DEBUG\serialDebug.h"
#include "Local.h"
#include "process\process.h"

Application::Application()
	:	ITimer(TIMER_PERIOD)
{
	pTimer->start();
}

Application::~Application(){
}

void Application::start(){
	Display::getSingleton().print(LOCAL_MESSAGE_TEXT_INITIALIZATION, 33, 7, true);

	

	for (;;){
		MessageReceiver::messagesProccess();
		Process::getSingleton().step();
	}
}

void Application::onMessage(Message message){
}

void Application::timerHandler(){
}
