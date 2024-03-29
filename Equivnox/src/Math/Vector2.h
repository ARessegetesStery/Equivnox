#pragma once

#include "Math.h"

namespace EQX {

	class Vector2
	{
	public:
		float x;
		float y;
	public:
		Vector2();
		Vector2(const float, const float);
		Vector2(const Vector2&);
		Vector2(Vector2&&) noexcept;

		Vector2 operator- (const Vector2& p) const;
		Vector2 operator+ (const Vector2& p) const;
		Vector2& operator= (const Vector2& p);
		Vector2& operator-= (const Vector2& p);
		Vector2& operator+= (const Vector2& p);
		Vector2& operator*= (float s);

		// index operator returns x if the index exceeds 1
		float& operator[] (size_t n);
		const float& operator[] (size_t n) const;

		Vector2 Neg() const;
		float Norm() const;

		static const Vector2 Zero;
	};

	Vector2 operator* (const float, const Vector2&);
	Vector2 operator* (const Vector2&, const float);

	bool operator== (const Vector2&, const Vector2&);
	
}