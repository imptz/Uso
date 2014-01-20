#ifndef TOOLS_OFF_H 
#define TOOLS_OFF_H

#include "button.h"
#include "label.h"
#include "../local.h"

class ToolsOff : public Control{
protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const unsigned int BUTTON_WIDTH = 18;
	static const unsigned int BUTTON_HEIGHT = 3;

	static const char* OFF_BUTTON_TEXT;
	static const char* REBOOT_BUTTON_TEXT;
	static const char* EXIT_BUTTON_TEXT;
	static const char* OFF_LABEL_TEXT;
	static char* TOOLS_OFF_MESSAGE_OFF;

	Button* offButton;
	Button* rebootButton;
	Button* exitButton;

	void poweroff();
	ToolsOff();

public:
	static const char* REBOOT_LABEL_TEXT;
	static char* TOOLS_OFF_MESSAGE_REBOOT;

	void reboot();
	ToolsOff(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsOff();
	virtual void draw();
	virtual void onMessage(Message message);

	enum TOOLS_OFF_MESSAGE{
		TOOLS_OFF_MESSAGE_EXIT = 18
	};

	enum TOOLS_OFF_ACTOR{
		TOOLS_OFF_ACTOR_USER = 0,
		TOOLS_OFF_ACTOR_SYSTEM = 1
	};
};

#endif