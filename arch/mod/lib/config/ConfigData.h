#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#ifndef TEST
#include "../timer/Timer.h"
#include "../memory/memory.h"
#include "../log/Log.h"
#include "../hdd/hddManager.h"
#endif

#include "ConfigDataStruct.h"

class ConfigData : public ITimer
{
	protected:
		static const unsigned int TIMER_PERIOD = 100;
		virtual void timerHandler();
	
	private:
		ConfigDataStruct* cData;
		ConfigDataStruct* cDataNew;

	public:
		ConfigData();
		~ConfigData();

		bool setNewData(unsigned char* data, unsigned int length);

	private:
		bool dataSave;
	public:
		bool isDataSave();

		enum CONFIG_DATA_PHASE
		{
			CONFIG_DATA_PHASE_STOP = 0,
			CONFIG_DATA_PHASE_WRITE_1 = 1,
			CONFIG_DATA_PHASE_WRITE_2 = 2,
			CONFIG_DATA_PHASE_READ_1 = 3,
			CONFIG_DATA_PHASE_READ_2 = 4
		};
	private:
		CONFIG_DATA_PHASE phase;

		static const unsigned int START_SECTOR = 50000;
		static const unsigned int BUFFER_SIZE = 512 * 128; 
		unsigned char* buffer;
		unsigned int taskId;

	public:
		ConfigDataStructConst* getConfigDataStructConst();

		void setConfigDataStructPRPositionCount(unsigned int count);
		unsigned int getConfigDataStructPRPositionCount();
		ConfigDataStructPRPosition** getConfigDataStructPRPositions();		

		unsigned int getConfigDataStructIOBk16Count();
		void setConfigDataStructIOBk16Count(unsigned int count);
		ConfigDataStructIOBk16** getConfigDataStructIOBk16();		

		unsigned int getConfigDataStructIOSerialCount();
		void setConfigDataStructIOSerialCount(unsigned int count);
		ConfigDataStructIOSerial** getConfigDataStructIOSerial();		

		unsigned int getConfigDataStructInitSignalsCount();
		void setConfigDataStructInitSignalsCount(unsigned int count);
		ConfigDataStructInitSignal** getConfigDataStructInitSignals();

		unsigned int getConfigDataStructProgramCount();
		void setConfigDataStructProgramCount(unsigned int count);
		ConfigDataStructProgram** getConfigDataStructPrograms();
		unsigned int getConfigDataStructProgramInitSignal(unsigned int initSignalNumber, LOGIC_FUNCTION _function, unsigned int** pList);

		unsigned int getConfigDataStructFv300Count();
		void setConfigDataStructFv300Count(unsigned int count);
		ConfigDataStructFv300** getConfigDataStructFv300();		

		unsigned int getConfigDataStructTrajectoryCount();
		ConfigDataStructTrajectory** getConfigDataStructTrajectory();		

		unsigned int getConfigDataStructPressureCount();
		ConfigDataStructPressure** getConfigDataStructPressure();		

		unsigned int getConfigDataStructPenaBakCount();
		ConfigDataStructPenaBak** getConfigDataStructPenaBak();		
	private:
		void setData(unsigned char* data, unsigned int length);

	public:
		unsigned char getPRAddressByNumber(unsigned char number);
		unsigned char getPRIndexByAddress(unsigned char address);

		bool isTvExistsInConfig();
		bool isFv300ExistsInConfig();

		bool isReady();
};

#endif
