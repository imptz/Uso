#pragma once

float sqrt(float x);
float abs(float value);
float cos(float value);
float sin(float value);
float asin(float value);
float tan(float value);


extern "C"
{
    int __cdecl _ftol2(float f);
    int __cdecl _ftol2_sse(float f);
}

const float PI = 3.141592f;
const float MINIMUM_VALUE = 1.175494351e-38F;
const float INVALID_VALUE = 3.402823466e+38F;

template <typename T>
struct Point2
{
	T x;
	T y;

	Point2()
	{
	}

	Point2(T _x, T _y)
		:	x(_x), y(_y)
	{
	}

	Point2(const Point2& s)
		:	x(s.x), y(s.y)
	{
	}

	static Point2 getInvalidVector()
	{
		return Point2(INVALID_VALUE, INVALID_VALUE);
	}

	bool operator==(const Point2& rh)
	{
		return ((x == rh.x) && (y == rh.y));
	}

	Point2 operator-(const Point2& rh)
	{
		return Point2<T>(x - rh.x, y - rh.y);
	}

	Point2 operator+(const Point2& rh)
	{
		return Point2<T>(x + rh.x, y + rh.y);
	}

	Point2 operator*(float value)
	{
		return Point2<T>(x * value, y * value);
	}

	Point2 operator/(float value)
	{
		return Point2<T>(x / value, y / value);
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	float dotProduct(Point2& r)
	{
		float len = length() * r.length();
		
		if (len == 0)
			return 0;

		return (x * r.x + y * r.y) / (len);
	}
};

template <typename T>
struct Point3
{
	T x;
	T y;
	T z;

	Point3()
	{
	}

	Point3(T _x, T _y, T _z)
		:	x(_x), y(_y), z(_z)
	{
	}

	Point3(const Point3& s)
		:	x(s.x), y(s.y), z(s.z)
	{
	}

	bool operator==(const Point3& rh)
	{
		return ((x == rh.x) && (y == rh.y) && (z == rh.z));
	}

	Point3& operator=(Point3& rh)
	{
		x = rh.x;
		y = rh.y;
		z = rh.z;
		return *this;
	}

	Point3 operator-(const Point3& rh)
	{
		return Point3<T>(x - rh.x, y - rh.y, z - rh.z);
	}

	Point3 operator+(const Point3& rh)
	{
		return Point3<T>(x + rh.x, y + rh.y, z + rh.z);
	}

	Point3 operator*(float value)
	{
		return Point3<T>(x * value, y * value, z * value);
	}

	Point3 operator/(float value)
	{
		return Point3<T>(x / value, y / value, z / value);
	}

	float length()
	{
		return sqrt(x * x + y * y + z *z);
	}

	float distance(const Point3& rh)
	{
		return sqrt((x - rh.x) * (x - rh.x) + (y - rh.y) * (y - rh.y));// + (z - rh.z) * (z - rh.z));
	}

	Point3 crossProduct(Point3 vecTo)
	{
		Point3<T>  res;
 
		res.x = y * vecTo.z - vecTo.y * z;
		res.y = z * vecTo.x - vecTo.z * x;
		res.z = x * vecTo.y - vecTo.x * y;
 
		return res;
	}
};

class Sort
{
	public:
		template<typename T>
		static void maxToMin(T* data, unsigned int count)
		{
			if (count > 1)
			{
				for (unsigned int i = 0; i < count - 1; i++)
					for (unsigned int j = i + 1; j < count; j++)
					{
						if (data[i] < data[j])
						{
							T temp;
							temp = data[i];
							data[i] = data[j];
							data[j] = temp;
						}
					}
			}
		}
};

struct LineOperation
{
	static Point3<float> commonPerpendicular(Point3<float> p1, Point3<float> p2, Point3<float> v1, Point3<float> v2)
	{

		Point3<float> cross;
		Point3<float> point1;
		Point3<float> point2;
	
		float m11;
		float m12;
		float m13;
		float m14;
		float m21;
		float m22;
		float m23;
		float m24;
		float n1;
		float n2;
		float n3;
		float t1;
		float t2;

		cross = v1.crossProduct(v2);

		m11 = v1.y * cross.z - v1.z * cross.y;
		m12 = v1.x * cross.z - v1.z * cross.x;
		m13 = v1.y * cross.x - v1.x * cross.y;
		m14 = p1.x * m11 - p1.y * m12 - p1.z * m13;
	
		m21 = v2.y * cross.z - v2.z * cross.y;
		m22 = v2.x * cross.z - v2.z * cross.x;
		m23 = v2.y * cross.x - v2.x * cross.y;
		m24 = p2.x * m21 - p2.y * m22 - p2.z * m23;

		n1 = m11 - m21;
		n2 = m12 - m22;
		n3 = m13 - m23;

		float rProm = (-1 * v1.x * n1 + v1.y * n2 + v1.z * n3);
		if (abs(rProm) < 0.001)
			rProm = 0.001;

		t1 = -1 * (m14 - m24 - p1.x * n1 + p1.y * n2 + p1.z * n3) / rProm;

		rProm = (-1 * v2.x * n1 + v2.y * n2 + v2.z * n3);
		if (abs(rProm) < 0.001)
			rProm = 0.001;

		t2 = -1 * (m14 - m24 - p2.x * n1 + p2.y * n2 + p2.z * n3) / rProm;

		point1.x = p1.x + t1 * v1.x; 	
		point1.y = p1.y + t1 * v1.y; 	
		point1.z = p1.z + t1 * v1.z; 	

		point2.x = p2.x + t2 * v2.x; 	
		point2.y = p2.y + t2 * v2.y; 	
		point2.z = p2.z + t2 * v2.z; 

		cross = point1 - point2;
		cross = cross * 0.5;
		cross = cross + point2;

		return cross;
	}


