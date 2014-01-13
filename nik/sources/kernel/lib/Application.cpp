#include "application.h"

#include "touchpad\touchpad.h"
#include "message\messages.h"
#include "debug\serialDebug.h"
#include "local.h"
#include "process\process.h"
#include "config\config.h"
#include "message\messages.h"

Application::Application()
	:	ITimer(TIMER_PERIOD)
{
	pTimer->start();
}

Application::~Application(){
}

void Application::start(){
	Display::getSingleton().print(LOCAL_MESSAGE_TEXT_INITIALIZATION, 33, 7, true);
	Display::getSingleton().printUInt(sizeof(ConfigData), 0, 0);

	Config::getSingleton().addReceiver(this);
	if(Config::getSingleton().readConfigDataFromHdd())
		Display::getSingleton().print("start readConfigDataFromHdd OK", 0, 2);
	else
		Display::getSingleton().print("start readConfigDataFromHdd FAILED", 0, 2);

	for(;;){
		MessageReceiver::messagesProccess();
		Process::getSingleton().step();
	}
}

void Application::onMessage(Message message){
	if (message.from == MESSAGE_FROM_OFFSET_CONFIG)
		if (message.msg == MESSAGE_CONFIG_READ_FROM_HDD_COMPLETE){
			Display::getSingleton().print("complete readConfigDataFromHdd", 0, 3);
			switch (message.par1){
				case Config::MESSAGE_CONFIG_HDD_COMPLETE_OK:
					Display::getSingleton().print(" OK", 31, 3);
					break;
				case Config::MESSAGE_CONFIG_HDD_COMPLETE_FAILED:
					Display::getSingleton().print(" FAILED", 31, 3);
					break;
			}
		}
}

void Application::timerHandler(){
}
