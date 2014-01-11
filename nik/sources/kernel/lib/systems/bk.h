#ifndef BK_H
#define BK_H

#include "../Singleton.h"
#include "../process/process.h"

class SystemBk : public Task<SystemBk>, public Singleton<SystemBk>{
public:
	SystemBk();
	CPointer<SystemBk> process();
};

#endif
