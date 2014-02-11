#ifndef DESKTOP_H 
#define DESKTOP_H

#include "Control.h"

class Desktop : public Control
{
	public:
		Desktop();
		Desktop(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver = nullptr);
		virtual ~Desktop();
		virtual void draw();
		virtual void onMessage(Message message);
};

#endif