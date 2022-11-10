#pragma once

#include "eqxpch.h"

namespace EQX {

	/**
	 * A 4-dimensional vector
	 * @warning w should never be zero
	 */
	class Vector4
	{
	public:
		double x;
		double y;
		double z;
		double w;

	public:
		Vector4();
		Vector4(double, double, double, double w = 1.0);
		Vector4(const Vector4&);
		Vector4(Vector4&&) noexcept;

		Vector4 operator- (const Vector4& p);
		Vector4 operator+ (const Vector4& p);
		Vector4& operator= (const Vector4& p);

		void normalize();

		static const Vector4 ZERO;
	};

}