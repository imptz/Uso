#ifndef TOOLS_LOG_H 
#define TOOLS_LOG_H

#include "button.h"
#include "label.h"
#include "../local.h"

class ToolsLog : public Control{
protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const unsigned int BUTTON_WIDTH = 26;
	static const unsigned int BUTTON_HEIGHT = 3;
	static const char* CLEAR_BUTTON_TEXT;
	static const char* EXIT_BUTTON_TEXT;
	static const char* CLEAR_LABEL_TEXT;

	Button* clearButton;
	Button* exitButton;
	Label* statusLabel;

	ToolsLog();

public:
	ToolsLog(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsLog();
	virtual void draw();
	virtual void onMessage(Message message);
};

#endif