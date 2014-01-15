#include "config.h"
#include "../crc32.h"
#include "../debug/serialDebug.h"
#include "../hdd/hddManager.h"
#include "../debug/serialDebug.h"

const SERIAL_PORT Config::UPDATE_SERIAL_PORT = SERIAL_PORT_1;
const SERIAL_PORT_SPEED Config::SERIAL_PORT_UPDATE_SPEED = SERIAL_PORT_SPEED_57600;

Config::Config()
	:	Task(&Config::processStop), pConfigData(new ConfigData), hddBuffer(nullptr), hddTaskId(HddManager::UNDEFINED_ID), fReadWrite(false), fUpdate(false), 
	serialPort(SerialPortManager::getSingleton().getPort(UPDATE_SERIAL_PORT)), loadProgress(0), errorCode(0), loadBuffer(nullptr)
{
	Process::getSingleton().addTask(this);
	serialPort->open();
	SerialDebug::getSingleton().addReceiver(this);
	}

Config::~Config(){
	delete pConfigData;
}

ConfigData* Config::getConfigData(){
	return pConfigData;
}

unsigned int Config::calcDataCrc(){
	unsigned int headSize = sizeof(pConfigData->dataCrc) + sizeof(pConfigData->dataValid);
	return calcCRC32(reinterpret_cast<unsigned char*>(pConfigData) + headSize, sizeof(ConfigData) - headSize);
}

unsigned int Config::getLoadProgress(){
	return loadProgress;
}

bool Config::readConfigDataFromHdd(){
	if(fReadWrite)
		return false;

	SAFE_DELETE_ARRAY(hddBuffer)
	hddBuffer = new unsigned char[sizeof(ConfigData)];
	hddTaskId = HddManager::getSingleton().read(hddBuffer, HddManager::SECTOR_OFFSET_CONFIG, sizeof(ConfigData) / 512 + 1);
	
	if (hddTaskId != HddManager::UNDEFINED_ID){
		setPtr(&Config::processReadFromHdd);
	}else{
		SAFE_DELETE_ARRAY(hddBuffer)
		return false;
	}

	fReadWrite = true;
	return true;
}

bool Config::writeConfigDataToHdd(){
	if(fReadWrite)
		return false;

	hddTaskId = HddManager::getSingleton().write(reinterpret_cast<unsigned char*>(pConfigData), HddManager::SECTOR_OFFSET_CONFIG, sizeof(ConfigData) / 512 + 1);
	
	if (hddTaskId != HddManager::UNDEFINED_ID){
		setPtr(&Config::processWriteToHdd);
	}else{
		return false;
	}

	fReadWrite = true;
	return true;
}

CPointer<Config> Config::processStop(){
	return &Config::processStop;
}

CPointer<Config> Config::processReadFromHdd(){
	if(!HddManager::getSingleton().isTaskExecute(hddTaskId)){
		memcpy(pConfigData, hddBuffer, sizeof(ConfigData));

		unsigned int crc = calcDataCrc();
		if(pConfigData->dataCrc == crc){
			pConfigData->dataValid = true;
			sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_READ_COMPLETE, MESSAGE_CONFIG_HDD_COMPLETE_OK, 0));
		}else{
			pConfigData->dataValid = false;
			sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_READ_COMPLETE, MESSAGE_CONFIG_HDD_COMPLETE_FAILED, 0));
		}

		fReadWrite = false;
		return &Config::processStop;
	}

	return &Config::processReadFromHdd;
}

CPointer<Config> Config::processWriteToHdd(){
	if(!HddManager::getSingleton().isTaskExecute(hddTaskId)){
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_WRITE_COMPLETE, MESSAGE_CONFIG_HDD_COMPLETE_OK, 0));
		fReadWrite = false;
		return &Config::processStop;
	}

	return &Config::processWriteToHdd;
}

bool Config::readConfig(){
	return readConfigDataFromHdd();
}

bool Config::writeConfig(){
	pConfigData->dataCrc = calcDataCrc();
	return writeConfigDataToHdd();
}

