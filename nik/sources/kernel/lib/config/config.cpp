#include "config.h"
#include "../crc32.h"
#include "../debug/serialDebug.h"
#include "../hdd/hddManager.h"
#include "../debug/serialDebug.h"

const SERIAL_PORT Config::UPDATE_SERIAL_PORT = SERIAL_PORT_1;
const SERIAL_PORT_SPEED Config::SERIAL_PORT_UPDATE_SPEED = SERIAL_PORT_SPEED_57600;

Config::Config()
	:	Task(&Config::processStop), pConfigData(new ConfigData), hddBuffer(nullptr), hddTaskId(HddManager::UNDEFINED_ID), fReadWrite(false), fUpdate(false), 
	serialPort(SerialPortManager::getSingleton().getPort(UPDATE_SERIAL_PORT))
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

	fUpdate = true;
	SerialDebug::getSingleton().off();
	serialPort->getRecvFifo()->clear();
	setPtr(&Config::processUpdateConnection);

	return true;
}

void Config::cancelUpdate(){
	if(fUpdate){
		fUpdate = false;
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

			return &Config::processUpdateD;
		}else{
			data[0] = DOWNLOAD_RESULT_FAULT;
			unsigned int yr = data[1] & 0xffff0000;
			data[1] = yr + ConfigData::VERSION;
			serialPort->setNewSendData(reinterpret_cast<unsigned char*>(&data[0]), 8);
			serialPort->startSend();
			return &Config::processUpdateFailedConnection;
		}
	}

	return &Config::processUpdateConnection;
}

CPointer<Config> Config::processUpdateD(){
	cancelUpdate();
	sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_UPDATE_COMPLETE, MESSAGE_CONFIG_UPDATE_COMPLETE_OK, 0));
	return &Config::processStop;
}

