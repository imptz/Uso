#include "../array/dynamicArray.h"
#include "math.h"

class FireArea
{
public:
	struct Points
	{
		double X;
		double Y;
		bool PositivePoint;
		int Robot;
	};

	struct Ray
	{
		double X;
		double Y;
		double AngleX; // угол в градусах [0,360)
		bool LeftRay; //false - right ray. относительно угла зрения 
		int Robot; //робот
		int RobotNumber; //число роботов (+ 2 два угла для ограничения, если надо)
		SortedDynamicArray<FireArea::Points> pointOfIntersection;
	};

	struct SectionPoints
	{
		double X;
		double Y;
	};

	struct SectionPoints3
	{
		double X;
		double Y;
		double Z;
	};

	SortedDynamicArray<FireArea::SectionPoints>* FindRec(SortedDynamicArray<FireArea::Ray>& rays)
	{
		int n = rays.getSize();

		if (n < 4)
			return nullptr;

		SortedDynamicArray<FireArea::SectionPoints>* res = new SortedDynamicArray<FireArea::SectionPoints>();
		res->resize(2);

		int i, j;
		double x0, y0; // начало основного луча
		double x1, y1; //начало луча, который пересекаем с основным
		double x, y; //точка пересечения
		double a0, a1; //углы лучей в радианах от 0 до 2Pi
		bool leftRay;
		int t = 0;

		for (i = 0; i <= n - 1; i++)
		{
			x0 = rays[i].X;
			y0 = rays[i].Y;
			a0 = rays[i].AngleX * PI / 180;

			t = rays[i].pointOfIntersection.getSize();
			rays[i].pointOfIntersection.resize(t + 1);
			rays[i].pointOfIntersection[t].X = rays[i].X;
			rays[i].pointOfIntersection[t].Y = rays[i].Y;
			rays[i].pointOfIntersection[t].PositivePoint = true;
			rays[i].pointOfIntersection[t].Robot = rays[i].Robot;

			for (j = i + 1; j <= n - 1; j++)
			{
				if (rays[i].Robot == rays[j].Robot)
					continue;

				x1 = rays[j].X;
				y1 = rays[j].Y;
				a1 = rays[j].AngleX * PI / 180;
				leftRay = rays[j].LeftRay;

				//параллельные
				if (a0 == a1 || a0 == a1 + PI || a0 == a1 - PI)
				{
					if ((y0 - x0 * static_cast<double>(static_cast<double>(tan(static_cast<float>(a0)))) == y1 - x1 * static_cast<double>(tan(static_cast<float>(a1)))) && (a0 != PI / 2) && (a0 != 3 * PI / 2))
					{
						rays[i].RobotNumber--;
						rays[j].RobotNumber--;
					}
					
					if ((x0  ==  x1 ) && (a0 == PI / 2) && (a0 == 3 * PI / 2))
					{
						rays[i].RobotNumber--;
						rays[j].RobotNumber--;
					}
					
					continue;
				}
				else 
					if ((a1 != a0) && (a0 != a1 + PI) && ((a1 != PI / 2) || (a1 != 3 * PI / 2)) && ((a0 == PI / 2) || (a0 == 3 * PI / 2)))
					{
						x = x0;
						y = y1 + (x0 - x1) * static_cast<double>(tan(static_cast<float>(a1)));
					}
					else 
						if ((a1 != a0) && (a0 != a1 + PI) && ((a1 == PI / 2) || (a1 == 3 * PI / 2)) && ((a0 != PI / 2) || (a0 != 3 * PI / 2)))
						{
							x = x1;
							y = y0 + (x1 - x0) * static_cast<double>(tan(static_cast<float>(a0)));
						}
						else 
						{
							x = (y1 - y0 + x0 * static_cast<double>(tan(static_cast<float>(a0))) - x1 * static_cast<double>(tan(static_cast<float>(a1)))) / static_cast<double>((tan(static_cast<float>(a0)) - tan(static_cast<float>(a1))));
							y = y0 + (x - x0) * static_cast<double>(tan(static_cast<float>(a0)));
						}

				if (SeePoint(rays[j], x, y))
				{
					t = rays[i].pointOfIntersection.getSize();
					rays[i].pointOfIntersection.resize(t + 1);
					rays[i].pointOfIntersection[t].X = x;
					rays[i].pointOfIntersection[t].Y = y;
					rays[i].pointOfIntersection[t].Robot = rays[j].Robot;

					double a = a1 - a0;
					if (a < 0)
						a = a + 2 * PI;
								
					rays[i].pointOfIntersection[t].PositivePoint = (a > 0 && a <= PI);
				    if (!rays[j].LeftRay)
						rays[i].pointOfIntersection[t].PositivePoint = !rays[i].pointOfIntersection[t].PositivePoint;
				}

				if (SeePoint(rays[i], x, y))
				{
					t = rays[j].pointOfIntersection.getSize();
					rays[j].pointOfIntersection.resize(t + 1);
					rays[j].pointOfIntersection[t].X = x;
					rays[j].pointOfIntersection[t].Y = y;
					rays[j].pointOfIntersection[t].Robot = rays[i].Robot;

					double a = a0 - a1;
					if (a < 0)
						a = a + 2 * PI;
    
					rays[j].pointOfIntersection[t].PositivePoint = (a > 0 && a <= PI);
					if (!rays[i].LeftRay)
						rays[j].pointOfIntersection[t].PositivePoint = !rays[j].pointOfIntersection[t].PositivePoint;
				}
			} // цикл по j

			//сортировка
			if (a0 == PI / 2)
				rays[i].pointOfIntersection.orderBy(&Points::Y, &Points::PositivePoint, false, true);
			else 
				if (a0 == 3 * PI / 2)
                    rays[i].pointOfIntersection.orderBy(&Points::Y, &Points::PositivePoint, true, true);
                else 
					if (a0 > PI / 2 && a0 < 3 * PI / 2)
	                    rays[i].pointOfIntersection.orderBy(&Points::X, &Points::PositivePoint, true, true);
	                else
						rays[i].pointOfIntersection.orderBy(&Points::X, &Points::PositivePoint, false, true);
		} // цикл по i

		for (i = 0; i <= n - 1; i++)
		{
			for (auto point = rays[i].pointOfIntersection.begin(); point != rays[i].pointOfIntersection.end(); ++point)
			{
				if (!(*point).PositivePoint)
				{
					int locCount = 0;
					for (auto locPoint = rays[i].pointOfIntersection.begin(); locPoint != rays[i].pointOfIntersection.end(); ++locPoint)// неверный итератор
					{
						if ((*locPoint).PositivePoint && ((*locPoint).Robot == (*point).Robot))
							locCount++;
					}
                        
					if (locCount == 0)
						rays[i].RobotNumber--;
				}
			}
		}

		auto sectionPoints = GetSectionPoints(rays);

		if (sectionPoints->getSize() < 1)
			return nullptr;            

		(*res)[0].X = sectionPoints->getMax(&SectionPoints::X);
		(*res)[0].Y = sectionPoints->getMax(&SectionPoints::Y);
		(*res)[1].X = sectionPoints->getMin(&SectionPoints::X);
		(*res)[1].Y = sectionPoints->getMin(&SectionPoints::Y);

		delete sectionPoints;
		return res;
	}


