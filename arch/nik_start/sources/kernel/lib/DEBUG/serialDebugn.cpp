#include "serialDebugn.h"
#include "../display/display.h"
#include "../string.h"

SerialDebugn::SerialDebugn()
	:	clientsCount(0)
{	
}

SerialDebugn::~SerialDebugn()
{
}

bool SerialDebugn::regClient(MessageReceiver* pReceiver, const char* ctrlKeysString, const char* commandsString)
{
	if (clientsCount >= CLIENTS_MAX_COUNT)
		return false;

	clients[clientsCount++].set(pReceiver, ctrlKeysString, commandsString);
	return true;
}
