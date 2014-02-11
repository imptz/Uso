#include "UsoModeControl.h"
#include "../log/Log.h"
#include "../Local.h"
#include "../hdd/hddManager.h"
#include "../logic/Logic.h"

#pragma warning (disable : 4355)

char* UsoModeControl::modeFullAutoText = LOCAL_USO_MODE_CONTROL_MODE_FULL_AUTO_TEXT;
char* UsoModeControl::modeHalfAutoText = LOCAL_USO_MODE_CONTROL_MODE_HALF_AUTO_TEXT;
char* UsoModeControl::modeToolsText = LOCAL_USO_MODE_CONTROL_MODE_TOOLS_TEXT;

char* UsoModeControl::USO_MODE_SET_MESSAGE_FULL_AUTO = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_FULL_AUTO;
char* UsoModeControl::USO_MODE_SET_MESSAGE_HALF_AUTO = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_HALF_AUTO;
char* UsoModeControl::USO_MODE_SET_MESSAGE_TOOLS = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_TOOLS;

// M13112012
void UsoModeControl::setMode(USO_MODE _mode, USO_MODE_CONTROL_ACTOR actor, bool forced)
{
	if ((lockMode) && (!forced))
		return;
// M13112012E
	if (_mode == USO_MODE_PREV)
	{
		_mode = prevMode;
	}

	prevMode = mode;
	mode = _mode;
	
	if ((mode != USO_MODE_FULL_AUTO) &&(mode != USO_MODE_HALF_AUTO) &&(mode != USO_MODE_TOOLS))// &&(mode != USO_MODE_PREV))
	{
		prevMode = USO_MODE_FULL_AUTO;
		mode = USO_MODE_HALF_AUTO;
	}
	
	if (mode != USO_MODE_TOOLS)
	{
		memcpy(buffer, &mode, sizeof(mode));
		unsigned int _id = HddManager::getSingleton().write(buffer, START_SECTOR, BUFFER_SIZE / 512);
		while (HddManager::getSingleton().isTaskExecute(_id)) {}
	}

	switch(mode)
	{
		case USO_MODE_FULL_AUTO:
			modeButton->setName(modeFullAutoText);
			Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_FULL_AUTO, actor, 0);
			break;
		case USO_MODE_HALF_AUTO:
			modeButton->setName(modeHalfAutoText);
			Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_HALF_AUTO, actor, 0);
			break;
		case USO_MODE_TOOLS:
			Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_TOOLS, actor, 0);
			break;
	}

	draw();

	sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, USO_MODE_CONTROL_MESSAGE_NEW_MODE, prevMode, mode));
}

UsoModeControl::UsoModeControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver),
	modeButton(new Button(_positionX, _positionY, WIDTH_AUTO, HEIGHT, "", Window::BORDER_STYLE_INVISIBLE, this)),
	toolsButton(new Button(_positionX + POSITION_OFFSET_TOOLS, _positionY, WIDTH_TOOLS, HEIGHT, modeToolsText, Window::BORDER_STYLE_INVISIBLE, this)),
	prevMode(USO_MODE_FULL_AUTO), buffer(new unsigned char[BUFFER_SIZE]), lockMode(false), enabled(false)
{
	this->addChildControl(modeButton);
	this->addChildControl(toolsButton);

	unsigned int _id = HddManager::getSingleton().read(buffer, START_SECTOR, BUFFER_SIZE / 512);
	if (_id != HddManager::UNDEFINED_ID)
	{
		while (HddManager::getSingleton().isTaskExecute(_id)) {}
		setMode(*reinterpret_cast<USO_MODE*>(buffer), USO_MODE_CONTROL_ACTOR_BOOT);
	}
	else
		setMode(USO_MODE_FULL_AUTO, USO_MODE_CONTROL_ACTOR_BOOT);
}

UsoModeControl::~UsoModeControl()
{

}

void UsoModeControl::draw()
{
	drawChildControls();
}

void UsoModeControl::onMessage(Message message)
{
	if (enabled)
	{
		if (message.from == modeButton->getId())
		{
			if (mode != USO_MODE_TOOLS)
				if (message.msg == Button::BUTTON_MESSAGE_HOLD)
				{
					switch(mode)
					{
						case USO_MODE_FULL_AUTO:
							setMode(USO_MODE_HALF_AUTO, USO_MODE_CONTROL_ACTOR_USER);
							break;
						case USO_MODE_HALF_AUTO:
							setMode(USO_MODE_FULL_AUTO, USO_MODE_CONTROL_ACTOR_USER);
							break;
					}
				}
		}

		if (message.from == toolsButton->getId())
		{
			if (message.msg == Button::BUTTON_MESSAGE_HOLD)
				if (mode != USO_MODE_TOOLS)
					setMode(USO_MODE_TOOLS, USO_MODE_CONTROL_ACTOR_USER);
		}
	}
}

UsoModeControl::USO_MODE UsoModeControl::getMode()
{
	return mode;
}

bool UsoModeControl::lock()
{
	if (mode != USO_MODE_TOOLS)
		lockMode = true;

	return lockMode;
}

void UsoModeControl::unLock()
{
	lockMode = false;
}

void UsoModeControl::setEnabled(bool value)
{
	enabled = value;
}
