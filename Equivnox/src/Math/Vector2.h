#pragma once

#include "eqxpch.h"

namespace EQX {

	class Vector2
	{
	public:
		float x;
		float y;
	public:
		Vector2();
		Vector2(float, float);
		Vector2(const Vector2&);
		Vector2(Vector2&&) noexcept;

		Vector2 operator- (const Vector2& p) const;
		Vector2 operator+ (const Vector2& p) const;
		Vector2& operator= (const Vector2& p);
		Vector2& operator-= (const Vector2& p);
		Vector2& operator+= (const Vector2& p);
		Vector2& operator*= (float s);

		// index operator returns x if the index exceeds 1
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		Vector2 Neg() const;

		static const Vector2 ZERO;
	};

	float Dot(const Vector2&, const Vector2&);

	bool operator== (const Vector2&, const Vector2&);
	
}