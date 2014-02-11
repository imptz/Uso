#pragma once

#include "button.h"
#include "label.h"

class ToolsJustirovka : public Control{
private:
	static const char* NEW_JUSTIROVKA_MESSAGE_TEXT;

	enum PHASE{
		PHASE_STOP
	};

	bool proccess;
	PHASE phase;
	void exit();


protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const char* ERROR_LABEL_TEXT;
	static const char* SUCCESSFUL_LABEL_TEXT;
	static const char* ACTION_LABEL_TEXT;
	static const char* LOG_MESSAGE_TEXT;
	static const unsigned int BUTTON_WIDTH = 5;
	static const unsigned int BUTTON_HEIGHT = 3;
	static const char* CLEAR_BUTTON_TEXT;
	static const char* ACTION_BUTTON_TEXT;
	static const char* EXIT_BUTTON_TEXT;

	Label* label;
	Button* button0;
	Button* button1;
	Button* button2;
	Button* button3;
	Button* button4;
	Button* button5;
	Button* button6;
	Button* button7;
	Button* button8;
	Button* button9;
	Button* clearButton;
	Button* actionButton;
	Button* exitButton;

	unsigned char number;
	unsigned int blinkPosition;
	void printNumber(unsigned int x, unsigned int y);

	void set(unsigned int value);
	void clear();
	void start();
	void action();
	ToolsJustirovka();

public:
	ToolsJustirovka(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsJustirovka();
	virtual void draw();
	virtual void onMessage(Message message);
};
