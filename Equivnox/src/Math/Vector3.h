#pragma once

#include "eqxpch.h"

namespace EQX {

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float, float, float);
		Vector3(const Vector3&);
		Vector3(Vector3&&) noexcept;

		Vector3 operator- (const Vector3& p);
		Vector3 operator+ (const Vector3& p);
		Vector3& operator= (const Vector3& p);

		// index operator returns x if the index exceeds 2
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
	};
}