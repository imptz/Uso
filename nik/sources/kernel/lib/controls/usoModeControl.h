#ifndef USO_MODE_CONTROL_H 
#define USO_MODE_CONTROL_H

#include "button.h"

class UsoModeControl : public Control{
public:
	enum USO_MODE{
		USO_MODE_FULL_AUTO,
		USO_MODE_HALF_AUTO,
		USO_MODE_TOOLS,
		USO_MODE_PREV
	};

private:
	static const int WIDTH = 40;
	static const int HEIGHT = 3;
	static const int WIDTH_AUTO = 20;
	static const int WIDTH_TOOLS = 12;
	static const int POSITION_OFFSET_TOOLS = 25;
	static char* USO_MODE_SET_MESSAGE_FULL_AUTO;
	static char* USO_MODE_SET_MESSAGE_HALF_AUTO;
	static char* USO_MODE_SET_MESSAGE_TOOLS;
	static const unsigned int START_SECTOR = 30000;
	static const unsigned int BUFFER_SIZE = 512; 

	Button *modeButton;
	Button *toolsButton;
	USO_MODE mode;
	USO_MODE prevMode;
	static char* modeFullAutoText;
	static char* modeHalfAutoText;
	static char* modeToolsText;
	unsigned char* buffer;
	bool lockMode;
	bool enabled;

	UsoModeControl();

public:
	enum USO_MODE_CONTROL_ACTOR{
		USO_MODE_CONTROL_ACTOR_USER = 0,
		USO_MODE_CONTROL_ACTOR_TOOLS = 1,
		USO_MODE_CONTROL_ACTOR_BOOT = 2, 
		USO_MODE_CONTROL_ACTOR_TIME_OUT = 3
	};

	USO_MODE getMode();
	void setMode(USO_MODE _mode, USO_MODE_CONTROL_ACTOR actor, bool forced = false);
	UsoModeControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~UsoModeControl();
	virtual void draw();
	virtual void onMessage(Message message);
	bool lock();
	void unLock();
	void setEnabled(bool value);
};

#endif