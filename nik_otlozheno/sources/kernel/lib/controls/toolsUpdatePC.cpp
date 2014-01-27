#include "toolsUpdatePC.h"
#include "../string.h"
#include "../log/log.h"
#include "../local.h"
#include "toolsOff.h"
#include "../debug/serialDebug.h"

const char* ToolsUpdatePC::EXIT_BUTTON_TEXT = LOCAL_TOOLS_UPDATE_PC_EXIT_BUTTON_TEXT;
const char* ToolsUpdatePC::LOAD_DATA_TEXT = LOCAL_TOOLS_UPDATE_PC_LOAD_DATA_TEXT;
const char* ToolsUpdatePC::PROCESS_TEXT = LOCAL_TOOLS_UPDATE_PC_PROCESS_TEXT;
const char* ToolsUpdatePC::FINISH_TEXT = LOCAL_TOOLS_UPDATE_PC_FINISH_TEXT;
const char* ToolsUpdatePC::ERROR_TEXT = LOCAL_TOOLS_UPDATE_PC_ERROR_TEXT;

char* ToolsUpdatePC::TOOLS_UPDATE_PC_MESSAGE_FINISH = LOCAL_TOOLS_UPDATE_PC_MESSAGE_FINISH_TEXT;

ToolsUpdatePC::ToolsUpdatePC(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	ITimer(TIMER_PERIOD), 
	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver), 
	updatePhase(-1)
{ 
	exitButton = new Button(_positionX + 67, _positionY + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	addChildControl(exitButton);
	Config::getSingleton().addReceiver(this);
}

ToolsUpdatePC::~ToolsUpdatePC(){}

void ToolsUpdatePC::draw(){
	char str2Num[3];

	switch (updatePhase){
		case 0:
			Display::getSingleton().print(LOAD_DATA_TEXT, positionX + 25, positionY + 1);
			Display::getSingleton().print(Display::getSingleton().toString2Num(Config::getSingleton().getLoadProgress(), &str2Num[0]), 
				positionX + 25 + strlen(const_cast<char*>(LOAD_DATA_TEXT)) - 4, 
				positionY + 1);
			break;
		case 1:
			Display::getSingleton().print(PROCESS_TEXT, positionX + 25, positionY + 1);
			updatePhase = -2;
			break;
		case 2:
			Display::getSingleton().print(FINISH_TEXT, positionX + 16, positionY + 1);
			reboot();
			break;
		case 3:
			Display::getSingleton().print(ERROR_TEXT, positionX + 25, positionY + 1, true);
			Display::getSingleton().print(Display::getSingleton().toString2Num(errorCode, &str2Num[0]), positionX + 25 + strlen(const_cast<char*>(ERROR_TEXT)) - 3, positionY + 1, true);
			updatePhase = -1;
			break;
	}

	drawChildControls();
}

void ToolsUpdatePC::onMessage(Message message){
	if ((message.from == exitButton->getId()) && (message.msg == MESSAGE_BUTTON_UP)){
		pTimer->stop();
		Config::getSingleton().cancelUpdate();
		updatePhase = -1;
		sendMessage(Message(id, MESSAGE_FROM_OFFSET_CONTROLS + MESSAGE_TOOLS_UPDATE_PC_EXIT, 0, 0));
		SerialDebug::getSingleton().enableDebugInput();
	}

	if ((message.from == MESSAGE_FROM_OFFSET_CONFIG) && (message.msg == MESSAGE_CONFIG_UPDATE_START))
		updatePhase = 1;

	if ((message.from == MESSAGE_FROM_OFFSET_CONFIG) && (message.msg == MESSAGE_CONFIG_UPDATE_COMPLETE)){
		if(message.par1 == Config::MESSAGE_CONFIG_UPDATE_COMPLETE_OK){
			pTimer->stop();
			Display::getSingleton().print("                                                                      ", positionX + 1, positionY + 1);
			Log::getSingleton().add(LOG_MESSAGE_FROM_CONFIG, LOG_MESSAGE_TYPE_SYSTEM, TOOLS_UPDATE_PC_MESSAGE_FINISH, 0, 0);
			updatePhase = 2;
			draw();
			SerialDebug::getSingleton().enableDebugInput();
		}else{
			errorCode = message.par2;
			pTimer->stop();
			Display::getSingleton().print("                                                                      ", positionX + 1, positionY + 1);
			updatePhase = 3;
			draw();
			SerialDebug::getSingleton().enableDebugInput();//  нормально включить дебаггер
		}
	}
}

void ToolsUpdatePC::timerHandler(){
	draw();
}

void ToolsUpdatePC::startUpdate(){
	SerialDebug::getSingleton().disableDebugInput();
	Config::getSingleton().update();
	pTimer->start();
	updatePhase = 0;
}

void ToolsUpdatePC::reboot(){
	Log::getSingleton().add(LOG_MESSAGE_FROM_CONFIG, LOG_MESSAGE_TYPE_SYSTEM, ToolsOff::TOOLS_OFF_MESSAGE_REBOOT, ToolsOff::TOOLS_OFF_ACTOR_SYSTEM, 0);
	Log::getSingleton().setShutdown();
	_asm cli
	Display::getSingleton().print(ToolsOff::REBOOT_LABEL_TEXT, positionX + 25, positionY + 2, true);
	
	for (int i = 0; i < 40000000; i++)
		Display::getSingleton().print("", positionX + 1, positionY + 1);

	outPort(0x70, 0x8d); 
	inPort(0x71);
	while ((inPort(0x64) & 0x02) != 0){}
	outPort(0x64, 0xfe); 
}