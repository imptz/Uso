#ifndef PANEL_H 
#define PANEL_H

#include "control.h"

class Panel : public Control{
protected:
	Window::BORDER_STYLE borderStyle;

public:
	Panel(){}

	Panel(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, Window::BORDER_STYLE _borderStyle, MessageReceiver* _messageReceiver = nullptr)
		:	Control(_positionX, _positionY, _width, _height, _messageReceiver), borderStyle(_borderStyle)
	{}

	virtual ~Panel(){}

	virtual void draw(){
		if (visible){
			Window::drawWindow(positionX, positionY, width, height, borderStyle, Display::COLOR_WHITE, blinking);
			drawChildControls();
		}
	}

	virtual void onMessage(Message message){}
};

#endif