#ifndef PANEL_H
#define PANEL_H

#include "system.h"

class SystemPanel : public System, public Task<SystemPanel>, public Singleton<SystemPanel>{
public:
	SystemPanel();
	CPointer<SystemPanel> process();
};

#endif
