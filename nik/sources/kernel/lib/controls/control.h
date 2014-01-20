#ifndef CONTROL_H 
#define CONTROL_H

#include "../message/messages.h"
#include "../display/display.h"
#include "window.h"


class Control : public MessageReceiver, public MessageSender{
private:
	static unsigned int nextId;

protected:
	unsigned int id;
	static const int MAX_CONTROLS_NUMBER = 32;
	Control** controls;
	bool visible;
	bool blinking;

	unsigned int getNextId();
	virtual void drawChildControls();

public:
	unsigned int positionX;
	unsigned int positionY;
	unsigned int width;
	unsigned int height;

	Control();
	Control(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver = nullptr);
	virtual ~Control();
	bool inControlCoords(unsigned int x, unsigned int y);
	unsigned int getId();
	virtual void draw() = 0;
	int addChildControl(Control* control);
	Control* getChildControl(int index);
	bool getVisible();
	virtual void setVisible(bool _visible, bool childVisible = true);
	void setBlinking(bool value);
};

#endif