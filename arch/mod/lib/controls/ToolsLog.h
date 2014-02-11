#ifndef TOOLS_LOG_H 
#define TOOLS_LOG_H

#include "Button.h"
#include "Label.h"
#include "../Local.h"

class ToolsLog : public Control
{
	protected:
		static const int WIDTH = 78;
		static const int HEIGHT = 9;
		static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

		static const unsigned int BUTTON_WIDTH = 26;
		static const unsigned int BUTTON_HEIGHT = 3;
		Button* clearButton;
		static const char* CLEAR_BUTTON_TEXT;
		Button* exitButton;
		static const char* EXIT_BUTTON_TEXT;

		Label* statusLabel;
		static const char* CLEAR_LABEL_TEXT;

		ToolsLog();

	public:
		ToolsLog(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
		virtual ~ToolsLog();
		virtual void draw();
		virtual void onMessage(Message message);

		enum TOOLS_LOG_MESSAGE
		{
			TOOLS_LOG_MESSAGE_EXIT = 17
		};
};

#endif