#ifndef ACTION_H
#define ACTION_H

#include "../timer/Timer.h"
#include "../message/Messages.h"
#include "../extension/subsystems/rpk/iRpkDevice.h"
#include "RpkCommands.h"

class Action : public ITimer, public MessageReceiver
{
	private:
		static const unsigned int TIMER_PERIOD = 1000;
		virtual void timerHandler() = 0;

	protected:
		unsigned char deviceAddress;
	public:
		unsigned char getDeviceAddress()
		{
			return deviceAddress;
		}
		
	public:
		enum STATE
		{
			STATE_UNDEFINED,
			STATE_READY,
			STATE_ERROR
		};
	protected:
		STATE state;
	public:
		STATE getState()
		{
			return state;
		}

	public:
		Action(unsigned char _deviceAddress = 0)
			:	ITimer(TIMER_PERIOD), deviceAddress(_deviceAddress), state(STATE_UNDEFINED), frameId(-1), timeCounter(0)
		{
			pTimer->start();
		}

		virtual ~Action() = 0
		{
		}

		virtual void step() = 0;

	protected:
		unsigned int frameId;
		unsigned int timeCounter;
};

#endif