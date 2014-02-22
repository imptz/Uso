#pragma once

#include "action.h"
#include "../math/math.h"

class ActionMoveToPoint : public Action
{
	private:
		virtual void timerHandler();
		static const unsigned int TEST_TIME_OUT = 120;
		ActionMoveToPoint();

	public:
		ActionMoveToPoint(unsigned char _deviceAddress, Point2<unsigned int> _point, unsigned int _nasadok = 0);
		virtual ~ActionMoveToPoint();
		virtual void onMessage(Message message);
		virtual void step();

	private:
		enum PHASE
		{
			PHASE_COMMAND,
			PHASE_COMMAND_WAIT,
			PHASE_TEST,
			PHASE_TEST_WAIT,
			PHASE_STOP
		};
		PHASE phase;

		int frameId;

		void error();
		void finish();

		Point2<unsigned int> point;
		unsigned int nasadok;

	public:
		Point2<unsigned int> getPoint();
};
