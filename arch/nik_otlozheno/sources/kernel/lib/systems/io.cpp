#include "io.h"

#include "../display/display.h"

SystemIo::SystemIo()
	:	Task(&SystemIo::process), debugIsInputOnSwitch(false){
	Process::getSingleton().addTask(this);
}

CPointer<SystemIo> SystemIo::process(){
	return &SystemIo::process;
}

bool SystemIo::isInputOn(unsigned int number){
	return debugIsInputOnSwitch;
}
