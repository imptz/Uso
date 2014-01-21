#pragma once

#include "button.h"
#include "tabControl.h"
#include "toolsUpdatePC.h"
#include "toolsUpdateFR.h"

class ToolsUpdate : public Control{
protected:
	TabControl* tab;
	static const unsigned int TAB_COUNT = 3;
	static const unsigned int MAIN_TAB = 0;
	static const unsigned int PC_TAB = 1;
	static const unsigned int FR_TAB = 2;

	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

	static const unsigned int BUTTON_WIDTH = 26;
	static const unsigned int BUTTON_HEIGHT = 3;

	Button* exitButton;
	Button* pcButton;
	Button* frButton; 

	static const char* EXIT_BUTTON_TEXT;
	static const char* PC_BUTTON_TEXT;
	static const char* FR_BUTTON_TEXT;

	ToolsUpdatePC* toolsUpdatePC;
	ToolsUpdateFR* toolsUpdateFR;

	ToolsUpdate();

public:
	ToolsUpdate(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsUpdate();
	virtual void draw();
	virtual void onMessage(Message message);
};
