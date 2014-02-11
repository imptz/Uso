#ifndef CONTROL_H 
#define CONTROL_H

#include "../message/Messages.h"
#include "../display/display.h"
#include "Window.h"


class Control : public MessageReceiver, public MessageSender
{
	private:
		static unsigned int nextId;
	protected:
		unsigned int getNextId();
	protected:
		unsigned int id;
		static const int MAX_CONTROLS_NUMBER = 32;
		Control** controls;

	public:
		unsigned int positionX;
		unsigned int positionY;
		unsigned int width;
		unsigned int height;

		bool inControlCoords(unsigned int x, unsigned int y);

	protected:
		bool visible;

	public:
		unsigned int getId();
		Control();
		Control(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver = nullptr);
		virtual ~Control();
		virtual void draw() = 0;
	protected:
		virtual void drawChildControls();
	public:
		int addChildControl(Control* control);
		Control* getChildControl(int index);

		bool getVisible();
		virtual void setVisible(bool _visible, bool childVisible = true);
	protected:
		bool blinking;
	public:
		void setBlinking(bool value);
};

#endif