#include "Config.h"
#include "../crc32.h"
#include "../DEBUG/serialDebug.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"

Config::Config()
	:	ITimer(TIMER_PERIOD), pConfigData(new ConfigData), progress(-1), progressFR(-1), serialPortName(SERIAL_PORT_1), serialPortSpeed(SERIAL_PORT_SPEED_57600), 
	serialPort(SerialPortManager::getSingleton().getPort(serialPortName)), phase(CONFIG_PHASE_STOP), phaseFR(CONFIG_PHASE_FR_STOP), dataSize(0),
	prNumbers(nullptr), prNumbersCount(0), trajectoryNumbers(nullptr), trajectoryNumbersCount(0), pressureTableNumbers(nullptr), pressureTableNumbersCount(0)
{
	serialPort->setSpeed(serialPortSpeed);
	serialPort->open();

	SerialDebug::getSingleton().addReceiver(this);
}

Config::~Config()
{
	delete pConfigData;
}

ConfigData* Config::getConfigData()
{
	return pConfigData;
}

int Config::getUpdateProgress()
{
	return progress;
}

int Config::getUpdateProgressFR()
{
	return progressFR;
}

void Config::startUpdate()
{
	phase = CONFIG_PHASE_CONNECTION;
	progress = 0;
	serialPort->getRecvFifo()->clear();
	//serialPort->open();
//	pTimer->start();
}

void Config::stopUpdate()
{
	phase = CONFIG_PHASE_STOP;
	progress = 0;
	//serialPort->close();
	serialPort->getRecvFifo()->clear();
//	pTimer->stop();
}

void Config::startUpdateFR()
{
	prNumbersCount = createPrNumbers();
	if (prNumbersCount > 0)
	{
		prNumbersIndex = 0;
		phaseFR = CONFIG_PHASE_FR_CONNECTION;
		progressFR = 0;
		serialPort->getRecvFifo()->clear();
	}
	else
		frActionError(CONFIG_PHASE_FR_STOP,0);
}

void Config::stopUpdateFR()
{
	phaseFR = CONFIG_PHASE_FR_STOP;
	progressFR = -1;
}

void Config::timerHandler()
{
}

void Config::sendMessageToPort(unsigned char* pData, unsigned int size)
{
	if (phase == CONFIG_PHASE_STOP)
	{
		serialPort->setNewSendData(pData, size);
		serialPort->startSend();
	}
}

void Config::action()
{
	pcAction();
	frAction();
}

void Config::pcAction()
{
	unsigned int dLen;

	switch (phase)
	{
		case CONFIG_PHASE_CONNECTION:
			if (serialPort->getRecvFifo()->getDataSize() >= 4)
			{
				unsigned int data[2];
				serialPort->getRecvFifo()->get(reinterpret_cast<unsigned char*>(&data[1]), 4);
				if ((data[1] & 0x0000ffff) == DATA_VERSION)
				{
					data[0] = CONNECT_ANSWER_CODE;
					unsigned int yr = data[1] & 0xffff0000;
					data[1] = yr + DATA_VERSION;
					serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);

					serialPort->startSend();
					phase = CONFIG_PHASE_LENGTH;
				}
				else
				{
					data[0] = DOWNLOAD_RESULT_FAULT;
					unsigned int yr = data[1] & 0xffff0000;
					data[1] = yr + DATA_VERSION;
					serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
					serialPort->startSend();
					phase = CONFIG_PHASE_DISCONNECT_AFTER_ERROR;
					sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, CONFIG_MESSAGE_UPDATE_ERROR, 0, CONFIG_PHASE_CONNECTION));
				}
			}
			break;
		case CONFIG_PHASE_LENGTH:
			if (serialPort->getRecvFifo()->getDataSize() >= LENGTH_AREA_SIZE)
			{
				unsigned int data;
				serialPort->getRecvFifo()->get(reinterpret_cast<unsigned char*>(&data), LENGTH_AREA_SIZE);

				if (data <= MAX_DATA_SIZE - CRC_AREA_SIZE)
				{
					dataSize = data + CRC_AREA_SIZE;
					loadDataSize = 0;
					phase = CONFIG_PHASE_DATA;
				}
				else
				{
					sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, CONFIG_MESSAGE_UPDATE_ERROR, 0, CONFIG_PHASE_LENGTH));
					stopUpdate();
				}

//Display::getSingleton().printUInt(dataSize, 20, 0);
			}
			break;
		case CONFIG_PHASE_DATA:
			progress = (loadDataSize * 100) / dataSize;
			dLen = serialPort->getRecvFifo()->getDataSize();
			if (loadDataSize + dLen > dataSize)
				dLen = dataSize - loadDataSize;

			dLen = serialPort->getRecvFifo()->get(&buffer[loadDataSize], dLen);

			loadDataSize += dLen;

