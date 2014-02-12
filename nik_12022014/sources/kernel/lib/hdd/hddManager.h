#ifndef HDD_MANAGER_H
#define HDD_MANAGER_H

#include "../Singleton.h"
#include "../timer/Timer.h"
#include "hdd.h"

enum SECTOR_OFFSET{
	SECTOR_OFFSET_CONFIG = 50000
};

class HddManager : public Singleton<HddManager>, public ITimer
{
	private:
        static const unsigned int TIMER_PERIOD = 1;
        virtual void timerHandler();

		Hdd* hdd;

		unsigned int nextId;
		unsigned int getNextId();
	public:
		static const unsigned int UNDEFINED_ID = 0xffffffff;

	private:
		struct Task
		{
			bool isRead;
			unsigned int id;
			unsigned char* pBuffer;
			unsigned int startSector;
			unsigned int sectorsCount;
		};

		static const unsigned int MAX_TASK_COUNT = 100;
		Task* tasks;

	public:
		HddManager();
		~HddManager();
		
	private:
		unsigned int addTask(bool isRead, unsigned char* _pBuffer,	unsigned int _startSector, unsigned int _sectorsCount);

	public:
		unsigned int read(unsigned char* _pBuffer,	unsigned int _startSector, unsigned int _sectorsCount);
		unsigned int write(unsigned char* _pBuffer,	unsigned int _startSector, unsigned int _sectorsCount);
		bool isTaskExecute(unsigned int _id);

	private:
		bool close;

	public:
		void closeManager();
		bool isManagerClose();

	private:
		enum PHASE
		{
			PHASE_READY = 1,
			PHASE_ACTION = 2
		};

		PHASE phase;
		unsigned int activeTaskIndex;
};

#endif