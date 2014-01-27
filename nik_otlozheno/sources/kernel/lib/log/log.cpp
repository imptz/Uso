#include "log.h"
#include "../local.h"
#include "../hdd/hddManager.h"
#include "../debug/serialDebug.h"

char* Log::LOG_CREATE_MESSAGE = LOCAL_LOG_LOG_CREATE_MESSAGE;

void Log::createNewLog(){
	logMessages = new LogMessage[MAX_MESSAGE_COUNT];
	header.ms = LogHeader::_MS;
	header.isFull = false;
	readMessageNumber = 0;
	header.writeMessageNumber = 0;
}

Log::Log()
	:	ITimer(TIMER_PERIOD), buffer(new unsigned char[BUFFER_SIZE]), writeCounter(0), shutdown(false)
{
	createNewLog();
	
	unsigned int _id = HddManager::getSingleton().read(buffer, START_SECTOR, BUFFER_SIZE / 512);
	
	if (_id != HddManager::UNDEFINED_ID){
		int count = 10000000;
		while (HddManager::getSingleton().isTaskExecute(_id)){
			if (count == 0)
				break;
			else
				count--;
		}

		if (count != 0){
			memcpy(&header, buffer, sizeof(header));
			if (header.ms == LogHeader::_MS)
				memcpy(logMessages, &buffer[512], sizeof(LogMessage) * MAX_MESSAGE_COUNT);
			else{
				header.ms = LogHeader::_MS;
				header.isFull = false;
				readMessageNumber = 0;
				header.writeMessageNumber = 0;
				add(LOG_MESSAGE_FROM_LOG, LOG_MESSAGE_TYPE_SYSTEM, LOG_CREATE_MESSAGE, 0, LOG_CREATED_CODE_NEW);
			}
		}else{
			header.ms = LogHeader::_MS;
			header.isFull = false;
			readMessageNumber = 0;
			header.writeMessageNumber = 0;
			add(LOG_MESSAGE_FROM_LOG, LOG_MESSAGE_TYPE_SYSTEM, LOG_CREATE_MESSAGE, 0, LOG_CREATED_CODE_HDD_ERROR);
		}
	}

	pTimer->start();
}

Log::~Log(){
	writeLog();
	delete[] buffer;
}

void Log::add(LOG_MESSAGE_FROM _from, LOG_MESSAGE_TYPE _type, char* _str, int n1, int n2){
	if (!shutdown){
		LogMessage message(_from, _type, _str, n1, n2);

		logMessages[header.writeMessageNumber] = message;

		if (header.writeMessageNumber < MAX_MESSAGE_COUNT - 1)
			header.writeMessageNumber++;
		else{
			header.writeMessageNumber = 0;
			header.isFull = true;
		}

		sendMessage(Message(MESSAGE_FROM_OFFSET_LOG, MESSAGE_LOG_NEW_RECORD, getMessagesCount(), 0));

		if (writeCounter == 0)
			writeCounter++;
	}
}

LogMessage Log::get(LOG_POSITION position){
	if (!shutdown){
		if (header.isFull){
			switch (position){
				case LOG_POSITION_LAST:
					if (header.writeMessageNumber == 0)
						readMessageNumber = MAX_MESSAGE_COUNT - 1;
					else
						readMessageNumber = header.writeMessageNumber - 1;
					break;
				case LOG_POSITION_NEXT:
					if (readMessageNumber == MAX_MESSAGE_COUNT - 1)
						readMessageNumber = 0;
					else
						readMessageNumber++;
					break;
				case LOG_POSITION_PREV:
					if (readMessageNumber == 0)
						readMessageNumber = MAX_MESSAGE_COUNT - 1;
					else
						readMessageNumber--;
					break;
			}
		}else{
			if (header.writeMessageNumber == 0)
				readMessageNumber = 0;
		
			switch (position){
				case LOG_POSITION_LAST:
					readMessageNumber = header.writeMessageNumber - 1;
					break;
				case LOG_POSITION_NEXT:
					if (readMessageNumber == header.writeMessageNumber - 1)
						readMessageNumber = 0;
					else
						readMessageNumber++;
					break;
				case LOG_POSITION_PREV:
					if (readMessageNumber == 0)
						readMessageNumber = header.writeMessageNumber - 1;
					else
						readMessageNumber--;
					break;
			}
		}
	}

	return logMessages[readMessageNumber];
}

int Log::getMessagesCount(){
	if (header.isFull)
		return MAX_MESSAGE_COUNT;
	else
		return header.writeMessageNumber;
}

unsigned int Log::writeLog(){
	memcpy(buffer, &header, sizeof(header));
	memcpy(&buffer[512], logMessages, sizeof(LogMessage) * MAX_MESSAGE_COUNT);
	return HddManager::getSingleton().write(buffer, START_SECTOR, BUFFER_SIZE / 512);
}

int Log::getMessagePosition(){
	return readMessageNumber;
}

void Log::timerHandler(){
	if (writeCounter != 0){
		if (writeLog() != HddManager::UNDEFINED_ID)
			writeCounter--;
	}
}

void Log::clear(){
	header.ms = LogHeader::_MS;
	header.isFull = false;
	readMessageNumber = 0;
	header.writeMessageNumber = 0;
	writeCounter++;
	add(LOG_MESSAGE_FROM_LOG, LOG_MESSAGE_TYPE_SYSTEM, LOG_CREATE_MESSAGE, 0, LOG_CREATED_CODE_CLEAR);
	sendMessage(Message(MESSAGE_FROM_OFFSET_LOG, MESSAGE_LOG_CLEAR, getMessagesCount(), 0));
}

void Log::setShutdown(){
	shutdown = true;
	unsigned int _id = writeLog();
	while (HddManager::getSingleton().isTaskExecute(_id)){}
}
