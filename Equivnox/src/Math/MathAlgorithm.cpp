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

	Vec3 XLerp(const Vec3 v1, const Vec3 v2, float x)
	{
		float coeff = LerpCoeffX(v1, v2, x);
		return coeff * v1 + (1 - coeff) * v2;
	}

	Vec3 YLerp(const Vec3 v1, const Vec3 v2, float y)
	{
		float coeff = LerpCoeffY(v1, v2, y);
		return coeff * v1 + (1 - coeff) * v2;
	}

	Vec3 ZLerp(const Vec3 v1, const Vec3 v2, float z)
	{
		float coeff = LerpCoeffZ(v1, v2, z);
		return coeff * v1 + (1 - coeff) * v2;
	}

	Vec3 Lerp(const Vec3 v1, const Vec3 v2, float a, Axis axis)
	{
		switch (axis)
		{
		case Axis::AXIS_X:
			return XLerp(v1, v2, a);
		case Axis::AXIS_Y:
			return YLerp(v1, v2, a);
		case Axis::AXIS_Z:
			return ZLerp(v1, v2, a);
		default:
			return XLerp(v1, v2, a);
		}
	}

	Vec3 AlphaLerp(const Vec3 v1, const Vec3 v2, float alpha)
	{
		Vec3 diff = v2 - v1;
		return v1 + alpha * diff;
	}

	float LerpCoeffX(const Vec3 v1, const Vec3 v2, float x)
	{
		if (v1.x == v2.x)
			return 0.f;
		else
			return (x - v2.x) / (v1.x - v2.x);
	}

	float LerpCoeffY(const Vec3 v1, const Vec3 v2, float y)
	{
		if (v1.y == v2.y)
			return 0.f;
		else
			return (y - v2.y) / (v1.y - v2.y);
	}

	float LerpCoeffZ(const Vec3 v1, const Vec3 v2, float z)
	{
		if (v1.z == v2.z)
			return 0.f;
		else
			return (z - v2.z) / (v1.z - v2.z);
	}

	float LerpCoeff(const Vec3 v1, const Vec3 v2, float a, Axis axis)
	{
		switch (axis)
		{
		case Axis::AXIS_X:
			return LerpCoeffX(v1, v2, a);
		case Axis::AXIS_Y:
			return LerpCoeffY(v1, v2, a);
		case Axis::AXIS_Z:
			return LerpCoeffZ(v1, v2, a);
		default:
			return LerpCoeffX(v1, v2, a);
		}
	}

}
