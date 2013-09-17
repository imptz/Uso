#pragma once

#include "action.h"
#include "../math/math.h"

class ActionSearch : public Action
{
	private:
		virtual void timerHandler();
		unsigned int testTimeOut;
		static const unsigned int TEST_TIME_OUT = 180;
		ActionSearch();

	public:
		ActionSearch(unsigned char _deviceAddress, Point2<unsigned int> _point1, Point2<unsigned int> _point2, int _numberSourcesFlicker);
		virtual ~ActionSearch();
		virtual void onMessage(Message message);
		virtual void step();

	private:
		enum PHASE
		{
			PHASE_COMMAND,
			PHASE_COMMAND_WAIT,
			PHASE_TEST,
			PHASE_TEST_WAIT,
			PHASE_GET_RESULT,
			PHASE_GET_RESULT_WAIT,
			PHASE_STOP
		};
		PHASE phase;

		void error(int value = 0);
		void finish();

	public:
		Point2<unsigned int> point1;
		Point2<unsigned int> point2;
		Point2<unsigned int> resPoint1;
		Point2<unsigned int> resPoint2;
		int numberSourcesFlicker;

	public:
		bool getResult(Point2<int>& _point1, Point2<int>& _point2); 
};
