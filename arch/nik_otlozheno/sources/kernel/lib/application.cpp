#include "application.h"

#include "touchpad\touchpad.h"
#include "message\messages.h"
#include "debug\serialDebug.h"
#include "local.h"
#include "process\process.h"
#include "config\config.h"
#include "message\messages.h"
#include "controls\ui.h"
#include "logic\logic.h"

Application::Application()
	:	ITimer(TIMER_PERIOD)
{
	addReceiver(UI::getSingleton().getMainTabControl());
	UI::getSingleton().getMainTabControl()->addReceiver(this);

	pTimer->start();
}

Application::~Application(){
}

void Application::start(){
	SerialDebug::getSingleton().addReceiver(this);
	Config::getSingleton().addReceiver(this);

	while(!Config::getSingleton().readConfig()){
		Display::getSingleton().print("Config::getSingleton().readConfig() == false", 1, 9);
	}

	Display::getSingleton().print("Config::getSingleton().readConfig() == true", 30, 9);

	for(;;){
		MessageReceiver::messagesProccess();
		Process::getSingleton().step();
	}
}

void Application::up(){
	Logic::getSingleton().up();
}

void Application::onMessage(Message message){
	bool fTest;
	ConfigData* pcd;
	switch(message.from){
		case MESSAGE_FROM_OFFSET_CONFIG:
			switch(message.msg){
				case MESSAGE_CONFIG_READ_COMPLETE:
					Display::getSingleton().print("complete readConfigDataFromHdd", 1, 10);
					switch (message.par1){
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_OK:
							Display::getSingleton().print(" OK", 31, 10);
							break;
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_FAILED:
							Display::getSingleton().print(" FAILED", 31, 10);
							break;
					}
					up();
					break;
		//		case MESSAGE_CONFIG_UPDATE_COMPLETE:
		//			Display::getSingleton().print("                                ", 0, 23);
		//			Display::getSingleton().print("MESSAGE_CONFIG_UPDATE_COMPLETE ", 0, 24);
		//			switch (message.par1){
		//				case Config::MESSAGE_CONFIG_UPDATE_COMPLETE_OK:
		//					Display::getSingleton().print("OK", 31, 24);
		//					break;
		//				case Config::MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED:
		//					Display::getSingleton().print("FAILED ", 31, 24);
		//					Display::getSingleton().printUInt(message.par2, 38, 24);
		//					break;
		//			}
		//			break;
			}
			break;
	}
}

void Application::timerHandler(){
}