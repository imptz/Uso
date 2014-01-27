#include "mainInfo.h"
#include "../log/log.h"
#include "../config/config.h"
#include "button.h"
#include "../debug/serialDebug.h"

MainInfo::MainInfo(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver)
{ 
	draw();
}

MainInfo::~MainInfo(){}

void MainInfo::draw(){
	drawChildControls();
}

void MainInfo::onMessage(Message message){
}
