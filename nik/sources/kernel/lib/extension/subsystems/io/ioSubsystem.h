#pragma once

#include "../../../Singleton.h"
#include "../../../timer/Timer.h"
#include "../../../message/Messages.h"
#include "../ISubsystem.h"
#include "iIoDevice.h"
#include "../../../DEBUG/serialDebug.h"
#include "../../ExtensionSystem.h"

class IOSubsystem : public Singleton<IOSubsystem>, public ISubsystem
{
	private:
		static const unsigned int SUBSYSTEM_TYPE = 1;

	private:
		virtual void timerHandler();

	public:
		IOSubsystem();
		~IOSubsystem();

		virtual void onMessage(Message message);

#pragma region registration
	private:
		static const unsigned int MAX_DEVICE_COUNT = 32;
		struct RegDevice
		{
			unsigned char id;
			IIODevice* (*pCreateDevice) (unsigned char, unsigned int);
			RegDevice()
				:	id(0), pCreateDevice(nullptr)
			{
			}
		};
		RegDevice devices[MAX_DEVICE_COUNT];
	public:
		bool regDevice(unsigned char id, IIODevice* (*pCreateDevice) (unsigned char, unsigned int))
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
		IIODevice* pDevices[MAX_DEVICE_COUNT];
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

#pragma region ioSpecific
		IIODevice::INPUT_STATE getInput(unsigned int number);
		void setOutput(unsigned int number, IIODevice::OUTPUT_STATE value);
		void setOutputs(unsigned int* numbers, unsigned int count, IIODevice::OUTPUT_STATE value);

		enum IO_SUBSYSTEM_MESSAGE
		{
			IO_SUBSYSTEM_MESSAGE_SUBSYSTEM_READY = 28,
			IO_SUBSYSTEM_MESSAGE_INPUT_STATE_CHANGE = 29,
			IO_SUBSYSTEM_MESSAGE_DEVICE_FAULT = 30,
			IO_SUBSYSTEM_MESSAGE_DEVICE_RECOVERY = 31
		};

		void enableAllFireAlarmOutputs();
		void disableAllFireAlarmOutputs();
		void enableAllHardwareOutputs();
		void disableAllHardwareOutputs();
		void enableAllPumpStationOutputs();
		void disableAllPumpStationOutputs();
		void enablePrPressureOutputs(unsigned char number);
		void disablePrPressureOutputs(unsigned char number);
		void enableGateOutputs(unsigned char number);
		void disableGateOutputs(unsigned char number);
		void enableSystemFaultOutputs();
		void disableSystemFaultOutputs();
		void enablePrFaultOutputs(unsigned char number);
		void disablePrFaultOutputs(unsigned char number);
		void enableGateFaultOutputs(unsigned char number);
		void disableGateFaultOutputs(unsigned char number);
		void enableResetPozharSignalisacijaOutputs();
		void disableResetPozharSignalisacijaOutputs();
#pragma endregion 		
};
				
