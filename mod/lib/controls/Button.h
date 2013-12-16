#ifndef BUTTON_H 
#define BUTTON_H

#include "Control.h"
#include "../timer/Timer.h"
#include "../string.h"

class Button : public Control, public ITimer
{
	private:
        static const unsigned int TIMER_PERIOD = 100;
        virtual void timerHandler();

		char* name;
        enum BUTTON_STATE
        {
            BUTTON_STATE_UP,
            BUTTON_STATE_DOWN
        };
		BUTTON_STATE state;

	public:
		BUTTON_STATE getState();

	private:
		int downCounter;
		static const int DOWN_COUNTER_MAX = 5;

		Window::BORDER_STYLE borderStyle;

	public:
		Button();
		Button(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, char* _name, Window::BORDER_STYLE _borderStyle, MessageReceiver* _messageReceiver = nullptr);
		virtual ~Button();
		virtual void draw();
		virtual void onMessage(Message message);

		enum BUTTON_MESSAGE
		{
			BUTTON_MESSAGE_UP = 7,
			BUTTON_MESSAGE_DOWN = 8,
			BUTTON_MESSAGE_PRESS = 9,
			BUTTON_MESSAGE_HOLD = 10
		};

		void setName(char* _name, bool redraw = false);

	private:
		bool hold;
};

#endif