//Display::getSingleton().printUInt(loadDataSize, 40, 0);

			if (loadDataSize == dataSize)
			{
				if (calcCRC32(buffer, dataSize - 4) == *reinterpret_cast<unsigned int*>(&buffer[dataSize - CRC_AREA_SIZE]))
				{
//Display::getSingleton().printUInt(0xddccaabb, 70, 0);
					phase = CONFIG_PHASE_DISCONNECT;
					pConfigData->setNewData(buffer, dataSize - CRC_AREA_SIZE);
//Display::getSingleton().printUInt(0x03142596, 70, 1);
					unsigned int data[2];
					data[0] = DISCONNECT_CODE;
					data[1] = DOWNLOAD_RESULT_OK;
					serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
					serialPort->startSend();
				}
				else
				{
					phase = CONFIG_PHASE_DISCONNECT_AFTER_ERROR;
					
					unsigned int data[2];
					data[0] = DISCONNECT_CODE;
					data[1] = DOWNLOAD_RESULT_FAULT;
					serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
					serialPort->startSend();
				}
			}
			break;
		case CONFIG_PHASE_DISCONNECT:
			if ((!serialPort->isSendActive()) && (pConfigData->isDataSave()))
			{
				stopUpdate();
				sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, CONFIG_MESSAGE_UPDATE_FINISH, 0, 0));
			}
			break;
		case CONFIG_PHASE_DISCONNECT_AFTER_ERROR:
			if (!serialPort->isSendActive())
			{
				stopUpdate();
				sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, CONFIG_MESSAGE_UPDATE_ERROR, 0, CONFIG_PHASE_DATA));
			}
			break;
	}
}

void Config::frActionError(CONFIG_PHASE_FR phase, unsigned char prNumber)
{
	stopUpdateFR();
	sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, CONFIG_MESSAGE_UPDATE_ERROR_FR, phaseFR, prNumber));
}

void Config::frActionFinish()
{
	stopUpdateFR();
	sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, CONFIG_MESSAGE_UPDATE_FINISH_FR, 0, 0));
}

void Config::frAction()
{
	switch (phaseFR)
	{
		case CONFIG_PHASE_FR_CONNECTION:
			configPhaseFrConnection();
			break;
		case CONFIG_PHASE_FR_CONNECTION_WAIT:
			configPhaseFrConnectionWait();
			break;
		case CONFIG_PHASE_FR_TRAJECTORY:
			configPhaseFrTrajectory();
			break;
		case CONFIG_PHASE_FR_TRAJECTORY_WAIT:
			configPhaseFrTrajectoryWait();
			break;
		case CONFIG_PHASE_FR_PRESSURE:
			configPhaseFrPressure();
			break;
		case CONFIG_PHASE_FR_PRESSURE_WAIT:
			configPhaseFrPressureWait();
			break;
		case CONFIG_PHASE_FR_DISCONNECTION:
			configPhaseFrDisconnection();
			break;
		case CONFIG_PHASE_FR_DISCONNECTION_WAIT:
			configPhaseFrDisconnectionWait();
			break;
	}
}

void Config::configPhaseFrConnection()
{
	if (prNumbersIndex == prNumbersCount)
	{
		progressFR = 99;
		frActionFinish();
	}
	else
	{
		trajectoryNumbersCount = createTrajectoryNumbers(prNumbers[prNumbersIndex]);
		trajectoryNumbersIndex = 0;

		progressFRStep = 100 / prNumbersCount;

		unsigned char resetObjects = 0;
		unsigned int tr0PointNumbers = 0;
		unsigned int tr1PointNumbers = 0;
		unsigned int tr2PointNumbers = 0;
		unsigned int tr3PointNumbers = 0;

		switch (trajectoryNumbersCount)
		{
			case 1:
				resetObjects = 0x01;
				tr0PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[0]);
				break;
			case 2:
				resetObjects = 0x03;
				tr0PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[0]);
				tr1PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[1]);
				break;
			case 3:
				resetObjects = 0x07;
				tr0PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[0]);
				tr1PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[1]);
				tr2PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[2]);
				break;
			case 4:
				resetObjects = 0x0f;
				tr0PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[0]);
				tr1PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[1]);
				tr2PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[2]);
				tr3PointNumbers = getTrajectoryPointsCount(prNumbers[prNumbersIndex], trajectoryNumbers[3]);
				break;
		}

		pressureTableNumbersCount = createPressureTableNumbers(prNumbers[prNumbersIndex]);
		pressureTableNumbersIndex = 0;

		trajectoryPointCount = getTrajectoryPointsCount(prNumbers[prNumbersIndex]);
		trajectoryPointIndex = 0;

		unsigned char frame[20];

		unsigned char address = getConfigData()->getPRAddressByNumber(prNumbers[prNumbersIndex]);				

		frame[0] = address;
		frame[1] = 0;
		frame[2] = COMMAND_SET_TRAJECTORY;
		frame[3] = 11;
		frame[4] = SUBCOMMAND_START;
		frame[5] = resetObjects + 0x80;
		frame[6] = tr0PointNumbers >> 8;
		frame[7] = tr0PointNumbers;
		frame[8] = tr1PointNumbers >> 8;
		frame[9] = tr1PointNumbers;
		frame[10] = tr2PointNumbers >> 8;
		frame[11] = tr2PointNumbers;
		frame[12] = tr3PointNumbers >> 8;
		frame[13] = tr3PointNumbers;
		frame[14] = pressureTableNumbersCount;

		frameId = RpkSubsystem::getSingleton().write(frame);
		if (frameId == IRpkDevice::BAD_FRAME_ID)
			frActionError(CONFIG_PHASE_FR_CONNECTION, address);
		else
			phaseFR = CONFIG_PHASE_FR_CONNECTION_WAIT;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Config::configPhaseFrConnection: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(frame, 15);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");
	}
}

