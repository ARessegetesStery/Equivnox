#pragma once

#include "eqxpch.h"

namespace EQX {

	class Vector3
	{
	public:
		double x;
		double y;
		double z;

		Vector3();
		Vector3(double, double, double);
		Vector3(const Vector3&);
		Vector3(Vector3&&) noexcept;

		Vector3 operator- (const Vector3& p);
		Vector3 operator+ (const Vector3& p);
		Vector3& operator= (const Vector3& p);

		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
	};
}