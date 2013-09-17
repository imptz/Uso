#ifndef CONFIG_DATA_STRUCT_H
#define CONFIG_DATA_STRUCT_H

#include "../math/math.h"
#include "../id.h"

const unsigned short DATA_VERSION = 0x0001;

enum LOGIC_FUNCTION
{
	LOGIC_FUNCTION_SEARCHING = 0,
	LOGIC_FUNCTION_COOLING_LINE = 1,
	LOGIC_FUNCTION_COOLING_POINT = 2,
	LOGIC_FUNCTION_SEARCHING_PENA = 3,
	LOGIC_FUNCTION_SEARCHING_PENA_ZATVOR = 4,
	LOGIC_FUNCTION_UNDEFINED = 100000,
	LOGIC_FUNCTION_ALL = LOGIC_FUNCTION_UNDEFINED + 1
};

struct ConfigDataStructConst
{
// M061112
	static const unsigned int SIZE = 30;
// M061112E
	unsigned short year;
	unsigned char maxPR;
	unsigned char timeOutBeforeStart;
	unsigned int timeOutBeforeFinish;
	unsigned char numberFireToAnalize;
	unsigned char minimumDistanceForCompactJet;
	bool permissionTesting;
	unsigned char testingHour;
	unsigned char testingMinute;
	bool permissionTestingInfo;
// M061112
	int timeControlUserAction;
// M061112E
	Point3<float> protectedZone;
	bool tv;

	ConfigDataStructConst()
		:	maxPR(0),
			timeOutBeforeStart(0),
			timeOutBeforeFinish(0),
			numberFireToAnalize(0),
			minimumDistanceForCompactJet(0),
			permissionTesting(false),
			testingHour(0),
			testingMinute(0),
			permissionTestingInfo(false),
// M061112
			timeControlUserAction(0),
// M061112E
			protectedZone(Point3<float>()),
			tv(false)
	{
	}

	bool operator==(const ConfigDataStructConst& rh)
	{
		return ((maxPR == rh.maxPR) && 
			(timeOutBeforeStart == rh.timeOutBeforeStart) &&
			(timeOutBeforeFinish == rh.timeOutBeforeFinish) &&
			(numberFireToAnalize == rh.numberFireToAnalize) &&
			(minimumDistanceForCompactJet == rh.minimumDistanceForCompactJet) &&
			(permissionTesting == rh.permissionTesting) &&
			(testingHour == rh.testingHour) &&
			(testingMinute == rh.testingMinute) &&
			(permissionTestingInfo == rh.permissionTestingInfo) &&
// M061112
			(timeControlUserAction == rh.timeControlUserAction) &&
// M061112E
			(protectedZone == rh.protectedZone) &&
			(tv == rh.tv));
	}

	bool operator!=(const ConfigDataStructConst& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructConst(unsigned char** buffer)
	{
		year = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(year);

		maxPR = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(maxPR);

		timeOutBeforeStart = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(timeOutBeforeStart);

		timeOutBeforeFinish = *reinterpret_cast<unsigned int*>(*buffer);
		*buffer += sizeof(timeOutBeforeFinish);

		numberFireToAnalize = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(numberFireToAnalize);

		minimumDistanceForCompactJet = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(minimumDistanceForCompactJet);

		permissionTesting = *reinterpret_cast<bool*>(*buffer);
		*buffer += sizeof(permissionTesting);

		testingHour = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(testingHour);

		testingMinute = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(testingMinute);

		permissionTestingInfo = *reinterpret_cast<bool*>(*buffer);
		*buffer += sizeof(permissionTestingInfo);
// M061112
		timeControlUserAction = *reinterpret_cast<int*>(*buffer);
		*buffer += sizeof(timeControlUserAction);
// M061112E
		protectedZone = *reinterpret_cast<Point3<float>*>(*buffer);
		*buffer += sizeof(protectedZone);

		tv = *reinterpret_cast<bool*>(*buffer);
		*buffer += sizeof(bool);
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned short*>(*buffer) = year;
		*buffer += sizeof(year);

		*reinterpret_cast<unsigned char*>(*buffer) = maxPR;
		*buffer += sizeof(maxPR);

		*reinterpret_cast<unsigned char*>(*buffer) = timeOutBeforeStart;
		*buffer += sizeof(timeOutBeforeStart);

		*reinterpret_cast<unsigned int*>(*buffer) = timeOutBeforeFinish;
		*buffer += sizeof(timeOutBeforeFinish);

		*reinterpret_cast<unsigned char*>(*buffer) = numberFireToAnalize;
		*buffer += sizeof(numberFireToAnalize);

		*reinterpret_cast<unsigned char*>(*buffer) = minimumDistanceForCompactJet;
		*buffer += sizeof(minimumDistanceForCompactJet);

		*reinterpret_cast<bool*>(*buffer) = permissionTesting;
		*buffer += sizeof(permissionTesting);

		*reinterpret_cast<unsigned char*>(*buffer) = testingHour;
		*buffer += sizeof(testingHour);

		*reinterpret_cast<unsigned char*>(*buffer) = testingMinute;
		*buffer += sizeof(testingMinute);

		*reinterpret_cast<bool*>(*buffer) = permissionTestingInfo;
		*buffer += sizeof(permissionTestingInfo);
// M061112
		*reinterpret_cast<int*>(*buffer) = timeControlUserAction;
		*buffer += sizeof(timeControlUserAction);
// M061112E
		*reinterpret_cast<Point3<float>*>(*buffer) = protectedZone;
		*buffer += sizeof(protectedZone);

		*reinterpret_cast<bool*>(*buffer) = tv;
		*buffer += sizeof(tv);
	}
};

struct ConfigDataStructPRPosition{
	static const unsigned int SIZE = 19;
	unsigned char projectNumber;
	unsigned char address;
	Point3<float> position;
	Point2<float> orientation;
	unsigned char networkIndexNumber;
	Point3<float> axis;
	unsigned char zatvorCount;

