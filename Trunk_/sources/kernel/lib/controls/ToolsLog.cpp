#include "ToolsLog.h"
#include "../log/Log.h"

const char* ToolsLog::CLEAR_LABEL_TEXT = LOCAL_TOOLS_LOG_CLEAR_LABEL_TEXT;
const char* ToolsLog::CLEAR_BUTTON_TEXT = LOCAL_TOOLS_LOG_CLEAR_BUTTON_TEXT;
const char* ToolsLog::EXIT_BUTTON_TEXT = LOCAL_TOOLS_LOG_EXIT_BUTTON_TEXT;

ToolsLog::ToolsLog(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver)
{ 
	exitButton = new Button(_positionX + 67, _positionY + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	clearButton = new Button(_positionX + BUTTON_WIDTH, _positionY + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(CLEAR_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	statusLabel = new Label(_positionX + 19, _positionY + 1, 40, 1);
	statusLabel->setText("");
	addChildControl(exitButton);
	addChildControl(clearButton);
	addChildControl(statusLabel);
}

ToolsLog::~ToolsLog()
{

}

void ToolsLog::draw()
{
	drawChildControls();
}

void ToolsLog::onMessage(Message message)
{
	if ((message.from == exitButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
	{
		statusLabel->setText("", true);
		sendMessage(Message(id, MESSAGE_FROM_OFFSET_CONTROLS + TOOLS_LOG_MESSAGE_EXIT, 0, 0));
	}

	if ((message.from == clearButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
	{
		Log::getSingleton().clear();
		statusLabel->setBlinking(true);
		statusLabel->setText(const_cast<char*>(CLEAR_LABEL_TEXT), true);
	}
}
