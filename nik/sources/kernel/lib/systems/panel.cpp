#include "panel.h"

#include "../display/display.h"

SystemPanel::SystemPanel()
	:	Task(&SystemPanel::process){
	Process::getSingleton().addTask(this);
}

CPointer<SystemPanel> SystemPanel::process(){
	static int p = 0;

	Display::getSingleton().printUInt(p, 10, 11);
	p += 8;

	return &SystemPanel::process;
}
