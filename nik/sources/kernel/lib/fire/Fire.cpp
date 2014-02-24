#include "Fire.h"
#include "../DEBUG/serialDebug.h"
#include "../config/Config.h"
#include "../math/sectorIntersection.h"

void Fire::calcFire(PreFire* preFires, FireObject* pFire, unsigned int preFiresCount)
{
// TODO зачем этот цикл???	
	//for (unsigned int preFireIndex = 0; preFireIndex < preFiresCount; preFireIndex++)
	//{
		Point2<float>* cmg = new Point2<float>[preFiresCount * preFiresCount];
		Point2<float>* cmv = new Point2<float>[preFiresCount * preFiresCount];
		int cmgCount = 0;
		int cmvCount = 0;

		for (unsigned int i = 0; i < preFiresCount - 1; i++)
			for (unsigned int j = i + 1; j < preFiresCount; j++)
			{
				Point2<float> p1 = Point2<float>(preFires[i].pivotPoint.x,preFires[i].pivotPoint.y);
				Point2<float> p2 = Point2<float>(preFires[j].pivotPoint.x,preFires[j].pivotPoint.y);
				Point2<float> p3 = Point2<float>(preFires[i].pivotPoint.x,preFires[i].pivotPoint.z);
				Point2<float> p4 = Point2<float>(preFires[j].pivotPoint.x,preFires[j].pivotPoint.z);

				float cs = preFires[i].center.x;
				cs /= 57.3;
//				Point2<float> v1 = Point2<float>(cos(cs), -sin(cs));
				Point2<float> v1 = Point2<float>(cos(cs), sin(cs));
				cs = preFires[j].center.x;
				cs /= 57.3;
//				Point2<float> v2 = Point2<float>(cos(cs), -sin(cs));
				Point2<float> v2 = Point2<float>(cos(cs), sin(cs));
				cs = preFires[i].center.y;
				cs /= 57.3;
//				Point2<float> v3 = Point2<float>(cos(cs), -sin(cs));
				Point2<float> v3 = Point2<float>(cos(cs), sin(cs));
				cs = preFires[j].center.y;
				cs /= 57.3;
//				Point2<float> v4 = Point2<float>(cos(cs), -sin(cs));
				Point2<float> v4 = Point2<float>(cos(cs), sin(cs));

				//cmg[cmgCount++] = LineOperation::lineIntersect(p1, p1 + v1, p2, p2 + v2); 
				//cmv[cmvCount++] = LineOperation::lineIntersect(p3, p3 + v3, p4, p4 + v4); 

//				cmg[cmgCount++] = LineOperation::lineIntersect(p1, p1 + v1, p2, p2 + v2); 
//				cmv[cmvCount++] = LineOperation::lineIntersect(p3, p3 + v3, p4, p4 + v4); 

//				LineSegment l1(Point2<float>(p1.x, p1.y), Point2<float>(v1.x * 10000,v1.y * 10000));
//				LineSegment l2(Point2<float>(p2.x, p2.y), Point2<float>(v2.x * 10000,v2.y * 10000));


//				Line l1(Point2<float>(p1.x, p1.y), Point2<float>(v1.x * 1000,v1.y * 1000));
//				Line l2(Point2<float>(p2.x, p2.y), Point2<float>(v2.x * 1000,v2.y * 1000));
				Line l1(Point2<float>(p1.x, p1.y), Point2<float>(p1.x + v1.x * 10, p1.y + v1.y * 10));
				Line l2(Point2<float>(p2.x, p2.y), Point2<float>(p2.x + v2.x * 10, p2.y + v2.y * 10));

				if (!l1.isParallelIntersect(l2))
				{
					if (l1.isIntersect(l2))
					{
						Point2<float> vIntersect = l1.getIntersectPoint(l2);
						cmg[cmgCount++] = Point2<float>(vIntersect.x, vIntersect.y);
					}				
					else
					{
					}
				}
				else
				{
				}
			}

		pFire->massCenter = Point3<float>(0, 0, 0);
		
		for (int i = 0; i < cmgCount; i++)
		{
			pFire->massCenter.x += cmg[i].x;
			pFire->massCenter.y += cmg[i].y;
			pFire->massCenter.z += 0;//cmv[i].y;
		}

		pFire->massCenter = pFire->massCenter / static_cast<float>(cmgCount);

		DEBUG_PUT_METHOD("pFire->massCenter.x = %f, pFire->massCenter.y = %f\n", pFire->massCenter.x, pFire->massCenter.y)

		delete[] cmg;
		delete[] cmv;
	//}
}