	bool operator==(const ConfigDataStructPRPosition& rh){
		return ((projectNumber == rh.projectNumber) &&
			(address == rh.address) &&
			(position == rh.position) &&
			(orientation == rh.orientation) &&
			(networkIndexNumber == rh.networkIndexNumber) &&
			(axis == rh.axis) &&
			(zatvorCount == rh.zatvorCount));
	}

	bool operator!=(const ConfigDataStructPRPosition& rh){
		return !(*this == rh);
	}

	ConfigDataStructPRPosition()
		:	projectNumber(0),
			address(0),
			position(Point3<float>()),
			orientation(Point2<float>()),
			networkIndexNumber(0),
			axis(Point3<float>()),
			zatvorCount(2)
	{
	}

	ConfigDataStructPRPosition(unsigned char** buffer){
		projectNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(projectNumber);

		address = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(address);

		position = *reinterpret_cast<Point3<float>*>(*buffer);
		*buffer += sizeof(position);

		orientation = *reinterpret_cast<Point2<float>*>(*buffer);
		*buffer += sizeof(orientation);

		networkIndexNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(networkIndexNumber);

		axis = *reinterpret_cast<Point3<float>*>(*buffer);
		*buffer += sizeof(axis);

		zatvorCount = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(zatvorCount);
	}

	void serialize(unsigned char** buffer){
		*reinterpret_cast<unsigned char*>(*buffer) = projectNumber;
		*buffer += sizeof(projectNumber);

		*reinterpret_cast<unsigned char*>(*buffer) = address;
		*buffer += sizeof(address);

		*reinterpret_cast<Point3<float>*>(*buffer) = position;
		*buffer += sizeof(position);

		*reinterpret_cast<Point2<float>*>(*buffer) = orientation;
		*buffer += sizeof(orientation);

		*reinterpret_cast<unsigned char*>(*buffer) = networkIndexNumber;
		*buffer += sizeof(networkIndexNumber);

		*reinterpret_cast<Point3<float>*>(*buffer) = axis;
		*buffer += sizeof(axis);

		*reinterpret_cast<unsigned char*>(*buffer) = zatvorCount;
		*buffer += sizeof(zatvorCount);
	}
};

struct ConfigDataStructIOBk16
{
	static const unsigned int SIZE = 6;
	unsigned char bkAddress;
	unsigned char numberOnDevice;

	enum OUTPUT_FUNCTION_GROUP
	{
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

	enum INPUT_FUNCTION_GROUP
	{
		INPUT_FUNCTION_GROUP_UNDEFINED = 0,
		INPUT_FUNCTION_GROUP_NORMAL_OPEN = 1,
		INPUT_FUNCTION_GROUP_NORMAL_CLOSE = 2
	};
	INPUT_FUNCTION_GROUP inputFunctionGroup;

	unsigned char projectNumber;

	bool operator==(const ConfigDataStructIOBk16& rh)
	{
		return ((bkAddress == rh.bkAddress) &&
			(numberOnDevice == rh.numberOnDevice) &&
			(outputFunctionGroup == rh.outputFunctionGroup) &&
			(prGateNumber == rh.prGateNumber) && 
			(inputFunctionGroup == rh.inputFunctionGroup) &&
			(projectNumber == rh.projectNumber));
	}

