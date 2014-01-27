#ifndef IO_H
#define IO_H

#include "system.h"

class SystemIo : public System, public Task<SystemIo>, public Singleton<SystemIo>{
public:
	SystemIo();
	CPointer<SystemIo> process();

	bool isInputOn(unsigned int number);

	bool debugIsInputOnSwitch;
};

#endif