//void Fire::calcFireNEW(PreFire* preFires, FireObject* pFire, unsigned int preFiresCount)
//{
//	SortedDynamicArray<FireArea::Ray> data;
//
//	data.resize(preFiresCount * 2);
//	
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("Fire::calcFire___preFiresCount: ", preFiresCount);
//
//	for (unsigned int preFireIndex = 0; preFireIndex < preFiresCount; preFireIndex++)
//	{
//		data[preFireIndex].X = preFires[preFireIndex].pivotPoint.x;
//		data[preFireIndex].Y = preFires[preFireIndex].pivotPoint.y;
//		data[preFireIndex]. = preFires[preFireIndex].pivotPoint.x;
//		data[preFireIndex].X = preFires[preFireIndex].pivotPoint.x;
//
//			
//			float cs = preFires[i].center.x;
//				cs /= 57.3;
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("                   Fire::calcFire_____cs1: ", cs);
//				Point2<float> v1 = Point2<float>(cos(cs), -sin(cs));
//				cs = preFires[j].center.x;
//				cs /= 57.3;
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("                   Fire::calcFire_____cs2: ", cs);
//				Point2<float> v2 = Point2<float>(cos(cs), -sin(cs));
//				cs = preFires[i].center.y;
//				cs /= 57.3;
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("                   Fire::calcFire_____cs3: ", cs);
//				Point2<float> v3 = Point2<float>(cos(cs), -sin(cs));
//				cs = preFires[j].center.y;
//				cs /= 57.3;
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("                   Fire::calcFire_____cs4: ", cs);
//				Point2<float> v4 = Point2<float>(cos(cs), -sin(cs));
//
//				//cmg[cmgCount++] = LineOperation::lineIntersect(p1, p1 + v1, p2, p2 + v2); 
//				//cmv[cmvCount++] = LineOperation::lineIntersect(p3, p3 + v3, p4, p4 + v4); 
//
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("                   Fire::calcFire_____p1: ", p1.x, p1.y);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("                   Fire::calcFire_____p2: ", p2.x, p2.y);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("                   Fire::calcFire_____v1: ", v1.x, v1.y);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("                   Fire::calcFire_____v2: ", v2.x, v2.y);
//
//				cmg[cmgCount++] = LineOperation::lineIntersect(p1, p1 + v1, p2, p2 + v2); 
//				cmv[cmvCount++] = LineOperation::lineIntersect(p3, p3 + v3, p4, p4 + v4); 
//
//
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("Fire::calcFire: ", cmg[cmgCount - 1].x, cmg[cmgCount - 1].y);
//			}
//
//		pFire->massCenter = Point3<float>(0, 0, 0);
//		
//		for (int i = 0; i < cmgCount; i++)
//		{
//			pFire->massCenter.x += cmg[i].x;
//			pFire->massCenter.y += cmg[i].y;
//			pFire->massCenter.z += 0;//cmv[i].y;
//		}
//
//		pFire->massCenter = pFire->massCenter / cmgCount;
//	}
//}