	bool operator!=(const ConfigDataStructIOBk16& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructIOBk16()
		:	bkAddress(0),
			numberOnDevice(0),
			outputFunctionGroup(OUTPUT_FUNCTION_GROUP_UNDEFINED),
			prGateNumber(0),
			inputFunctionGroup(INPUT_FUNCTION_GROUP_UNDEFINED),
			projectNumber(0)
	{
	}

	ConfigDataStructIOBk16(unsigned char** buffer)
	{
		bkAddress = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(bkAddress);

		numberOnDevice = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(numberOnDevice);

		outputFunctionGroup = *reinterpret_cast<OUTPUT_FUNCTION_GROUP*>(*buffer);
		*buffer += sizeof(outputFunctionGroup);

		prGateNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(prGateNumber);

		inputFunctionGroup = *reinterpret_cast<INPUT_FUNCTION_GROUP*>(*buffer);
		*buffer += sizeof(inputFunctionGroup);

		projectNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(projectNumber);
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned char*>(*buffer) = bkAddress;
		*buffer += sizeof(bkAddress);

		*reinterpret_cast<unsigned char*>(*buffer) = numberOnDevice;
		*buffer += sizeof(numberOnDevice);

		*reinterpret_cast<OUTPUT_FUNCTION_GROUP*>(*buffer) = outputFunctionGroup;
		*buffer += sizeof(outputFunctionGroup);

		*reinterpret_cast<unsigned char*>(*buffer) = prGateNumber;
		*buffer += sizeof(prGateNumber);

		*reinterpret_cast<INPUT_FUNCTION_GROUP*>(*buffer) = inputFunctionGroup;
		*buffer += sizeof(inputFunctionGroup);

		*reinterpret_cast<unsigned char*>(*buffer) = projectNumber;
		*buffer += sizeof(projectNumber);
	}
};

struct ConfigDataStructIOSerial
{
	static const unsigned int SIZE = 3;
	unsigned char address;

	unsigned char normalState;

	unsigned char projectNumber;

	bool operator==(const ConfigDataStructIOSerial& rh)
	{
		return ((address == rh.address) &&
				(normalState == rh.normalState) &&
				(projectNumber == rh.projectNumber));
	}

	bool operator!=(const ConfigDataStructIOSerial& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructIOSerial()
		:	address(0),
			normalState(0),
			projectNumber(0)
	{
	}

	ConfigDataStructIOSerial(unsigned char** buffer)
	{
		address = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(address);

		normalState = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(normalState);

		projectNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(projectNumber);
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned char*>(*buffer) = address;
		*buffer += sizeof(address);

		*reinterpret_cast<unsigned char*>(*buffer) = normalState;
		*buffer += sizeof(normalState);

		*reinterpret_cast<unsigned char*>(*buffer) = projectNumber;
		*buffer += sizeof(projectNumber);
	}
};

struct ConfigDataStructInitSignal
{
// M061112
	static const unsigned int SIZE = 5;
// M061112
	LOGIC_FUNCTION function;
	unsigned short number;
	unsigned int firstInputNumber;
	unsigned int secondInputNumber;
// M061112
	unsigned int thirdInputNumber;
// M061112
	bool ignorable;

	ConfigDataStructInitSignal()
		:	function(LOGIC_FUNCTION_COOLING_LINE), number(0), firstInputNumber(0), secondInputNumber(0), 
// M061112
		thirdInputNumber(0), 
// M061112E
		ignorable(false)
	{
	}

	bool operator==(const ConfigDataStructInitSignal& rh)
	{
		return ((function == rh.function) && (number == rh.number) && (firstInputNumber == rh.firstInputNumber) && 
			(secondInputNumber == rh.secondInputNumber)
// M061112
			&& (thirdInputNumber == rh.thirdInputNumber)
// M061112E
			);
	}

