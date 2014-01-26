#include "logic.h"
#include "../controls/ui.h"
#include "../controls/usoModeControl.h"
#include "../systems/io.h"
#include "../config/config.h"

#include "../display/display.h"

Logic::Logic()
	:	Task(&Logic::waitInitSignals), fUp(false){
	Process::getSingleton().addTask(this);
}

void Logic::up(){
	fUp = true;
}

CPointer<Logic> Logic::stop(){
	Display::getSingleton().print("Logic::stop", 1, 4);
	if (!fUp)
		return &Logic::stop;

	return &Logic::waitInitSignals;
}

CPointer<Logic> Logic::waitInitSignals(){
	Display::getSingleton().print("Logic::waitInitSignals", 1, 4);
	UsoModeControl::USO_MODE usoMode = UI::getSingleton().getUsoModeControl()->getMode();
	bool inTools = UI::getSingleton().getUsoModeControl()->isInTools();
	
	if(((usoMode != UsoModeControl::USO_MODE_FULL_AUTO) && (usoMode != UsoModeControl::USO_MODE_HALF_AUTO)) || (!inTools))
		return &Logic::waitInitSignals;

	ConfigData* config = Config::getSingleton().getConfigData();

	for(unsigned int i = 0; i < config->initSignals_count; ++i){
		bool io1 = true;
		bool io2 = true;
		bool io3 = true;

		if(config->initSignal[i].firstInputNumber != 0)
			io1 = SystemIo::getSingleton().isInputOn(config->initSignal[i].firstInputNumber);

		if(config->initSignal[i].secondInputNumber != 0)
			io2 = SystemIo::getSingleton().isInputOn(config->initSignal[i].secondInputNumber);

		if(config->initSignal[i].thirdInputNumber != 0)
			io3 = SystemIo::getSingleton().isInputOn(config->initSignal[i].thirdInputNumber);

		if(io1 && io2 && io3){
			if(!config->initSignal[i].ignorable){
				activeInitSignal = i;
				if(usoMode == UsoModeControl::USO_MODE_FULL_AUTO)
					return &Logic::start;
				else
					return &Logic::halfAutoStart;
			}
		}else
			config->initSignal[i].ignorable = false;
	}

	return &Logic::waitInitSignals;
}

CPointer<Logic> Logic::start(){
	Display::getSingleton().print("Logic::start", 1, 4);
	return &Logic::start;
}

CPointer<Logic> Logic::halfAutoStart(){
	Display::getSingleton().print("Logic::halfAutoStart", 1, 4);
	return &Logic::halfAutoStart;
}

