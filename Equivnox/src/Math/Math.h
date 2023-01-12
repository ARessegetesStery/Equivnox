#pragma once

#include "eqxpch.h"

namespace EQX
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix4x4;

	const float FLOAT_PREC = FLT_EPSILON;
	const float FLOAT_MAX = FLT_MAX;
	const float Z_MAX = 1e4;				// The largest achievable absolute displacement in Z direction

	/// Declaration of functions in MathAlgorithm.cpp
	extern float InvSqrt(float x);
	extern float sgn(float x);
}