	bool operator!=(const ConfigDataStructInitSignal& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructInitSignal(unsigned char** buffer)
		:	ignorable(false)
	{
		function = *reinterpret_cast<LOGIC_FUNCTION*>(*buffer);
		*buffer += sizeof(function);

		number = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(number);

		firstInputNumber = *reinterpret_cast<unsigned int*>(*buffer);
		*buffer += sizeof(firstInputNumber);

		secondInputNumber = *reinterpret_cast<unsigned int*>(*buffer);
		*buffer += sizeof(secondInputNumber);
// M061112
		thirdInputNumber = *reinterpret_cast<unsigned int*>(*buffer);
		*buffer += sizeof(thirdInputNumber);
// M061112E
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<LOGIC_FUNCTION*>(*buffer) = function;
		*buffer += sizeof(function);

		*reinterpret_cast<unsigned short*>(*buffer) = number;
		*buffer += sizeof(number);

		*reinterpret_cast<unsigned int*>(*buffer) = firstInputNumber;
		*buffer += sizeof(firstInputNumber);

		*reinterpret_cast<unsigned int*>(*buffer) = secondInputNumber;
		*buffer += sizeof(secondInputNumber);
// M061112
		*reinterpret_cast<unsigned int*>(*buffer) = thirdInputNumber;
		*buffer += sizeof(thirdInputNumber);
// M061112E
	}
};

struct ConfigDataStructProgram
{
	static const unsigned int SIZE = 15;
	unsigned int initSignalNumber; 
	unsigned char prNumber;

	LOGIC_FUNCTION function;

	Point2<unsigned int> point1;
	Point2<unsigned int> point2;

	unsigned short nPointProgram;

	unsigned int nasadok;

	bool operator==(const ConfigDataStructProgram& rh)
	{
		return ((initSignalNumber == rh.initSignalNumber) && 
			(prNumber == rh.prNumber) &&
			(function == rh.function) &&
			(point1 == rh.point1) &&
			(point2 == rh.point2) &&
			(nPointProgram == rh.nPointProgram) &&
			(nasadok == rh.nasadok));
	}

	bool operator!=(const ConfigDataStructProgram& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructProgram()
		:	initSignalNumber(0), 
			prNumber(0),
			function(LOGIC_FUNCTION_SEARCHING),
			point1(Point2<unsigned int>()),
			point2(Point2<unsigned int>()),
			nPointProgram(0),
			nasadok(0)
	{
	}

	ConfigDataStructProgram(unsigned char** buffer)
	{
		initSignalNumber = *reinterpret_cast<unsigned int*>(*buffer);
		*buffer += sizeof(initSignalNumber);

		prNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(prNumber);

		function = *reinterpret_cast<LOGIC_FUNCTION*>(*buffer);
		*buffer += sizeof(function);

		point1 = *reinterpret_cast<Point2<unsigned int>*>(*buffer);
		*buffer += sizeof(point1);

		point2 = *reinterpret_cast<Point2<unsigned int>*>(*buffer);
		*buffer += sizeof(point2);

		nPointProgram = *reinterpret_cast<unsigned int*>(*buffer);
		*buffer += sizeof(nPointProgram);

		nasadok = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(nasadok);
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned int*>(*buffer) = initSignalNumber;
		*buffer += sizeof(initSignalNumber);

		*reinterpret_cast<unsigned char*>(*buffer) = prNumber;
		*buffer += sizeof(prNumber);

		*reinterpret_cast<LOGIC_FUNCTION*>(*buffer) = function;
		*buffer += sizeof(function);

		*reinterpret_cast<Point2<unsigned int>*>(*buffer) = point1;
		*buffer += sizeof(point1);

		*reinterpret_cast<Point2<unsigned int>*>(*buffer) = point2;
		*buffer += sizeof(point2);

		*reinterpret_cast<unsigned int*>(*buffer) = nPointProgram;
		*buffer += sizeof(nPointProgram);

		*reinterpret_cast<unsigned short*>(*buffer) = nasadok;
		*buffer += sizeof(nasadok);
	}
};

struct ConfigDataStructFv300
{
	static const unsigned int SIZE = 13;

	unsigned char address;
	unsigned char prNumber;
	unsigned char projectNumber;

	Point3<float> position;
	Point2<float> orientation;

	ConfigDataStructFv300()
		:	address(0),
			prNumber(0),
			projectNumber(0),
			position(Point3<float>()),
			orientation(Point2<float>())
	{
	}

	bool operator==(const ConfigDataStructFv300& rh)
	{
		return ((address == rh.address) && 
			(prNumber == rh.prNumber) && 
			(projectNumber == rh.projectNumber) &&
			(position == rh.position) &&
			(orientation == rh.orientation));
	}

