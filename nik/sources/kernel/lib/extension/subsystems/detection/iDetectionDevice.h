#pragma once

#include "../../devices/IDevice.h"
#include "../../../DEBUG/serialDebug.h"
#include "../../../math/math.h"
#include "../../../action/actionMoveToPoint.h"

class IDetectionDevice : public IDevice
{
	protected:
		enum PHASE
		{
			PHASE_STOP,
			PHASE_CONFIG,
//11062013
			PHASE_CONFIG_0,
			PHASE_CONFIG_1,
//11062013_
			PHASE_CONFIG_WAIT,
			PHASE_INIT_WAIT,
			PHASE_START,
			PHASE_SET_UPDATE_LIST,
			PHASE_SET_UPDATE_LIST_WAIT,
			PHASE_SET_GET_UPDATE_FLAG,
			PHASE_SET_GET_UPDATE_FLAG_WAIT,
			PHASE_SET_GET_FIRE,
			PHASE_SET_GET_FIRE_WAIT,
		};
		PHASE phase;
	public:
		enum COMMAND
		{
			COMMAND_GET_ID = 1, 
			COMMAND_INIT = 2, 
			COMMAND_GET_INITIALIZE_RESULT = 3,
			COMMAND_SET_UPDATE_LIST = 4,
			COMMAND_GET_FIRE = 5,
			COMMAND_GET_STATE = 6,
			COMMAND_RESET_DEVICES = 7
		};

		IDetectionDevice(unsigned char _address, unsigned int _type)
			:	IDevice(_address, _type), phase(PHASE_STOP), fireFrame(nullptr), updateChannels(nullptr), updateChannelsCount(0), fireTimeout(0)
		{}

#pragma region detectionSpecific
		struct Device
		{
			unsigned int address;
		};
	protected:
public:
		Device* pDevices;
		unsigned int devicesCount;
#pragma region 

	protected:
		virtual void disablingDevice()
		{
			disabled = true;
			phase = PHASE_START;
		}

	protected:
		unsigned char* fireFrame;
	public:
		virtual void updateFire(unsigned char* pAddress, unsigned int count, ActionMoveToPoint** actionMoveToPoint = nullptr) = 0;

		struct ChannelInfo
		{
			unsigned char address;
			Point2<unsigned int> point1;
			Point2<unsigned int> point2;
			unsigned int mode;
		};

		virtual void updateFire(ChannelInfo* pChannelInfo, unsigned int count) = 0;
		virtual unsigned char* getFire() = 0;
		virtual void reset() = 0;

	protected:
		unsigned char* updateChannels;
		unsigned int updateChannelsCount;
		unsigned int fireTimeout;
};
