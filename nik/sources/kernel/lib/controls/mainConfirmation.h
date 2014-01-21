#ifndef MAIN_CONFIRMATION_H 
#define MAIN_CONFIRMATION_H

#include "button.h"
#include "label.h"
#include "../local.h"

class MainConfirmation : public Control{
public:
	enum CONFIRMATION_RESULT{
		CONFIRMATION_RESULT_YES = 0,
		CONFIRMATION_RESULT_NO = 1
	};

	enum CONFIRMATION_OWNER{
		CONFIRMATION_OWNER_1,
		CONFIRMATION_OWNER_2
	};

private:
	CONFIRMATION_OWNER owner;

protected:
	static const int WIDTH = 78;
	static const int HEIGHT = 9;
	static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;
	static const unsigned int BUTTON_WIDTH = 18;
	static const unsigned int BUTTON_HEIGHT = 3;
	static const char* YES_BUTTON_TEXT;
	static const char* NO_BUTTON_TEXT;
	static const unsigned int CONFIRMATION_TEXT_MAX_LENGTH = 60;

	Button* yesButton;
	Button* noButton;
	Label* label;
	char confirmationText[CONFIRMATION_TEXT_MAX_LENGTH];

	MainConfirmation();

public:
	void setConfirmationText(char* text);
	MainConfirmation(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~MainConfirmation();
	virtual void draw();
	virtual void onMessage(Message message);
	void setOwner(CONFIRMATION_OWNER _owner);
};

#endif