#ifndef BK_H
#define BK_H

#include "system.h"

class SystemBk : public System, public Task<SystemBk>, public Singleton<SystemBk>{
public:
	SystemBk();
	CPointer<SystemBk> process();
};

#endif
