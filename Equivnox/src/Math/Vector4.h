#pragma once

#include "eqxpch.h"

#include "Vector3.h"

namespace EQX {

	/**
	 * A 4-dimensional vector
	 * @warning w should never be zero
	 */
	class Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

	public:
		Vector4();
		Vector4(const float, const float, const float, const float w = 1.0);
		Vector4(const Vector3&);
		Vector4(const Vector4&);
		Vector4(Vector4&&) noexcept;

		Vector4 operator- (const Vector4& p) const;
		Vector4 operator+ (const Vector4& p) const;
		Vector4 operator+ (const Vector3& p) const;
		Vector4& operator= (const Vector4& p);
		Vector4& operator-= (const Vector4& p);
		Vector4& operator+= (const Vector4& p);
		Vector4& operator+= (const Vector3& p);
		Vector4& operator*= (float s);

		Vector3 ToVec3() const;

		// index operator returns x if the index exceeds 3
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		void Normalize();
		Vector4 Neg() const;

		static const Vector4 ZERO;
	};

	Vector4 Cross(const Vector4&, const Vector4&);

	bool operator== (const Vector4&, const Vector4&);

	Vector4 operator* (const Vector4&, const float);
	Vector4 operator* (const float, const Vector4&);

}