#pragma once

#include "eqxpch.h"

namespace EQX {

	class Vector2
	{
	public:
		double x;
		double y;
	public:
		Vector2();
		Vector2(double, double);
		Vector2(const Vector2&);
		Vector2(Vector2&&) noexcept;

		Vector2 operator- (const Vector2& p);
		Vector2 operator+ (const Vector2& p);
		Vector2& operator= (const Vector2& p);

		static const Vector2 ZERO;
	};

	

	

	
}