void Fire::calcPressureTable(unsigned int prNumber, Fire::FireObject* fire, FireScanProgram* programs, unsigned int i)
{
	const float  pcon = 2360;
	const float  pis = 3.1457/180.0;
	const float  pis1 = 180.0/3.1457;

	double tempd;
	int an;
	float angle = 0.0f;
 
	ConfigDataStructPRPosition** prp = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();

	float length = fire->massCenter.distance(prp[Config::getSingleton().getConfigData()->getPRIndexByAddress(Config::getSingleton().getConfigData()->getPRAddressByNumber(prNumber))]->position) / 100.0f;

	an = static_cast<int>(angle * pis1);

	if (sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.6f) < 1.0f)
	{
		tempd=(asin(sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.6f)) + angle) / (pis * 2.0f);
		programs[i].pressureTable0 = static_cast<unsigned int>((tempd - an + 0.5f));
	}
	else 
		programs[i].pressureTable0 = 0;
   
	if (sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.64f) < 1.0f)
	{
		tempd=(asin(sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.64f)) + angle) / (pis * 2.0f);
		programs[i].pressureTable1 = static_cast<unsigned int>((tempd - an + 0.5f));
	}
	else 
		programs[i].pressureTable1 = 0; 

	if (sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.68f) < 1.0f)
	{
		tempd=(asin(sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.68f)) + angle) / (pis * 2.0f);
		programs[i].pressureTable2 = static_cast<unsigned int>((tempd - an + 0.5f));
	}
	else 
		programs[i].pressureTable2 = 0;

	if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.72f) < 1.0f)
	{
		tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.72f))+angle)/(pis*2.0f);
		programs[i].pressureTable3 = static_cast<unsigned int>((tempd-an+0.5f));
	}
	else 
		programs[i].pressureTable3 = 0;

	if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.76f) < 1.0f)
	{
		tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.76f))+angle)/(pis*2.0f);
		programs[i].pressureTable4 = static_cast<unsigned int>((tempd-an+0.5f));
	}
	else 
		programs[i].pressureTable4 = 0;

  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.80f) < 1.0f)
  {
   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.80f))+angle)/(pis*2.0f);
   programs[i].pressureTable5 = static_cast<unsigned int>((tempd-an+0.5f));
  }
  else 
	  programs[i].pressureTable5 = 0;

  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.84f) < 1.0f)
  {
   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.84f))+angle)/(pis*2.0f);
   programs[i].pressureTable6 = static_cast<unsigned int>((tempd-an+0.5f));
  }
  else 
	  programs[i].pressureTable6 = 0;

  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.86f) < 1.0f)
  {
   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.86f))+angle)/(pis*2.0f);
   programs[i].pressureTable7 = static_cast<unsigned int>((tempd-an+0.5f));
  }
  else 
	  programs[i].pressureTable7 = 0;

  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.92f) < 1.0f)
  {
   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.92f))+angle)/(pis*2.0f);
   programs[i].pressureTable8 = static_cast<unsigned int>((tempd-an+0.5f));
  }
  else 
	  programs[i].pressureTable8 = 0;

  programs[i].pressureTable9 = 0;
}

void Fire::calcProgram(unsigned int fireCount, PreFire* localFires, Fire::FireObject* fire, Fire::FireScanProgram** programs)
{
	ConfigDataStructPRPosition** prp = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();
	ConfigDataStructConst* dsConst = Config::getSingleton().getConfigData()->getConfigDataStructConst();

	if (*programs != nullptr)
		delete[] *programs;

	*programs = new FireScanProgram[fireCount];
	
	for (unsigned int i = 0; i < fireCount; i++)
	{
		(*programs)[i].prNumber = localFires[i].channel;
		(*programs)[i].point1 = Point2<unsigned int>(static_cast<unsigned int>(localFires[i].leftAngle), static_cast<unsigned int>(localFires[i].topAngle));
		(*programs)[i].point2 = Point2<unsigned int>(static_cast<unsigned int>(localFires[i].rightAngle), static_cast<unsigned int>(localFires[i].bottomAngle));

//		(*programs)[i].point1.x += localFires[i].prProg.x;
//		(*programs)[i].point2.x += localFires[i].prProg.x;
//		(*programs)[i].point1.y += localFires[i].prProg.y;
//		(*programs)[i].point2.y += localFires[i].prProg.y;

		(*programs)[i].point1.x += (360 - static_cast<int>(prp[Config::getSingleton().getConfigData()->getPRIndexByAddress(localFires[i].channel)]->orientation.x));
		(*programs)[i].point2.x += (360 - static_cast<int>(prp[Config::getSingleton().getConfigData()->getPRIndexByAddress(localFires[i].channel)]->orientation.x));

//		static const int BORDER = 5;

		(*programs)[i].point1.x -= Config::getSingleton().getConfigData()->getConfigDataStructConst()->leftField;
		(*programs)[i].point1.y += Config::getSingleton().getConfigData()->getConfigDataStructConst()->topField;
		(*programs)[i].point2.x += Config::getSingleton().getConfigData()->getConfigDataStructConst()->rightField;
		(*programs)[i].point2.y -= Config::getSingleton().getConfigData()->getConfigDataStructConst()->bottomField;

		(*programs)[i].point1.x += 360;
		(*programs)[i].point2.x += 360;
		(*programs)[i].point1.y += 360;
		(*programs)[i].point2.y += 360;

		if ((*programs)[i].point1.x > 359)
			(*programs)[i].point1.x = (*programs)[i].point1.x % 360;

		if ((*programs)[i].point2.x > 359)
			(*programs)[i].point2.x = (*programs)[i].point2.x % 360;

		if ((*programs)[i].point1.y > 359)
			(*programs)[i].point1.y = (*programs)[i].point1.y % 360;

		if ((*programs)[i].point2.y > 359)
			(*programs)[i].point2.y = (*programs)[i].point2.y % 360;

		if (fire->massCenter.distance(prp[Config::getSingleton().getConfigData()->getPRIndexByAddress(localFires[i].channel)]->position) < dsConst->minimumDistanceForCompactJet * 100) 
			(*programs)[i].nasadokPosition = 0;
		else
			(*programs)[i].nasadokPosition = 50000;
		
		(*programs)[i].step = 3;

		DEBUG_PUT_METHOD("i = %i, (*programs)[i].prNumber = %i, programs = %i\n", i, (*programs)[i].prNumber, programs);
		calcPressureTable((*programs)[i].prNumber, fire, *programs, i); /*180 gradusov*/
	}
}


