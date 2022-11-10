#include "Vector2.h"

#include "eqxpch.h"

namespace EQX
{
	const Vector2 Vector2::ZERO(0, 0);

	Vector2::Vector2() : x(0), y(0) {  }

	Vector2::Vector2(double x, double y) : x(x), y(y) {  }

	Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {  }

	Vector2::Vector2(Vector2&& v) noexcept : x(v.x), y(v.y) {  }

	Vector2 Vector2::operator- (const Vector2& p)
	{
		return Vector2(this->x - p.x, this->y - p.y);
	}

	Vector2 Vector2::operator+ (const Vector2& p)
	{
		return Vector2(this->x + p.x, this->y + p.y);
	}

	Vector2& Vector2::operator= (const Vector2& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	
}