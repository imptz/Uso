#include "toolsUpdateFR.h"
#include "../string.h"
#include "../log/log.h"
#include "../local.h"
#include "toolsOff.h"

const char* ToolsUpdateFR::EXIT_BUTTON_TEXT = LOCAL_TOOLS_UPDATE_FR_EXIT_BUTTON_TEXT;
const char* ToolsUpdateFR::LOAD_DATA_TEXT = LOCAL_TOOLS_UPDATE_FR_LOAD_DATA_TEXT;
const char* ToolsUpdateFR::PROCESS_TEXT = LOCAL_TOOLS_UPDATE_FR_PROCESS_TEXT;
const char* ToolsUpdateFR::FINISH_TEXT = LOCAL_TOOLS_UPDATE_FR_FINISH_TEXT;
const char* ToolsUpdateFR::ERROR_TEXT = LOCAL_TOOLS_UPDATE_FR_ERROR_TEXT;

char* ToolsUpdateFR::TOOLS_UPDATE_FR_MESSAGE_FINISH = LOCAL_TOOLS_UPDATE_FR_MESSAGE_FINISH_TEXT;

ToolsUpdateFR::ToolsUpdateFR(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	ITimer(TIMER_PERIOD), 
	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver), 
	updatePhase(-1)
{ 
	exitButton = new Button(_positionX + 67, _positionY + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	addChildControl(exitButton);
	Config::getSingleton().addReceiver(this);
}

ToolsUpdateFR::~ToolsUpdateFR(){}

void ToolsUpdateFR::draw(){
	char str2Num[3];

	//switch (updatePhase){
	//	case 0:
	//		Display::getSingleton().print(LOAD_DATA_TEXT, positionX + 25, positionY + 1);
	//		Display::getSingleton().print(Display::getSingleton().toString2Num(Config::getSingleton().getUpdateProgressFR(), &str2Num[0]), 
	//			positionX + 25 + strlen(const_cast<char*>(LOAD_DATA_TEXT)) - 4, 
	//			positionY + 1);
	//		break;
	//	case 1:
	//		Display::getSingleton().print(PROCESS_TEXT, positionX + 25, positionY + 1);
	//		updatePhase = -2;
	//		break;
	//	case 2:
	//		Display::getSingleton().print(FINISH_TEXT, positionX + 16, positionY + 1, true);
	//		updatePhase = -1;
	//		break;
	//}

	drawChildControls();
}

void ToolsUpdateFR::onMessage(Message message){
	char str2Num[3];

//	if ((message.from == exitButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP)){
//		pTimer->stop();
//		Config::getSingleton().stopUpdateFR();
//		updatePhase = -1;
//		sendMessage(Message(id, MESSAGE_FROM_OFFSET_CONTROLS + TOOLS_UPDATE_FR_MESSAGE_EXIT, 0, 0));
//	}
//
//	if ((message.from == MESSAGE_FROM_OFFSET_CONFIG) && (message.msg == Config::CONFIG_MESSAGE_START_DEVICES_UPDATE_FR))
//		updatePhase = 1;
//
//	if ((message.from == MESSAGE_FROM_OFFSET_CONFIG) && (message.msg == Config::CONFIG_MESSAGE_UPDATE_FINISH_FR)){
//		pTimer->stop();
//		Display::getSingleton().print("                                                                      ", positionX + 1, positionY + 1);
//		Log::getSingleton().add(LOG_MESSAGE_FROM_CONFIG, LOG_MESSAGE_TYPE_SYSTEM, TOOLS_UPDATE_FR_MESSAGE_FINISH, 0, 0);
//		updatePhase = 2;
//		draw();
//	}
//
//	if ((message.from == MESSAGE_FROM_OFFSET_CONFIG) && (message.msg == Config::CONFIG_MESSAGE_UPDATE_ERROR_FR)){
//		Display::getSingleton().print("                                                                      ", positionX + 1, positionY + 1);
//		Display::getSingleton().print(ERROR_TEXT, positionX + 25, positionY + 1, true);
//		Display::getSingleton().print(Display::getSingleton().toString2Num(message.par2, &str2Num[0]), positionX + 53, positionY + 1, true);
////		Display::getSingleton().print(Display::getSingleton().toString2Num(message.par1, &str2Num[0]), positionX + 56, positionY + 1, true);
//
//		updatePhase = -1;
//	}
}

void ToolsUpdateFR::timerHandler(){
	draw();
}

void ToolsUpdateFR::startUpdate(){
//	Config::getSingleton().startUpdateFR();
//	pTimer->start();
//	updatePhase = 0;
}
