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
		Vector3 operator* (const float coeff);
		Vector3& operator= (const Vector3& p);
		Vector3 operator-= (const Vector3& p);
		Vector3 operator+= (const Vector3& p);
		Vector3 operator*= (float s);

		// index operator returns x if the index exceeds 2
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		Vector3 Neg() const;
		Vector3 Normalize() const;

		float Norm() const;

		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
	};

	float Dot(const Vector3&, const Vector3&);
	Vector3 Cross(const Vector3&, const Vector3&);

	bool operator== (const Vector3&, const Vector3&);

}