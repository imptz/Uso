#include "ToolsUpdate.h"
#include "../Local.h"

const char* ToolsUpdate::EXIT_BUTTON_TEXT = LOCAL_TOOLS_UPDATE_EXIT_BUTTON_TEXT;
const char* ToolsUpdate::PC_BUTTON_TEXT = LOCAL_TOOLS_UPDATE_PC_BUTTON_TEXT;
const char* ToolsUpdate::FR_BUTTON_TEXT = LOCAL_TOOLS_UPDATE_FR_BUTTON_TEXT;

ToolsUpdate::ToolsUpdate(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver),
	tab(new TabControl(_positionX, _positionY, _width, _height, TAB_COUNT, Window::BORDER_STYLE_DOUBLE))
{ 
	exitButton = new Button(_positionX + 68, _positionY + 1 + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	pcButton = new Button(_positionX + 5, _positionY + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(PC_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	frButton = new Button(_positionX + 5 + BUTTON_WIDTH + 2, _positionY + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(FR_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);

	tab->getTabPanel(MAIN_TAB)->addChildControl(exitButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(pcButton);
	tab->getTabPanel(MAIN_TAB)->addChildControl(frButton);

	toolsUpdatePC = new ToolsUpdatePC(_positionX + 1, _positionY + 1, this);
	toolsUpdateFR = new ToolsUpdateFR(_positionX + 1, _positionY + 1, this);

	tab->getTabPanel(PC_TAB)->addChildControl(toolsUpdatePC);
	tab->getTabPanel(FR_TAB)->addChildControl(toolsUpdateFR);

	addChildControl(tab);
	tab->setActiveTab(MAIN_TAB);
}

ToolsUpdate::~ToolsUpdate()
{

}

void ToolsUpdate::draw()
{
	drawChildControls();
}

void ToolsUpdate::onMessage(Message message)
{
	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + exitButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, TOOLS_UPDATE_MESSAGE_EXIT, 0, 0));

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + pcButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
	{
		toolsUpdatePC->startUpdate();
		tab->setActiveTab(PC_TAB);
	}

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + frButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
	{
		toolsUpdateFR->startUpdate();
		tab->setActiveTab(FR_TAB);
	}

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsUpdatePC->getId()) && (message.msg == ToolsUpdatePC::TOOLS_UPDATE_PC_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsUpdateFR->getId()) && (message.msg == ToolsUpdateFR::TOOLS_UPDATE_FR_MESSAGE_EXIT))
		tab->setActiveTab(MAIN_TAB);
}
