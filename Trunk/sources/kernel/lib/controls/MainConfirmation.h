#ifndef MAIN_CONFIRMATION_H 
#define MAIN_CONFIRMATION_H

#include "Button.h"
#include "Label.h"
#include "../Local.h"

class MainConfirmation : public Control
{
	protected:
		static const int WIDTH = 78;
		static const int HEIGHT = 9;
		static const Window::BORDER_STYLE BORDER = Window::BORDER_STYLE_INVISIBLE;

		static const unsigned int BUTTON_WIDTH = 18;
		static const unsigned int BUTTON_HEIGHT = 3;
		Button* yesButton;
		static const char* YES_BUTTON_TEXT;
		Button* noButton;
		static const char* NO_BUTTON_TEXT;

		Label* label;
		static const unsigned int CONFIRMATION_TEXT_MAX_LENGTH = 60;
		char confirmationText[CONFIRMATION_TEXT_MAX_LENGTH];
	public:
		void setConfirmationText(char* text);

	protected:
		MainConfirmation();

	public:
		MainConfirmation(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
		virtual ~MainConfirmation();
		virtual void draw();
		virtual void onMessage(Message message);

		enum CONFIRMATION_MESSAGE
		{
			CONFIRMATION_MESSAGE_RESULT = 12
		};

		enum CONFIRMATION_RESULT
		{
			CONFIRMATION_RESULT_YES = 0,
			CONFIRMATION_RESULT_NO = 1
		};

		enum CONFIRMATION_OWNER
		{
			CONFIRMATION_OWNER_1,
			CONFIRMATION_OWNER_2
		};
		void setOwner(CONFIRMATION_OWNER _owner);
	private:
		CONFIRMATION_OWNER owner;
		
};

#endif