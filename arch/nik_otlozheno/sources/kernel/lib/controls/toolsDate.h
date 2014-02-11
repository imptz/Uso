#ifndef TOOLS_DATE_H 
#define TOOLS_DATE_H

#include "button.h"
#include "label.h"
#include "../local.h"
#include "../clock/clock.h"

class ToolsDate : public Control{
private:
	static const char* NEW_DATE_MESSAGE_TEXT;

protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const char* ERROR_LABEL_TEXT;
	static const unsigned int BUTTON_WIDTH = 5;
	static const unsigned int BUTTON_HEIGHT = 3;
	static const char* CLEAR_BUTTON_TEXT;
	static const char* EXIT_BUTTON_TEXT;

	Label* errorLabel;
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
	Button* exitButton;
	Clock::DateTime dt;
	unsigned int blinkPosition;
	
	void printDateTime(unsigned int x, unsigned int y);
	void set(unsigned int value);
	void setNewDateTime();
	void clearDateTime();
	bool test();
	bool isIntercalaryYear(int value);
	int getDayCountInMonth(int month, int year);
	ToolsDate();

public:
	ToolsDate(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsDate();
	virtual void draw();
	virtual void onMessage(Message message);
};

#endif