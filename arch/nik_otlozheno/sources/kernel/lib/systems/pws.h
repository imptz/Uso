#ifndef PWS_H
#define PWS_H

#include "system.h"

class SystemPanel : public System, public Task<SystemPanel>, public Singleton<SystemPanel>{
public:
	SystemPanel();
	CPointer<SystemPanel> process();
};

#endif
