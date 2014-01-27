#include "control.h" 
#include "../log/log.h"

unsigned int Control::nextId = 0;

unsigned int Control::getNextId(){
	return ++nextId;
}

unsigned int Control::getId(){
	return id;
}

Control::Control()
	:	id(getNextId()), visible(true)
{
	controls = new Control*[MAX_CONTROLS_NUMBER];
	for (int i = 0; i < MAX_CONTROLS_NUMBER; ++i)
		controls[i] = nullptr;
}

Control::Control(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver)
	:	id(getNextId()), visible(true), blinking(false), positionX(_positionX), positionY(_positionY), width(_width), height(_height)
{
	controls = new Control*[MAX_CONTROLS_NUMBER];
	for (int i = 0; i < MAX_CONTROLS_NUMBER; ++i)
		controls[i] = nullptr;

	if (_messageReceiver != nullptr)
		addReceiver(_messageReceiver);
}

Control::~Control(){
	for (unsigned int i = 0; i < MAX_CONTROLS_NUMBER; ++i)
		if (controls[i] != nullptr)
			delete controls[i];
	delete[] controls;
}

int Control::addChildControl(Control* control){
	int index = -1;
	for (unsigned int i = 0; i < MAX_CONTROLS_NUMBER; i++)
		if (controls[i] == nullptr){
			index = i;
			break;
		}

	if (index == -1)
		return -1;

	controls[index] = control;
	return index;
}

Control* Control::getChildControl(int index){
	return controls[index];
}

void Control::drawChildControls(){
	if (visible){
		for (unsigned int i = 0; i < MAX_CONTROLS_NUMBER; ++i)
			if (controls[i] != nullptr)
				controls[i]->draw();
	}
}

bool Control::inControlCoords(unsigned int x, unsigned int y){
	if (
		(x > positionX * 8) &&
		(x < positionX * 8 + width * 8) &&
		(y > positionY * 16) &&
		(y < positionY * 16 + height * 16)
		)
		return true;
	else
		return false;
}

bool Control::getVisible(){
	return visible;
}

void Control::setVisible(bool _visible, bool childVisible){
	visible = _visible;
	messageEnable = _visible;

	if (childVisible)
		for (unsigned int i = 0; i < MAX_CONTROLS_NUMBER; ++i)
			if (controls[i] != nullptr)
				controls[i]->setVisible(_visible);
}

void Control::setBlinking(bool value){
	blinking = value;
}
