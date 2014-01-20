#include "mainFinish.h"
#include "../message/messages.h"
#include "../config/config.h"
#include "../string.h"
#include "../debug/serialDebug.h"

const char* MainFinish::FINISH_BUTTON_TEXT = LOCAL_FINISH_FINISH_BUTTON_TEXT;
const char* MainFinish::FINISH_LABEL_TEXT = LOCAL_FINISH_FINISH_LABEL_TEXT;
const char* MainFinish::START_LABEL_TEXT = LOCAL_FINISH_START_LABEL_TEXT;

MainFinish::MainFinish(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver)
{ 
	unsigned int len = strlen(const_cast<char*>(FINISH_BUTTON_TEXT));
	finishButton = new Button(_positionX + 40 - len / 2 - 1 , _positionY + BUTTON_HEIGHT * 2, len + 2, BUTTON_HEIGHT, const_cast<char*>(FINISH_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	finishButton->setBlinking(true);
	addChildControl(finishButton);

	len = strlen(const_cast<char*>(FINISH_LABEL_TEXT));
	finishLabel = new Label(_positionX + 40 - len / 2 - 1, _positionY + BUTTON_HEIGHT * 1, len, 1);
	finishLabel->setText(const_cast<char*>(FINISH_LABEL_TEXT));
	finishLabel->setBlinking(true);
	finishLabel->setVisible(false);
	addChildControl(finishLabel);

	len = strlen(const_cast<char*>(START_LABEL_TEXT));
	startLabel = new Label(_positionX + 40 - len / 2 - 1, _positionY + BUTTON_HEIGHT * 1, len, 1);
	startLabel->setText(const_cast<char*>(START_LABEL_TEXT));
	startLabel->setBlinking(true);
	startLabel->setVisible(false);
	addChildControl(startLabel);
}

MainFinish::~MainFinish(){}

void MainFinish::draw(){
	for (unsigned int w = 1; w < width; ++w)
		for (unsigned int h = 1; h < height; ++h){
			Display::getSingleton().printChar(' ', positionX + w, positionY + h, Display::COLOR_WHITE);
		}
	drawChildControls();
}

void MainFinish::onMessage(Message message){
	if ((message.from == finishButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, FINISH_MESSAGE_RESULT, FINISH_RESULT_FINISH, 0));

	if (message.msg == FINISH_MESSAGE_LABEL){
		if (message.par1 == FINISH_MESSAGE_PARAM_START){
			finishButton->setVisible(true);
			finishLabel->setVisible(false);
			startLabel->setVisible(false);
			draw();
		}else{
			finishButton->setVisible(false);
			finishLabel->setVisible(true);
			startLabel->setVisible(false);
		}
	}
}

void MainFinish::disableFinishMessage(){
	finishButton->setVisible(false);
	finishLabel->setVisible(false);
	startLabel->setVisible(true);
	draw();
}
