#pragma once

#include <Windows.h>

namespace Fr
{
	byte* rectangleCenterStringReflection(byte* dst, byte* src, unsigned int width, unsigned int height)
	{
		for (unsigned int i = 0; i < height / 2 ; i++)
		{
			memcpy(dst + i * width, src + (height - i - 1) * width, width);
			memcpy(dst + (height - i - 1) * width, src + i * width, width);
		}

		return dst;
	}
}
