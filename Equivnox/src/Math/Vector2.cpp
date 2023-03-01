#include "Vector2.h"

#include "eqxpch.h"

namespace EQX
{
	const Vector2 Vector2::Zero(0, 0);

	Vector2::Vector2() : x(0), y(0) {  }

	Vector2::Vector2(const float x, const float y) : x(x), y(y) {  }

	Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {  }

	Vector2::Vector2(Vector2&& v) noexcept : x(v.x), y(v.y) {  }

	Vector2 Vector2::operator- (const Vector2& p) const
	{
		return Vector2(this->x - p.x, this->y - p.y);
	}

	Vector2 Vector2::operator+ (const Vector2& p) const
	{
		return Vector2(this->x + p.x, this->y + p.y);
	}

	Vector2& Vector2::operator= (const Vector2& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	Vector2& Vector2::operator-= (const Vector2& p)
	{
		*this = *this - p;
		return *this;
	}

	Vector2& Vector2::operator+= (const Vector2& p)
	{
		*this = *this + p;
		return *this;
	}

	Vector2& Vector2::operator*= (float s)
	{
		this->x *= s;
		this->y *= s;
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

	Vector2 Vector2::Neg() const
	{
		return Vector2(-x, -y);
	}

	float Vector2::Norm() const
	{
		return std::sqrt(this->x * this->x + this->y * this->y);
	}

	bool operator== (const Vector2& v1, const Vector2& v2)
	{
		return (v1.x == v2.x) && (v1.y == v2.y);
	}

	Vector2 operator*(const float c, const Vector2& v)
	{
		return Vector2(c * v.x, c * v.y);
	}

	Vector2 operator*(const Vector2& v, const float c)
	{
		return Vector2(c * v.x, c * v.y);
	}

}