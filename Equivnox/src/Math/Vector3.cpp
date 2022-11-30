#include "eqxpch.h"

#include "Vector3.h"

namespace EQX
{
	const Vector3 Vector3::ZERO(0, 0, 0);
	const Vector3 Vector3::UNIT_X(1, 0, 0);
	const Vector3 Vector3::UNIT_Y(0, 1, 0);
	const Vector3 Vector3::UNIT_Z(0, 0, 1);

	Vector3::Vector3() : x(0), y(0), z(0) {  }

	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {  }

	Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {  }

	Vector3::Vector3(Vector3&& v) noexcept : x(v.x), y(v.y), z(v.z) {  }

	Vector3 Vector3::operator- (const Vector3& p)
	{
		return Vector3(this->x - p.x, this->y - p.y, this->z - p.z);
	}

	Vector3 Vector3::operator+ (const Vector3& p)
	{
		return Vector3(this->x + p.x, this->y + p.y, this->z + p.z);
	}

	Vector3 Vector3::operator* (const float coeff)
	{
		return Vector3(x * coeff, y * coeff, z * coeff);
	}

	Vector3& Vector3::operator= (const Vector3& p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		return *this;
	}

	Vector3 Vector3::operator-= (const Vector3& p)
	{
		*this = *this - p;
		return *this;
	}

	Vector3 Vector3::operator+= (const Vector3& p)
	{
		*this = *this + p;
		return *this;
	}

	Vector3 Vector3::operator*= (float s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		return *this;
	}

	float& Vector3::operator[] (std::size_t n)
	{
		switch (n)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return x;
		}
	}

	const float& Vector3::operator[] (std::size_t n) const
	{
		switch (n)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return x;
		}
	}

	bool operator== (const Vector3& v1, const Vector3& v2)
	{
		return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
	}

	Vector3 Vector3::Neg() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::Normalize() const
	{
		float norm = this->Norm();
		return Vector3(x / norm, y / norm, z / norm);
	}

	float Vector3::Norm() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y, 
			v1.z * v2.x - v1.x * v2.z, 
			v1.x * v2.y - v1.y * v2.x);
	}

}