//void Fire::calcPressureTable(unsigned int prNumber, Fire::FireObject* fire, FireScanProgram* programs, unsigned int i)
//{
//	const float  pcon = 2360;
//	const float  pis = 3.1457/180.0;
//	const float  pis1 = 180.0/3.1457;
//
//	double tempd;
//	int an;
//	float angle = 0.0f;
// 
//	ConfigDataStructPRPosition** prp = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();
//
//	float length = fire->massCenter.distance(prp[Config::getSingleton().getConfigData()->getPRIndexByAddress(Config::getSingleton().getConfigData()->getPRAddressByNumber(prNumber))]->position) / 100.0f;
//	float length = 0;
//	an = static_cast<int>(angle * pis1);
//
//	if (sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.6f) < 1.0f)
//	{
//		tempd=(asin(sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.6f)) + angle) / (pis * 2.0f);
//		programs[i].pressureTable0 = static_cast<unsigned int>((tempd - an + 0.5f));
//	}
//	else 
//		programs[i].pressureTable0 = 0;
//   
//	if (sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.64f) < 1.0f)
//	{
//		tempd=(asin(sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.64f)) + angle) / (pis * 2.0f);
//		programs[i].pressureTable1 = static_cast<unsigned int>((tempd - an + 0.5f));
//	}
//	else 
//		programs[i].pressureTable1 = 0; 
//
//	if (sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.68f) < 1.0f)
//	{
//		tempd=(asin(sin(angle) + cos(angle) * 9.81f * length / (pcon * 0.68f)) + angle) / (pis * 2.0f);
//		programs[i].pressureTable2 = static_cast<unsigned int>((tempd - an + 0.5f));
//	}
//	else 
//		programs[i].pressureTable2 = 0;
//
//	if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.72f) < 1.0f)
//	{
//		tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.72f))+angle)/(pis*2.0f);
//		programs[i].pressureTable3 = static_cast<unsigned int>((tempd-an+0.5f));
//	}
//	else 
//		programs[i].pressureTable3 = 0;
//
//	if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.76f) < 1.0f)
//	{
//		tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.76f))+angle)/(pis*2.0f);
//		programs[i].pressureTable4 = static_cast<unsigned int>((tempd-an+0.5f));
//	}
//	else 
//		programs[i].pressureTable4 = 0;
//
//  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.80f) < 1.0f)
//  {
//   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.80f))+angle)/(pis*2.0f);
//   programs[i].pressureTable5 = static_cast<unsigned int>((tempd-an+0.5f));
//  }
//  else 
//	  programs[i].pressureTable5 = 0;
//
//  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.84f) < 1.0f)
//  {
//   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.84f))+angle)/(pis*2.0f);
//   programs[i].pressureTable6 = static_cast<unsigned int>((tempd-an+0.5f));
//  }
//  else 
//	  programs[i].pressureTable6 = 0;
//
//  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.86f) < 1.0f)
//  {
//   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.86f))+angle)/(pis*2.0f);
//   programs[i].pressureTable7 = static_cast<unsigned int>((tempd-an+0.5f));
//  }
//  else 
//	  programs[i].pressureTable7 = 0;
//
//  if (sin(angle)+cos(angle)*9.81f*length/(pcon*0.92f) < 1.0f)
//  {
//   tempd=(asin(sin(angle)+cos(angle)*9.81f*length/(pcon*0.92f))+angle)/(pis*2.0f);
//   programs[i].pressureTable8 = static_cast<unsigned int>((tempd-an+0.5f));
//  }
//  else 
//	  programs[i].pressureTable8 = 0;
//
//  programs[i].pressureTable9 = 0;
//}
