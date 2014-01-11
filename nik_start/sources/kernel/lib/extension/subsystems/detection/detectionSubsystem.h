#pragma once

#include "../../../Singleton.h"
#include "../../../timer/Timer.h"
#include "../../../message/Messages.h"
#include "../ISubsystem.h"
#include "iDetectionDevice.h"
#include "../../../DEBUG/serialDebug.h"
#include "../../ExtensionSystem.h"
#include "../../../math/math.h"
#include "../../../fire/Fire.h"
#include "../../../action/actionMoveToPoint.h"
//#include "../../devices/scannerDevice.h"

class DetectionSubsystem : public Singleton<DetectionSubsystem>, public ISubsystem
{
	private:
		static const unsigned int SUBSYSTEM_TYPE = 2;

	private:
		virtual void timerHandler();

	public:
		DetectionSubsystem();
		~DetectionSubsystem();

		virtual void onMessage(Message message);

#pragma region detectionSpecific
		enum DETECTION_PHASE
		{
			DETECTION_PHASE_STOP,
			DETECTION_PHASE_MOVE_PR,
			DETECTION_PHASE_GET_FIRE,
			DETECTION_PHASE_SCANNER_WAIT
		};
		DETECTION_PHASE detectionPhase;
		void searchFire(unsigned int* listProgramIndex, unsigned int count);
		void searchFireJustirovka(unsigned char address);

	private:
		unsigned char* pAddress;
		Point2<unsigned int>* pPoints;
		unsigned int addressPointsCount;

	public:
		enum DETECTION_MANAGER_MESSAGE
		{
			DETECTION_MANAGER_MESSAGE_MANAGER_READY = 23,
			DETECTION_MANAGER_MESSAGE_DEVICE_FAULT = 24,
			DETECTION_MANAGER_MESSAGE_DEVICE_RECOVERY = 25,
			DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED = 26
		};

		ActionMoveToPoint** actionMoveToPoint;
		unsigned int actionMoveToPointCount;
		void movePR();
		void createPointsList(unsigned int* listProgramIndex, unsigned int count);
		void createPointsListForScanner(unsigned int* listProgramIndex, unsigned int count);

		enum DETECTION
		{
			DETECTION_PROCCESS,
			DETECTION_FAULT,
			DETECTION_READY
		};
		DETECTION detection;

		bool createPreFires(PreFire** preFires, unsigned int* count);
		void correctionPreFires(PreFire* preFires, unsigned int count);
		void convertionPreFiresToObjectSpace(PreFire* preFires, unsigned int count);

		unsigned int getFire(PreFire** preFires, Fire::FireObject* pFire);
		bool getFireJustirovka();

	private:
		bool isFireExists(unsigned char* _frame);

	public:
		void resetDiveces();

	private:
		static const unsigned int COORD_OFFSET_COUNT = 40;
		Point3<float> coordOffset[COORD_OFFSET_COUNT];
		static const unsigned int START_SECTOR = 60000;
		static const unsigned int BUFFER_SIZE = 512; 
		unsigned char buffer[BUFFER_SIZE];
		unsigned int hddTaskId;

		enum HDD_PHASE
		{
			HDD_PHASE_STOP,
			HDD_PHASE_WRITE_1,
			HDD_PHASE_WRITE_2,
			HDD_PHASE_READ_1,
			HDD_PHASE_READ_2
		};
		HDD_PHASE hddPhase;
		void hddAction();

	public:
		void setCoordOffset(unsigned int address, Point3<float> value);
		Point3<float> getCoordOffset(unsigned int address);

	private:
		IDetectionDevice::ChannelInfo* pChannelInfo;

#pragma endregion

#pragma region registration
	private:
		static const unsigned int MAX_DEVICE_COUNT = 32;
		struct RegDevice
		{
			unsigned char id;
			IDetectionDevice* (*pCreateDevice) (unsigned char, unsigned int);
			RegDevice()
				:	id(0), pCreateDevice(nullptr)
			{
			}
		};
		RegDevice devices[MAX_DEVICE_COUNT];
	public:
		bool regDevice(unsigned char id, IDetectionDevice* (*pCreateDevice) (unsigned char, unsigned int))
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
		IDetectionDevice* pDevices[MAX_DEVICE_COUNT];
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
};
