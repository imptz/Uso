#include "bk.h"

#include "../display/display.h"

SystemBk::SystemBk()
	:	Task(&SystemBk::process){
	Process::getSingleton().addTask(this);
}

CPointer<SystemBk> SystemBk::process(){
	return &SystemBk::process;
}
