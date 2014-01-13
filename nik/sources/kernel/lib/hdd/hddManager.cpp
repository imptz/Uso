#include "hddManager.h"
#include "../display/display.h"

HddManager::HddManager()
	: ITimer(TIMER_PERIOD), hdd(new Hdd), nextId(0), tasks(new Task[MAX_TASK_COUNT]), close(false), phase(PHASE_READY)
{
	for (unsigned int i = 0; i < MAX_TASK_COUNT; i++)
		tasks[i].id = UNDEFINED_ID;

	pTimer->start();
}

HddManager::~HddManager(){
	delete[] tasks;
	delete hdd;
}

unsigned int HddManager::getNextId(){
	if (nextId < 0xfffffffe)
		nextId++;
	else
		nextId = 0;

	return nextId;
}

unsigned int HddManager::addTask(bool isRead, unsigned char* _pBuffer,	unsigned int _startSector, unsigned int _sectorsCount){
	if (close)
		return UNDEFINED_ID;

	unsigned int resultId = UNDEFINED_ID;

	for (unsigned int i = 0; i < MAX_TASK_COUNT; i++){
		if (tasks[i].id == UNDEFINED_ID){
			tasks[i].id = getNextId();
			tasks[i].isRead = isRead;
			tasks[i].pBuffer = _pBuffer;
			tasks[i].startSector = _startSector;
			tasks[i].sectorsCount = _sectorsCount;
			return tasks[i].id;
		}
	}
		
	return resultId;
}

unsigned int HddManager::read(unsigned char* _pBuffer,	unsigned int _startSector, unsigned int _sectorsCount){
	return addTask(true, _pBuffer, _startSector, _sectorsCount);
}

unsigned int HddManager::write(unsigned char* _pBuffer,	unsigned int _startSector, unsigned int _sectorsCount){
	return addTask(false, _pBuffer, _startSector, _sectorsCount);
}

bool HddManager::isTaskExecute(unsigned int _id){
	bool result = false;

	for (unsigned int i = 0; i < MAX_TASK_COUNT; i++)
		if (tasks[i].id == _id){
			result = true;
			break;
		}

	return result;
}

void HddManager::closeManager(){
	close = true;
}

bool HddManager::isManagerClose(){
	bool result = true;

	for (unsigned int i = 0; i < MAX_TASK_COUNT; i++)
		if (tasks[i].id != UNDEFINED_ID)
			result = false;

	return result && close;
}

void HddManager::timerHandler(){
	switch (phase){
		case PHASE_READY:
			for (unsigned int i = 0; i < MAX_TASK_COUNT; i++)
				if (tasks[i].id != UNDEFINED_ID){
					unsigned int sectorCount = tasks[i].sectorsCount;
					if (sectorCount > Hdd::MAX_SECTOR_COUNT)
						sectorCount = Hdd::MAX_SECTOR_COUNT;

					tasks[i].sectorsCount -= sectorCount;

					if (tasks[i].isRead){
						hdd->read(tasks[i].startSector, sectorCount, tasks[i].pBuffer);
					}
					else
						hdd->write(tasks[i].startSector, sectorCount, tasks[i].pBuffer);

					tasks[i].startSector += sectorCount;

					activeTaskIndex = i;
					phase = PHASE_ACTION;
					tasks[i].pBuffer += (sectorCount * Hdd::SECTOR_SIZE);
					break;
				}
			break;
		case PHASE_ACTION:
			if (hdd->getActionCounter() == 0){
				if (tasks[activeTaskIndex].sectorsCount == 0)
					tasks[activeTaskIndex].id = UNDEFINED_ID;

				phase = PHASE_READY;
			}
			break;
	}
}
