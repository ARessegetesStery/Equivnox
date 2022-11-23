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
		float x;
		float y;
		float z;
		float w;

	public:
		Vector4();
		Vector4(float, float, float, float w = 1.0);
		Vector4(const Vector4&);
		Vector4(Vector4&&) noexcept;

		Vector4 operator- (const Vector4& p);
		Vector4 operator+ (const Vector4& p);
		Vector4& operator= (const Vector4& p);
		Vector4 operator-= (const Vector4& p);
		Vector4 operator+= (const Vector4& p);
		Vector4 operator*= (float s);

		// index operator returns x if the index exceeds 3
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		void normalize();

		static const Vector4 ZERO;
	};

	bool operator== (const Vector4&, const Vector4&);

}