void Config::configPhaseFrConnectionWait()
{
	unsigned char* pFrame = nullptr;
	IRpkDevice::FRAME_RESULT result;

	result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
	SAFE_DELETE_ARRAY(pFrame)
	switch (result)
	{
		case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
			frActionError(CONFIG_PHASE_FR_CONNECTION_WAIT, prNumbers[prNumbersIndex]);
			break;
		case IRpkDevice::FRAME_RESULT_ERROR:
			frActionError(CONFIG_PHASE_FR_CONNECTION_WAIT, prNumbers[prNumbersIndex]);
			break;
		case IRpkDevice::FRAME_RESULT_READY:
			phaseFR = CONFIG_PHASE_FR_TRAJECTORY;
			break;
	}
}

void Config::configPhaseFrTrajectory()
{
	static int trIndex = 0;
	if (trajectoryPointIndex == trajectoryPointCount)
	{
		phaseFR = CONFIG_PHASE_FR_PRESSURE;
		trIndex = 0;
	}
	else
	{
		unsigned char frame[20];

		unsigned char address = getConfigData()->getPRAddressByNumber(prNumbers[prNumbersIndex]);				

		trIndex = getNextTrajectoryPointsIndex(trIndex, prNumbers[prNumbersIndex]);
		ConfigDataStructTrajectory** trajectoryConfig = getConfigData()->getConfigDataStructTrajectory();


		frame[0] = address;
		frame[1] = 0;
		frame[2] = COMMAND_SET_TRAJECTORY;
		frame[3] = 11;
		frame[4] = SUBCOMMAND_POINT;
		frame[5] = trajectoryConfig[trIndex]->trajectoryNumber;
		frame[6] = trajectoryConfig[trIndex]->pointNumber >> 8;
		frame[7] = static_cast<unsigned char>(trajectoryConfig[trIndex]->pointNumber);
		frame[8] = trajectoryConfig[trIndex]->position.x >> 8;
		frame[9] = static_cast<unsigned char>(trajectoryConfig[trIndex]->position.x);
		frame[10] = trajectoryConfig[trIndex]->position.y >> 8;
		frame[11] = static_cast<unsigned char>(trajectoryConfig[trIndex]->position.y);
		frame[12] = trajectoryConfig[trIndex]->nasadok >> 8;
		frame[13] = static_cast<unsigned char>(trajectoryConfig[trIndex]->nasadok);
		frame[14] = static_cast<unsigned char>(trajectoryConfig[trIndex]->pressureNumber);

		trIndex++;

		frameId = RpkSubsystem::getSingleton().write(frame);
		if (frameId == IRpkDevice::BAD_FRAME_ID)
			frActionError(CONFIG_PHASE_FR_TRAJECTORY, address);
		else
			phaseFR = CONFIG_PHASE_FR_TRAJECTORY_WAIT;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Config::configPhaseFrTrajectory: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(frame, 15);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("        __         ", trajectoryPointIndex);
	}
}

void Config::configPhaseFrTrajectoryWait()
{
	unsigned char* pFrame = nullptr;
	IRpkDevice::FRAME_RESULT result;

	result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
	SAFE_DELETE_ARRAY(pFrame)
	switch (result)
	{
		case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
			frActionError(CONFIG_PHASE_FR_TRAJECTORY, prNumbers[prNumbersIndex]);
			break;
		case IRpkDevice::FRAME_RESULT_ERROR:
			frActionError(CONFIG_PHASE_FR_TRAJECTORY, prNumbers[prNumbersIndex]);
			break;
		case IRpkDevice::FRAME_RESULT_READY:
			trajectoryPointIndex++;
			phaseFR = CONFIG_PHASE_FR_TRAJECTORY;
			break;
	}
}

