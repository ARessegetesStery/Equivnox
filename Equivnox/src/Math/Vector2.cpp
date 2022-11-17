#include "Vector2.h"

#include "eqxpch.h"

namespace EQX
{
	const Vector2 Vector2::ZERO(0, 0);

	Vector2::Vector2() : x(0), y(0) {  }

	Vector2::Vector2(float x, float y) : x(x), y(y) {  }

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

	float& Vector2::operator[] (std::size_t n)
	{
		switch (n)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			return x;
		}
	}

	const float& Vector2::operator[] (std::size_t n) const
	{
		switch (n)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			return x;
		}
	}

	bool operator== (const Vector2& v1, const Vector2& v2)
	{
		return (v1.x == v2.x) && (v1.y == v2.y);
	}

}