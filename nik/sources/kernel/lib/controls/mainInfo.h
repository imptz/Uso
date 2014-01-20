#ifndef MAIN_INFO_H 
#define MAIN_INFO_H

#include "button.h"
#include "label.h"
#include "../local.h"

class MainInfo : public Control{
protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

	MainInfo();

public:
	MainInfo(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~MainInfo();
	virtual void draw();
	virtual void onMessage(Message message);
};

#endif