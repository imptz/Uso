#include "toolsTabControl.h" 
#include "button.h"
#include "../local.h"
#include "../log/log.h"
#include "../debug/serialDebug.h"

//#include "../logic/povorotniy.h"

const char* ToolsTabControl::EXIT_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_EXIT_BUTTON_TEXT;
const char* ToolsTabControl::UPDATE_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_UPDATE_BUTTON_TEXT;
const char* ToolsTabControl::DATE_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_DATE_BUTTON_TEXT;
const char* ToolsTabControl::OFF_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_OFF_BUTTON_TEXT;
const char* ToolsTabControl::LOG_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_LOG_BUTTON_TEXT;
const char* ToolsTabControl::DETECTION_RESET_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_DETECTION_RESET_BUTTON_TEXT;
const char* ToolsTabControl::JUSTIROVKA_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_JUSTIROVKA_BUTTON_TEXT;
const char* ToolsTabControl::POVOROTNIY_BUTTON_TEXT = LOCAL_TOOLS_TAB_CONTROL_POVOROTNIY_BUTTON_TEXT;

ToolsTabControl::ToolsTabControl(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, _width, _height, _messageReceiver),
		tab(new TabControl(_positionX, _positionY, _width, _height, TAB_COUNT, Window::BORDER_STYLE_DOUBLE))
{
	addChildControl(tab);
	updateButton = new Button(_positionX + 1, _positionY + 1, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(UPDATE_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	dateButton = new Button(_positionX + 1 + BUTTON_WIDTH, _positionY + 1, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(DATE_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this); 
	offButton = new Button(_positionX + 1 + BUTTON_WIDTH * 2, _positionY + 1, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(OFF_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	logButton = new Button(_positionX + 1, _positionY + 1 + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(LOG_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	detectionResetButton = new Button(_positionX + 1 + BUTTON_WIDTH, _positionY + 1 + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(DETECTION_RESET_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	justirovkaButton = new Button(_positionX + 1 + BUTTON_WIDTH * 2, _positionY + 1 + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(JUSTIROVKA_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	povorotniyButton = new Button(_positionX + 1, _positionY + 1 + BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(POVOROTNIY_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);

	cancelButton = new Button(_positionX + 68, _positionY + 1 + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);

	tab->getTabPanel(MAIN_TAB)->addChildControl(cancelButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(updateButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(dateButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(offButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(logButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(detectionResetButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(justirovkaButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(povorotniyButton);
	
	toolsLog = new ToolsLog(_positionX + 1, _positionY + 1, this);
	toolsDate = new ToolsDate(_positionX + 1, _positionY + 1, this);
	toolsOff = new ToolsOff(_positionX + 1, _positionY + 1, this);
	toolsUpdate = new ToolsUpdate(_positionX, _positionY, width, height, this);
	toolsJustirovka = new ToolsJustirovka(_positionX + 1, _positionY + 1, this);

	tab->getTabPanel(LOG_TAB)->addChildControl(toolsLog);
	tab->getTabPanel(DATE_TAB)->addChildControl(toolsDate);
	tab->getTabPanel(OFF_TAB)->addChildControl(toolsOff);
	tab->getTabPanel(UPDATE_TAB)->addChildControl(toolsUpdate);
	tab->getTabPanel(JUSTIROVKA_TAB)->addChildControl(toolsJustirovka);

	tab->setActiveTab(MAIN_TAB);
}

ToolsTabControl::~ToolsTabControl(){}

void ToolsTabControl::draw(){
	tab->draw();
}

void ToolsTabControl::onMessage(Message message){
	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + cancelButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, TOOLS_TAB_CONTROL_MESSAGE_EXIT, 0, 0));

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + dateButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		tab->setActiveTab(DATE_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + logButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		tab->setActiveTab(LOG_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + offButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		tab->setActiveTab(OFF_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + updateButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		tab->setActiveTab(UPDATE_TAB);

//	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + detectionResetButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//		DetectionSubsystem::getSingleton().resetDiveces();

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + justirovkaButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		tab->setActiveTab(JUSTIROVKA_TAB);

//	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + povorotniyButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//		Povorotniy::getSingleton().start();

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsLog->getId()) && (message.msg == ToolsLog::TOOLS_LOG_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsDate->getId()) && (message.msg == ToolsDate::TOOLS_DATE_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsOff->getId()) && (message.msg == ToolsOff::TOOLS_OFF_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsUpdate->getId()) && (message.msg == ToolsUpdate::TOOLS_UPDATE_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsJustirovka->getId()) && (message.msg == ToolsJustirovka::TOOLS_JUSTIROVKA_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);
}
