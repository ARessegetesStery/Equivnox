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

		Vector2 operator- (const Vector2& p);
		Vector2 operator+ (const Vector2& p);
		Vector2& operator= (const Vector2& p);

		// index operator returns x if the index exceeds 1
		float& operator[] (std::size_t n);
		const float& operator[] (std::size_t n) const;

		static const Vector2 ZERO;
	};

	

	

	
}