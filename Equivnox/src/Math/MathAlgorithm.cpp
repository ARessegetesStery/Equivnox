#pragma once

#include "eqxpch.h"

namespace EQX
{
	/**
	 * Quick algorithm of calculating inverse sqrt
	 * 
	 * @param x x
	 * @return 1/sqrt(x)
	 */
	float InvSqrt(float x)
	{
		float xhalf = 0.5f * x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i >> 1); 
		x = *(float*)&i;
		x = x * (1.5f - xhalf * x * x); 
		return x;
	}

	float sgn(float x)
	{
		if (x == 0.f)
			return 0.f;
		else if (x > 0.f)
			return 1.f;
		else
			return -1.f;
	}
}
