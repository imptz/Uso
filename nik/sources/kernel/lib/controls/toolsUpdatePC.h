#ifndef TOOLS_UPDATE_PC_H 
#define TOOLS_UPDATE_PC_H

#include "button.h"
#include "../local.h"
#include "../config/config.h"

class ToolsUpdatePC : public Control, public ITimer{
protected:
    static const unsigned int TIMER_PERIOD = 100;
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const unsigned int BUTTON_WIDTH = 26;
	static const unsigned int BUTTON_HEIGHT = 3;
	static const char* EXIT_BUTTON_TEXT;
	static const char* LOAD_DATA_TEXT;
	static const char* PROCESS_TEXT;
	static const char* FINISH_TEXT;
	static const char* ERROR_TEXT;

	Button* exitButton;
	int updatePhase;

	static char* TOOLS_UPDATE_PC_MESSAGE_FINISH;

	virtual void timerHandler();
	void reboot();
	ToolsUpdatePC();

	unsigned int errorCode;

public:
	ToolsUpdatePC(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsUpdatePC();
	virtual void draw();
	virtual void onMessage(Message message);
	void startUpdate();
};

#endif