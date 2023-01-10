#pragma once

#include "eqxpch.h"

namespace EQX {

	extern class Vector4;

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float, float, float);
		Vector3(const Vector3&);
		Vector3(const Vector4&);
		Vector3(Vector3&&) noexcept;

		Vector3 operator- (const Vector3& p);
		Vector3 operator+ (const Vector3& p);
		Vector3 operator/ (const float k);
		Vector3& operator= (const Vector3& p);
		Vector3 operator-= (const Vector3& p);
		Vector3 operator+= (const Vector3& p);
		Vector3 operator*= (float s);

		// index operator returns x if the index exceeds 2
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		Vector3 Neg() const;
		Vector3 Normalize() const;
		Vector4 ToVec4() const;

		float NormSquare() const;
		float Norm() const;

		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
	};

	float Dot(const Vector3&, const Vector3&);
	Vector3 Cross(const Vector3&, const Vector3&);

	bool operator== (const Vector3&, const Vector3&);

	Vector3 operator* (const float, const Vector3&);
	Vector3 operator* (const Vector3&, const float);
	
#ifdef EQX_DEBUG
	void Print(const Vector3&);
#endif
}