void Config::configPhaseFrPressure()
{
	if (pressureTableNumbersIndex == pressureTableNumbersCount)
	{
		phaseFR = CONFIG_PHASE_FR_DISCONNECTION;
	}
	else
	{
		frameIdRes = 0;
		frameId1Res = 0;
		unsigned char frame[20];

		unsigned char address = getConfigData()->getPRAddressByNumber(prNumbers[prNumbersIndex]);				

		unsigned int* pIndex;

		if (!getPressureTableItemsIndex(pressureTableNumbers[pressureTableNumbersIndex], &pIndex))
			frActionError(CONFIG_PHASE_FR_PRESSURE, address);
		else
		{
			ConfigDataStructPressure** pressureConfig = getConfigData()->getConfigDataStructPressure();

			frame[0] = address;
			frame[1] = 0;
			frame[2] = COMMAND_SET_TRAJECTORY;
			frame[3] = 13;
			frame[4] = SUBCOMMAND_PRESSURE;
			frame[5] = static_cast<unsigned char>(pressureConfig[pIndex[0]]->arNumber);
			frame[6] = 0;
			frame[7] = pressureConfig[pIndex[0]]->pressure;
			frame[8] = pressureConfig[pIndex[0]]->delta;
			frame[9] = pressureConfig[pIndex[1]]->pressure;
			frame[10] = pressureConfig[pIndex[1]]->delta;
			frame[11] = pressureConfig[pIndex[2]]->pressure;
			frame[12] = pressureConfig[pIndex[2]]->delta;
			frame[13] = pressureConfig[pIndex[3]]->pressure;
			frame[14] = pressureConfig[pIndex[3]]->delta;
			frame[15] = pressureConfig[pIndex[4]]->pressure;
			frame[16] = pressureConfig[pIndex[4]]->delta;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Config::configPhaseFrPressure...1: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(frame, 17);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("        __         ", trajectoryPointIndex);

			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
			{
				delete[] pIndex;
				frActionError(CONFIG_PHASE_FR_PRESSURE, address);
			}
			else
			{
				frame[0] = address;
				frame[1] = 0;
				frame[2] = COMMAND_SET_TRAJECTORY;
				frame[3] = 11;
				frame[4] = SUBCOMMAND_PRESSURE;
				frame[5] = static_cast<unsigned char>(pressureConfig[pIndex[0]]->arNumber);
				frame[6] = 5;
				frame[7] = pressureConfig[pIndex[5]]->pressure;
				frame[8] = pressureConfig[pIndex[5]]->delta;
				frame[9] = pressureConfig[pIndex[6]]->pressure;
				frame[10] = pressureConfig[pIndex[6]]->delta;
				frame[11] = pressureConfig[pIndex[7]]->pressure;
				frame[12] = pressureConfig[pIndex[7]]->delta;
				frame[13] = pressureConfig[pIndex[8]]->pressure;
				frame[14] = pressureConfig[pIndex[8]]->delta;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Config::configPhaseFrPressure...2: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(frame, 15);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("        __         ", trajectoryPointIndex);

				frameId1 = RpkSubsystem::getSingleton().write(frame);
				if (frameId1 == IRpkDevice::BAD_FRAME_ID)
				{
					delete[] pIndex;
					frActionError(CONFIG_PHASE_FR_PRESSURE, address);
				}
				else
				{
					delete[] pIndex;
					phaseFR = CONFIG_PHASE_FR_PRESSURE_WAIT;
				}
			}
		}
	}
}

void Config::configPhaseFrPressureWait()
{
	unsigned char* pFrame = nullptr;
	IRpkDevice::FRAME_RESULT result;

	if (frameIdRes == 0)
	{
		result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
		SAFE_DELETE_ARRAY(pFrame)
		switch (result)
		{
			case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
				frameIdRes = 2;
				break;
			case IRpkDevice::FRAME_RESULT_ERROR:
				frameIdRes = 2;
				break;
			case IRpkDevice::FRAME_RESULT_READY:
				frameIdRes = 1;
				break;
		}
	}

	if (frameId1Res == 0)
	{
		result = RpkSubsystem::getSingleton().read(frameId1, &pFrame);
		SAFE_DELETE_ARRAY(pFrame)
		switch (result)
		{
			case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
				frameId1Res = 2;
				break;
			case IRpkDevice::FRAME_RESULT_ERROR:
				frameId1Res = 2;
				break;
			case IRpkDevice::FRAME_RESULT_READY:
				frameId1Res = 1;
				break;
		}
	}

	if ((frameIdRes == 2) && (frameId1Res == 2))
	{
		frActionError(CONFIG_PHASE_FR_PRESSURE, prNumbers[prNumbersIndex]);
	}
	else
		if ((frameIdRes == 1) && (frameId1Res == 1))
		{
			pressureTableNumbersIndex++;
			phaseFR = CONFIG_PHASE_FR_PRESSURE;
		}
}	

