#include "Button.h" 
#include "../touchpad/touchpad.h"

Button::Button()
	:	ITimer(TIMER_PERIOD)
{
}

Button::Button(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, char* _name, Window::BORDER_STYLE _borderStyle, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, _width, _height, _messageReceiver), ITimer(TIMER_PERIOD), state(BUTTON_STATE_UP), downCounter(0), borderStyle(_borderStyle), hold(false)
{
	if (width < 3)
		width = 3;
	if (height < 3)
		height = 3;

	name = new char[_width - 1];
	name[_width - 2] = 0;

	setName(_name, false);

	Touchpad::getSingleton().addReceiver(this);
	pTimer->start();
}

Button::~Button()
{
	delete[] name;
}

void Button::draw()
{
	if (visible)
	{
		switch (state)
		{
			case BUTTON_STATE_DOWN:
				Window::drawWindow(positionX, positionY, width, height, borderStyle, Display::COLOR_GRAY, blinking);
				Display::getSingleton().print(name, positionX + 1, positionY + height / 2);
				drawChildControls();
				break;
			default:
				Window::drawWindow(positionX, positionY, width, height, borderStyle, Display::COLOR_WHITE, blinking);
				Display::getSingleton().print(name, positionX + 1, positionY + height / 2);
				drawChildControls();
				break;
		}
	}
}

void Button::onMessage(Message message)
{
	if (visible)
	{
		switch (message.from)
		{
			case Touchpad::TOUCHPAD_MESSAGE_FROM:
				if (message.msg == Touchpad::TOUCHPAD_MESSAGE_PEN_DOWN)
				{
					if (inControlCoords(message.par1, message.par2))
					{
						downCounter = 1;
						state = BUTTON_STATE_DOWN;
						sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, BUTTON_MESSAGE_DOWN, 0, 0));
						draw();
					}
				}
				else
					if (message.msg == Touchpad::TOUCHPAD_MESSAGE_PEN_UP)
					{
						downCounter = 0;
						if (state == BUTTON_STATE_DOWN)
						{
							sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, BUTTON_MESSAGE_UP, 0, 0));
							hold = false;
						}

						state = BUTTON_STATE_UP;
						draw();
					}
				break;
		}
	}
}

void Button::timerHandler()
{
	if (visible)
	{
		if (downCounter != 0)
		{
			if (downCounter != DOWN_COUNTER_MAX)
				downCounter++;
			else
			{
				if (!hold)
				{
					hold = true;
					sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, BUTTON_MESSAGE_HOLD, 0, 0));
				}
				sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, BUTTON_MESSAGE_DOWN, 0, 0));
			}
		}
	}
	else
		downCounter = 0;
}

void Button::setName(char* _name, bool redraw)
{
	if (static_cast<unsigned int>(strlen(_name)) > (width - 2))
		_name[width - 2] = 0;
	memset(name, ' ', width - 2);
	memcpy(&name[((width - 2) / 2) - (strlen(_name) / 2)], _name, strlen(_name));

	if (redraw)
		draw();
}

Button::BUTTON_STATE Button::getState()
{
	return state;
}