	static bool testLineIntersect(Point3<float> p1, Point3<float> p2, Point3<float> v1, Point3<float> v2)
	{
		float l1;
		float l2;

		l1 = (p1 - p2).length();
		l2 = ((p1 + (v1 * 0.01)) - (p2 + (v2 * 0.01))).length();

		return l1 > l2;
	}

	static Point2<float> lineIntersect(Point2<float> LineAP1, Point2<float> LineAP2, Point2<float> LineBP1, Point2<float> LineBP2)
	{
		float LDetLineA, LDetLineB, LDetDivInv; 
		Point2<float> LDiffLA, LDiffLB; 

		Point2<float> result;

		LDetLineA = LineAP1.x * LineAP2.y - LineAP1.y * LineAP2.x; 
		LDetLineB = LineBP1.x * LineBP2.y - LineBP1.y * LineBP2.x; 

		LDiffLA = LineAP1 - LineAP2; 
		LDiffLB = LineBP1 - LineBP2; 

		if (abs((LDiffLA.x * LDiffLB.y) - (LDiffLA.y * LDiffLB.x)) > 0.001)
			LDetDivInv = 1 / ((LDiffLA.x * LDiffLB.y) - (LDiffLA.y * LDiffLB.x)); 
		else
			LDetDivInv = 100000;

		result.x = ((LDetLineA * LDiffLB.x) - (LDiffLA.x * LDetLineB)) * LDetDivInv; 
		result.y = ((LDetLineA * LDiffLB.y) - (LDiffLA.y * LDetLineB)) * LDetDivInv; 

		return result;
	}
};

class Line
{
protected:
public:
	Point2<float> p1;
	Point2<float> p2;

public:
	Line()
	{}

	Line(Point2<float> _p1, Point2<float> _p2)
		:	p1(_p1), p2(_p2)
	{
		// упорядочивание по x
		if (p1.x > p2.x)
		{
			Point2<float> t = p1;
			p1 = p2;
			p2 = t;
		}
	}

	bool isIntersect(Line& line)
	{
		// не учитывается параллельное пересечение
		return abs(((p2.y - p1.y) * (line.p1.x - line.p2.x) - (line.p2.y - line.p1.y) * (p1.x - p2.x))) > MINIMUM_VALUE;
	}

	bool isParallelIntersect(Line& line)
	{
		float f1 = (line.p1.x - p1.x) * (p2.y - p1.y);
		float f2 = (line.p1.y - p1.y) * (p2.x - p1.x);
		float f3 = (line.p2.x - p1.x) * (p2.y - p1.y);
		float f4 = (line.p2.y - p1.y) * (p2.x - p1.x);

		if ((f1 == f2) && (f2 == f3) && (f3 == f4))
			return true;
		else
			return false;
	}

	Point2<float> getIntersectPoint(Line& line)
	{
		// не учитывается параллельное пересечение
		float x = ((p1.x - p2.x) * (line.p2.x * line.p1.y - line.p1.x * line.p2.y) - (line.p1.x - line.p2.x) * (p2.x * p1.y - p1.x * p2.y)) / ((p2.y - p1.y) * (line.p1.x - line.p2.x) - (line.p2.y - line.p1.y) * (p1.x - p2.x));
		float y = ((p2.x * p1.y - p1.x * p2.y) * (line.p2.y - line.p1.y) - (line.p2.x * line.p1.y - line.p1.x * line.p2.y) * (p2.y - p1.y)) / ((p2.y - p1.y) * (line.p1.x - line.p2.x) - (line.p2.y - line.p1.y) * (p1.x - p2.x));

		return Point2<float>(x, y);
	}
};

class LineSegment : public Line
{
public:
	LineSegment()
	{}

