#ifndef LOGIC_H
#define LOGIC_H

#include "../singleton.h"
#include "../process/process.h"

class Logic : public Task<Logic>, public Singleton<Logic>{
private:
	unsigned int activeInitSignal;
	bool fUp;

public:
	Logic();
	CPointer<Logic> stop();
	CPointer<Logic> waitInitSignals();
	CPointer<Logic> start();
	CPointer<Logic> halfAutoStart();

	void up();
};

#endif
