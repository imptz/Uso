#ifndef MAIN_FINISH_H 
#define MAIN_FINISH_H

#include "button.h"
#include "label.h"
#include "../local.h"

class MainFinish : public Control{
protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const unsigned int BUTTON_WIDTH = 50;
	static const unsigned int BUTTON_HEIGHT = 3;
	static const char* FINISH_BUTTON_TEXT;
	static const char* FINISH_LABEL_TEXT;
	static const char* START_LABEL_TEXT;

	Button* finishButton;
	Label* finishLabel;
	Label* startLabel;

	MainFinish();

public:
	MainFinish(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~MainFinish();
	virtual void draw();
	virtual void onMessage(Message message);

	enum FINISH_MESSAGE_PARAM{
		FINISH_MESSAGE_PARAM_START, 
		FINISH_MESSAGE_PARAM_FINISH
	};

	enum FINISH_RESULT{
		FINISH_RESULT_FINISH = 0
	};

	void disableFinishMessage();
};

#endif