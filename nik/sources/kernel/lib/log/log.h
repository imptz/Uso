#ifndef LOG_H
#define LOG_H

#include "../string.h"
#include "../singleton.h"
#include "../message/messages.h"
#include "../display/display.h"
#include "../timer/timer.h"
#include "../clock/clock.h"

enum LOG_MESSAGE_FROM{
	LOG_MESSAGE_FROM_UNDEFINED,
	LOG_MESSAGE_FROM_LOG,
	LOG_MESSAGE_FROM_APPLICATION,
	LOG_MESSAGE_FROM_CONFIGURATION,
	LOG_MESSAGE_FROM_NETWORK_CONTROLLER,
	LOG_MESSAGE_FROM_IODEVICE,
	LOG_MESSAGE_FROM_CONFIG,
	LOG_MESSAGE_FROM_TOOLS_DATE,
	LOG_MESSAGE_FROM_LOGIC,
	LOG_MESSAGE_FROM_EXTENSION_SYSTEM,
	LOG_MESSAGE_FROM_MONITORING
};

enum LOG_MESSAGE_TYPE{
	LOG_MESSAGE_TYPE_UNDEFINED,
	LOG_MESSAGE_TYPE_SYSTEM,
	LOG_MESSAGE_TYPE_ERROR,
	LOG_MESSAGE_TYPE_INFO,
	LOG_MESSAGE_TYPE_MESSAGE
};

enum LOG_POSITION{
	LOG_POSITION_LAST,
	LOG_POSITION_NEXT,
	LOG_POSITION_PREV
};

struct LogMessage{
	static const int STR_LENGTH = 40;

	bool valid;				// 4
	LOG_MESSAGE_FROM from;	// 4 
	LOG_MESSAGE_TYPE type;	// 4
	char str[STR_LENGTH];	// 40
	unsigned int n1;		// 4 
	unsigned int n2;		// 4	
	unsigned int mDate;		// 4
	unsigned int mTime;		// 4
							// 4+4+4+40+4+4+4+4 = 68
	LogMessage()
		:	valid(false), from(LOG_MESSAGE_FROM_UNDEFINED),
		type(LOG_MESSAGE_TYPE_UNDEFINED),
		n1(0), n2(0)
	{		
		memset(&str[0], 0, STR_LENGTH);
		Clock::DateTime dt = Clock::getSingleton().getClock();
		mTime = dt.second + dt.minute * 256 + dt.hour * 256 * 256;
		mDate = dt.day + dt.month * 256 + dt.year * 256 * 256;
	}

	LogMessage(LOG_MESSAGE_FROM _from,
		LOG_MESSAGE_TYPE _type,
		char* _str,
		int _n1,
		int _n2)
		:	valid(true),
		from(_from),
		type(_type),
		n1(_n1),
		n2(_n2)
	{		
		memset(&str[0], ' ', STR_LENGTH);
		str[39] = 0;
		unsigned int strLen = strlen(_str);
		if (strLen > STR_LENGTH)
			strLen = STR_LENGTH;
		memcpy(&str[0], _str, strLen);
		Clock::DateTime dt = Clock::getSingleton().getClock();
		mTime = dt.second + dt.minute * 256 + dt.hour * 256 * 256;
		mDate = dt.day + dt.month * 256 + dt.year * 256 * 256;
	}

	LogMessage& operator=(LogMessage& logMessage){
		valid = logMessage.valid;
		from = logMessage.from;
		type = logMessage.type;

		memcpy(str, logMessage.str, STR_LENGTH);
		
		n1 = logMessage.n1;
		n2 = logMessage.n2;	
		mDate = logMessage.mDate;
		mTime = logMessage.mTime;
		
		return *this;
	}

	void toString(char* _str){
		char st[20];
		memcpy(&_str[0],Display::getSingleton().toStringDate(mDate, &st[0]), 8);
		_str[8] = ' ';
		memcpy(&_str[9],Display::getSingleton().toStringTime(mTime, &st[0]), 8);
		_str[17] = ' ';
		memcpy(&_str[18], str, STR_LENGTH);
		for (int i = 18 + strlen(str); i < 18 + STR_LENGTH; i++)
			_str[i] = ' '; 
		_str[18 + STR_LENGTH] = ' ';
		_str[19 + STR_LENGTH] = ' ';
		_str[20 + STR_LENGTH] = ' ';
		memcpy(&_str[20 + STR_LENGTH + 1], Display::getSingleton().toString2Num(n1, &st[0]), 2);
		_str[20 + STR_LENGTH + 3] = ' ';
		memcpy(&_str[20 + STR_LENGTH + 4], Display::getSingleton().toString2Num(n2, &st[0]), 2);
		_str[67] = 0;
	}
};

struct LogHeader{
	static const unsigned int _MS = 0x9a2f04d7; 
	unsigned int ms; 
	bool isFull;
	unsigned long writeMessageNumber;
};

class Log : public Singleton<Log>, public MessageSender, public ITimer{
private:
	static const unsigned int TIMER_PERIOD = 1000 * 60;
	static const unsigned int START_SECTOR = 20000;
	static const unsigned int BUFFER_SIZE = 65536; 
	static const int MAX_MESSAGE_COUNT = 920;

	enum LOG_CREATED_CODE{
		LOG_CREATED_CODE_HDD_ERROR = 1,
		LOG_CREATED_CODE_NEW = 2,
		LOG_CREATED_CODE_CLEAR = 3
	};

	unsigned char* buffer;
	LogHeader header;
	LogMessage* logMessages;
	unsigned long readMessageNumber;
	static char* LOG_CREATE_MESSAGE;
	unsigned int writeCounter;
	bool shutdown;

	unsigned int writeLog();
	void createNewLog();
	virtual void timerHandler();

public:
	Log();
	virtual ~Log();
	void add(LOG_MESSAGE_FROM _from, LOG_MESSAGE_TYPE _type, char* _str, int n1, int n2);
	LogMessage get(LOG_POSITION position);
	int getMessagesCount();
	int getMessagePosition();
	bool isUpdated();
	void asyncWrite();	
	void clear();
	void setShutdown();
};

#endif
