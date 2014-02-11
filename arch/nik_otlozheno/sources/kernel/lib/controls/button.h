#ifndef BUTTON_H 
#define BUTTON_H

#include "control.h"
#include "../timer/timer.h"
#include "../string.h"

class Button : public Control, public ITimer{
private:
    static const unsigned int TIMER_PERIOD = 100;
    virtual void timerHandler();

	char* name;

	enum BUTTON_STATE{
        BUTTON_STATE_UP,
        BUTTON_STATE_DOWN
    };
	BUTTON_STATE state;

	int downCounter;
	static const int DOWN_COUNTER_MAX = 5;

	Window::BORDER_STYLE borderStyle;

	bool hold;

public:
	Button();
	Button(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, char* _name, Window::BORDER_STYLE _borderStyle, MessageReceiver* _messageReceiver = nullptr);
	virtual ~Button();

	BUTTON_STATE getState();
	virtual void draw();
	virtual void onMessage(Message message);
	void setName(char* _name, bool redraw = false);
};

#endif