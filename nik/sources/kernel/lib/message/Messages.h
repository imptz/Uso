#pragma once

#include "../fifo.h"
#include "../display/display.h"

enum MESSAGE_FROM_OFFSET{
	MESSAGE_FROM_OFFSET_CONTROLS = 0,
	MESSAGE_FROM_OFFSET_SYSTEM = 1000,
	MESSAGE_FROM_OFFSET_LOG = 2000,
	MESSAGE_FROM_OFFSET_CLOCK = 3000,
	MESSAGE_FROM_OFFSET_CONFIG = 4000,
	MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM = 5000,
	MESSAGE_FROM_OFFSET_APPLICATION = 6000,
	MESSAGE_FROM_OFFSET_LOGIC = 7000,
	MESSAGE_FROM_OFFSET_MONITOR_MANAGER = 8000,
	MESSAGE_FROM_OFFSET_DETECTION_MANAGER = 9000,
	MESSAGE_FROM_OFFSET_SERIAL_DEBUG = 10000
};

enum MESSAGE{
	MESSAGE_CONFIG_READ_FROM_HDD_COMPLETE,
	MESSAGE_CONFIG_WRITE_TO_HDD_COMPLETE
};

class MessageReceiver;

struct Message{
	MessageReceiver* to;
	unsigned int from;
	unsigned int msg;
	unsigned int par1;
	unsigned int par2;

	Message(){}

	Message(unsigned int _from, unsigned int _msg, unsigned int _par1, unsigned int _par2)
		:	from(_from), msg(_msg), par1(_par1), par2(_par2)
	{}
};

class MessageReceiver
{
public:
	static void init();
	virtual void onMessage(Message message) = 0;
protected:
	static const int MESSAGES_COUNT_MAX = 150;
	static Fifo<Message>* messages;

public:
	static void messagesProccess(){
		Message msg;
		if (messages->get(&msg) != 0)
			msg.to->onMessage(msg);
	}

	static void addMessage(Message message){
		messages->put(message);
	}

	static void clearAllMessages(){
		messages->clear();
	}
};

class MessageSender
{
private:
	static const int RECEIVER_MAX_COUNT = 150;
	typedef MessageReceiver* PReceiver;
	PReceiver receivers[RECEIVER_MAX_COUNT];
public:
	bool messageEnable;
	MessageSender()
		:	messageEnable(true)
	{
		for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
			receivers[i] = nullptr;
	}

	virtual ~MessageSender(){}

	void addReceiver(MessageReceiver* receiver)
	{
		for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
			if (receivers[i] == nullptr){
				receivers[i] = receiver;
				break;
			}
	}

	void removeReceiver(MessageReceiver* receiver){
		for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
			if (receivers[i] == receiver){
				receivers[i] = nullptr;
				break;
			}
	}

	virtual void sendMessage(Message message){
		if (messageEnable){
			for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
				if (receivers[i] != nullptr){
					message.to = receivers[i];
					MessageReceiver::addMessage(message);
				}
		}
	}
};

