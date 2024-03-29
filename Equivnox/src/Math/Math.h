#pragma once

/* Math tools are independent of the rest of the project.
 * All files in `Math` directory should NOT include `CoreMinimal.h` or other enhanced versions of it.
 *
 **/

#include <float.h>

namespace EQX
{
	// Forward declaration of math classes
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix4x4;
	class Quaternion;

	class Line;
	class Plane;

	const float EQX_PI = 3.1415892653589793f;

	using Vec2 = Vector2;
	using Vec3 = Vector3;
	using Vec4 = Vector4;
	using Mat4 = Matrix4x4;
	using Quat = Quaternion;

	const float FLOAT_PREC = FLT_EPSILON;
	const float FLOAT_MAX = FLT_MAX;
	const float Z_MAX = 1e4;				// The largest achievable absolute displacement in Z direction

	const float SLOPE_MAX = 1000;

	enum class Axis
	{
		AXIS_X, AXIS_Y, AXIS_Z
	};

	/*  Declaration of functions in MathAlgorithm.cpp  */
	extern float InvSqrt(float x);
	extern float sgn(float x);
	
	extern Vec3 AlphaLerp(const Vec3 v1, const Vec3 v2, float alpha);

	// returns v1 if there exists no or infinite solutions
	extern Vec3 XLerp(const Vec3 v1, const Vec3 v2, float x);
	extern Vec3 YLerp(const Vec3 v1, const Vec3 v2, float y);
	extern Vec3 ZLerp(const Vec3 v1, const Vec3 v2, float z);
	extern Vec3 Lerp(const Vec3 v1, const Vec3 v2, float z, Axis axis);

	// returns 0 if there exists no or infinite solutions
	// returns {alpha}: {vector-with-x} = {alpha} * v1 + (1 - {alpha}) * v2
	extern float LerpCoeffX(const Vec3 v1, const Vec3 v2, float x);
	extern float LerpCoeffY(const Vec3 v1, const Vec3 v2, float y);
	extern float LerpCoeffZ(const Vec3 v1, const Vec3 v2, float z);
	extern float LerpCoeff(const Vec3 v1, const Vec3 v2, float z, Axis axis);

	extern float EvalGaussian(float x);
	extern float EvalBSplineCubic(float x);
	extern float EvalCRCubic(float x);

	/*  Simple inline function  */
	inline int BoolToInt(bool b) { return b ? 1 : 0; }
	/**
	 * @param a - one boundary
	 * @param b - the other boundary
	 * @param x - variable to be evaluated
	 * @return true if is in the interval; false otherwise
	 */
	inline bool IsInInterval(float a, float b, float x) { return (a <= x && x <= b) || (a >= x && x >= b); }
}
