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

	SerialDebug::getSingleton().addReceiver(this);
	Config::getSingleton().addReceiver(this);

	memset(Config::getSingleton().getConfigData(), 0x6c, sizeof(ConfigData));

	if(Config::getSingleton().writeConfig())
		Display::getSingleton().print("start writeConfigDataToHdd OK", 0, 2);
	else
		Display::getSingleton().print("start writeConfigDataToHdd FAILED", 0, 2);

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
				case MESSAGE_CONFIG_WRITE_COMPLETE:
					Display::getSingleton().print("complete writeConfigDataToHdd", 0, 3);
					switch (message.par1){
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_OK:
							Display::getSingleton().print(" OK", 31, 3);
							memset(Config::getSingleton().getConfigData(), 0x3b, sizeof(ConfigData));
							if(Config::getSingleton().readConfig())
								Display::getSingleton().print("start readConfigDataFromHdd OK", 0, 4);
							else
								Display::getSingleton().print("start readConfigDataFromHdd FAILED", 0, 4);
							break;
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_FAILED:
							Display::getSingleton().print(" FAILED", 31, 3);
							break;
					}
					break;
				case MESSAGE_CONFIG_READ_COMPLETE:
					Display::getSingleton().print("complete readConfigDataFromHdd", 0, 5);
					switch (message.par1){
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_OK:
							Display::getSingleton().print(" OK", 31, 5);
							Display::getSingleton().print(" verify  ", 39, 5);
							if(Config::getSingleton().getConfigData()->dataValid)
								Display::getSingleton().print(" OK  ", 48, 5);
							else
								Display::getSingleton().print(" FAILED  ", 48, 5);
							break;
						case Config::MESSAGE_CONFIG_HDD_COMPLETE_FAILED:
							Display::getSingleton().print(" FAILED", 31, 5);
							break;
					}
					break;
				case MESSAGE_CONFIG_UPDATE_COMPLETE:
					Display::getSingleton().print("                                ", 0, 15);
					Display::getSingleton().print("MESSAGE_CONFIG_UPDATE_COMPLETE ", 0, 16);
					switch (message.par1){
						case Config::MESSAGE_CONFIG_UPDATE_COMPLETE_OK:
							Display::getSingleton().print("OK", 31, 16);
							break;
						case Config::MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED:
							Display::getSingleton().print("FAILED ", 31, 16);
							Display::getSingleton().printUInt(message.par2, 38, 16);
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
							Display::getSingleton().print("                                                             ", 0, 16);
							Display::getSingleton().print("COMMAND_DEBUG_TEST_CONFIG_UPDATE", 0, 15);
							Config::getSingleton().update();
							break;
					}
			}
			break;
	}
}

void Application::timerHandler(){
}
