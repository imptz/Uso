#ifndef TOOLS_OFF_H 
#define TOOLS_OFF_H

#include "Button.h"
#include "Label.h"
#include "../Local.h"

class ToolsOff : public Control
{
	protected:
		static const int WIDTH = 78;
		static const int HEIGHT = 9;
		static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

		static const unsigned int BUTTON_WIDTH = 18;
		static const unsigned int BUTTON_HEIGHT = 3;
		Button* offButton;
		static const char* OFF_BUTTON_TEXT;
		Button* rebootButton;
		static const char* REBOOT_BUTTON_TEXT;
		Button* exitButton;
		static const char* EXIT_BUTTON_TEXT;

		static const char* OFF_LABEL_TEXT;
	public:
		static const char* REBOOT_LABEL_TEXT;

	public:
		void reboot();
		static char* TOOLS_OFF_MESSAGE_REBOOT;

	protected:
		void poweroff();
		static char* TOOLS_OFF_MESSAGE_OFF;

		ToolsOff();

	public:
		ToolsOff(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
		virtual ~ToolsOff();
		virtual void draw();
		virtual void onMessage(Message message);

		enum TOOLS_OFF_MESSAGE
		{
			TOOLS_OFF_MESSAGE_EXIT = 18
		};

		enum TOOLS_OFF_ACTOR
		{
			TOOLS_OFF_ACTOR_USER = 0,
			TOOLS_OFF_ACTOR_SYSTEM = 1
		};


};

#endif