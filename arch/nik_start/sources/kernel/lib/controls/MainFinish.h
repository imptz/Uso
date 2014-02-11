#ifndef MAIN_FINISH_H 
#define MAIN_FINISH_H

#include "Button.h"
#include "Label.h"
#include "../Local.h"

class MainFinish : public Control
{
	protected:
		static const int WIDTH = 78;
		static const int HEIGHT = 9;
		static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

		static const unsigned int BUTTON_WIDTH = 50;
		static const unsigned int BUTTON_HEIGHT = 3;
		Button* finishButton;
		static const char* FINISH_BUTTON_TEXT;

		Label* finishLabel;
		static const char* FINISH_LABEL_TEXT;
		Label* startLabel;
		static const char* START_LABEL_TEXT;

	protected:
		MainFinish();

	public:
		MainFinish(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
		virtual ~MainFinish();
		virtual void draw();
		virtual void onMessage(Message message);

		enum FINISH_MESSAGE
		{
			FINISH_MESSAGE_RESULT = 13,
			FINISH_MESSAGE_LABEL = 39
		};

		enum FINISH_MESSAGE_PARAM
		{
			FINISH_MESSAGE_PARAM_START, 
			FINISH_MESSAGE_PARAM_FINISH
		};

		enum FINISH_RESULT
		{
			FINISH_RESULT_FINISH = 0
		};

		void disableFinishMessage();
};

#endif