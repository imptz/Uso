#include "ConfigData.h"
#include "../log/Log.h"
#include "../DEBUG/serialDebug.h"

ConfigData::ConfigData()
	:	ITimer(TIMER_PERIOD), cData(nullptr), cDataNew(nullptr), phase(CONFIG_DATA_PHASE_READ_1), buffer(new unsigned char[BUFFER_SIZE])
{
	pTimer->start();
}

ConfigData::~ConfigData()
{
	delete cData;
}

bool ConfigData::setNewData(unsigned char* data, unsigned int length)
{
//Display::getSingleton().printUInt(length, 0, 0);
//Display::getSingleton().printMemoryDump(reinterpret_cast<unsigned int>(data), 128, 0, 2);
//_asm cli
//for(;;);
//
	memcpy(buffer, data, length);
	
	dataSave = false;

	setData(buffer, length);
	SAFE_DELETE(cData)
	cData = cDataNew;
	cDataNew = nullptr;
	cData->serialize(buffer);
	phase = CONFIG_DATA_PHASE_WRITE_1;
	pTimer->start();

	return true;
}

bool ConfigData::isDataSave()
{
	return dataSave;
}

void ConfigData::timerHandler()
{
	switch (phase)
	{
		case CONFIG_DATA_PHASE_WRITE_1:
			taskId = HddManager::getSingleton().write(buffer, START_SECTOR, BUFFER_SIZE / 512);
			if (taskId != HddManager::UNDEFINED_ID) 
				phase = CONFIG_DATA_PHASE_WRITE_2;
			break;
		case CONFIG_DATA_PHASE_WRITE_2:
			if (!HddManager::getSingleton().isTaskExecute(taskId))
			{
				pTimer->stop();
				dataSave = true;
				phase = CONFIG_DATA_PHASE_STOP;
			}
			break;
		case CONFIG_DATA_PHASE_READ_1:
			taskId = HddManager::getSingleton().read(buffer, START_SECTOR, BUFFER_SIZE / 512);
			if (taskId != HddManager::UNDEFINED_ID) 
				phase = CONFIG_DATA_PHASE_READ_2;

			break;
		case CONFIG_DATA_PHASE_READ_2:
			if (!HddManager::getSingleton().isTaskExecute(taskId))
			{
				pTimer->stop();

				SAFE_DELETE(cData)
				cData = new ConfigDataStruct(buffer);
				phase = CONFIG_DATA_PHASE_STOP;
			}
			break;
	}
}

ConfigDataStructConst* ConfigData::getConfigDataStructConst()
{
	return cData->_Const;
}

void ConfigData::setConfigDataStructPRPositionCount(unsigned int count)
{
	cData->number_PRPositions = count;
	cData->_PRPositions = new ConfigDataStructPRPosition*[count];
}

unsigned int ConfigData::getConfigDataStructPRPositionCount()
{
	return cData->number_PRPositions;
}

ConfigDataStructPRPosition** ConfigData::getConfigDataStructPRPositions()
{
	return cData->_PRPositions;
}

unsigned int ConfigData::getConfigDataStructIOBk16Count()
{
	return cData->number_IOBk16;
}

void ConfigData::setConfigDataStructIOBk16Count(unsigned int count)
{
	cData->number_IOBk16 = count;
	cData->_IOBk16 = new ConfigDataStructIOBk16*[count];
}

ConfigDataStructIOBk16** ConfigData::getConfigDataStructIOBk16()
{
	return cData->_IOBk16;
}

unsigned int ConfigData::getConfigDataStructIOSerialCount()
{
	return cData->number_IOSerial;
}

void ConfigData::setConfigDataStructIOSerialCount(unsigned int count)
{
	cData->number_IOSerial = count;
	cData->_IOSerial = new ConfigDataStructIOSerial*[count];
}

ConfigDataStructIOSerial** ConfigData::getConfigDataStructIOSerial()
{
	return cData->_IOSerial;
}

unsigned int ConfigData::getConfigDataStructInitSignalsCount()
{
	return cData->number_InitSignals;
}

void ConfigData::setConfigDataStructInitSignalsCount(unsigned int count)
{
	cData->number_InitSignals = count;
	cData->_InitSignals = new ConfigDataStructInitSignal*[count];
}

ConfigDataStructInitSignal** ConfigData::getConfigDataStructInitSignals()
{
	return cData->_InitSignals;
}

unsigned int ConfigData::getConfigDataStructProgramCount()
{
	return cData->number_Programs;
}

