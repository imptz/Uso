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


	virtual void onMessage(Message message);
	unsigned int calcDataCrc();
	bool readConfigDataFromHdd();
	bool writeConfigDataToHdd();

public:
	static const unsigned int MESSAGE_CONFIG_HDD_COMPLETE_OK = 0;
	static const unsigned int MESSAGE_CONFIG_HDD_COMPLETE_FAILED = 1;
	static const unsigned int MESSAGE_CONFIG_UPDATE_COMPLETE_OK = 0;
	static const unsigned int MESSAGE_CONFIG_UPDATE_COMPLETE_FAILED = 1;
	static const unsigned int MESSAGE_CONFIG_UPDATE_FAILED_CODE_CONNECTION = 1;

	Config();
	~Config();

	CPointer<Config> processStop();
	CPointer<Config> processReadFromHdd();
	CPointer<Config> processWriteToHdd();
	CPointer<Config> processUpdateConnection();
	CPointer<Config> processUpdateD();
	CPointer<Config> processUpdateFailedConnection();

	ConfigData* getConfigData();
	bool readConfig();
	bool writeConfig();

	bool update();
	void cancelUpdate();
};
