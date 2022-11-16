#include "eqxpch.h"

#include "Vector4.h"

namespace EQX
{
	const Vector4 Vector4::ZERO(0, 0, 0, 1);

	Vector4::Vector4() : x(0), y(0), z(0), w(1) {  }

	Vector4::Vector4(float x, float y, float z, float w) 
		: x(x), y(y), z(z), w(w) {  }

	Vector4::Vector4(const Vector4& v)
		: x(v.x), y(v.y), z(v.z), w(v.w) {	}

	Vector4::Vector4(Vector4&& v) noexcept
		: x(v.x), y(v.y), z(v.z), w(v.w) {	}

	Vector4 Vector4::operator- (const Vector4& p)
	{
		return Vector4(this->x - p.x, this->y - p.y, this->z - p.z, this-> w - p.w);
	}

	Vector4 Vector4::operator+ (const Vector4& p)
	{
		return Vector4(this->x + p.x, this->y + p.y, this->z + p.z, this-> w + p.w);
	}

	Vector4& Vector4::operator= (const Vector4& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		w = p.w;
		return *this;
	}

	void Vector4::normalize()
	{
		x /= w;
		y /= w;
		z /= w; 
		w = 1;
	}

	float& Vector4::operator[] (std::size_t n)
	{
		switch (n)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return x;
		}
	}

	const float& Vector4::operator[] (std::size_t n) const
	{
		switch (n)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return x;
		}
	}
}
