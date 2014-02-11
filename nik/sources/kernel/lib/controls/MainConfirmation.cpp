#include "MainConfirmation.h"
#include "../message/Messages.h"
#include "../config/Config.h"

const char* MainConfirmation::YES_BUTTON_TEXT = LOCAL_CONFIRMATION_YES_BUTTON_TEXT;
const char* MainConfirmation::NO_BUTTON_TEXT = LOCAL_CONFIRMATION_NO_BUTTON_TEXT;

MainConfirmation::MainConfirmation(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver)
{ 
	yesButton = new Button(_positionX + BUTTON_WIDTH, _positionY + BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(YES_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	noButton = new Button(_positionX + BUTTON_WIDTH * 2 + 7, _positionY + BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char*>(NO_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	addChildControl(yesButton);
	addChildControl(noButton);

	label = new Label(_positionX + 10, _positionY + 2, CONFIRMATION_TEXT_MAX_LENGTH, 1);
	label->setBlinking(true);
	addChildControl(label);
}

MainConfirmation::~MainConfirmation()
{

}

void MainConfirmation::draw()
{
	drawChildControls();
}

void MainConfirmation::onMessage(Message message)
{
	if ((message.from == yesButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, CONFIRMATION_MESSAGE_RESULT, CONFIRMATION_RESULT_YES, owner));
	if ((message.from == noButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, CONFIRMATION_MESSAGE_RESULT, CONFIRMATION_RESULT_NO, owner));
}

void MainConfirmation::setConfirmationText(char* text)
{
	if (strlen(text) < CONFIRMATION_TEXT_MAX_LENGTH - 1)
	{
		strcpy(confirmationText, text);
		label->setText(confirmationText);
	}
}

void MainConfirmation::setOwner(CONFIRMATION_OWNER _owner)
{
	owner = _owner;
}
