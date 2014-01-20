#ifndef LABEL_H 
#define LABEL_H

#include "control.h"

#include "../string.h"

class Label : public Control{
protected:
	char* text;

public:
	Label(){
		text = new char[1];
		text[0] = 0;
	}

	Label(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver = nullptr)
		:	Control(_positionX, _positionY, _width, _height, _messageReceiver)
	{
		text = new char[1];
		text[0] = 0;
	}

	Label(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, char* _text, MessageReceiver* _messageReceiver = nullptr)
		:	Control(_positionX, _positionY, _width, _height, _messageReceiver)
	{
		unsigned char length = strlen(_text) + 1;
		text = new char[length + 1];
		memcpy(text, _text, length);
		text[strlen(_text) + 1] = 0;
	}

	virtual ~Label(){
		if (text != nullptr)
			delete[] text;
	}

	virtual void draw(){
		if (visible){
			Display::getSingleton().print(text, positionX + width / 2 - strlen(text) / 2, positionY + height / 2, blinking);
			drawChildControls();
		}
	}

	virtual void onMessage(Message message){}

	void setText(char* _text, bool redraw = false){
		if (strlen(text) < strlen(_text)){
			SAFE_DELETE_ARRAY(text)
			text = new char[strlen(_text) + 1];
		}

		strcpy(text, _text);

		if (redraw)
			draw();
	}
};

#endif