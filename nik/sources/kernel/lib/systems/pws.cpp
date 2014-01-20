#include "pws.h"

#include "../display/display.h"

SystemPanel::SystemPanel()
	:	Task(&SystemPanel::process){
	Process::getSingleton().addTask(this);
}

CPointer<SystemPanel> SystemPanel::process(){

	return &SystemPanel::process;
}
