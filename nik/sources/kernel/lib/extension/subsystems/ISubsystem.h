#pragma once

class ISubsystem : public ITimer, public MessageReceiver, public MessageSender
{
	protected:
		enum PHASE
		{
			PHASE_STOP,
			PHASE_INIT,
			PHASE_START
		};
		PHASE phase;

		static const unsigned int TIMER_PERIOD = 50;

	public:
		virtual bool addDevice(unsigned char id, unsigned char address) = 0;
		virtual void action() = 0;
		virtual void init() = 0;
		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false) = 0;
		virtual bool isReady() = 0;

		ISubsystem()
			:	ITimer(TIMER_PERIOD), phase(PHASE_STOP), deviceActionIndex(0)
		{}

		virtual void testMissingDevices() = 0;
	protected:
		unsigned int deviceActionIndex;
};
