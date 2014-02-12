#pragma once

#include "../singleton.h"
#include "../timer/timer.h"
#include "../display/display.h"
#include "../message/messages.h"
#include "../serialport/serialport.h"
#include "../process/process.h"
#include "../serialport/serialport.h"

#include "configData.h"

class Config : public Task<Config>, public Singleton<Config>, public MessageSender, public MessageReceiver{
private:
	ConfigData* pConfigData;
	unsigned char* hddBuffer;	
	unsigned int hddTaskId;
	bool fReadWrite;
	bool fUpdate;

	SerialPort* serialPort;
	static const SERIAL_PORT UPDATE_SERIAL_PORT;
	static const SERIAL_PORT_SPEED SERIAL_PORT_UPDATE_SPEED;
	
	static const unsigned int CONNECT_CODE = 0xfa12c64b;
	static const unsigned int CONNECT_ANSWER_CODE = 0x45dc69a3;
	static const unsigned int DISCONNECT_CODE = 0x8e94ac13;
	static const unsigned int DOWNLOAD_RESULT_OK = 0x00000000;
	static const unsigned int DOWNLOAD_RESULT_FAULT = 0xffffffff;
	
	static const unsigned int LENGTH_SIZE = 4;
	static const unsigned int CRC_SIZE = 4;

	unsigned int totalSize;
	unsigned int loadSize;
	unsigned int loadProgress;
	
	unsigned int errorCode;

	unsigned char* loadBuffer;

	virtual void onMessage(Message message);
	unsigned int calcDataCrc();
	bool readConfigDataFromHdd();
	bool writeConfigDataToHdd();
	void initConfigAfterLoad();

public:
	static const unsigned int MESSAGE_CONFIG_HDD_COMPLETE_OK = 0;
	static const unsigned int MESSAGE_CONFIG_HDD_COMPLETE_FAILED = 1;
	static const unsigned int MESSAGE_CONFIG_UPDATE_COMPLETE_OK = 0;
	static const unsigned int MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED = 1;
	
	static const unsigned int UPDATE_FAILED_CODE_CONNECTION_VERSION = 1;
	static const unsigned int UPDATE_FAILED_CODE_TOTAL_SIZE = 2;
	static const unsigned int UPDATE_FAILED_CODE_CRC = 3;
	static const unsigned int UPDATE_FAILED_CODE_BLOCK_CODE = 4;
	static const unsigned int UPDATE_FAILED_CODE_CONSTANTS_SIZE = 5;
	static const unsigned int UPDATE_FAILED_CODE_PR_POSITIONS_SIZE = 6;
	static const unsigned int UPDATE_FAILED_CODE_IOBK_SIZE = 7;
	static const unsigned int UPDATE_FAILED_CODE_IOBK_OUTPUT_FUNCTION_GROUP_UNKNOWN = 8;
	static const unsigned int UPDATE_FAILED_CODE_IOBK_INPUT_FUNCTION_GROUP_UNKNOWN = 9;
	static const unsigned int UPDATE_FAILED_CODE_IOSERIAL_SIZE = 10;
	static const unsigned int UPDATE_FAILED_CODE_INIT_SIGNAL_SIZE = 11;
	static const unsigned int UPDATE_FAILED_CODE_PROGRAM_SIZE = 12;
	static const unsigned int UPDATE_FAILED_CODE_PROGRAM_FUNCTION_UNKNOWN = 13;
	static const unsigned int UPDATE_FAILED_CODE_FV300_SIZE = 14;
	static const unsigned int UPDATE_FAILED_CODE_TRAJECTORY_SIZE = 15;
	static const unsigned int UPDATE_FAILED_CODE_PRESSURE_SIZE = 16;
	static const unsigned int UPDATE_FAILED_CODE_PENABAK_SIZE = 17;
	static const unsigned int UPDATE_FAILED_CODE_SAVE = 18;

	Config();
	~Config();

	CPointer<Config> processStop();
	CPointer<Config> processReadFromHdd();
	CPointer<Config> processWriteToHdd();
	CPointer<Config> processUpdateConnection();
	CPointer<Config> processUpdateGetLength();
	CPointer<Config> processUpdateLoadData();
	CPointer<Config> processUpdateSave();
	CPointer<Config> processUpdateFailedConnection();
	
	CPointer<Config> processUpdateD();

	ConfigData* getConfigData();
	unsigned int getLoadProgress();
	bool readConfig();
	bool writeConfig();

	bool update();
	void cancelUpdate();

	unsigned char getCharFromLoadData(unsigned char **loadData);
	unsigned short getShortFromLoadData(unsigned char **loadData);
	unsigned int updateApply();
	bool applyConstants(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyPrPosition(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyIoBk(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyIoSerial(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyInitSignal(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyProgram(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyFv300(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyTrajectory(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyPressure(unsigned char** loadData, ConfigData* pNewConfigData);
	bool applyPenabak(unsigned char** loadData, ConfigData* pNewConfigData);

	//void saveConfig(pNewConfigData)
	void printConfig();

	int getInitSignalIndexByNumber(unsigned int number);
	void sendMessageToPort(unsigned char* pData, unsigned int size);

	unsigned char getPRAddressByNumber(unsigned char number);
	unsigned char getPRNumberByAddress(unsigned char address);
	unsigned char getPRIndexByAddress(unsigned char address);

	unsigned int getConfigDataStructProgramInitSignal(unsigned int initSignalNumber, unsigned int _function, unsigned int** pList);

	bool isTvExistsInConfig();
	bool isFv300ExistsInConfig();
};
