#ifndef SERIAL_DEBUGN_H
#define SERIAL_DEBUGN_H

#include "../Singleton.h"
#include "../message/messages.h"
#include "../string.h"

class SerialDebugn : public Singleton<SerialDebugn>
{
private:
	struct Client
	{
		MessageReceiver* pReceiver;
		char* ctrlKeysString;
		char* commandsString;

		Client()
			:	pReceiver(nullptr), ctrlKeysString(nullptr), commandsString(nullptr)
		{}

		~Client()
		{
			SAFE_DELETE(pReceiver)
			SAFE_DELETE_ARRAY(ctrlKeysString)
			SAFE_DELETE_ARRAY(commandsString)
		}

		void set(MessageReceiver* _pReceiver, const char* _ctrlKeysString, const char* _commandsString)
		{
			pReceiver = _pReceiver;
			
			SAFE_DELETE_ARRAY(ctrlKeysString)
			ctrlKeysString = new char[strlen(_ctrlKeysString) + 1];
			strcpy(ctrlKeysString, _ctrlKeysString);

			SAFE_DELETE_ARRAY(commandsString)
			ctrlKeysString = new char[strlen(_commandsString) + 1];
			strcpy(commandsString, _commandsString);
		}
	};

	static const int CLIENTS_MAX_COUNT = 150;
	Client clients[CLIENTS_MAX_COUNT];
	unsigned int clientsCount;
	
public:
	SerialDebugn();
	~SerialDebugn();

private:

public:
	bool regClient(MessageReceiver* pReceiver, const char* ctrlKeysString, const char* commandsString);
};

#endif
