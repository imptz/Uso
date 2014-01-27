#ifndef LOG_CONTROL_H 
#define LOG_CONTROL_H

#include "Panel.h"
#include "List.h"
#include "Button.h"
#include "../log/Log.h"
#include "../Local.h"

class LogControl : public Control
{
	protected:
		Panel *panel;
		List *list;

		Button *upButton;
		Button *downButton;
		Button *lastButton;

		static const int PANEL_WIDTH = 80;
		static const int PANEL_HEIGHT = 11;
		static const Window::BORDER_STYLE PANEL_BORDER_STYLE = Window::BORDER_STYLE_DOUBLE;

		static const int LIST_POSITION_X = 1;
		static const int LIST_POSITION_Y = 1;
		static const int LIST_WIDTH = 66;
		static const int LIST_HEIGHT = 9;

		static const Window::BORDER_STYLE NAVIGATE_BUTTONS_BORDER_STYLE = Window::BORDER_STYLE_SINGLE;

		static const int UP_BUTTON_START_POSITION_X = 68;
		static const int UP_BUTTON_START_POSITION_Y = 1;
		static const int UP_BUTTON_WIDTH = 11;
		static const int UP_BUTTON_HEIGHT = 3;
		static char* UP_BUTTON_NAME;

		static const int DOWN_BUTTON_START_POSITION_X = 68;
		static const int DOWN_BUTTON_START_POSITION_Y = 4;
		static const int DOWN_BUTTON_WIDTH = 11;
		static const int DOWN_BUTTON_HEIGHT = 3;
		static char* DOWN_BUTTON_NAME;

		static const int LAST_BUTTON_START_POSITION_X = 68;
		static const int LAST_BUTTON_START_POSITION_Y = 7;
		static const int LAST_BUTTON_WIDTH = 11;
		static const int LAST_BUTTON_HEIGHT = 3;
		static char* LAST_BUTTON_NAME;

		void onUp();
		void onDown();
		void onLast();
		void onClear();

		LogControl();

		char logMessageString[67];

		bool directView;

	public:
		LogControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
		virtual ~LogControl();
		virtual void draw();
		virtual void onMessage(Message message);
};

#endif