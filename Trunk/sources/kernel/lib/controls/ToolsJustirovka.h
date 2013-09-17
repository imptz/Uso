#pragma once

#include "Button.h"
#include "Label.h"

class ToolsJustirovka : public Control
{
	protected:
		static const int WIDTH = 78;
		static const int HEIGHT = 9;
		static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

		Label* label;
		static const char* ERROR_LABEL_TEXT;
		static const char* SUCCESSFUL_LABEL_TEXT;
		static const char* ACTION_LABEL_TEXT;

		static const char* LOG_MESSAGE_TEXT;

		static const unsigned int BUTTON_WIDTH = 5;
		static const unsigned int BUTTON_HEIGHT = 3;
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
		static const char* CLEAR_BUTTON_TEXT;

		Button* actionButton;
		static const char* ACTION_BUTTON_TEXT;

		Button* exitButton;
		static const char* EXIT_BUTTON_TEXT;

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

		enum TOOLS_JUSTIROVKA_MESSAGE
		{
			TOOLS_JUSTIROVKA_MESSAGE_EXIT = 44
		};

	private:
		static const char* NEW_JUSTIROVKA_MESSAGE_TEXT;

		bool proccess;
		void exit();

		enum PHASE
		{
			PHASE_STOP,

		};
		PHASE phase;
};