void Config::configPhaseFrDisconnection()
{
	unsigned char frame[20];

	unsigned char address = getConfigData()->getPRAddressByNumber(prNumbers[prNumbersIndex]);				

	frame[0] = address;
	frame[1] = 0;
	frame[2] = COMMAND_SET_TRAJECTORY;
	frame[3] = 1;
	frame[4] = SUBCOMMAND_FINISH;

	frameId = RpkSubsystem::getSingleton().write(frame);
	if (frameId == IRpkDevice::BAD_FRAME_ID)
		frActionError(CONFIG_PHASE_FR_DISCONNECTION, address);
	else
		phaseFR = CONFIG_PHASE_FR_DISCONNECTION_WAIT;
}

void Config::configPhaseFrDisconnectionWait()
{
	unsigned char* pFrame = nullptr;
	IRpkDevice::FRAME_RESULT result;

	result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
	SAFE_DELETE_ARRAY(pFrame)
	switch (result)
	{
		case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
			frActionError(CONFIG_PHASE_FR_DISCONNECTION_WAIT, prNumbers[prNumbersIndex]);
			break;
		case IRpkDevice::FRAME_RESULT_ERROR:
			frActionError(CONFIG_PHASE_FR_DISCONNECTION_WAIT, prNumbers[prNumbersIndex]);
			break;
		case IRpkDevice::FRAME_RESULT_READY:
			prNumbersIndex++;
			progressFR += progressFRStep;
			phaseFR = CONFIG_PHASE_FR_CONNECTION;
			break;
	}
}	

unsigned int Config::createPrNumbers()
{
	SAFE_DELETE_ARRAY(prNumbers);
	unsigned int count = getConfigData()->getConfigDataStructTrajectoryCount();
	ConfigDataStructTrajectory** trajectoryConfig = getConfigData()->getConfigDataStructTrajectory();

	const unsigned int NUMBERS_COUNT = 100;
	bool numbers[NUMBERS_COUNT];
	memset(numbers, false, NUMBERS_COUNT);

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("Config::createPrNumbers:.........count: ", count);

	unsigned int resultCount = 0;
	for (unsigned int i = 0; i < count; i++)
		if (!numbers[trajectoryConfig[i]->prNumber])
		{
			numbers[trajectoryConfig[i]->prNumber] = true;
			resultCount++;
		}

	if (resultCount > 0)
	{
		prNumbers = new unsigned char[resultCount];
		unsigned int _prIndex = 0;
		for (unsigned int i = 0; i < NUMBERS_COUNT; i++)
			if (numbers[i])
				prNumbers[_prIndex++] = i;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("Config::createPrNumbers: ", resultCount);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(prNumbers, resultCount);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");
	}

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Config::createPrNumbers:......... \n");
	return resultCount;
}

unsigned int Config::createTrajectoryNumbers(unsigned char prNumber)
{
	SAFE_DELETE_ARRAY(trajectoryNumbers);
	unsigned int count = getConfigData()->getConfigDataStructTrajectoryCount();
	ConfigDataStructTrajectory** trajectoryConfig = getConfigData()->getConfigDataStructTrajectory();

	const unsigned int NUMBERS_COUNT = 2000;
	bool numbers[NUMBERS_COUNT];
	memset(numbers, false, NUMBERS_COUNT);

	unsigned int resultCount = 0;
	for (unsigned int i = 0; i < count; i++)
		if (trajectoryConfig[i]->prNumber == prNumber)
			if (!numbers[trajectoryConfig[i]->trajectoryNumber])
			{
				numbers[trajectoryConfig[i]->trajectoryNumber] = true;
				resultCount++;
			}

	if (resultCount > 0)
	{
		trajectoryNumbers = new unsigned char[resultCount];
		unsigned int _trajectoryIndex = 0;
		for (unsigned int i = 0; i < NUMBERS_COUNT; i++)
			if (numbers[i])
				trajectoryNumbers[_trajectoryIndex++] = i;
	}

	return resultCount;
}

