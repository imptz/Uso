#pragma once

#include "../subsystems/detection/detectionSubsystem.h"
#include "../subsystems/detection/iDetectionDevice.h"
#include "../../action/actionSearch.h"

class ScannerDevice : public IDetectionDevice, public MessageReceiver, public MessageSender
{
	public:
		static const unsigned int ID = 3;

#pragma region static methods
	public:
		static bool registered;
		static void registerDevice()
		{
			if (!registered)
				DetectionSubsystem::getSingleton().regDevice(ID, createDevice);

			registered = true;
		}

		static IDetectionDevice* createDevice(unsigned char address, unsigned int _type)
		{
			return new ScannerDevice(address, _type);
		}
#pragma endregion
		ScannerDevice(unsigned char _address, unsigned int _type);
		~ScannerDevice();

		virtual void onMessage(Message message);

		virtual void action();
		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false);
		virtual void init();
		virtual bool isReady();
		virtual void timerHandler();
		virtual unsigned int getId();
		virtual void reset();

	private:
		void createSetUpdateListFrame();
		void createGetFireFrame();
		void createGetDeviceStateFrame();

		ActionSearch** actionSearch;
		unsigned int actionCount;

		ChannelInfo* pChannelInfo;
		static const unsigned int TIME_OUT = 240 * 20;

		unsigned char* fireFrame;

	public:
		virtual void updateFire(unsigned char* pAddress, unsigned int count, ActionMoveToPoint** actionMoveToPoint = nullptr);
		virtual void updateFire(ChannelInfo* _pChannelInfo, unsigned int count);
		virtual unsigned char* getFire();
};
