#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "../math/math.h"



enum LOGIC_FUNCTION{
	LOGIC_FUNCTION_SEARCHING = 0,
	LOGIC_FUNCTION_COOLING_LINE = 1,
	LOGIC_FUNCTION_COOLING_POINT = 2,
	LOGIC_FUNCTION_SEARCHING_PENA = 3,
	LOGIC_FUNCTION_UNDEFINED = 100000,
	LOGIC_FUNCTION_ALL = LOGIC_FUNCTION_UNDEFINED + 1
};

struct ConfigData_constants{
	unsigned char maxPR;
	unsigned char timeOutBeforeStart;
	unsigned int timeOutBeforeFinish;
	unsigned char numberFireToAnalize;
	unsigned char minimumDistanceForCompactJet;
	bool permissionTesting;
	unsigned char testingHour;
	unsigned char testingMinute;
	bool permissionTestingInfo;
	int timeControlUserAction;
	Point3<float> protectedZone;
	bool tv;
	bool pc;
	int topField;
	int bottomField;
	int leftField;
	int rightField;
	int timeReturnFromeRemoteMode;
	bool stopSearchToRemote;
	bool requestUserBeforeSearch;
	bool autoPrToZero;
	int timeRepeatSearch;
	bool delayAfterReset;
};

struct ConfigData_pRPosition{
	unsigned char projectNumber;
	unsigned char address;
	Point3<float> position;
	Point2<float> orientation;
	unsigned char networkIndexNumber;
	Point3<float> axis;
};

struct ConfigData_ioBk{
	unsigned char bkAddress;
	unsigned char numberOnDevice;

	enum OUTPUT_FUNCTION_GROUP{
		OUTPUT_FUNCTION_GROUP_UNDEFINED = 0,
		OUTPUT_FUNCTION_GROUP_FIRE_ALARM = 1,
		OUTPUT_FUNCTION_GROUP_HARDWARE = 2,
		OUTPUT_FUNCTION_GROUP_PUMPING_STATION = 3,
		OUTPUT_FUNCTION_GROUP_PR_PRESSURE_UP = 4,
		OUTPUT_FUNCTION_GROUP_GATE_OPEN = 5,
		OUTPUT_FUNCTION_GROUP_SYSTEM_FAULT = 6,
		OUTPUT_FUNCTION_GROUP_PR_FAULT = 7,
		OUTPUT_FUNCTION_GROUP_GATE_FAULT = 8
	};

	OUTPUT_FUNCTION_GROUP outputFunctionGroup;
	unsigned char prGateNumber;

	enum INPUT_FUNCTION_GROUP{
		INPUT_FUNCTION_GROUP_UNDEFINED = 0,
		INPUT_FUNCTION_GROUP_NORMAL_OPEN = 1,
		INPUT_FUNCTION_GROUP_NORMAL_CLOSE = 2
	};
	INPUT_FUNCTION_GROUP inputFunctionGroup;

	unsigned char projectNumber;
};

struct ConfigData_ioSerial{
	unsigned char address;
	unsigned char normalState;
	unsigned char projectNumber;
};

struct ConfigData_initSignal{
	LOGIC_FUNCTION function;
	unsigned short number;
	unsigned int firstInputNumber;
	unsigned int secondInputNumber;
	unsigned int thirdInputNumber;
	bool ignorable;
};

struct ConfigData_program{
	unsigned int initSignalNumber; 
	unsigned char prNumber;
	LOGIC_FUNCTION function;
	Point2<unsigned int> point1;
	Point2<unsigned int> point2;
	unsigned short nPointProgram;
	unsigned int nasadok;
};

struct ConfigData_fv300{
	unsigned char address;
	unsigned char prNumber;
	unsigned char projectNumber;
	Point3<float> position;
	Point2<float> orientation;
};

struct ConfigData_trajectory{
	unsigned char prNumber;
	unsigned char trajectoryNumber;
	unsigned short pointNumber;
	Point2<unsigned short> position;
	unsigned short nasadok;
	unsigned short pressureNumber;
};

struct ConfigData_pressure{
	unsigned char prNumber;
	unsigned short arNumber;
	unsigned char pressure;
	unsigned char delta;
};

struct ConfigData_penabak{
	unsigned char number;
	unsigned char level;
	unsigned char address;
	unsigned char numberOnDevice;
};

struct ConfigData{
	static const unsigned int VERSION = 1;

	static const unsigned int DATA_VALID_TRUE = 1;
	static const unsigned int DATA_VALID_FALSE = 0;
	unsigned int dataValid;

	static const unsigned int CRC_ADD_MAGIC_CODE = 0x8b62ec27;
	unsigned int dataCrc;

	ConfigData_constants constants;

	static const unsigned int PR_POSITIONS_SIZE = 32;
	unsigned int prPositions_count;
	ConfigData_pRPosition pRPositions[PR_POSITIONS_SIZE];

	static const unsigned int IOBK_SIZE = 16 * 20;
	unsigned int ioBk_count;
	ConfigData_ioBk ioBk[IOBK_SIZE];

	static const unsigned int IOSERIAL_SIZE = 16 * 20;
	unsigned int ioSerial_count;
	ConfigData_ioSerial ioSerial[IOSERIAL_SIZE];

	static const unsigned int INIT_SIGNALS_SIZE = 500;
	unsigned int initSignals_count;
	ConfigData_initSignal initSignal[INIT_SIGNALS_SIZE];

	static const unsigned int PROGRAMS_SIZE = INIT_SIGNALS_SIZE;
	unsigned int programs_count;
	ConfigData_program programs[PROGRAMS_SIZE];

	static const unsigned int FV300_SIZE = PR_POSITIONS_SIZE;
	unsigned int fv300_count;
	ConfigData_fv300 fv300[FV300_SIZE];

	static const unsigned int TRAJECTORY_SIZE = PR_POSITIONS_SIZE * 4 * 500;
	unsigned int trajectory_count;
	ConfigData_trajectory trajectory[TRAJECTORY_SIZE];

	static const unsigned int PRESSURE_SIZE = PR_POSITIONS_SIZE * 4 * 50 * 10;
	unsigned int pressure_count;
	ConfigData_pressure pressure[PRESSURE_SIZE];

	static const unsigned int PENABAK_SIZE = 512;
	unsigned int penabak_count;
	ConfigData_penabak ppenabak[PENABAK_SIZE];
};

#endif
