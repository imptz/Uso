#include "mainTabControl.h" 
#include "label.h"
#include "button.h"
#include "../log/log.h"

#pragma warning (disable : 4355)

MainTabControl::MainTabControl(unsigned int _positionX, unsigned int _positionY, unsigned int _width, unsigned int _height, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, _width, _height, _messageReceiver),
		tab(new TabControl(_positionX, _positionY, _width, _height, TAB_COUNT, Window::BORDER_STYLE_DOUBLE, this))
{
	addChildControl(tab);

	mainInfo = new MainInfo(_positionX, _positionY, this); 
	mainConfirmation = new MainConfirmation(_positionX, _positionY, this);
	mainFinish = new MainFinish(_positionX, _positionY, this);
	addReceiver(mainFinish);
	tab->getTabPanel(INFO_TAB)->addChildControl(mainInfo);
	tab->getTabPanel(CONFIRMATION_TAB)->addChildControl(mainConfirmation);
	tab->getTabPanel(FINISH_TAB)->addChildControl(mainFinish);

	tab->setActiveTab(INFO_TAB);
}

MainTabControl::~MainTabControl(){}

void MainTabControl::draw(){
	tab->draw();
}

void MainTabControl::onMessage(Message message){
	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + mainConfirmation->getId()) && (message.msg == MESSAGE_MAIN_CONFIRMATION_RESULT)){
		tab->setActiveTab(INFO_TAB);
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, MESSAGE_MAIN_CONFIRMATION_RESULT, message.par1, message.par2));
	}

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + mainFinish->getId()) && (message.msg == MESSAGE_MAIN_FINISH_RESULT)){
		tab->setActiveTab(INFO_TAB);
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONTROLS + id, MESSAGE_MAIN_FINISH_RESULT, 0, 0));
	}

	if ((message.from == MESSAGE_FROM_OFFSET_APPLICATION) && (message.msg == MESSAGE_MAIN_FINISH_LABEL)){
		sendMessage(Message(MESSAGE_FROM_OFFSET_APPLICATION, MESSAGE_MAIN_FINISH_LABEL, message.par1, 0));
	}
}

void MainTabControl::activateConfirmationTab(){
	tab->setActiveTab(CONFIRMATION_TAB);
}

void MainTabControl::setConfirmationText(char* text){
	mainConfirmation->setConfirmationText(text);
}

void MainTabControl::setOwner(MainConfirmation::CONFIRMATION_OWNER _owner){
	mainConfirmation->setOwner(_owner);
}

void MainTabControl::activateFinishTab(){
	tab->setActiveTab(FINISH_TAB);
	mainFinish->disableFinishMessage();
}

void MainTabControl::activateMainTab(){
	tab->setActiveTab(INFO_TAB);
}
