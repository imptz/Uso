#include "tabControl.h" 
#include "../log/log.h"

TabControl::TabControl(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, unsigned int _tabCount, 
	Window::BORDER_STYLE _borderStyle, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, _width, _height, _messageReceiver), visibleControlIndex(0)
{
	if (_tabCount > MAX_CONTROLS_NUMBER)
		tabCount = MAX_CONTROLS_NUMBER;
	else
		tabCount = _tabCount;

	for (unsigned int i = 1; i < tabCount + 1; ++i)
		addChildControl(new Panel(_positionX, _positionY, _width, _height, _borderStyle));
}

TabControl::~TabControl(){}

void TabControl::draw(){
	if (getVisible()){
		for (unsigned int w = 0; w < width; ++w)
			for (unsigned int h = 0; h < height; ++h){
				Display::getSingleton().printChar(' ', positionX + w, positionY + h, Display::COLOR_WHITE);
			}

			drawChildControls();
	}
}

void TabControl::onMessage(Message message){}

Panel* TabControl::getTabPanel(int index){
	return static_cast<Panel*>(controls[index]);
}

void TabControl::setActiveTab(int index, bool redraw){
	for (unsigned int i = 0; i < tabCount; ++i){
		if (i != index)
			controls[i]->setVisible(false);
	}

	visibleControlIndex = index;
	controls[index]->setVisible(true);
	sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, TAB_CONTROL_MESSAGE_SET_ACTIVE, index, 0));

	if (redraw)
		draw();
}

void TabControl::setVisible(bool _visible, bool childVisible){
	visible = _visible;
	messageEnable = _visible;

	if (_visible)
		controls[visibleControlIndex]->setVisible(_visible);
	else
		for (unsigned int i = 0; i < tabCount; ++i)
			controls[i]->setVisible(false);
}