void ConfigData::setConfigDataStructProgramCount(unsigned int count)
{
	cData->number_Programs = count;
	cData->_Programs = new ConfigDataStructProgram*[count];
}

ConfigDataStructProgram** ConfigData::getConfigDataStructPrograms()
{
	return cData->_Programs;
}

unsigned int ConfigData::getConfigDataStructProgramInitSignal(unsigned int initSignalNumber, LOGIC_FUNCTION _function, unsigned int** pList)
{
	LOGIC_FUNCTION _function1 = LOGIC_FUNCTION_UNDEFINED;
	if (_function == LOGIC_FUNCTION_SEARCHING)
		_function1 = LOGIC_FUNCTION_SEARCHING_PENA;
	else if (_function == LOGIC_FUNCTION_SEARCHING_PENA)
		_function1 = LOGIC_FUNCTION_SEARCHING;

	unsigned int count = 0;
	for (unsigned int i = 0; i < cData->number_Programs; i++)
	{
		if ((cData->_Programs[i]->initSignalNumber == initSignalNumber) && ((cData->_Programs[i]->function == _function) || (cData->_Programs[i]->function == _function1)))
			count++;
	}

	if (count == 0)
		return 0;

	*pList = new unsigned int[count];
	unsigned int resultIndex = 0;
	for (unsigned int i = 0; i < cData->number_Programs; i++)
		if ((cData->_Programs[i]->initSignalNumber == initSignalNumber) && ((cData->_Programs[i]->function == _function) || (cData->_Programs[i]->function == _function1)))
		{
			(*pList)[resultIndex++] = i;
		}

	return count;
}

unsigned int ConfigData::getConfigDataStructFv300Count()
{
	return cData->number_Fv300;
}

void ConfigData::setConfigDataStructFv300Count(unsigned int count)
{
	cData->number_Fv300 = count;
	cData->_Fv300 = new ConfigDataStructFv300*[count];
}

ConfigDataStructFv300** ConfigData::getConfigDataStructFv300()
{
	return cData->_Fv300;
}

unsigned int ConfigData::getConfigDataStructTrajectoryCount()
{
	return cData->number_Trajectory;
}

ConfigDataStructTrajectory** ConfigData::getConfigDataStructTrajectory()
{
	return cData->_Trajectory;
}

unsigned int ConfigData::getConfigDataStructPressureCount()
{
	return cData->number_Pressure;
}

ConfigDataStructPressure** ConfigData::getConfigDataStructPressure()
{
	return cData->_Pressure;
}

unsigned int ConfigData::getConfigDataStructPenaBakCount()
{
	return cData->number_PenaBak;
}

ConfigDataStructPenaBak** ConfigData::getConfigDataStructPenaBak()
{
	return cData->_PenaBak;
}