unsigned int Config::createPressureTableNumbers(unsigned char prNumber)
{
	SAFE_DELETE_ARRAY(pressureTableNumbers);
	unsigned int count = getConfigData()->getConfigDataStructPressureCount();
	ConfigDataStructPressure** pressureTableConfig = getConfigData()->getConfigDataStructPressure();

	const unsigned int NUMBERS_COUNT = 2000;
	bool numbers[NUMBERS_COUNT];
	memset(numbers, false, NUMBERS_COUNT);

	unsigned int resultCount = 0;
	for (unsigned int i = 0; i < count; i++)
		if (pressureTableConfig[i]->prNumber == prNumber)
			if (!numbers[pressureTableConfig[i]->arNumber])
			{
				numbers[pressureTableConfig[i]->arNumber] = true;
				resultCount++;
			}

	if (resultCount > 0)
	{
		pressureTableNumbers = new unsigned char[resultCount];
		unsigned int _pressureTableIndex = 0;
		for (unsigned int i = 0; i < NUMBERS_COUNT; i++)
			if (numbers[i])
				pressureTableNumbers[_pressureTableIndex++] = i;
	}

	return resultCount;
}

unsigned int Config::getTrajectoryPointsCount(unsigned char prNumber, unsigned char trajectoryNumber)
{
	unsigned int count = getConfigData()->getConfigDataStructTrajectoryCount();
	ConfigDataStructTrajectory** trajectoryConfig = getConfigData()->getConfigDataStructTrajectory();

	unsigned int resultCount = 0;
	for (unsigned int i = 0; i < count; i++)
		if (trajectoryConfig[i]->prNumber == prNumber)
			if (trajectoryNumber != 255)
			{
				if (trajectoryConfig[i]->trajectoryNumber == trajectoryNumber)
					resultCount++;
			}
			else
				resultCount++;

	return resultCount;
}

unsigned int Config::getPressureCount(unsigned char prNumber)
{
	unsigned int count = getConfigData()->getConfigDataStructPressureCount();
	ConfigDataStructPressure** pressureConfig = getConfigData()->getConfigDataStructPressure();

	unsigned int resultCount = 0;
	for (unsigned int i = 0; i < count; i++)
		if (pressureConfig[i]->prNumber == prNumber)
			resultCount++;

	return resultCount;
}

int Config::getNextTrajectoryPointsIndex(unsigned int firstIndex, unsigned char prNumber)
{
	unsigned int count = getConfigData()->getConfigDataStructTrajectoryCount();
	ConfigDataStructTrajectory** trajectoryConfig = getConfigData()->getConfigDataStructTrajectory();

	for (unsigned int i = firstIndex; i < count; i++)
	{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING4("Config::getNextTrajectoryPointsIndex: ", i, prNumber, firstIndex, count);
		if (trajectoryConfig[i]->prNumber == prNumber)
		{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("Config::getNextTrajectoryPointsIndex_RETURN: ", i);
			return i;
		}
	}

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("Config::getNextTrajectoryPointsIndex_RETURN: ........-1\n");
	return -1;
}

bool Config::getPressureTableItemsIndex(unsigned char pressureTableNumber, unsigned int** pIndex)
{
	*pIndex = nullptr;
	unsigned int count = getConfigData()->getConfigDataStructPressureCount();
	ConfigDataStructPressure** pressureConfig = getConfigData()->getConfigDataStructPressure();

	unsigned int ptCount = 0;
	for (unsigned int i = 0; i < count; i++)
		if (pressureConfig[i]->arNumber == pressureTableNumber)
			ptCount++;

	if (ptCount == 0)
		return false;

	*pIndex = new unsigned int[ptCount];

	unsigned int index = 0;
	for (unsigned int i = 0; i < count; i++)
		if (pressureConfig[i]->arNumber == pressureTableNumber)
			(*pIndex)[index++] = i;

	return true;
}

