#include "toolsJustirovka.h"
#include "../log/log.h"
#include "../local.h"
#include "../debug/serialDebug.h"
//#include "../extension/subsystems/detection/detectionSubsystem.h"

const char* ToolsJustirovka::ERROR_LABEL_TEXT = LOCAL_TOOLS_JUSTIROVKA_ERROR_LABEL_TEXT;
const char* ToolsJustirovka::SUCCESSFUL_LABEL_TEXT = LOCAL_TOOLS_JUSTIROVKA_SUCCESSFUL_LABEL_TEXT;
const char* ToolsJustirovka::ACTION_LABEL_TEXT = LOCAL_TOOLS_JUSTIROVKA_ACTION_LABEL_TEXT;
const char* ToolsJustirovka::ACTION_BUTTON_TEXT = LOCAL_TOOLS_JUSTIROVKA_ACTION_BUTTON_TEXT;
const char* ToolsJustirovka::CLEAR_BUTTON_TEXT = LOCAL_TOOLS_JUSTIROVKA_CLEAR_BUTTON_TEXT;
const char* ToolsJustirovka::EXIT_BUTTON_TEXT = LOCAL_TOOLS_JUSTIROVKA_EXIT_BUTTON_TEXT;
const char* ToolsJustirovka::LOG_MESSAGE_TEXT = LOCAL_TOOLS_JUSTIROVKA_LOG_MESSAGE_TEXT;

void ToolsJustirovka::printNumber(unsigned int x, unsigned int y){
	char str[15];

	Display::getSingleton().toString2Num(number, str);
	for (unsigned int i = 0; i < 2; ++i){
		bool blink = false;
		if (i == blinkPosition)
			blink = true;

		Display::getSingleton().printChar(str[i], x + i, y, Display::COLOR_WHITE, blink);
	}
}

ToolsJustirovka::ToolsJustirovka(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver), proccess(false), phase(PHASE_STOP), blinkPosition(0)
{ 
	label = new Label(_positionX + 14, _positionY + 2, 2, 1); 
	label->setBlinking(true);
	
	exitButton = new Button(_positionX + 67, _positionY + BUTTON_HEIGHT * 2, 11, BUTTON_HEIGHT, const_cast<char*>(EXIT_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	clearButton = new Button(_positionX + 2, _positionY + 6, 12, BUTTON_HEIGHT, const_cast<char*>(CLEAR_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);
	actionButton = new Button(_positionX + 15, _positionY + 6, 12, BUTTON_HEIGHT, const_cast<char*>(ACTION_BUTTON_TEXT), Window::BORDER_STYLE_SINGLE, this);


	button0 = new Button(_positionX + 46, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "0", Window::BORDER_STYLE_SINGLE, this);
	button1 = new Button(_positionX + 28, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "1", Window::BORDER_STYLE_SINGLE, this);
	button2 = new Button(_positionX + 34, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "2", Window::BORDER_STYLE_SINGLE, this);
	button3 = new Button(_positionX + 40, _positionY + 6, BUTTON_WIDTH, BUTTON_HEIGHT, "3", Window::BORDER_STYLE_SINGLE, this);
	button4 = new Button(_positionX + 28, _positionY + 3, BUTTON_WIDTH, BUTTON_HEIGHT, "4", Window::BORDER_STYLE_SINGLE, this);
	button5 = new Button(_positionX + 34, _positionY + 3, BUTTON_WIDTH, BUTTON_HEIGHT, "5", Window::BORDER_STYLE_SINGLE, this);
	button6 = new Button(_positionX + 40, _positionY + 3, BUTTON_WIDTH, BUTTON_HEIGHT, "6", Window::BORDER_STYLE_SINGLE, this);
	button7 = new Button(_positionX + 28, _positionY + 0, BUTTON_WIDTH, BUTTON_HEIGHT, "7", Window::BORDER_STYLE_SINGLE, this);
	button8 = new Button(_positionX + 34, _positionY + 0, BUTTON_WIDTH, BUTTON_HEIGHT, "8", Window::BORDER_STYLE_SINGLE, this);
	button9 = new Button(_positionX + 40, _positionY + 0, BUTTON_WIDTH, BUTTON_HEIGHT, "9", Window::BORDER_STYLE_SINGLE, this);
	
	addChildControl(label);
	addChildControl(exitButton);
	addChildControl(clearButton);
	addChildControl(actionButton);
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

//	DetectionSubsystem::getSingleton().addReceiver(this);
}

ToolsJustirovka::~ToolsJustirovka(){}

void ToolsJustirovka::draw(){
	printNumber(positionX + 13, positionY + 4);
	drawChildControls();
}

void ToolsJustirovka::set(unsigned int value){
		switch (blinkPosition){
			case 0:
				number = value * 10;
				blinkPosition = 1;
				break;
			case 1:
				number += value;
				blinkPosition = 2;
				break;
		}
		draw();
}

void ToolsJustirovka::action(){}

void ToolsJustirovka::start(){
	proccess = true;
	//DetectionSubsystem::getSingleton().searchFireJustirovka(number);
	label->setText(const_cast<char*>(ACTION_LABEL_TEXT));
	draw();
}

void ToolsJustirovka::clear(){
	blinkPosition = 0;
	number = 0;
	label->setText("                      ");
	draw();
}

void ToolsJustirovka::exit(){
	proccess = false;
	clear();
	sendMessage(Message(id, TOOLS_JUSTIROVKA_MESSAGE_EXIT, 0, 0));
}

void ToolsJustirovka::onMessage(Message message){
	if ((message.from == exitButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
		exit();

	if (!proccess){
		if ((message.from == actionButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			start();

		if ((message.from == clearButton->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			clear();

		if ((message.from == button0->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(0);

		if ((message.from == button1->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(1);

		if ((message.from == button2->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(2);

		if ((message.from == button3->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(3);

		if ((message.from == button4->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(4);

		if ((message.from == button5->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(5);

		if ((message.from == button6->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(6);

		if ((message.from == button7->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(7);

		if ((message.from == button8->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(8);

		if ((message.from == button9->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
			set(9);
	}
	
	if (proccess){
		switch (message.msg){
			//case DetectionSubsystem::DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED:
			//	switch (message.par1)
			//	{
			//		case DetectionSubsystem::DETECTION_FAULT:
			//			proccess = false;
			//			label->setText(const_cast<char*>(ERROR_LABEL_TEXT));
			//			draw();
			//			break;
			//		case DetectionSubsystem::DETECTION_READY:
			//			if (DetectionSubsystem::getSingleton().getFireJustirovka())
			//			{
			//				//DetectionSubsystem::getSingleton().setCoordOffset(number, Point3<float>(fire.x, fire.y,0));
			//				Log::getSingleton().add(LOG_MESSAGE_FROM_LOGIC, LOG_MESSAGE_TYPE_INFO, const_cast<char*>(LOG_MESSAGE_TEXT), number, 0);
			//				label->setText(const_cast<char*>(SUCCESSFUL_LABEL_TEXT));
			//			}
			//			else
			//			{
			//				label->setText(const_cast<char*>(ERROR_LABEL_TEXT));
			//			}

			//			proccess = false;
			//			draw();
			//			break;
			//	}
		}
	}
}
