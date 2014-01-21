#include "toolsDate.h"
#include "../log/log.h"
#include "../local.h"

const char* ToolsDate::ERROR_LABEL_TEXT = LOCAL_TOOLS_DATE_ERROR_LABEL_TEXT;
const char* ToolsDate::CLEAR_BUTTON_TEXT = LOCAL_TOOLS_DATE_CLEAR_BUTTON_TEXT;
const char* ToolsDate::EXIT_BUTTON_TEXT = LOCAL_TOOLS_DATE_EXIT_BUTTON_TEXT;
const char* ToolsDate::NEW_DATE_MESSAGE_TEXT = LOCAL_TOOLS_DATE_NEW_DATE_MESSAGE_TEXT;

void ToolsDate::printDateTime(unsigned int x, unsigned int y){
	char str[15];

	dt.toString(&str[0]);
	for (unsigned int i = 0; i < 14; ++i){
		bool blink = false;
		if (i == blinkPosition)
			blink = true;

		Display::getSingleton().printChar(str[i], x + i, y, Display::COLOR_WHITE, blink);
	}
}

ToolsDate::ToolsDate(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver), blinkPosition(0)
{ 
	errorLabel = new Label(_positionX + 2, _positionY + 2, 12, 1); 
	errorLabel->setBlinking(true);
	
	exitButton = new Button(_positionX + 67, _positionY + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	clearButton = new Button(_positionX + 2, _positionY + 6, 12, BUTTON_HEIGHT, const_cast<char*>(CLEAR_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	button0 = new Button(_positionX + 38, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "0", Window::BORDER_STYLE_SINGLE, this);
	button1 = new Button(_positionX + 20, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "1", Window::BORDER_STYLE_SINGLE, this);
	button2 = new Button(_positionX + 26, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "2", Window::BORDER_STYLE_SINGLE, this);
	button3 = new Button(_positionX + 32, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "3", Window::BORDER_STYLE_SINGLE, this);
	button4 = new Button(_positionX + 20, _positionY + 3, BUTTON_WIDTH, BUTTON_HEIGHT, "4", Window::BORDER_STYLE_SINGLE, this);
	button5 = new Button(_positionX + 26, _positionY + 3, BUTTON_WIDTH, BUTTON_HEIGHT, "5", Window::BORDER_STYLE_SINGLE, this);
	button6 = new Button(_positionX + 32, _positionY + 3, BUTTON_WIDTH, BUTTON_HEIGHT, "6", Window::BORDER_STYLE_SINGLE, this);
	button7 = new Button(_positionX + 20, _positionY + 0, BUTTON_WIDTH, BUTTON_HEIGHT, "7", Window::BORDER_STYLE_SINGLE, this);
	button8 = new Button(_positionX + 26, _positionY + 0, BUTTON_WIDTH, BUTTON_HEIGHT, "8", Window::BORDER_STYLE_SINGLE, this);
	button9 = new Button(_positionX + 32, _positionY + 0, BUTTON_WIDTH, BUTTON_HEIGHT, "9", Window::BORDER_STYLE_SINGLE, this);
	
	addChildControl(errorLabel);
	addChildControl(exitButton);
	addChildControl(clearButton);
	addChildControl(button0);
	addChildControl(button1);
	addChildControl(button2);
	addChildControl(button3);
	addChildControl(button4);
	addChildControl(button5);
	addChildControl(button6);
	addChildControl(button7);
	addChildControl(button8);
	addChildControl(button9);
}

ToolsDate::~ToolsDate(){}

void ToolsDate::draw(){
	printDateTime(positionX + 1, positionY + 4);
	drawChildControls();
}

void ToolsDate::set(unsigned int value){
	switch (blinkPosition){
		case 0:
			dt.day = value << 4;
			blinkPosition = 1;
			break;
		case 1:
			dt.day += value;
			blinkPosition = 3;
			break;
		case 3:
			dt.month = value << 4;
			blinkPosition = 4;
			break;
		case 4:
			dt.month += value;
			blinkPosition = 6;
			break;
		case 6:
			dt.year = value << 4;
			blinkPosition = 7;
			break;
		case 7:
			dt.year += value;
			blinkPosition = 9;
			break;
		case 9:
			dt.hour = value << 4;
			blinkPosition = 10;
			break;
		case 10:
			dt.hour += value;
			blinkPosition = 12;
			break;
		case 12:
			dt.minute = value << 4;
			blinkPosition = 13;
			break;
		case 13:
			dt.minute += value;
			blinkPosition = 14;
			setNewDateTime();
			break;
	}
	draw();
}

bool ToolsDate::isIntercalaryYear(int value){
	if ((value % 4) == 0)
		return true;
	else
		return false;
}

int ToolsDate::getDayCountInMonth(int month, int year){
	int result = 0;

	switch (month){
		case 1:
			result = 31;
			break;
		case 2:
			if (isIntercalaryYear(year))
				result = 29;
			else
				result = 28;
			break;
		case 3:
			result = 31;
			break;
		case 4:
			result = 30;
			break;
		case 5:
			result = 31;
			break;
		case 6:
			result = 30;
			break;
		case 7:
			result = 31;
			break;
		case 8:
			result = 31;
			break;
		case 9:
			result = 30;
			break;
		case 10:
			result = 31;
			break;
		case 11:
			result = 30;
			break;
		case 12:
			result = 31;
			break;
	}

	return result;
}

bool ToolsDate::test(){
	bool result = true;

	int minute = ((dt.minute & 0xf0) >> 4) * 10 + (dt.minute & 0x0f);
	int hour = ((dt.hour & 0xf0) >> 4) * 10 + (dt.hour & 0x0f);
	int day = ((dt.day & 0xf0) >> 4) * 10 + (dt.day & 0x0f);
	int month = ((dt.month & 0xf0) >> 4) * 10 + (dt.month & 0x0f);
	int year = ((dt.year & 0xf0) >> 4) * 10 + (dt.year & 0x0f);

	if (minute > 59)
		result = false;

	if (hour > 23)
		result = false;

	if ((month > 12) || (month == 0))
		result = false;

	if ((day == 0) || (day > getDayCountInMonth(month, year)))
		result = false;

	return result;
}

void ToolsDate::setNewDateTime(){
	if (test()){
		Log::getSingleton().add(LOG_MESSAGE_FROM_TOOLS_DATE, LOG_MESSAGE_TYPE_SYSTEM, const_cast<char*>(NEW_DATE_MESSAGE_TEXT), 0, 0);
		Clock::getSingleton().setClock(dt);
	}else
		errorLabel->setText(const_cast<char*>(ERROR_LABEL_TEXT));
}

void ToolsDate::clearDateTime(){
	dt.second = 0;
	dt.minute = 0;
	dt.hour = 0;
	dt.day = 0;
	dt.month = 0;
	dt.year = 0;
	blinkPosition = 0;
	errorLabel->setText("            ");
}

void ToolsDate::onMessage(Message message){
	if ((message.from == exitButton->getId()) && (message.msg == MESSAGE_BUTTON_UP)){
		clearDateTime();
		sendMessage(Message(id, MESSAGE_TOOLS_DATE_EXIT, 0, 0));
	}

	if ((message.from == clearButton->getId()) && (message.msg == MESSAGE_BUTTON_UP)){
		clearDateTime();
		draw();
	}

	if ((message.from == button0->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(0);

	if ((message.from == button1->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(1);

	if ((message.from == button2->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(2);

	if ((message.from == button3->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(3);

	if ((message.from == button4->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(4);

	if ((message.from == button5->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(5);

	if ((message.from == button6->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(6);

	if ((message.from == button7->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(7);

	if ((message.from == button8->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(8);

	if ((message.from == button9->getId()) && (message.msg == MESSAGE_BUTTON_UP))
		set(9);
}
