#pragma once

#include "../../../Singleton.h"
#include "../../../timer/Timer.h"
#include "../../../message/Messages.h"
#include "../ISubsystem.h"
#include "iRpkDevice.h"
#include "../../../DEBUG/serialDebug.h"
#include "../../ExtensionSystem.h"

class RpkSubsystem : public Singleton<RpkSubsystem>, public ISubsystem
{
	private:
		static const unsigned int SUBSYSTEM_TYPE = 4;

	private:
		virtual void timerHandler();

	public:
		RpkSubsystem();
		~RpkSubsystem();

		virtual void onMessage(Message message);

		enum IO_SUBSYSTEM_MESSAGE
		{
			IO_SUBSYSTEM_MESSAGE_SUBSYSTEM_READY = 41
		};

	private:
		static const unsigned int MAX_DEVICE_COUNT = 1;
		struct RegDevice
		{
			unsigned char id;
			IRpkDevice* (*pCreateDevice) (unsigned char, unsigned int);
			RegDevice()
				:	id(0), pCreateDevice(nullptr)
			{
			}
		};
		RegDevice devices[MAX_DEVICE_COUNT];
	public:
		bool regDevice(unsigned char id, IRpkDevice* (*pCreateDevice) (unsigned char, unsigned int))
		{
			for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
				if (devices[i].pCreateDevice == nullptr)
				{
					devices[i].id = id;
					devices[i].pCreateDevice = pCreateDevice;
					return true;
				}
			return false;
		}
		IRpkDevice* pDevices[MAX_DEVICE_COUNT];
		virtual bool addDevice(unsigned char id, unsigned char address);

		void printRegDev()
		{
			for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
				if (devices[i].pCreateDevice != nullptr)
				{
				}
		}

		virtual void action();
		virtual void init();
   		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false);
		virtual bool isReady();

	private:
		static const char* MISSING_DEVICE_LOG_TEXT;
	public:
		virtual void testMissingDevices();

		unsigned int write(unsigned char* pFrame);
		IRpkDevice::FRAME_RESULT read(unsigned int id, unsigned char** pFrame);
};
				
