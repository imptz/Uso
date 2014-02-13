#include "usoModeControl.h"
#include "../log/log.h"
#include "../local.h"
#include "../hdd/hddManager.h"
//#include "../logic/Logic.h"

#pragma warning (disable : 4355)

char* UsoModeControl::modeFullAutoText = LOCAL_USO_MODE_CONTROL_MODE_FULL_AUTO_TEXT;
char* UsoModeControl::modeHalfAutoText = LOCAL_USO_MODE_CONTROL_MODE_HALF_AUTO_TEXT;
char* UsoModeControl::modeRemoteText = LOCAL_USO_MODE_CONTROL_MODE_REMOTE_TEXT;
char* UsoModeControl::modeToolsText = LOCAL_USO_MODE_CONTROL_MODE_TOOLS_TEXT;

char* UsoModeControl::USO_MODE_SET_MESSAGE_FULL_AUTO = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_FULL_AUTO;
char* UsoModeControl::USO_MODE_SET_MESSAGE_HALF_AUTO = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_HALF_AUTO;
char* UsoModeControl::USO_MODE_SET_MESSAGE_REMOTE = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_REMOTE;
char* UsoModeControl::USO_MODE_SET_MESSAGE_TOOLS_ON = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_TOOLS_ON;
char* UsoModeControl::USO_MODE_SET_MESSAGE_TOOLS_OFF = LOCAL_USO_MODE_CONTROL_USO_MODE_SET_MESSAGE_TOOLS_OFF;

UsoModeControl::UsoModeControl(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver),
	modeButton(new Button(_positionX, _positionY, WIDTH_AUTO, HEIGHT, "", Window::BORDER_STYLE_INVISIBLE, this)),
	toolsButton(new Button(_positionX + POSITION_OFFSET_TOOLS, _positionY, WIDTH_TOOLS, HEIGHT, modeToolsText, Window::BORDER_STYLE_INVISIBLE, this)),
	buffer(new unsigned char[BUFFER_SIZE]), mode(USO_MODE_HALF_AUTO), fLock(false), inTools(false)
{
	this->addChildControl(modeButton);
	this->addChildControl(toolsButton);

	unsigned int _id = HddManager::getSingleton().read(buffer, SECTOR_OFFSET_USO_MODE, BUFFER_SIZE / 512);
	if (_id != HddManager::UNDEFINED_ID){
		while (HddManager::getSingleton().isTaskExecute(_id)){}

		switch(*reinterpret_cast<USO_MODE*>(buffer)){
			case USO_MODE_FULL_AUTO:
				setMode(USO_MODE_FULL_AUTO, USO_MODE_CONTROL_ACTOR_BOOT);
				break;
			case USO_MODE_HALF_AUTO:
				setMode(USO_MODE_HALF_AUTO, USO_MODE_CONTROL_ACTOR_BOOT);
				break;
			case USO_MODE_REMOTE:
				setMode(USO_MODE_HALF_AUTO, USO_MODE_CONTROL_ACTOR_BOOT);
				break;
			default:
				setMode(USO_MODE_HALF_AUTO, USO_MODE_CONTROL_ACTOR_BOOT);
				break;
		}		
	}else
		setMode(USO_MODE_HALF_AUTO, USO_MODE_CONTROL_ACTOR_BOOT);
}

UsoModeControl::~UsoModeControl(){}

void UsoModeControl::draw(){
	drawChildControls();
}

void UsoModeControl::onMessage(Message message){
	if (message.from == modeButton->getId()){
		if (message.msg == Button::BUTTON_MESSAGE_HOLD)
			change_cycle();
	}

	if (message.from == toolsButton->getId()){
		if (message.msg == Button::BUTTON_MESSAGE_HOLD)
			if(!inTools)
				change_tools();
	}
}

void UsoModeControl::setMode(USO_MODE _mode, USO_MODE_CONTROL_ACTOR actor){
	mode = _mode;

	switch(_mode){
		case USO_MODE_FULL_AUTO:
			modeButton->setName(modeFullAutoText);
			Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_FULL_AUTO, actor, 0);
			break;
		case USO_MODE_HALF_AUTO:
			modeButton->setName(modeHalfAutoText);
			Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_HALF_AUTO, actor, 0);
			break;
		case USO_MODE_REMOTE:
			modeButton->setName(modeRemoteText);
			Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_REMOTE, actor, 0);
			break;
	}

	memcpy(buffer, &mode, sizeof(mode));
	unsigned int _id = HddManager::getSingleton().write(buffer, SECTOR_OFFSET_USO_MODE, BUFFER_SIZE / 512);
	while (HddManager::getSingleton().isTaskExecute(_id)) {}

	sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, MESSAGE_USO_MODE_CONTROL_NEW_MODE, 0, 0));
	
	draw();
}

void UsoModeControl::lock(){
	fLock = true;
}

void UsoModeControl::unLock(){
	fLock = false;
}

void UsoModeControl::change_tools(){
	if(inTools){
		inTools = false;

		Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_TOOLS_OFF, USO_MODE_CONTROL_ACTOR_USER, 0);
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, MESSAGE_USO_MODE_CONTROL_FROM_TOOLS, 0, 0));
	}else{
		inTools = true;

		Log::getSingleton().add(LOG_MESSAGE_FROM_APPLICATION, LOG_MESSAGE_TYPE_SYSTEM, USO_MODE_SET_MESSAGE_TOOLS_ON, USO_MODE_CONTROL_ACTOR_USER, 0);
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, MESSAGE_USO_MODE_CONTROL_TO_TOOLS, 0, 0));
	}
}

void UsoModeControl::change_cycle(){
	if(inTools || fLock)
		return;

	switch(mode){
		case USO_MODE_FULL_AUTO:
			setMode(USO_MODE_HALF_AUTO, USO_MODE_CONTROL_ACTOR_USER);
			break;
		case USO_MODE_HALF_AUTO:
			setMode(USO_MODE_REMOTE, USO_MODE_CONTROL_ACTOR_USER);
			break;
		case USO_MODE_REMOTE:
			setMode(USO_MODE_FULL_AUTO, USO_MODE_CONTROL_ACTOR_USER);
			break;
	}
}

void UsoModeControl::change_toRemote(){
	setMode(USO_MODE_REMOTE, USO_MODE_CONTROL_ACTOR_USER);
}

void UsoModeControl::change_fromRemote(){
	setMode(USO_MODE_FULL_AUTO, USO_MODE_CONTROL_ACTOR_USER);
}

UsoModeControl::USO_MODE UsoModeControl::getMode(){
	return mode;
}

bool UsoModeControl::isInTools(){
	return inTools;
}