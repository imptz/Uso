#pragma once

#include "../Singleton.h"
#include "../timer/Timer.h"
#include "../message/Messages.h"
#include "../serialport/serialport.h"
#include "bus.h"
#include "../crc16.h"

#include "subsystems\ISubsystem.h"

#include "../DEBUG/serialDebug.h"

class ExtensionSystem : public Singleton<ExtensionSystem>, public ITimer, public MessageReceiver, public MessageSender
{
	private:
		static const unsigned int TIMER_PERIOD = 1;
		virtual void timerHandler();

	public:
		ExtensionSystem();
		~ExtensionSystem();

		virtual void onMessage(Message message);

		enum EXTENSION_SYSTEM_MESSAGE
		{
			EXTENSION_SYSTEM_MESSAGE_DATA_READY = 27,
			EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA = 40
		};

	private:
		static const SERIAL_PORT PORT_NAME;
		static const SERIAL_PORT_SPEED PORT_SPEED;
		SerialPort* serialPort;

	public:
		static const unsigned char ADDRESS = 0x01;
	private:
		enum PHASE
		{
			PHASE_READY,
			PHASE_1,
			PHASE_2
		};
		PHASE phase;

		unsigned int length;
		unsigned int position;
		static const unsigned int MAX_TIME_COUNTER = 5;
		unsigned int timeCounter;

		enum PRE_START_PHASE
		{
			PRE_START_PHASE_STOP,
			PRE_START_PHASE_GET_ID_SEND,
			PRE_START_PHASE_GET_ID_RECV,
			PRE_START_PHASE_CONFIG_WAIT,
			PRE_START_PHASE_FINISH
		};
		PRE_START_PHASE preStartPhase;

	private:
		enum TRANSFER_STATE
		{
			TRANSFER_STATE_ACTIVE,
			TRANSFER_STATE_TIME_OUT,
			TRANSFER_STATE_READY
		};
		volatile TRANSFER_STATE transferState;
		static const unsigned int MINIMAL_TRANSFER_TIME = 200;
		unsigned int transferTimer;
		void enableTransfer(unsigned int _dataSize);

		static const unsigned int FIRST_DEVICE_ADDRESS = 2;
		static const unsigned int EXTENSION_DEVICE_COUNT = 32;
		unsigned int extDevIndex;

		void preStartReadHandlerGetId();

	private:
		unsigned char* pArea;

		enum STOP_REASON
		{
			STOP_REASON_NORMAL,
			STOP_REASON_TIME_OUT
		};
		void stopLoadTime(STOP_REASON value);

	private:
		bool sendGetID(unsigned char deviceAddress);

	public:
		void init();
	private:
		static const unsigned int MAX_SUBSYSTEM_COUNT = 32;
		struct RegSubsystem
		{
			unsigned char type;
			ISubsystem* pISubsystem;

			RegSubsystem()
				:	type(0), pISubsystem(nullptr)
			{
			}
		};
		RegSubsystem subsystems[MAX_SUBSYSTEM_COUNT];
		int getSubsystemIndexByType(unsigned char type)
		{
			for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
				if (subsystems[i].type == type)
					return i;

			return -1;
		}

	public:
		bool regSubsystem(unsigned char type, ISubsystem* pISubsystem)
		{
			for (unsigned int i = 0; i < MAX_SUBSYSTEM_COUNT; i++)
				if (subsystems[i].pISubsystem == nullptr)
				{
					subsystems[i].type = type;
					subsystems[i].pISubsystem = pISubsystem;
					return true;
				}

			return false;
		}

		void printRegSubsystem();
		bool isReady();

		void action();
		void busAction();
		bool getIdAction();

		enum PACKING
		{
			PACKING_ON,
			PACKING_OFF
		};

	private:
		static const unsigned int FIFO_FRAME_SIZE = 1000;
		Fifo<unsigned char*>* fifoFrame;
		void sendFrame(unsigned char* _pData);
		void testMissingDevices();

		static const unsigned int MAX_TRANSFER_TRY_COUNT = 3;
		unsigned char transferTryCount;
	private:
		unsigned int subsytemActionIndex;
		unsigned char* pSendData;

		static const unsigned int MAX_FRAME_LENGTH = 1000;

		void putFrame(unsigned char* _pArea, bool isNotTransfer = false);

		bool recvTimeOutEvent;
		static const unsigned int AFTER_RECV_TIME_OUT = 2;
		unsigned int afterRecvTimeOut;
		bool isRecvFinish;
		void initAfterRecvTimeOut();

	public:
		void forceAddDevice(unsigned int _type, unsigned int _id, unsigned int _address);
};