	bool operator!=(const ConfigDataStructFv300& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructFv300(unsigned char** buffer)
	{
		address = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(address);

		prNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(prNumber);

		projectNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(projectNumber);

		position = *reinterpret_cast<Point3<float>*>(*buffer);
		*buffer += sizeof(position);

		orientation = *reinterpret_cast<Point2<float>*>(*buffer);
		*buffer += sizeof(orientation);
	}

	void serialize(unsigned char** buffer)
	{
		
		*reinterpret_cast<unsigned char*>(*buffer) = address;
		*buffer += sizeof(address);

		*reinterpret_cast<unsigned char*>(*buffer) = prNumber;
		*buffer += sizeof(prNumber);

		*reinterpret_cast<unsigned char*>(*buffer) = projectNumber;
		*buffer += sizeof(projectNumber);

		*reinterpret_cast<Point3<float>*>(*buffer) = position;
		*buffer += sizeof(position);

		*reinterpret_cast<Point2<float>*>(*buffer) = orientation;
		*buffer += sizeof(orientation);
	}
};

struct ConfigDataStructTrajectory
{
// M061112
	static const unsigned int SIZE = 12;
// M061112E
	unsigned char prNumber;
	unsigned char trajectoryNumber;
	unsigned short pointNumber;

	Point2<unsigned short> position;
	unsigned short nasadok;
// M061112
	unsigned short pressureNumber;
// M061112E
	ConfigDataStructTrajectory()
		:	prNumber(0),
			trajectoryNumber(0),
			pointNumber(0),
			position(Point2<unsigned short>()),
			nasadok(0),
			pressureNumber(0)
	{
	}

	bool operator==(const ConfigDataStructTrajectory& rh)
	{
		return ((prNumber == rh.prNumber) && 
			(trajectoryNumber == rh.trajectoryNumber) &&
			(pointNumber == rh.pointNumber) &&
			(position == rh.position) &&
			(nasadok == rh.nasadok) &&
			(pressureNumber == rh.pressureNumber));
	}

	bool operator!=(const ConfigDataStructTrajectory& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructTrajectory(unsigned char** buffer)
	{
		prNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(prNumber);

		trajectoryNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(trajectoryNumber);

		pointNumber = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(pointNumber);

		position = *reinterpret_cast<Point2<unsigned short>*>(*buffer);
		*buffer += sizeof(position);

		nasadok = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(nasadok);
// M061112
		pressureNumber = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(pressureNumber);
// M061112E
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned char*>(*buffer) = prNumber;
		*buffer += sizeof(prNumber);

		*reinterpret_cast<unsigned char*>(*buffer) = trajectoryNumber;
		*buffer += sizeof(trajectoryNumber);

		*reinterpret_cast<unsigned short*>(*buffer) = pointNumber;
		*buffer += sizeof(pointNumber);

		*reinterpret_cast<Point2<unsigned short>*>(*buffer) = position;
		*buffer += sizeof(position);

		*reinterpret_cast<unsigned short*>(*buffer) = nasadok;
		*buffer += sizeof(nasadok);
// M061112
		*reinterpret_cast<unsigned short*>(*buffer) = pressureNumber;
		*buffer += sizeof(pressureNumber);
// M061112E
	}
};

struct ConfigDataStructPressure
{
// M061112
	static const unsigned int SIZE = 5;
// M061112E
	unsigned char prNumber;
// M061112
	unsigned short arNumber;
// M061112E
	unsigned char pressure;
	unsigned char delta;

	ConfigDataStructPressure()
		:	prNumber(0), arNumber(0), pressure(0), delta(0)
	{
	}

	bool operator==(const ConfigDataStructPressure& rh)
	{
		return ((prNumber == rh.prNumber) && (arNumber == rh.arNumber) && (pressure == rh.pressure) && (delta == rh.delta));
	}

	bool operator!=(const ConfigDataStructPressure& rh)
	{
		return !(*this == rh);
	}

	ConfigDataStructPressure(unsigned char** buffer)
	{
		prNumber = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(prNumber);
// M061112
		arNumber = *reinterpret_cast<unsigned short*>(*buffer);
		*buffer += sizeof(arNumber);
// M061112E
		pressure = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(pressure);

		delta = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(delta);
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned char*>(*buffer) = prNumber;
		*buffer += sizeof(prNumber);
// M061112
		*reinterpret_cast<unsigned short*>(*buffer) = arNumber;
		*buffer += sizeof(arNumber);
// M061112E
		*reinterpret_cast<unsigned char*>(*buffer) = pressure;
		*buffer += sizeof(pressure);

		*reinterpret_cast<unsigned char*>(*buffer) = delta;
		*buffer += sizeof(delta);
	}
};

struct ConfigDataStructPenaBak{
	static const unsigned int SIZE = 4;
	unsigned char number;
	unsigned char level;
	unsigned char address;
	unsigned char numberOnDevice;

