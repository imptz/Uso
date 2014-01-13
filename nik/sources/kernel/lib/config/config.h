#pragma once

#include "../singleton.h"
#include "../timer/timer.h"
#include "../display/display.h"
#include "../message/messages.h"
#include "../serialport/serialport.h"
#include "../process/process.h"

#include "configData.h"

class Config : public Task<Config>, public Singleton<Config>, public MessageSender, public MessageReceiver{
private:
	ConfigData* pConfigData;
	unsigned char* hddBuffer;	
	unsigned int hddTaskId;
	bool fReadWrite;

	virtual void onMessage(Message message);
	unsigned int calcDataCrc();

public:
	static const unsigned int MESSAGE_CONFIG_HDD_COMPLETE_OK = 0;
	static const unsigned int MESSAGE_CONFIG_HDD_COMPLETE_FAILED = 1;

	Config();
	~Config();

	CPointer<Config> processStop();
	CPointer<Config> processReadFromHdd();
	CPointer<Config> processWriteToHdd();

	ConfigData* getConfigData();
	bool readConfigDataFromHdd();
	bool writeConfigDataToHdd();
};
