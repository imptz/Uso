	#ifndef USO_MODE_CONTROL_H 
#define USO_MODE_CONTROL_H

#include "Button.h"

class UsoModeControl : public Control
{
	private:
		UsoModeControl();
		Button *modeButton;
		Button *toolsButton;
		static const int WIDTH = 40;
		static const int HEIGHT = 3;

		static const int WIDTH_AUTO = 20;
		static const int WIDTH_TOOLS = 12;
		static const int POSITION_OFFSET_TOOLS = 25;

	public:
		enum USO_MODE
		{
			USO_MODE_FULL_AUTO,
			USO_MODE_HALF_AUTO,
			USO_MODE_TOOLS,
			USO_MODE_PREV
		};

	private:
		USO_MODE mode;
	public:
		USO_MODE getMode();
	private:
		USO_MODE prevMode;
		static char* modeFullAutoText;
		static char* modeHalfAutoText;
		static char* modeToolsText;

	public:
		enum USO_MODE_CONTROL_ACTOR
		{
			USO_MODE_CONTROL_ACTOR_USER = 0,
			USO_MODE_CONTROL_ACTOR_TOOLS = 1,
			// M061112
			USO_MODE_CONTROL_ACTOR_BOOT = 2, 
			USO_MODE_CONTROL_ACTOR_TIME_OUT = 3
			// M061112E
		};

		void setMode(USO_MODE _mode, USO_MODE_CONTROL_ACTOR actor, bool forced = false);

	public:
		UsoModeControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver = nullptr);
		virtual ~UsoModeControl();
		virtual void draw();
		virtual void onMessage(Message message);

	private:
		static char* USO_MODE_SET_MESSAGE_FULL_AUTO;
		static char* USO_MODE_SET_MESSAGE_HALF_AUTO;
		static char* USO_MODE_SET_MESSAGE_TOOLS;

		static const unsigned int START_SECTOR = 30000;
		static const unsigned int BUFFER_SIZE = 512; 
		unsigned char* buffer;

	public:
		enum USO_MODE_CONTROL_MESSAGE
		{
			USO_MODE_CONTROL_MESSAGE_NEW_MODE = 22
		};

	private:
		bool lockMode;
	public:
		bool lock();
		void unLock();

	private:
		bool enabled;
	public:
		void setEnabled(bool value);
};

#endif