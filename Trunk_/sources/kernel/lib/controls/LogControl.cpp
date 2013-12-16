#include "LogControl.h"

#pragma warning (disable : 4355)

char* LogControl::UP_BUTTON_NAME = LOCAL_LOG_CONTROL_UP_BUTTON_NAME;
char* LogControl::DOWN_BUTTON_NAME = LOCAL_LOG_CONTROL_DOWN_BUTTON_NAME;
char* LogControl::LAST_BUTTON_NAME = LOCAL_LOG_CONTROL_LAST_BUTTON_NAME;

LogControl::LogControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, PANEL_WIDTH, PANEL_HEIGHT, _messageReceiver), 
	panel(new Panel(_positionX, _positionY, PANEL_WIDTH, PANEL_HEIGHT, PANEL_BORDER_STYLE)),
	list(new List(LIST_HEIGHT, LIST_WIDTH, _positionX + LIST_POSITION_X, _positionY + LIST_POSITION_Y)),
	upButton(new Button(_positionX + UP_BUTTON_START_POSITION_X, _positionY + UP_BUTTON_START_POSITION_Y, UP_BUTTON_WIDTH, UP_BUTTON_HEIGHT, UP_BUTTON_NAME, NAVIGATE_BUTTONS_BORDER_STYLE, this)),
	downButton(new Button(_positionX + DOWN_BUTTON_START_POSITION_X, _positionY + DOWN_BUTTON_START_POSITION_Y, DOWN_BUTTON_WIDTH, DOWN_BUTTON_HEIGHT, DOWN_BUTTON_NAME, NAVIGATE_BUTTONS_BORDER_STYLE, this)),
	lastButton(new Button(_positionX + LAST_BUTTON_START_POSITION_X, _positionY + LAST_BUTTON_START_POSITION_Y, LAST_BUTTON_WIDTH, LAST_BUTTON_HEIGHT, LAST_BUTTON_NAME, NAVIGATE_BUTTONS_BORDER_STYLE, this)),
	directView(true)
{ 
	addChildControl(panel);	
	panel->addChildControl(list);	
	panel->addChildControl(upButton);	
	panel->addChildControl(downButton);	
	panel->addChildControl(lastButton);	

	Log::getSingleton().addReceiver(this);

	onLast();
}

LogControl::~LogControl()
{
	delete panel;
}

void LogControl::draw()
{
	panel->setBlinking(blinking);
	drawChildControls();
}

void LogControl::onMessage(Message message)
{
	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + upButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_DOWN))
		onUp();
	else
		if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + downButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_DOWN))
			onDown();
		else
			if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + lastButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_DOWN))
				onLast();
			else
				if ((message.from == MESSAGE_FROM_OFFSET_LOG) && (message.msg == Log::LOG_MESSAGE_NEW_RECORD))
					onLast();
				else
					if ((message.from == MESSAGE_FROM_OFFSET_LOG) && (message.msg == Log::LOG_MESSAGE_CLEAR))
						onClear();
}

void LogControl::onUp()
{
	if (Log::getSingleton().getMessagesCount() > LIST_HEIGHT)
	{
		if (directView)
		{
			for (int i = 0; i < LIST_HEIGHT - 1; i++)
				Log::getSingleton().get(LOG_POSITION_PREV);
		}

		Log::getSingleton().get(LOG_POSITION_PREV).toString(logMessageString);
		list->addToTop(logMessageString, true);	
		directView = false;
	}
}

void LogControl::onDown()
{
	if (Log::getSingleton().getMessagesCount() > LIST_HEIGHT)
	{
		if (!directView)
		{
			for (int i = 0; i < LIST_HEIGHT - 1; i++)
				Log::getSingleton().get(LOG_POSITION_NEXT);
		}

		Log::getSingleton().get(LOG_POSITION_NEXT).toString(logMessageString);
		list->addToEnd(logMessageString, true);
		directView = true;
	}
}

void LogControl::onLast()
{
	Log::getSingleton().get(LOG_POSITION_LAST).toString(logMessageString);
	int count = Log::getSingleton().getMessagesCount();
	
	if (count > LIST_HEIGHT)
		count = LIST_HEIGHT;

	for (int i = count; i > 0; i--)
	{
		list->add(logMessageString, i - 1, false);
		Log::getSingleton().get(LOG_POSITION_PREV).toString(logMessageString);
	}
	Log::getSingleton().get(LOG_POSITION_NEXT).toString(logMessageString);

	list->draw();

	directView = false;
}

void LogControl::onClear()
{
	list->clear();
	onLast();
}
