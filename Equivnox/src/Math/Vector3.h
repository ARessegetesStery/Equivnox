#pragma once

#include "Math.h"

namespace EQX {

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3();
		Vector3(const float, const float, const float);
		Vector3(const Vector3&);
		Vector3(const Vector4&);
		Vector3(Vector3&&) noexcept;

		Vector3 operator- (const Vector3& p) const;
		Vector3 operator+ (const Vector3& p) const;
		Vector3 operator/ (const float k) const;
		Vector3& operator= (const Vector3& p);
		Vector3& operator-= (const Vector3& p);
		Vector3& operator+= (const Vector3& p);
		Vector3& operator*= (float s);

		// index operator returns x if the index exceeds 2
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		/**
		 * @return a vector that is orthogonal to the original one.
		 */
		Vector3 Binormal() const;
		Vector3 Neg() const;
		Vector3 Normalize() const;
		Vector4 ToVec4() const;

		float NormSquare() const;
		float Norm() const;

		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
	};

	float Dot(const Vector3&, const Vector3&);
	Vector3 Cross(const Vector3&, const Vector3&);
	Vector3 TermWiseProduct(const Vector3&, const Vector3&);

	float Distance(const Vector3&, const Vector3&);

	bool operator== (const Vector3&, const Vector3&);
	bool operator!= (const Vector3&, const Vector3&);
	Vector3 operator* (const float, const Vector3&);
	Vector3 operator* (const Vector3&, const float);
	
}