	bool operator==(const ConfigDataStructPenaBak& rh){
		return ((number == rh.number) &&
			(address == rh.address) &&
			(numberOnDevice == rh.numberOnDevice) &&
			(level == rh.level));
	}

	bool operator!=(const ConfigDataStructPenaBak& rh){
		return !(*this == rh);
	}

	ConfigDataStructPenaBak()
		:	number(0),
			level(0),
			address(0),
			numberOnDevice(0)
	{}

	ConfigDataStructPenaBak(unsigned char** buffer)
	{
		number = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(number);

		level = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(level);

		address = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(address);

		numberOnDevice = *reinterpret_cast<unsigned char*>(*buffer);
		*buffer += sizeof(numberOnDevice);
	}

	void serialize(unsigned char** buffer)
	{
		*reinterpret_cast<unsigned char*>(*buffer) = number;
		*buffer += sizeof(number);

		*reinterpret_cast<unsigned char*>(*buffer) = level;
		*buffer += sizeof(level);

		*reinterpret_cast<unsigned char*>(*buffer) = address;
		*buffer += sizeof(address);

		*reinterpret_cast<unsigned char*>(*buffer) = numberOnDevice;
		*buffer += sizeof(numberOnDevice);
	}
};

struct ConfigDataStruct
{
	ConfigDataStructConst* _Const;

	static const unsigned int MAX_NUMBER_PR_POSITION = 64;
	unsigned int number_PRPositions;
	ConfigDataStructPRPosition** _PRPositions;

	static const unsigned int MAX_NUMBER_IOBK16 = 160;
	unsigned int number_IOBk16;
	ConfigDataStructIOBk16** _IOBk16;

	static const unsigned int MAX_NUMBER_IOSERIAL = 160;
	unsigned int number_IOSerial;
	ConfigDataStructIOSerial** _IOSerial;

	static const unsigned int MAX_NUMBER_INIT_SIGNALS = 500;
	unsigned int number_InitSignals;
	ConfigDataStructInitSignal** _InitSignals;

	static const unsigned int MAX_NUMBER_PROGRAMS = 500;
	unsigned int number_Programs;
	ConfigDataStructProgram** _Programs;

	static const unsigned int MAX_NUMBER_FV300 = 500;
	unsigned int number_Fv300;
	ConfigDataStructFv300** _Fv300;

	static const unsigned int MAX_NUMBER_Trajectory = 2000;
	unsigned int number_Trajectory;
	ConfigDataStructTrajectory** _Trajectory;

	static const unsigned int MAX_NUMBER_PRESSURE = 4000;
	unsigned int number_Pressure;
	ConfigDataStructPressure** _Pressure;

	static const unsigned int MAX_NUMBER_PENABAK = 512;
	unsigned int number_PenaBak;
	ConfigDataStructPenaBak** _PenaBak;

	static const unsigned int MAGIC_CODE = 0x19b7f4d3;

	void serialize(unsigned char* _buffer)
	{
		*reinterpret_cast<unsigned int*>(_buffer) = MAGIC_CODE;
		_buffer += sizeof(unsigned int);
		
		_Const->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_PRPositions;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_PRPositions; i++)
			_PRPositions[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_IOBk16;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_IOBk16; i++)
			_IOBk16[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_IOSerial;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_IOSerial; i++)
			_IOSerial[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_InitSignals;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_InitSignals; i++)
			_InitSignals[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_Programs;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_Programs; i++)
			_Programs[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_Fv300;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_Fv300; i++)
			_Fv300[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_Trajectory;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_Trajectory; i++)
			_Trajectory[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_Pressure;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_Pressure; i++)
			_Pressure[i]->serialize(&_buffer);

		*reinterpret_cast<unsigned int*>(_buffer) = number_PenaBak;
		_buffer += sizeof(unsigned int);
		for (unsigned int i = 0; i < number_PenaBak; i++)
			_PenaBak[i]->serialize(&_buffer);
	}

	ConfigDataStruct()
		:	_Const(nullptr), 
			number_PRPositions(0), _PRPositions(nullptr),
			number_IOBk16(0), _IOBk16(nullptr),
			number_IOSerial(0), _IOSerial(nullptr),
			number_InitSignals(0), _InitSignals(nullptr),
			number_Programs(0), _Programs(nullptr),
			number_Fv300(0), _Fv300(nullptr),
			number_Trajectory(0), _Trajectory(nullptr),
			number_Pressure(0), _Pressure(nullptr),
			number_PenaBak(0), _PenaBak(nullptr)
	{
	}