bool Config::update(){
	if(fReadWrite || fUpdate)
		return false;

	errorCode = 0;
	fUpdate = true;
	SerialDebug::getSingleton().off();
	serialPort->getRecvFifo()->clear();
	setPtr(&Config::processUpdateConnection);

	return true;
}

void Config::cancelUpdate(){
	if(fUpdate){
		fUpdate = false;
		SAFE_DELETE_ARRAY(loadBuffer)
		setPtr(&Config::processStop);
		SerialDebug::getSingleton().on();
	}
}

CPointer<Config> Config::processUpdateConnection(){
	if (serialPort->getRecvFifo()->getDataSize() >= 4){
		unsigned int data[2];
		serialPort->getRecvFifo()->get(reinterpret_cast<unsigned char*>(&data[1]), 4);
		if ((data[1] & 0x0000ffff) == ConfigData::VERSION){
			data[0] = CONNECT_ANSWER_CODE;
			unsigned int yr = data[1] & 0xffff0000;
			data[1] = yr + ConfigData::VERSION;
			serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
			serialPort->startSend();

			totalSize = 0;
			return &Config::processUpdateGetLength;
		}else{
			data[0] = DOWNLOAD_RESULT_FAULT;
			unsigned int yr = data[1] & 0xffff0000;
			data[1] = yr + ConfigData::VERSION;
			serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
			serialPort->startSend();

			errorCode = MESSAGE_CONFIG_UPDATE_FAILED_CODE_CONNECTION_VERSION;
			return &Config::processUpdateFailedConnection;
		}
	}

	return &Config::processUpdateConnection;
}

CPointer<Config> Config::processUpdateGetLength(){
	static const unsigned int LENGTH_SIZE = 4;
	static const unsigned int CRC_SIZE = 4;
	static const unsigned int MAX_LOAD_SIZE = sizeof(ConfigData) + 6 * 10 + CRC_SIZE;

	if (serialPort->getRecvFifo()->getDataSize() >= LENGTH_SIZE){
		serialPort->getRecvFifo()->get(reinterpret_cast<unsigned char*>(&totalSize), LENGTH_SIZE);

		totalSize += CRC_SIZE;

		if (totalSize <= MAX_LOAD_SIZE){
			loadSize = 0;
			SAFE_DELETE_ARRAY(loadBuffer)
			loadBuffer = new unsigned char[totalSize];

			return &Config::processUpdateLoadData;
		}else{
			errorCode = MESSAGE_CONFIG_UPDATE_FAILED_CODE_TOTAL_SIZE;
			return &Config::processUpdateFailedConnection;
		}
	}

	return &Config::processUpdateGetLength;
}

CPointer<Config> Config::processUpdateLoadData(){
	unsigned int blockSize = serialPort->getRecvFifo()->getDataSize();
	if (loadSize + blockSize > totalSize)
		blockSize = totalSize - loadSize;

	blockSize = serialPort->getRecvFifo()->get(&loadBuffer[loadSize], blockSize);
	loadSize += blockSize;
	loadProgress = (loadSize * 100) / totalSize;

	if (loadSize == totalSize){
		if (calcCRC32(loadBuffer, totalSize - CRC_SIZE) == *reinterpret_cast<unsigned int*>(&loadBuffer[loadSize - CRC_SIZE])){
			unsigned int data[2];
			data[0] = DISCONNECT_CODE;
			data[1] = DOWNLOAD_RESULT_OK;
			serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
			serialPort->startSend();
			return &Config::processUpdateApply;
		}else{
			unsigned int data[2];
			data[0] = DISCONNECT_CODE;
			data[1] = DOWNLOAD_RESULT_FAULT;
			serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
			serialPort->startSend();
			errorCode = MESSAGE_CONFIG_UPDATE_FAILED_CODE_CRC;
			return &Config::processUpdateFailedConnection;
		}
	}

	return &Config::processUpdateLoadData;
}

