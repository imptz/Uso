#include "ToolsOff.h"
#include "../log/Log.h"
#include "../low.h"

const char* ToolsOff::OFF_LABEL_TEXT = LOCAL_TOOLS_OFF_OFF_LABEL_TEXT;
const char* ToolsOff::REBOOT_LABEL_TEXT = LOCAL_TOOLS_OFF_REBOOT_LABEL_TEXT;
const char* ToolsOff::OFF_BUTTON_TEXT = LOCAL_TOOLS_OFF_OFF_BUTTON_TEXT;
const char* ToolsOff::REBOOT_BUTTON_TEXT = LOCAL_TOOLS_OFF_REBOOT_BUTTON_TEXT;
const char* ToolsOff::EXIT_BUTTON_TEXT = LOCAL_TOOLS_OFF_EXIT_BUTTON_TEXT;

char* ToolsOff::TOOLS_OFF_MESSAGE_OFF = LOCAL_TOOLS_OFF_MESSAGE_OFF_TEXT;
char* ToolsOff::TOOLS_OFF_MESSAGE_REBOOT = LOCAL_TOOLS_OFF_MESSAGE_REBOOT_TEXT;

ToolsOff::ToolsOff(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver)
{ 
	exitButton = new Button(_positionX + 67, _positionY + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	offButton = new Button(_positionX + BUTTON_WIDTH, _positionY + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(OFF_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	rebootButton = new Button(_positionX + BUTTON_WIDTH * 2, _positionY + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(REBOOT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	addChildControl(exitButton);
	addChildControl(offButton);
	addChildControl(rebootButton);
}

ToolsOff::~ToolsOff()
{

}

void ToolsOff::draw()
{
	drawChildControls();
}

void ToolsOff::onMessage(Message message)
{
	if ((message.from == exitButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		sendMessage(Message(id, MESSAGE_FROM_OFFSET_CONTROLS + TOOLS_OFF_MESSAGE_EXIT, 0, 0));

	if ((message.from == offButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		poweroff();
	if ((message.from == rebootButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		reboot();
}

void ToolsOff::reboot()
{
	Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, TOOLS_OFF_MESSAGE_REBOOT, TOOLS_OFF_ACTOR_USER, 0);
	Log::getSingleton().setShutdown();
	_asm cli
	Display::getSingleton().print(REBOOT_LABEL_TEXT, positionX + 25, positionY + 1, true);
	
	outPort(0x70, 0x8d); 
	inPort(0x71);
	while ((inPort(0x64) & 0x02) != 0){}
	outPort(0x64, 0xfe); 
}

void ToolsOff::poweroff()
{
	Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, TOOLS_OFF_MESSAGE_OFF, 0, 0);
	Log::getSingleton().setShutdown();
	_asm cli
	Display::getSingleton().print(OFF_LABEL_TEXT, positionX + 25, positionY + 1, true);
	for (;;);
}
