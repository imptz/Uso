#include "math.h"

float sqrt(float x)
{
	__asm
	{
		fld x
		fsqrt 
		fstp x
	}
	return x;
}

float abs(float value)
{
	if (value < 0)
		return value * (-1);
	else
		return value;
}

float cos(float value)
{
	float value1 = 0.0f;
	__asm
	{
		fld value
		fcos 
		fstp value1
	}
	return value1;
};

float sin(float value)
{
	__asm
	{
		fld value
		fsin 
		fstp value
	}
	return value;
};

float asin(float value)
{
	__asm
	{
		fld value
		fld st(0)
		fmul st,st(0)
		fld1
		fsubr
		fsqrt
		fpatan
		fst value
	}
	return value;
};

float tan(float value)
{
	float vSin = sin(value);
	float vCos = cos(value);
	
	if (vCos == 0)
		return 0.0f;
	else
		return vSin / vCos;
};

extern "C"
{
    int __cdecl _ftol2(float f)
    {
        volatile int result;
        __asm fistp result;
        return result;
    }

    int __cdecl _ftol2_sse(float f)
    {
        volatile int result;
        __asm fistp result;
        return result;
    }
}