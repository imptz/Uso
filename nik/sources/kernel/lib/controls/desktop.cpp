#include "desktop.h" 

Desktop::Desktop(){}

Desktop::Desktop(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, _width, _height, _messageReceiver)
{}

Desktop::~Desktop(){}

void Desktop::draw(){
	Display::getSingleton().clearScreen();
	drawChildControls();
}

void Desktop::onMessage(Message message){}
