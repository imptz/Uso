#pragma once

#include "../subsystems/detection/detectionSubsystem.h"
#include "../subsystems/detection/iDetectionDevice.h"

class Fv300Device : public IDetectionDevice, public MessageReceiver, public MessageSender
{
	public:
		static const unsigned int ID = 1;

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
			return new Fv300Device(address, _type);
		}
#pragma endregion
		Fv300Device(unsigned char _address, unsigned int _type);
		~Fv300Device();

		virtual void onMessage(Message message);

		virtual void action();
		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false);
		virtual void init();
		virtual bool isReady();
		virtual void timerHandler();
		virtual unsigned int getId();
		virtual void reset();

	private:
		void createInitFrame();
		void createGetInitializeFrame();
		void createSetUpdateListFrame();
		void createGetFireFrame();
		void createGetDeviceStateFrame();
		void createResetFrame();

		void buildFire();

		//struct ChannelInfo
		//{
		//	unsigned int chNumber;
		//	Point2<unsigned int> points;		
		//};
		ChannelInfo* points;
		unsigned int pointsCount;

	public:
		virtual void updateFire(unsigned char* pAddress, unsigned int count, ActionMoveToPoint** actionMoveToPoint = nullptr);
		virtual void updateFire(ChannelInfo* pChannelInfo, unsigned int count);
		virtual unsigned char* getFire();
};
