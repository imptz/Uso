#ifndef TOOLS_TAB_CONTROL_H 
#define TOOLS_TAB_CONTROL_H

#include "tabControl.h"
#include "button.h"
#include "toolsLog.h"
#include "toolsDate.h"
#include "toolsOff.h"
#include "toolsUpdate.h"
#include "toolsJustirovka.h"

class ToolsTabControl : public Control{
protected:
	static const unsigned int TAB_COUNT = 6;
	static const unsigned int MAIN_TAB = 0;
	static const unsigned int UPDATE_TAB = 1;
	static const unsigned int DATE_TAB = 2;
	static const unsigned int OFF_TAB = 3;
	static const unsigned int LOG_TAB = 4;
	static const unsigned int JUSTIROVKA_TAB = 5;
	static const unsigned int BUTTON_WIDTH = 26;
	static const unsigned int BUTTON_HEIGHT = 3;

	TabControl* tab;
	Button* cancelButton;
	Button* updateButton; 
	Button* dateButton; 
	Button* offButton; 
	Button* logButton; 
	Button* detectionResetButton; 
	Button* justirovkaButton; 
	Button* povorotniyButton; 

	static const char* EXIT_BUTTON_TEXT;
	static const char* UPDATE_BUTTON_TEXT;
	static const char* DATE_BUTTON_TEXT;
	static const char* OFF_BUTTON_TEXT;
	static const char* LOG_BUTTON_TEXT;
	static const char* DETECTION_RESET_BUTTON_TEXT;
	static const char* JUSTIROVKA_BUTTON_TEXT;
	static const char* POVOROTNIY_BUTTON_TEXT;

	ToolsLog* toolsLog;
	ToolsDate* toolsDate;
	ToolsOff* toolsOff;
	ToolsUpdate* toolsUpdate;
	ToolsJustirovka* toolsJustirovka;

	ToolsTabControl();

public:
	ToolsTabControl(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver = nullptr);
	virtual ~ToolsTabControl();
	virtual void draw();
	virtual void onMessage(Message message);
};

#endif