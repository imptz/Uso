#include "clockControl.h"
#include "../string.h"
#include "../clock/clock.h"

ClockControl::ClockControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, 19, 1, _messageReceiver)
{
	text = new char[20];
	memset(text, 0, 20);
	Clock::getSingleton().addReceiver(this);
}

ClockControl::~ClockControl(){
	Clock::getSingleton().removeReceiver(this);
	delete[] text;
}

void ClockControl::setDateTimeString(unsigned int mDate, unsigned int mTime){
	char st[20];
	memcpy(&text[0], Display::getSingleton().toStringDate(mDate, &st[0]), 8);
	text[8] = ' ';
	memcpy(&text[9], Display::getSingleton().toStringTime(mTime, &st[0]), 8);
	text[17] = 0;
}

void ClockControl::draw(){
	Display::getSingleton().print(text, positionX + width / 2 - strlen(text) / 2, positionY, blinking);
	drawChildControls();
}

void ClockControl::onMessage(Message message){
	if ((message.from == MESSAGE_FROM_OFFSET_CLOCK) && (message.msg == Clock::CLOCK_MESSAGE_TICK)){
		setDateTimeString(message.par2, message.par1);
		draw();
	}
}