	ConfigDataStruct(unsigned char* _buffer)
	{
		if (*reinterpret_cast<unsigned int*>(_buffer) == MAGIC_CODE)
		{
			_buffer += sizeof(unsigned int);

			_Const = new ConfigDataStructConst(&_buffer);

			number_PRPositions = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_PRPositions < MAX_NUMBER_PR_POSITION)
			{
				_PRPositions = new ConfigDataStructPRPosition*[number_PRPositions];
				for (unsigned int i = 0; i < number_PRPositions; i++)
					_PRPositions[i] = new ConfigDataStructPRPosition(&_buffer);
			}
			else
				number_PRPositions = 0;

			number_IOBk16 = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_IOBk16 < MAX_NUMBER_IOBK16)
			{
				_IOBk16 = new ConfigDataStructIOBk16*[number_IOBk16];
				for (unsigned int i = 0; i < number_IOBk16; i++)
					_IOBk16[i] = new ConfigDataStructIOBk16(&_buffer);
			}
			else
				number_IOBk16 = 0;
		
			number_IOSerial = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_IOSerial < MAX_NUMBER_IOSERIAL)
			{
				_IOSerial = new ConfigDataStructIOSerial*[number_IOSerial];
				for (unsigned int i = 0; i < number_IOSerial; i++)
					_IOSerial[i] = new ConfigDataStructIOSerial(&_buffer);
			}
			else
				_IOSerial = 0;

			number_InitSignals = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_InitSignals < MAX_NUMBER_INIT_SIGNALS)
			{
				_InitSignals = new ConfigDataStructInitSignal*[number_InitSignals];
				for (unsigned int i = 0; i < number_InitSignals; i++)
					_InitSignals[i] = new ConfigDataStructInitSignal(&_buffer);
			}
			else
				number_InitSignals = 0;

			number_Programs = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_Programs < MAX_NUMBER_PROGRAMS)
			{
				_Programs = new ConfigDataStructProgram*[number_Programs];
				for (unsigned int i = 0; i < number_Programs; i++)
					_Programs[i] = new ConfigDataStructProgram(&_buffer);
			}
			else
				number_Programs = 0;

			number_Fv300 = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_Fv300 < MAX_NUMBER_FV300)
			{
				_Fv300 = new ConfigDataStructFv300*[number_Fv300];
				for (unsigned int i = 0; i < number_Fv300; i++)
					_Fv300[i] = new ConfigDataStructFv300(&_buffer);
			}
			else
				number_Fv300 = 0;

			number_Trajectory = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_Trajectory < MAX_NUMBER_Trajectory)
			{
				_Trajectory = new ConfigDataStructTrajectory*[number_Trajectory];
				for (unsigned int i = 0; i < number_Trajectory; i++)
					_Trajectory[i] = new ConfigDataStructTrajectory(&_buffer);
			}
			else
				number_Trajectory = 0;

			number_Pressure = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_Pressure < MAX_NUMBER_PRESSURE)
			{
				_Pressure = new ConfigDataStructPressure*[number_Pressure];
				for (unsigned int i = 0; i < number_Pressure; i++)
					_Pressure[i] = new ConfigDataStructPressure(&_buffer);
			}
			else
				number_Pressure = 0;

