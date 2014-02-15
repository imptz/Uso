#ifndef USO_MODE_CONTROL_H 
#define USO_MODE_CONTROL_H

#include "button.h"

class UsoModeControl : public ITimer, public Control{
public:
	enum USO_MODE{
		USO_MODE_NULL,
		USO_MODE_FULL_AUTO,
		USO_MODE_HALF_AUTO,
		USO_MODE_REMOTE
	};

	enum USO_MODE_CONTROL_ACTOR{
		USO_MODE_CONTROL_ACTOR_USER = 0,
		USO_MODE_CONTROL_ACTOR_TOOLS = 1,
		USO_MODE_CONTROL_ACTOR_BOOT = 2, 
		USO_MODE_CONTROL_ACTOR_TIME_OUT = 3,
		USO_MODE_CONTROL_ACTOR_PDU = 4
	};

private:
	static const int WIDTH = 40;
	static const int HEIGHT = 3;
	static const int WIDTH_AUTO = 20;
	static const int WIDTH_TOOLS = 12;
	static const int POSITION_OFFSET_TOOLS = 25;
	static char* USO_MODE_SET_MESSAGE_FULL_AUTO;
	static char* USO_MODE_SET_MESSAGE_HALF_AUTO;
	static char* USO_MODE_SET_MESSAGE_REMOTE;
	static char* USO_MODE_SET_MESSAGE_TOOLS_ON;
	static char* USO_MODE_SET_MESSAGE_TOOLS_OFF;
	static const unsigned int BUFFER_SIZE = 512; 

	Button *modeButton;
	Button *toolsButton;
	static char* modeFullAutoText;
	static char* modeHalfAutoText;
	static char* modeRemoteText;
	static char* modeToolsText;
	unsigned char* buffer;

	USO_MODE mode;
	bool fLock;
	bool inTools;

	UsoModeControl();
	void setMode(USO_MODE _mode, USO_MODE_CONTROL_ACTOR actor);
	void change_cycle();
	bool fRemoteTimerStart;
	int remoteTimer;
	bool fChangeFromRemote;

	static const unsigned int TIMER_PERIOD = 1000;
	virtual void timerHandler();

public:
	UsoModeControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
	virtual ~UsoModeControl();

	virtual void draw();
	virtual void onMessage(Message message);
	void change_tools();
	void change_toRemote();
	void change_fromRemote();
	void lock();
	void unLock();
	USO_MODE getMode();
	bool isInTools();

	void startRemoteTimer();
	void stopRemoteTimer();
	void clearRemoteTimer();
	void action();
};

#endif