void ConfigData::setData(unsigned char* data, unsigned int length)
{
	SAFE_DELETE(cDataNew)

	cDataNew = new ConfigDataStruct();
//Display::getSingleton().printUInt(0, 0, 10);
#pragma region ConfigDataStructConst

	cDataNew->_Const = new ConfigDataStructConst();
	cDataNew->_Const->year = Clock::getSingleton().getClock().year;

	data += 6;
	cDataNew->_Const->maxPR = data[0];
	cDataNew->_Const->timeOutBeforeStart = data[1];
	cDataNew->_Const->timeOutBeforeFinish = data[2] * 60;
	cDataNew->_Const->numberFireToAnalize = data[3];
	cDataNew->_Const->minimumDistanceForCompactJet = data[4];
	if (data[5] == 1)
		cDataNew->_Const->permissionTesting = true;
	else
		cDataNew->_Const->permissionTesting = false;
	cDataNew->_Const->testingHour = data[6];
	cDataNew->_Const->testingMinute = data[7];
	
	if (data[8] == 1)
		cDataNew->_Const->permissionTestingInfo = true;
	else
		cDataNew->_Const->permissionTestingInfo = false;
// M061112
	cDataNew->_Const->timeControlUserAction = data[9];
	cDataNew->_Const->timeControlUserAction *= 60;

	unsigned int x = (data[10] << 8) + data[11];
	unsigned int y = (data[12] << 8) + data[13];
	unsigned int z = (data[14] << 8) + data[15];
	cDataNew->_Const->protectedZone = Point3<float>(static_cast<float>(x),static_cast<float>(y),static_cast<float>(z));

	x = (data[17] << 8) + data[16];
// M061112E
	if (x != 0)
		cDataNew->_Const->tv = true;
	else
		cDataNew->_Const->tv = false;

	data += ConfigDataStructConst::SIZE;
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(1, 0, 11);
#pragma region ConfigDataStructPRPosition

	data += 2;
	cDataNew->number_PRPositions = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructPRPosition::SIZE;
	data += 4;
	cDataNew->_PRPositions = new ConfigDataStructPRPosition*[cDataNew->number_PRPositions];

	for (unsigned int i = 0; i < cDataNew->number_PRPositions;i++)
	{
		cDataNew->_PRPositions[i] = new ConfigDataStructPRPosition();

		cDataNew->_PRPositions[i]->projectNumber = data[0];
		cDataNew->_PRPositions[i]->address = data[1];
		unsigned int x = (data[3] << 8) + data[2];
		unsigned int y = (data[5] << 8) + data[4];
		unsigned int z = (data[7] << 8) + data[6];
		cDataNew->_PRPositions[i]->position = Point3<float>(static_cast<float>(x),static_cast<float>(y),static_cast<float>(z));
		x = (data[9] << 8) + data[8];
		y = data[10];
		cDataNew->_PRPositions[i]->orientation = Point2<float>(static_cast<float>(x),static_cast<float>(y));
		cDataNew->_PRPositions[i]->networkIndexNumber = data[11];
		x = (data[13] << 8) + data[12];
		y = (data[15] << 8) + data[14];
		z = (data[17] << 8) + data[16];
		cDataNew->_PRPositions[i]->axis = Point3<float>(static_cast<float>(x),static_cast<float>(y),static_cast<float>(z));

		cDataNew->_PRPositions[i]->zatvorCount = data[18];
		data += ConfigDataStructPRPosition::SIZE;
	}
#pragma endregion 
	//=====================================================
//Display::getSingleton().printUInt(2, 0, 12);
#pragma region ConfigDataStructIOBk16
	data += 2;
	cDataNew->number_IOBk16 = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructIOBk16::SIZE;

	data += 4;
	cDataNew->_IOBk16 = new ConfigDataStructIOBk16*[cDataNew->number_IOBk16];

	for (unsigned int i = 0; i < cDataNew->number_IOBk16;i++)
	{
		cDataNew->_IOBk16[i] = new ConfigDataStructIOBk16();

		cDataNew->_IOBk16[i]->bkAddress = data[0];
		cDataNew->_IOBk16[i]->numberOnDevice = data[1];

		switch (data[2])
		{
			case 0:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_UNDEFINED;
				break;
			case 1:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_FIRE_ALARM;
				break;
			case 2:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_HARDWARE;
				break;
			case 3:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PUMPING_STATION;
				break;
			case 4:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PR_PRESSURE_UP;
				break;
			case 5:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_GATE_OPEN;
				break;
			case 6:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_SYSTEM_FAULT;
				break;
			case 7:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PR_FAULT;
				break;
			case 8:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_GATE_FAULT;
				break;
			case 9:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_SBROS_SIGNALIZACII;
				break;
			case 10:
				cDataNew->_IOBk16[i]->outputFunctionGroup = ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_BK_ZATVOR;
				break;
		}

		cDataNew->_IOBk16[i]->prGateNumber = data[3];

		switch (data[4])
		{
			case 0:
				cDataNew->_IOBk16[i]->inputFunctionGroup = ConfigDataStructIOBk16::INPUT_FUNCTION_GROUP_UNDEFINED;
				break;
			case 1:
				cDataNew->_IOBk16[i]->inputFunctionGroup = ConfigDataStructIOBk16::INPUT_FUNCTION_GROUP_NORMAL_OPEN;
				break;
			case 2:
				cDataNew->_IOBk16[i]->inputFunctionGroup = ConfigDataStructIOBk16::INPUT_FUNCTION_GROUP_NORMAL_CLOSE;
				break;
		}

		cDataNew->_IOBk16[i]->projectNumber = data[5];

		data += ConfigDataStructIOBk16::SIZE;
	}
#pragma endregion 
	//=====================================================
//Display::getSingleton().printUInt(3, 0, 13);
#pragma region ConfigDataStructIOSerial
	data += 2;
	cDataNew->number_IOSerial = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructIOSerial::SIZE;
	data += 4;
	cDataNew->_IOSerial = new ConfigDataStructIOSerial*[cDataNew->number_IOSerial];

	for (unsigned int i = 0; i < cDataNew->number_IOSerial;i++)
	{
		cDataNew->_IOSerial[i] = new ConfigDataStructIOSerial();

		cDataNew->_IOSerial[i]->address = data[0];
		cDataNew->_IOSerial[i]->normalState = data[1];
		cDataNew->_IOSerial[i]->projectNumber = data[2];

		data += ConfigDataStructIOSerial::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(5, 0, 14);
#pragma region ConfigDataStructInitSignal
	data += 2;
	cDataNew->number_InitSignals = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructInitSignal::SIZE;
	data += 4;
	cDataNew->_InitSignals = new ConfigDataStructInitSignal*[cDataNew->number_InitSignals];

	for (unsigned int i = 0; i < cDataNew->number_InitSignals;i++)
	{
		cDataNew->_InitSignals[i] = new ConfigDataStructInitSignal();

		unsigned int x = (data[1] << 8) + data[0];
		cDataNew->_InitSignals[i]->number = x;
		cDataNew->_InitSignals[i]->firstInputNumber = data[2];
		cDataNew->_InitSignals[i]->secondInputNumber = data[3];
// M061112
		cDataNew->_InitSignals[i]->thirdInputNumber = data[4];
// M061112E
		data += ConfigDataStructInitSignal::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(5, 0, 15);
#pragma region ConfigDataStructProgram
	data += 2;
	cDataNew->number_Programs = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructProgram::SIZE;
	data += 4;
	cDataNew->_Programs = new ConfigDataStructProgram*[cDataNew->number_Programs];

	for (unsigned int i = 0; i < cDataNew->number_Programs;i++)
	{
		cDataNew->_Programs[i] = new ConfigDataStructProgram();

		unsigned int x = (data[1] << 8) + data[0];
		cDataNew->_Programs[i]->initSignalNumber = x;
		cDataNew->_Programs[i]->prNumber = data[2];

		switch (data[3])
		{
			case 0:
				cDataNew->_Programs[i]->function = LOGIC_FUNCTION_SEARCHING;
				break;
			case 1:
				cDataNew->_Programs[i]->function = LOGIC_FUNCTION_COOLING_LINE;
				break;
			case 2:
				cDataNew->_Programs[i]->function = LOGIC_FUNCTION_COOLING_POINT;
				break;
			case 3:
				cDataNew->_Programs[i]->function = LOGIC_FUNCTION_SEARCHING_PENA;
				break;
			case 4:
				cDataNew->_Programs[i]->function = LOGIC_FUNCTION_SEARCHING_PENA_ZATVOR;
				break;
		}

		x = (data[5] << 8) + data[4];
		unsigned int y = (data[7] << 8) + data[6];
		cDataNew->_Programs[i]->point1 = Point2<unsigned int>(x,y);
		x = (data[9] << 8) + data[8];
		y = (data[11] << 8) + data[10];
		cDataNew->_Programs[i]->point2 = Point2<unsigned int>(x,y);
		
		x = (data[13] << 8) + data[12];
		cDataNew->_Programs[i]->nasadok = x;

		cDataNew->_Programs[i]->nPointProgram = data[14];

		data += ConfigDataStructProgram::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(6, 0, 16);
#pragma region ConfigDataStructFv300
	data += 2;
	cDataNew->number_Fv300 = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructFv300::SIZE;
	data += 4;
	cDataNew->_Fv300 = new ConfigDataStructFv300*[cDataNew->number_Fv300];

	for (unsigned int i = 0; i < cDataNew->number_Fv300;i++)
	{
		cDataNew->_Fv300[i] = new ConfigDataStructFv300();

		cDataNew->_Fv300[i]->address = data[0];
		cDataNew->_Fv300[i]->prNumber = data[1];
		cDataNew->_Fv300[i]->projectNumber = data[2];

		unsigned int x = (data[4] << 8) + data[3];
		unsigned int y = (data[6] << 8) + data[5];
		unsigned int z = (data[8] << 8) + data[7];
		cDataNew->_Fv300[i]->position = Point3<float>(static_cast<float>(x),static_cast<float>(y),static_cast<float>(z));
		
		x = (data[10] << 8) + data[9];
		y = (data[12] << 8) + data[11];
		cDataNew->_Fv300[i]->orientation = Point2<float>(static_cast<float>(x),static_cast<float>(y));

		data += ConfigDataStructFv300::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(7, 0, 17);
#pragma region ConfigDataStructTrajectory
	data += 2;
	cDataNew->number_Trajectory = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructTrajectory::SIZE;

	data += 4;
	cDataNew->_Trajectory = new ConfigDataStructTrajectory*[cDataNew->number_Trajectory];

	for (unsigned int i = 0; i < cDataNew->number_Trajectory;i++)
	{
		cDataNew->_Trajectory[i] = new ConfigDataStructTrajectory();

		cDataNew->_Trajectory[i]->prNumber = data[0];
		cDataNew->_Trajectory[i]->trajectoryNumber = data[1];

		unsigned int x = (data[3] << 8) + data[2];
		cDataNew->_Trajectory[i]->pointNumber = x;

		x = (data[5] << 8) + data[4];
		unsigned int y = (data[7] << 8) + data[6];
		cDataNew->_Trajectory[i]->position = Point2<unsigned short>(x,y);

		x = (data[9] << 8) + data[8];
		cDataNew->_Trajectory[i]->nasadok = x;
// M061112
		cDataNew->_Trajectory[i]->pressureNumber = (data[11] << 8) + data[10];
// M061112E
		data += ConfigDataStructTrajectory::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(8, 0, 18);
#pragma region ConfigDataStructPressure
	data += 2;
	cDataNew->number_Pressure = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructPressure::SIZE;
	data += 4;
	cDataNew->_Pressure = new ConfigDataStructPressure*[cDataNew->number_Pressure];

	for (unsigned int i = 0; i < cDataNew->number_Pressure;i++)
	{
		cDataNew->_Pressure[i] = new ConfigDataStructPressure();

		cDataNew->_Pressure[i]->prNumber = data[0];
// M061112
		cDataNew->_Pressure[i]->arNumber = (data[2] << 8) + data[1];
// M061112E
		cDataNew->_Pressure[i]->pressure = data[2];
		cDataNew->_Pressure[i]->delta = data[3];

		data += ConfigDataStructPressure::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(9, 0, 19);
#pragma region ConfigDataStructPenaBak
	data += 2;
	cDataNew->number_PenaBak = reinterpret_cast<unsigned int*>(data)[0] / ConfigDataStructPenaBak::SIZE;
//Display::getSingleton().printUInt(cDataNew->number_PenaBak, 0, 5);
	data += 4;
	cDataNew->_PenaBak = new ConfigDataStructPenaBak*[cDataNew->number_PenaBak];

	for (unsigned int i = 0; i < cDataNew->number_PenaBak;i++)
	{
		cDataNew->_PenaBak[i] = new ConfigDataStructPenaBak();

//Display::getSingleton().printUInt(cDataNew->_PenaBak[i]->number, 10 + i * 10, 5);
		cDataNew->_PenaBak[i]->number = data[0];
		cDataNew->_PenaBak[i]->level = data[1];
		cDataNew->_PenaBak[i]->address = data[2];
		cDataNew->_PenaBak[i]->numberOnDevice = data[3] - 1;

		data += ConfigDataStructPenaBak::SIZE;
	}
#pragma endregion
	//=====================================================
//Display::getSingleton().printUInt(0x19, 0, 20);
//Display::getSingleton().printUInt(cDataNew->number_InitSignals, 10, 20);
	for (unsigned int i = 0; i < cDataNew->number_InitSignals; i++)
	{
		cDataNew->_InitSignals[i]->function = LOGIC_FUNCTION_SEARCHING;
		for (unsigned int j = 0; j < cDataNew->number_Programs; j++)
		{
			if (cDataNew->_InitSignals[i]->number == cDataNew->_Programs[j]->initSignalNumber)
				cDataNew->_InitSignals[i]->function = cDataNew->_Programs[j]->function;
		}
	}
}

unsigned char ConfigData::getPRAddressByNumber(unsigned char number)
{
	for (unsigned int i = 0; i < cData->number_PRPositions;i++)
	{
		if (cData->_PRPositions[i]->projectNumber == number)
			return cData->_PRPositions[i]->address;
	}

	return -1;
}

unsigned char ConfigData::getPRNumberByAddress(unsigned char address)
{
	for (unsigned int i = 0; i < cData->number_PRPositions;i++)
	{
		if (cData->_PRPositions[i]->address == address)
			return cData->_PRPositions[i]->projectNumber;
	}

	return -1;
}

unsigned char ConfigData::getPRIndexByAddress(unsigned char address)
{
	for (unsigned int i = 0; i < cData->number_PRPositions;i++)
	{
		if (cData->_PRPositions[i]->address == address)
			return i;
	}

	return -1;
}

bool ConfigData::isTvExistsInConfig()
{
	return cData->_Const->tv;
}

bool ConfigData::isFv300ExistsInConfig()
{
	return (cData->number_Fv300 > 0);
}

bool ConfigData::isReady()
{
	return phase == CONFIG_DATA_PHASE_STOP;
}