	LineSegment(Point2<float> _p1, Point2<float> _p2)
		:	Line(_p1, _p2)
	{}

	bool isIntersect(LineSegment& lineSegment)
	{
		// не учитывается параллельное пересечение
		return !(getIntersectPoint(lineSegment) == Point2<float>::getInvalidVector());
	}

	bool isParallelIntersect(LineSegment& lineSegment)
	{
		if (Line::isParallelIntersect(lineSegment))
		{
			if ((((lineSegment.p1.x >= p1.x) && (lineSegment.p1.x <= p2.x)) ||
				((lineSegment.p2.x >= p1.x) && (lineSegment.p2.x <= p2.x))) &&
				(((lineSegment.p1.y >= p1.y) && (lineSegment.p1.y <= p2.y)) ||
				((lineSegment.p2.y >= p1.y) && (lineSegment.p2.y <= p2.y))))
				return true;

			if ((((lineSegment.p1.x <= p1.x) && (lineSegment.p2.x >= p2.x))) &&
				(((lineSegment.p1.y <= p1.y) && (lineSegment.p2.y >= p2.y))))
				return true;

			return false;
		}
		else
			return false;
	}

	Point2<float> getIntersectPoint(LineSegment& lineSegment)
	{
		// не учитывается параллельное пересечение
		if (abs(((p2.y - p1.y) * (lineSegment.p1.x - lineSegment.p2.x) - (lineSegment.p2.y - lineSegment.p1.y) * (p1.x - p2.x))) > MINIMUM_VALUE)
		{
			Point2<float> intersectionPoint(((p1.x - p2.x) * (lineSegment.p2.x * lineSegment.p1.y - lineSegment.p1.x * lineSegment.p2.y) - (lineSegment.p1.x - lineSegment.p2.x) * (p2.x * p1.y - p1.x * p2.y)) / 
				((p2.y - p1.y) * (lineSegment.p1.x - lineSegment.p2.x) - (lineSegment.p2.y - lineSegment.p1.y) * (p1.x - p2.x)),
				((p2.x * p1.y - p1.x * p2.y) * (lineSegment.p2.y - lineSegment.p1.y) - (lineSegment.p2.x * lineSegment.p1.y - lineSegment.p1.x * lineSegment.p2.y) * (p2.y - p1.y)) / 
				((p2.y - p1.y) * (lineSegment.p1.x - lineSegment.p2.x) - (lineSegment.p2.y - lineSegment.p1.y) * (p1.x - p2.x)));

			Point2<float> p1_intersection(intersectionPoint - p1);
			Point2<float> p1_p2(p2 - p1);

			if ((p1_intersection.dotProduct(p1_p2) > 0) && (p1_intersection.length() <= p1_p2.length()))
			{
				Point2<float> lineSegment_p1_intersection(intersectionPoint - lineSegment.p1);
				Point2<float> lineSegment_p1_p2(lineSegment.p2 - lineSegment.p1);
				
				if ((lineSegment_p1_intersection.dotProduct(lineSegment_p1_p2) > 0) && (lineSegment_p1_intersection.length() <= lineSegment_p1_p2.length()))
					return intersectionPoint;
				else
					return Point2<float>::getInvalidVector();
			}
			else
				return Point2<float>::getInvalidVector();
		}

		return Point2<float>::getInvalidVector();
	}
};

struct PreFire
		{
			unsigned char channel;
			
			Point3<float> pivotPoint;
			
			Point2<float> center;
			float leftAngle;
			float rightAngle;
			float topAngle;
			float bottomAngle;

			Point2<float> prProg;

			PreFire()
				:	channel(0),
				pivotPoint(Point3<float>(0,0,0)),
				center(Point2<float>(0,0)),
				leftAngle(0),
				rightAngle(0),
				topAngle(0),
				bottomAngle(0),
				prProg(Point2<float>(0,0))
			{
			}

			PreFire& operator=(PreFire& rh)
			{	
				channel = rh.channel;
				pivotPoint = rh.pivotPoint;
				center = rh.center;
				leftAngle = rh.leftAngle;
				rightAngle = rh.rightAngle;
				topAngle = rh.topAngle;
				bottomAngle = rh.bottomAngle;
				prProg = rh.prProg;

				return *this;
			}
		};