CPointer<Config> Config::processUpdateFailedConnection(){
	if (!serialPort->isSendActive()){
		cancelUpdate();
		sendMessage(Message(MESSAGE_FROM_OFFSET_CONFIG, MESSAGE_CONFIG_UPDATE_COMPLETE, MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED, MESSAGE_CONFIG_UPDATE_FAILED_CODE_CONNECTION));
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
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_YELLOW, SerialDebug::TEXT_ATTR_BOLD, "������������ ���:\n\n")

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ���������:\n\n")
						//DEBUG_PUT("   ������������ ����� ��, ����������� � �������������:              %u\n", pConfigData->getConfigDataStructConst()->maxPR)
						//DEBUG_PUT("   �������� ����� ������� ����� ���������:                          %u ���\n", pConfigData->getConfigDataStructConst()->timeOutBeforeStart)				
						//DEBUG_PUT("   �������� �� ��������� �������������:                             %u ���\n", pConfigData->getConfigDataStructConst()->timeOutBeforeFinish)				
						//DEBUG_PUT("   ���������� ������������� ���������� ��������� (��� �� �������):  %u\n", pConfigData->getConfigDataStructConst()->numberFireToAnalize)
						//DEBUG_PUT("   ����������� ���������� ��� ���������� �����:                     %u �\n", pConfigData->getConfigDataStructConst()->minimumDistanceForCompactJet)
						//DEBUG_PUT("   �������� ���������� ���� - X:                                    %f �\n", pConfigData->getConfigDataStructConst()->protectedZone.x / 100.0f)
						//DEBUG_PUT("   �������� ���������� ���� - Y:                                    %f �\n\n", pConfigData->getConfigDataStructConst()->protectedZone.y / 100.0f)

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ��������� � ���������� ��:\n\n")
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPRPositionCount(); ++i){
						//	DEBUG_PUT("   �����:                                     %u\n", pConfigData->getConfigDataStructPRPositions()[i]->projectNumber)
						//	DEBUG_PUT("   �����:                                     %u\n", pConfigData->getConfigDataStructPRPositions()[i]->address)
						//	DEBUG_PUT("   ���������� X:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.x / 100.0f)
						//	DEBUG_PUT("   ���������� Y:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.y / 100.0f)
						//	DEBUG_PUT("   ���������� Z:                              %f\n", pConfigData->getConfigDataStructPRPositions()[i]->position.z / 100.0f)
						//	DEBUG_PUT("   ���������� �� � �������������� ���������:  %f\n", pConfigData->getConfigDataStructPRPositions()[i]->orientation.x)
						//	DEBUG_PUT("   ���������� �� � ������������ ���������:    %f\n", pConfigData->getConfigDataStructPRPositions()[i]->orientation.y)
						//	DEBUG_PUT("   ���������� ����� �� ����������:            %u\n\n", pConfigData->getConfigDataStructPRPositions()[i]->networkIndexNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ������������ ����� � ������ ��-16:\n\n")
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructIOBk16Count(); ++i){
						//	DEBUG_PUT("   ����� ���������� (��16):       %u\n", pConfigData->getConfigDataStructIOBk16()[i]->bkAddress)
						//	DEBUG_PUT("   ����� �����/������:            %u\n", pConfigData->getConfigDataStructIOBk16()[i]->numberOnDevice)
						//	DEBUG_PUT("   �������������� ������ ������:  %u\n", pConfigData->getConfigDataStructIOBk16()[i]->outputFunctionGroup)
						//	DEBUG_PUT("   ����� �� (��������� �������):  %u\n", pConfigData->getConfigDataStructIOBk16()[i]->projectNumber)
						//	DEBUG_PUT("   ������������� �����:           %u\n", pConfigData->getConfigDataStructIOBk16()[i]->prGateNumber)
						//	DEBUG_PUT("   ����� ����������:              %u\n\n", pConfigData->getConfigDataStructIOBk16()[i]->projectNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ������ ������������ ��������:\n\n")
						//DEBUG_PUT("   ���������� ��������:  %u\n\n", pConfigData->getConfigDataStructInitSignalsCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructInitSignalsCount(); ++i){
						//	DEBUG_PUT("   ����� ������������� �������:                   %u\n", pConfigData->getConfigDataStructInitSignals()[i]->number)
						//	DEBUG_PUT("   ����� �������� ������� �������� ������������:  %u\n", pConfigData->getConfigDataStructInitSignals()[i]->firstInputNumber)
						//	DEBUG_PUT("   ����� �������� ������� �������� ������������:  %u\n\n", pConfigData->getConfigDataStructInitSignals()[i]->secondInputNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ������� ��� �� ��� ������ ����� ��������� � ����������:\n\n")
//						DEBUG_PUT("   ���������� �������:  %u\n\n", pConfigData->getConfigDataStructProgramCount())
/*						for (unsigned int i = 0; i < pConfigData->getConfigDataStructProgramCount(); ++i)
						{
							DEBUG_PUT("   ����� ������������� �������:    %u\n", pConfigData->getConfigDataStructPrograms()[i]->initSignalNumber)
							DEBUG_PUT("   ����� ��:                       %u\n", pConfigData->getConfigDataStructPrograms()[i]->prNumber)
							DEBUG_PUT("   �������:                        %u\n", pConfigData->getConfigDataStructPrograms()[i]->function)
							DEBUG_PUT("   1-� �������������� ����������:  %u\n", pConfigData->getConfigDataStructPrograms()[i]->point1.x)
							DEBUG_PUT("   1-� ������������ ����������:    %u\n", pConfigData->getConfigDataStructPrograms()[i]->point1.y)
							DEBUG_PUT("   2-� �������������� ����������:  %u\n", pConfigData->getConfigDataStructPrograms()[i]->point2.x)
							DEBUG_PUT("   2-� ������������ ����������:    %u\n", pConfigData->getConfigDataStructPrograms()[i]->point2.y)
							DEBUG_PUT("   ����� ����������:               %u\n\n", pConfigData->getConfigDataStructPrograms()[i]->nPointProgram)
						}
*/
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ���������� FV300:\n\n")
						//DEBUG_PUT("   ���������� �����������:  %u\n\n", pConfigData->getConfigDataStructFv300Count())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructFv300Count(); ++i){
						//	DEBUG_PUT("   ����� ����������:  %u\n", pConfigData->getConfigDataStructFv300()[i]->address)
						//	DEBUG_PUT("   ����� ��:          %u\n", pConfigData->getConfigDataStructFv300()[i]->prNumber)
						//	DEBUG_PUT("   ����� ����������:  %u\n\n", pConfigData->getConfigDataStructFv300()[i]->projectNumber)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ���������� ��:\n\n")
//						DEBUG_PUT("   ���������� ����� ���� ����������:  %u\n\n", pConfigData->getConfigDataStructTrajectoryCount())

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ������� ��������:\n\n")
//						DEBUG_PUT("   ���������� ������:  %u\n\n", pConfigData->getConfigDataStructPressureCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPressureCount() / 100; ++i)
						//{
						//	DEBUG_PUT("   ����� ��:      %u\n", pConfigData->getConfigDataStructPressure()[i]->prNumber)
						//	DEBUG_PUT("   ����� �������: %u\n", pConfigData->getConfigDataStructPressure()[i]->arNumber)
						//	DEBUG_PUT("   ��������:      %u\n", pConfigData->getConfigDataStructPressure()[i]->pressure)
						//	DEBUG_PUT("   ����:          %u\n\n", pConfigData->getConfigDataStructPressure()[i]->delta)
						//}

						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_WHITE, SerialDebug::TEXT_ATTR_BOLD, "  ���� ����������������:\n\n")
						//DEBUG_PUT("   ���������� ������� �����:  %u\n\n", pConfigData->getConfigDataStructPenaBakCount())
						//for (unsigned int i = 0; i < pConfigData->getConfigDataStructPenaBakCount(); ++i){
						//	DEBUG_PUT("   ����� ����:        %u\n", pConfigData->getConfigDataStructPenaBak()[i]->number)
						//	DEBUG_PUT("   �������:           %u\n", pConfigData->getConfigDataStructPenaBak()[i]->level)
						//	DEBUG_PUT("   ����� �����������: %u\n", pConfigData->getConfigDataStructPenaBak()[i]->address)
						//	DEBUG_PUT("   ����� �����:       %u\n\n", pConfigData->getConfigDataStructPenaBak()[i]->numberOnDevice)
						//}
						break;
					case SerialDebug::COMMAND_DEBUG_CONFIG:
						break;
				}
			break;
		}
	}
}
