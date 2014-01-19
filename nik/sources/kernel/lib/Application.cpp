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
	SerialDebug::getSingleton().addReceiver(this);
	Config::getSingleton().addReceiver(this);

	Config::getSingleton().readConfig();

	for(;;){
		MessageReceiver::messagesProccess();
		Process::getSingleton().step();
	}
}

void Application::onMessage(Message message){
	bool fTest;
	ConfigData* pcd;
	switch(message.from){
		case MESSAGE_FROM_OFFSET_CONFIG:
			switch(message.msg){
				case MESSAGE_CONFIG_READ_COMPLETE:
					Display::getSingleton().print("complete readConfigDataFromHdd", 0, 22);
					switch (message.par1){
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_OK:
							Display::getSingleton().print(" OK", 31, 22);
							break;
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_FAILED:
							Display::getSingleton().print(" FAILED", 31, 22);
							break;
					}
					break;
				case MESSAGE_CONFIG_UPDATE_COMPLETE:
					Display::getSingleton().print("                                ", 0, 23);
					Display::getSingleton().print("MESSAGE_CONFIG_UPDATE_COMPLETE ", 0, 24);
					switch (message.par1){
						case Config::MESSAGE_CONFIG_UPDATE_COMPLETE_OK:
							Display::getSingleton().print("OK", 31, 24);
							break;
						case Config::MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED:
							Display::getSingleton().print("FAILED ", 31, 24);
							Display::getSingleton().printUInt(message.par2, 38, 24);
							break;
					}
					break;
			}
			break;
		case MESSAGE_FROM_OFFSET_SERIAL_DEBUG:
			switch (message.msg){
				case SerialDebug::COMMAND_DEBUG_TEST_CONFIG_UPDATE:
					switch (message.par1){
						case 0:
							Config::getSingleton().cancelUpdate();
							break;
						case 1:
							Display::getSingleton().print("                                                             ", 0, 24);
							Display::getSingleton().print("COMMAND_DEBUG_TEST_CONFIG_UPDATE", 0, 23);
							Config::getSingleton().update();
							break;
					}
			}
			break;
	}
}

void Application::timerHandler(){
}
