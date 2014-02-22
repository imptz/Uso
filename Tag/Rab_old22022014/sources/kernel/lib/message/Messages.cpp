#include "Messages.h"

Fifo<Message>* MessageReceiver::messages;

void MessageReceiver::init()
{
	messages = new Fifo<Message>(MESSAGES_COUNT_MAX);
}
