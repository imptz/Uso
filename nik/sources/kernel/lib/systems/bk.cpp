#include "bk.h"

#include "../display/display.h"

SystemBk::SystemBk()
	:	Task(&SystemBk::process){
	Process::getSingleton().addTask(this);
}

CPointer<SystemBk> SystemBk::process(){
	static int p = 0;

	Display::getSingleton().printUInt(p++, 10, 10);

	return &SystemBk::process;
}