void Config::onMessage(Message message)
{
	if (message.from == MESSAGE_FROM_OFFSET_SERIAL_DEBUG)
	{
		switch (message.msg)
		{
			case SerialDebug::SERIAL_DEBUG_MESSAGE_RECV_COMMAND:
				switch (message.par1)
				{
					case SerialDebug::COMMAND_GET_SETTINGS:
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_YELLOW, SerialDebug::TEXT_ATTR_BOLD, "Конфигурация УСО:\n\n")

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Константы:\n\n")
						DEBUG_PUT("   максимальное число ПР, участвующих в пожаротушении:                        %u\n", pConfigData->getConfigDataStructConst()->maxPR)
						DEBUG_PUT("   задержка перед поиском очага загорания:                                    %u сек\n", pConfigData->getConfigDataStructConst()->timeOutBeforeStart)				
						DEBUG_PUT("   задержка на окончание пожаротушения:                                       %u мин\n", pConfigData->getConfigDataStructConst()->timeOutBeforeFinish)				
						DEBUG_PUT("   количество анализируемых источников излучения (для ИК сканера):            %u\n", pConfigData->getConfigDataStructConst()->numberFireToAnalize)
						DEBUG_PUT("   минимальное расстояние для компактной струи:                               %u м\n", pConfigData->getConfigDataStructConst()->minimumDistanceForCompactJet)
						DEBUG_PUT("   Разрешение тестирования {0, 1}:                                            %u\n", pConfigData->getConfigDataStructConst()->permissionTesting)
						DEBUG_PUT("   Время начала тестирования (часы):                                          %u\n", pConfigData->getConfigDataStructConst()->testingHour)
						DEBUG_PUT("   Время начала тестирования (минуты):                                        %u\n", pConfigData->getConfigDataStructConst()->testingMinute)
						DEBUG_PUT("   Разрешение передачи информации о результатах тестирования {0, 1}:          %u\n", pConfigData->getConfigDataStructConst()->permissionTestingInfo)
						DEBUG_PUT("   Время ожидания действия оператора перед стартом поиска или орошения (сек): %u\n", pConfigData->getConfigDataStructConst()->timeControlUserAction)						
						DEBUG_PUT("   габариты защищаемой зоны - X:                                              %f м\n", pConfigData->getConfigDataStructConst()->protectedZone.x / 100.0f)
						DEBUG_PUT("   габариты защищаемой зоны - Y:                                              %f м\n", pConfigData->getConfigDataStructConst()->protectedZone.y / 100.0f)
						DEBUG_PUT("   габариты защищаемой зоны - Z:                                              %f м\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)

						DEBUG_PUT("   Наличие адаптера УСО - 'телевизионная система мониторинга':                %u\n\n", pConfigData->getConfigDataStructConst()->tv)
						//DEBUG_PUT("   Наличие адаптера УСО - 'PC':                                               %u\n", pConfigData->getConfigDataStructConst()->)
						//DEBUG_PUT("   Верхнее поле при тушении очага (град):                                     %u\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)
						//DEBUG_PUT("   Нижнее поле при тушении очага (град):                                      %u\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)
						//DEBUG_PUT("   Левое поле при тушении очага (град):                                       %u\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)
						//DEBUG_PUT("   Правое поле при тушении очага (град):                                      %u\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)
						//DEBUG_PUT("   Время возврата из дистанционного режима (мин):                             %u\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)
						//DEBUG_PUT("   Разрешение на переход в дистанционный режим при поиске очага {0, 1}:       %u\n\n", pConfigData->getConfigDataStructConst()->protectedZone.z / 100.0f)

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Положение и ориентация ПР:\n\n")
						for (unsigned int i = 0; i < pConfigData->getConfigDataStructPRPositionCount(); ++i)
						{
							DEBUG_PUT("   номер:                                     %u\n", pConfigData->getConfigDataStructPRPositions()[i]->projectNumber)
							DEBUG_PUT("   адрес:                                     %u\n", pConfigData->getConfigDataStructPRPositions()[i]->address)
//							DEBUG_PUT("   координата X:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.x / 100.0f)
//							DEBUG_PUT("   координата Y:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.y / 100.0f)
//							DEBUG_PUT("   координата Z:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.z / 100.0f)
							DEBUG_PUT("   координата X:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.x)
							DEBUG_PUT("   координата Y:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.y)
							DEBUG_PUT("   координата Z:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.z)
							DEBUG_PUT("   ориентация ПР в горизонтальной плоскости:  %f\n", pConfigData->getConfigDataStructPRPositions()[i]->orientation.x)
							DEBUG_PUT("   ориентация ПР в вертикальной плоскости:    %f\n", pConfigData->getConfigDataStructPRPositions()[i]->orientation.y)
							DEBUG_PUT("   порядковый номер по магистрали:            %u\n", pConfigData->getConfigDataStructPRPositions()[i]->networkIndexNumber)
							DEBUG_PUT("   Количество затворов:                       %u\n\n", pConfigData->getConfigDataStructPRPositions()[i]->zatvorCount)
						}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Используемые входы и выходы БК-16:\n\n")
						for (unsigned int i = 0; i < pConfigData->getConfigDataStructIOBk16Count(); ++i)
						{
							DEBUG_PUT("   адрес устройства (БК16):       %u\n", pConfigData->getConfigDataStructIOBk16()[i]->bkAddress)
							DEBUG_PUT("   номер входа/выхода:            %u\n", pConfigData->getConfigDataStructIOBk16()[i]->numberOnDevice)
							DEBUG_PUT("   функциональная группа выхода:  %u\n", pConfigData->getConfigDataStructIOBk16()[i]->outputFunctionGroup)
							DEBUG_PUT("   номер ПР (дискового затвора):  %u\n", pConfigData->getConfigDataStructIOBk16()[i]->projectNumber)
							DEBUG_PUT("   использование входа:           %u\n", pConfigData->getConfigDataStructIOBk16()[i]->prGateNumber)
							DEBUG_PUT("   номер извещателя:              %u\n\n", pConfigData->getConfigDataStructIOBk16()[i]->projectNumber)
						}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Список инициирующих сигналов:\n\n")
						DEBUG_PUT("   количество сигналов:  %u\n\n", pConfigData->getConfigDataStructInitSignalsCount())
						for (unsigned int i = 0; i < pConfigData->getConfigDataStructInitSignalsCount(); ++i)
						{
							DEBUG_PUT("   номер инициирующего сигнала:                   %u\n", pConfigData->getConfigDataStructInitSignals()[i]->number)
							DEBUG_PUT("   номер входного сигнала пожарной сигнализации:  %u\n", pConfigData->getConfigDataStructInitSignals()[i]->firstInputNumber)
							DEBUG_PUT("   номер входного сигнала пожарной сигнализации:  %u\n\n", pConfigData->getConfigDataStructInitSignals()[i]->secondInputNumber)
						}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Задания для ПР при поиске очага загорания и охлаждении:\n\n")
						DEBUG_PUT("   количество заданий:  %u\n\n", pConfigData->getConfigDataStructProgramCount())
						for (unsigned int i = 0; i < pConfigData->getConfigDataStructProgramCount(); ++i)
						{
							DEBUG_PUT("   номер инициирующего сигнала:    %u\n", pConfigData->getConfigDataStructPrograms()[i]->initSignalNumber)
							DEBUG_PUT("   номер ПР:                       %u\n", pConfigData->getConfigDataStructPrograms()[i]->prNumber)
							DEBUG_PUT("   функция:                        %u\n", pConfigData->getConfigDataStructPrograms()[i]->function)
							DEBUG_PUT("   1-я горизонтальная координата:  %u\n", pConfigData->getConfigDataStructPrograms()[i]->point1.x)
							DEBUG_PUT("   1-я вертикальная координата:    %u\n", pConfigData->getConfigDataStructPrograms()[i]->point1.y)
							DEBUG_PUT("   2-я горизонтальная координата:  %u\n", pConfigData->getConfigDataStructPrograms()[i]->point2.x)
							DEBUG_PUT("   2-я вертикальная координата:    %u\n", pConfigData->getConfigDataStructPrograms()[i]->point2.y)
							DEBUG_PUT("   номер траектории:               %u\n\n", pConfigData->getConfigDataStructPrograms()[i]->nPointProgram)
						}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Извещатели FV300:\n\n")
						DEBUG_PUT("   количество извещателей:  %u\n\n", pConfigData->getConfigDataStructFv300Count())
						for (unsigned int i = 0; i < pConfigData->getConfigDataStructFv300Count(); ++i)
						{
							DEBUG_PUT("   адрес извещателя:  %u\n", pConfigData->getConfigDataStructFv300()[i]->address)
							DEBUG_PUT("   номер ПР:          %u\n", pConfigData->getConfigDataStructFv300()[i]->prNumber)
							DEBUG_PUT("   номер извещателя:  %u\n\n", pConfigData->getConfigDataStructFv300()[i]->projectNumber)
						}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Траектории ПР:\n\n")
						DEBUG_PUT("   количество точек всех траекторий:  %u\n\n", pConfigData->getConfigDataStructTrajectoryCount())

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Таблицы давления:\n\n")
						DEBUG_PUT("   количество таблиц:  %u\n\n", pConfigData->getConfigDataStructPressureCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPressureCount() / 100; ++i)
						//{
						//	DEBUG_PUT("   номер ПР:      %u\n", pConfigData->getConfigDataStructPressure()[i]->prNumber)
						//	DEBUG_PUT("   номер таблицы: %u\n", pConfigData->getConfigDataStructPressure()[i]->arNumber)
						//	DEBUG_PUT("   давление:      %u\n", pConfigData->getConfigDataStructPressure()[i]->pressure)
						//	DEBUG_PUT("   угол:          %u\n\n", pConfigData->getConfigDataStructPressure()[i]->delta)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Баки пенообразователя:\n\n")
						DEBUG_PUT("   количество записей баков:  %u\n\n", pConfigData->getConfigDataStructPenaBakCount())
						for (unsigned int i = 0; i < pConfigData->getConfigDataStructPenaBakCount(); ++i){
							DEBUG_PUT("   номер бака:        %u\n", pConfigData->getConfigDataStructPenaBak()[i]->number)
							DEBUG_PUT("   уровень:           %u\n", pConfigData->getConfigDataStructPenaBak()[i]->level)
							DEBUG_PUT("   адрес контроллера: %u\n", pConfigData->getConfigDataStructPenaBak()[i]->address)
							DEBUG_PUT("   номер входа:       %u\n\n", pConfigData->getConfigDataStructPenaBak()[i]->numberOnDevice)
						}
					break;
				}
			break;
		}
	}
}