CPointer<Config> Config::processUpdateApply(){
	if(updateApply())
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_UPDATE_COMPLETE, MESSAGE_CONFIG_UPDATE_COMPLETE_OK, 0));
	else{
		return &Config::processUpdateFailedConnection;
	}

	cancelUpdate();
	return &Config::processStop;
}

CPointer<Config> Config::processUpdateFailedConnection(){
	if (!serialPort->isSendActive()){
		cancelUpdate();
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_UPDATE_COMPLETE, MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED, errorCode));
		return &Config::processStop;
	}

	return &Config::processUpdateFailedConnection;
}

void Config::onMessage(Message message){
	if (message.from == MESSAGE_FROM_OFFSET_SERIAL_DEBUG){
		switch (message.msg){
			case SerialDebug::SERIAL_DEBUG_MESSAGE_RECV_COMMAND:
				switch (message.par1){
					case SerialDebug::COMMAND_GET_SETTINGS:
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_YELLOW, SerialDebug::TEXT_ATTR_BOLD, "Конфигурация УСО:\n\n")

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Константы:\n\n")
						//DEBUG_PUT("   максимальное число ПР, участвующих в пожаротушении:              %u\n", pConfigData->getConfigDataStructConst()->maxPR)
						//DEBUG_PUT("   задержка перед поиском очага загорания:                          %u сек\n", pConfigData->getConfigDataStructConst()->timeOutBeforeStart)				
						//DEBUG_PUT("   задержка на окончание пожаротушения:                             %u сек\n", pConfigData->getConfigDataStructConst()->timeOutBeforeFinish)				
						//DEBUG_PUT("   количество анализируемых источников излучения (для ИК сканера):  %u\n", pConfigData->getConfigDataStructConst()->numberFireToAnalize)
						//DEBUG_PUT("   минимальное расстояние для компактной струи:                     %u м\n", pConfigData->getConfigDataStructConst()->minimumDistanceForCompactJet)
						//DEBUG_PUT("   габариты защищаемой зоны - X:                                    %f м\n", pConfigData->getConfigDataStructConst()->protectedZone.x / 100.0f)
						//DEBUG_PUT("   габариты защищаемой зоны - Y:                                    %f м\n\n", pConfigData->getConfigDataStructConst()->protectedZone.y / 100.0f)

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Положение и ориентация ПР:\n\n")
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPRPositionCount(); ++i){
						//	DEBUG_PUT("   номер:                                     %u\n", pConfigData->getConfigDataStructPRPositions()[i]->projectNumber)
						//	DEBUG_PUT("   адрес:                                     %u\n", pConfigData->getConfigDataStructPRPositions()[i]->address)
						//	DEBUG_PUT("   координата X:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.x / 100.0f)
						//	DEBUG_PUT("   координата Y:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.y / 100.0f)
						//	DEBUG_PUT("   координата Z:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.z / 100.0f)
						//	DEBUG_PUT("   ориентация ПР в горизонтальной плоскости:  %f\n", pConfigData->getConfigDataStructPRPositions()[i]->orientation.x)
						//	DEBUG_PUT("   ориентация ПР в вертикальной плоскости:    %f\n", pConfigData->getConfigDataStructPRPositions()[i]->orientation.y)
						//	DEBUG_PUT("   порядковый номер по магистрали:            %u\n\n", pConfigData->getConfigDataStructPRPositions()[i]->networkIndexNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Используемые входы и выходы БК-16:\n\n")
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructIOBk16Count(); ++i){
						//	DEBUG_PUT("   адрес устройства (БК16):       %u\n", pConfigData->getConfigDataStructIOBk16()[i]->bkAddress)
						//	DEBUG_PUT("   номер входа/выхода:            %u\n", pConfigData->getConfigDataStructIOBk16()[i]->numberOnDevice)
						//	DEBUG_PUT("   функциональная группа выхода:  %u\n", pConfigData->getConfigDataStructIOBk16()[i]->outputFunctionGroup)
						//	DEBUG_PUT("   номер ПР (дискового затвора):  %u\n", pConfigData->getConfigDataStructIOBk16()[i]->projectNumber)
						//	DEBUG_PUT("   использование входа:           %u\n", pConfigData->getConfigDataStructIOBk16()[i]->prGateNumber)
						//	DEBUG_PUT("   номер извещателя:              %u\n\n", pConfigData->getConfigDataStructIOBk16()[i]->projectNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Список инициирующих сигналов:\n\n")
						//DEBUG_PUT("   количество сигналов:  %u\n\n", pConfigData->getConfigDataStructInitSignalsCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructInitSignalsCount(); ++i){
						//	DEBUG_PUT("   номер инициирующего сигнала:                   %u\n", pConfigData->getConfigDataStructInitSignals()[i]->number)
						//	DEBUG_PUT("   номер входного сигнала пожарной сигнализации:  %u\n", pConfigData->getConfigDataStructInitSignals()[i]->firstInputNumber)
						//	DEBUG_PUT("   номер входного сигнала пожарной сигнализации:  %u\n\n", pConfigData->getConfigDataStructInitSignals()[i]->secondInputNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Задания для ПР при поиске очага загорания и охлаждении:\n\n")
//						DEBUG_PUT("   количество заданий:  %u\n\n", pConfigData->getConfigDataStructProgramCount())
/*						for (unsigned int i = 0; i < pConfigData->getConfigDataStructProgramCount(); ++i)
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
*/
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Извещатели FV300:\n\n")
						//DEBUG_PUT("   количество извещателей:  %u\n\n", pConfigData->getConfigDataStructFv300Count())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructFv300Count(); ++i){
						//	DEBUG_PUT("   адрес извещателя:  %u\n", pConfigData->getConfigDataStructFv300()[i]->address)
						//	DEBUG_PUT("   номер ПР:          %u\n", pConfigData->getConfigDataStructFv300()[i]->prNumber)
						//	DEBUG_PUT("   номер извещателя:  %u\n\n", pConfigData->getConfigDataStructFv300()[i]->projectNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Траектории ПР:\n\n")
//						DEBUG_PUT("   количество точек всех траекторий:  %u\n\n", pConfigData->getConfigDataStructTrajectoryCount())

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Таблицы давления:\n\n")
//						DEBUG_PUT("   количество таблиц:  %u\n\n", pConfigData->getConfigDataStructPressureCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPressureCount() / 100; ++i)
						//{
						//	DEBUG_PUT("   номер ПР:      %u\n", pConfigData->getConfigDataStructPressure()[i]->prNumber)
						//	DEBUG_PUT("   номер таблицы: %u\n", pConfigData->getConfigDataStructPressure()[i]->arNumber)
						//	DEBUG_PUT("   давление:      %u\n", pConfigData->getConfigDataStructPressure()[i]->pressure)
						//	DEBUG_PUT("   угол:          %u\n\n", pConfigData->getConfigDataStructPressure()[i]->delta)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  Баки пенообразователя:\n\n")
						//DEBUG_PUT("   количество записей баков:  %u\n\n", pConfigData->getConfigDataStructPenaBakCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPenaBakCount(); ++i){
						//	DEBUG_PUT("   номер бака:        %u\n", pConfigData->getConfigDataStructPenaBak()[i]->number)
						//	DEBUG_PUT("   уровень:           %u\n", pConfigData->getConfigDataStructPenaBak()[i]->level)
						//	DEBUG_PUT("   адрес контроллера: %u\n", pConfigData->getConfigDataStructPenaBak()[i]->address)
						//	DEBUG_PUT("   номер входа:       %u\n\n", pConfigData->getConfigDataStructPenaBak()[i]->numberOnDevice)
						//}
						break;
					case SerialDebug::COMMAND_DEBUG_CONFIG:
						break;
				}
			break;
		}
	}
}

bool Config::updateApply(){
	ConfigData* pNewConfigData = new ConfigData;




	errorCode = MESSAGE_CONFIG_UPDATE_FAILED_CODE_CONSTANTS_SIZE;
	return false;
}
