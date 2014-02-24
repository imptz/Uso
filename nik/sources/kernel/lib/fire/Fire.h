#ifndef FIRE_H
#define FIRE_H

#include "../math/math.h"

class Fire
{
	private:
		Fire();
		virtual ~Fire() = 0
		{
		}

	public:
		struct FireObject
		{
			Point3<float> pivotPoint;
			Point3<float> axisX;
			Point3<float> axisY;
			Point3<float> axisZ;
			Point3<float> massCenter;
		};

		struct FireScanProgram
		{
//			unsigned int pressureTable[100];
			unsigned int pressureTable0;
			unsigned int pressureTable1;
			unsigned int pressureTable2;
			unsigned int pressureTable3;
			unsigned int pressureTable4;
			unsigned int pressureTable5;
			unsigned int pressureTable6;
			unsigned int pressureTable7;
			unsigned int pressureTable8;
			unsigned int pressureTable9;
			unsigned int prNumber;
			Point2<unsigned int> point1;
			Point2<unsigned int> point2;
			unsigned int step;
			unsigned int nasadokPosition;
		};

	private:
		static void calcPressureTable(unsigned int prNumber, Fire::FireObject* fire, FireScanProgram* programs, unsigned int i);

	public:
		static void calcFire(PreFire* preFires, FireObject* pFire, unsigned int preFiresCount);
		static void calcFireNEW(PreFire* preFires, FireObject* pFire, unsigned int preFiresCount);
		static void calcProgram(unsigned int fireCount, PreFire* localFires, Fire::FireObject* fire, Fire::FireScanProgram** programs);
};

#endif