	bool SeePoint(Ray& ray, double x, double y)
	{
		double robot_x = ray.X;
		double robot_y = ray.Y;
		double angle = ray.AngleX * PI / 180;

		if (angle < 0)
		{
			angle = angle + 2 * PI;
		}
		if (angle == 0)
		{
			return x > robot_x;
		}
		else 
			if (angle == PI)
			{
				return x < robot_x;
			}
			else 
				if (angle > 0 && angle < PI)
				{
					return y > robot_y;
				}
				else
				{
					return y < robot_y;
				}
	}

	SortedDynamicArray<FireArea::SectionPoints>* GetSectionPoints(SortedDynamicArray<FireArea::Ray>& rays)
	{
		SortedDynamicArray<FireArea::SectionPoints>* sectionPoints = new SortedDynamicArray<FireArea::SectionPoints>();

		int k = 0;
		for(int i = 0; i < rays.getSize(); i++)
		{
			if (rays[i].pointOfIntersection.getSize() < 2)
				continue;

			int n = 1;

			for(int j = 0; j < rays[i].pointOfIntersection.getSize(); j++)
			{
				if (!rays[i].pointOfIntersection[j].PositivePoint && n <= rays[i].RobotNumber)
					break;

				if (n == rays[i].RobotNumber)
				{
					sectionPoints->resize(sectionPoints->getSize() + 1);
					(*sectionPoints)[k].X = rays[i].pointOfIntersection[j].X;
					(*sectionPoints)[k].Y = rays[i].pointOfIntersection[j].Y;
					k++;
				}

				if (n > rays[i].RobotNumber)
				{
					sectionPoints->resize(sectionPoints->getSize() + 1);
					(*sectionPoints)[k].X = rays[i].pointOfIntersection[j].X;
					(*sectionPoints)[k].Y = rays[i].pointOfIntersection[j].Y;
					k++;
					break;
				}
				
				n++;
			}
		}
        
		return sectionPoints;
	}
};