			number_PenaBak = *reinterpret_cast<unsigned int*>(_buffer);
			_buffer += sizeof(unsigned int);
			if (number_PenaBak < MAX_NUMBER_PENABAK)
			{
				_PenaBak = new ConfigDataStructPenaBak*[number_PenaBak];
				for (unsigned int i = 0; i < number_PenaBak; i++)
					_PenaBak[i] = new ConfigDataStructPenaBak(&_buffer);
			}
			else
				number_PenaBak = 0;
		}
		else
		{
			_Const = new ConfigDataStructConst();
			number_PRPositions = 0;
			number_IOBk16 = 0;
			number_IOSerial = 0;
			number_InitSignals = 0;
			number_Programs = 0;
			number_Fv300 = 0;
			number_Trajectory = 0;
			number_Pressure = 0;
			number_PenaBak = 0;
		}
	}

	bool operator==(const ConfigDataStruct& rh)
	{
		bool result = true;

		if ((_Const == nullptr) && (rh._Const != nullptr))
			result = false;
		if ((_Const != nullptr) && (rh._Const == nullptr))
			result = false;
		if ((_Const != nullptr) && (rh._Const != nullptr) && (*_Const != *rh._Const))
			result = false;

		if (number_PRPositions != rh.number_PRPositions)
			result = false;
		if ((_PRPositions == nullptr) && (rh._PRPositions != nullptr))
			result = false;
		if ((_PRPositions != nullptr) && (rh._PRPositions == nullptr))
			result = false;
		if ((_PRPositions != nullptr) && (rh._PRPositions != nullptr))
		{
			for (unsigned int i = 0; i < number_PRPositions; i++)
				if (*_PRPositions[i] != *rh._PRPositions[i])
					result = false;
		}

		if (number_IOBk16 != rh.number_IOBk16)
			result = false;
		if ((_IOBk16 == nullptr) && (rh._IOBk16 != nullptr))
			result = false;
		if ((_IOBk16 != nullptr) && (rh._IOBk16 == nullptr))
			result = false;
		if ((_IOBk16 != nullptr) && (rh._IOBk16 != nullptr))
		{
			for (unsigned int i = 0; i < number_IOBk16; i++)
				if (*_IOBk16[i] != *rh._IOBk16[i])
					result = false;
		}

		if (number_IOSerial != rh.number_IOSerial)
			result = false;
		if ((_IOSerial == nullptr) && (rh._IOSerial != nullptr))
			result = false;
		if ((_IOSerial != nullptr) && (rh._IOSerial == nullptr))
			result = false;
		if ((_IOSerial != nullptr) && (rh._IOSerial != nullptr))
		{
			for (unsigned int i = 0; i < number_IOSerial; i++)
				if (*_IOSerial[i] != *rh._IOSerial[i])
					result = false;
		}
		
		if (number_InitSignals != rh.number_InitSignals)
			result = false;
		if ((_InitSignals == nullptr) && (rh._InitSignals != nullptr))
			result = false;
		if ((_InitSignals != nullptr) && (rh._InitSignals == nullptr))
			result = false;
		if ((_InitSignals != nullptr) && (rh._InitSignals != nullptr))
		{
			for (unsigned int i = 0; i < number_InitSignals; i++)
				if (*_InitSignals[i] != *rh._InitSignals[i])
					result = false;
		}
		
		if (number_Programs != rh.number_Programs)
			result = false;
		if ((_Programs == nullptr) && (rh._Programs != nullptr))
			result = false;
		if ((_Programs != nullptr) && (rh._Programs == nullptr))
			result = false;
		if ((_Programs != nullptr) && (rh._Programs != nullptr))
		{
			for (unsigned int i = 0; i < number_Programs; i++)
				if (*_Programs[i] != *rh._Programs[i])
					result = false;
		}
		
		if (number_Fv300 != rh.number_Fv300)
			result = false;
		if ((_Fv300 == nullptr) && (rh._Fv300 != nullptr))
			result = false;
		if ((_Fv300 != nullptr) && (rh._Fv300 == nullptr))
			result = false;
		if ((_Fv300 != nullptr) && (rh._Fv300 != nullptr))
		{
			for (unsigned int i = 0; i < number_Fv300; i++)
				if (*_Fv300[i] != *rh._Fv300[i])
					result = false;
		}
		
		if (number_Trajectory != rh.number_Trajectory)
			result = false;
		if ((_Trajectory == nullptr) && (rh._Trajectory != nullptr))
			result = false;
		if ((_Trajectory != nullptr) && (rh._Trajectory == nullptr))
			result = false;
		if ((_Trajectory != nullptr) && (rh._Trajectory != nullptr))
		{
			for (unsigned int i = 0; i < number_Trajectory; i++)
				if (*_Trajectory[i] != *rh._Trajectory[i])
					result = false;
		}
		
		if (number_Pressure != rh.number_Pressure)
			result = false;
		if ((_Pressure == nullptr) && (rh._Pressure != nullptr))
			result = false;
		if ((_Pressure != nullptr) && (rh._Pressure == nullptr))
			result = false;
		if ((_Pressure != nullptr) && (rh._Pressure != nullptr))
		{
			for (unsigned int i = 0; i < number_Pressure; i++)
				if (*_Pressure[i] != *rh._Pressure[i])
					result = false;
		}
		
		if (number_PenaBak != rh.number_PenaBak)
			result = false;
		if ((_PenaBak == nullptr) && (rh._PenaBak != nullptr))
			result = false;
		if ((_PenaBak != nullptr) && (rh._PenaBak == nullptr))
			result = false;
		if ((_PenaBak != nullptr) && (rh._PenaBak != nullptr))
		{
			for (unsigned int i = 0; i < number_PenaBak; i++)
				if (*_PenaBak[i] != *rh._PenaBak[i])
					result = false;
		}
	
		return result;
	}
};

#endif
