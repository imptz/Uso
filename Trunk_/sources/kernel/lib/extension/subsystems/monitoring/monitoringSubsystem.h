#pragma once

#include "../../../Singleton.h"
#include "../../../timer/Timer.h"
#include "../../../message/Messages.h"
#include "../ISubsystem.h"
#include "iMonitoringDevice.h"
#include "../../../DEBUG/serialDebug.h"
#include "../../ExtensionSystem.h"

class MonitoringSubsystem : public Singleton<MonitoringSubsystem>, public ISubsystem
{
	private:
		static const unsigned int SUBSYSTEM_TYPE = 3;

	private:
		virtual void timerHandler();

	public:
		MonitoringSubsystem();
		~MonitoringSubsystem();

		virtual void onMessage(Message message);

#pragma region registration
	private:
		static const unsigned int MAX_DEVICE_COUNT = 32;
		struct RegDevice
		{
			unsigned char id;
			IMonitoringDevice* (*pCreateDevice) (unsigned char, unsigned int);
			RegDevice()
				:	id(0), pCreateDevice(nullptr)
			{
			}
		};
		RegDevice devices[MAX_DEVICE_COUNT];
	public:
		bool regDevice(unsigned char id, IMonitoringDevice* (*pCreateDevice) (unsigned char, unsigned int))
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
		IMonitoringDevice* pDevices[MAX_DEVICE_COUNT];
		virtual bool addDevice(unsigned char id, unsigned char address);

		void printRegDev()
		{
			for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
				if (devices[i].pCreateDevice != nullptr)
				{
				}
		}
#pragma endregion

#pragma region action
		virtual void action();
		virtual void init();
   		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false);
		virtual bool isReady();

	private:
		static const char* MISSING_DEVICE_LOG_TEXT;
	public:
		virtual void testMissingDevices();
#pragma endregion

#pragma region monitoringSpecific

	public:
		enum MONITOR_MANAGER_MESSAGE
		{
			MONITOR_MANAGER_MESSAGE_MANAGER_READY = 34,
			MONITOR_MANAGER_MESSAGE_EVENT = 35,
			MONITOR_MANAGER_MESSAGE_RPK = 36
		};

	public:
		void createAndSendMessage(IMonitoringDevice::MESSAGE_NUMBER messageNumber, unsigned char parameter1 = 0, unsigned char parameter2 = 0, unsigned char parameter3 = 0, unsigned char parameter4 = 0);
#pragma endregion
};

