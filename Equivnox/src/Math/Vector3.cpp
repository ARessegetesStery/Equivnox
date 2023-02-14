#include "eqxpch.h"

#include "Vector3.h"

namespace EQX
{
	const Vector3 Vector3::ZERO(0, 0, 0);
	const Vector3 Vector3::UNIT_X(1, 0, 0);
	const Vector3 Vector3::UNIT_Y(0, 1, 0);
	const Vector3 Vector3::UNIT_Z(0, 0, 1);

	Vector3::Vector3() : x(0), y(0), z(0) {  }

	Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {  }

	Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {  }

	Vector3::Vector3(const Vector4& v)
	{
		float w = (v.w == 0) ? 1 : v.w;
		this->x = v.x / w;
		this->y = v.y / w;
		this->z = v.z / w;
	}

	Vector3::Vector3(Vector3&& v) noexcept : x(v.x), y(v.y), z(v.z) {  }

	Vector3 Vector3::operator- (const Vector3& p) const
	{
		return Vector3(this->x - p.x, this->y - p.y, this->z - p.z);
	}

	Vector3 Vector3::operator+ (const Vector3& p) const
	{
		return Vector3(this->x + p.x, this->y + p.y, this->z + p.z);
	}

	Vector3 Vector3::operator/ (const float k) const
	{
		return Vector3(this->x / k, this->y / k, this->z / k);
	}

	Vector3& Vector3::operator= (const Vector3& p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		return *this;
	}

	Vector3& Vector3::operator-= (const Vector3& p)
	{
		*this = *this - p;
		return *this;
	}

	Vector3& Vector3::operator+= (const Vector3& p)
	{
		*this = *this + p;
		return *this;
	}

	Vector3& Vector3::operator*= (float s)
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

	Vector4 Vector3::ToVec4() const
	{
		return Vector4(this->x, this->y, this->z, 1.0f);
	}

	float Vector3::NormSquare() const
	{
		return x * x + y * y + z * z;
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

	float Distance(const Vector3& v1, const Vector3& v2)
	{
		return (v1 - v2).Norm();
	}

	Vector3 operator* (const float k, const Vector3& v)
	{
		return Vec3(k * v.x, k * v.y, k * v.z);
	}

	Vector3 operator* (const Vector3& v, const float k)
	{
		return Vec3(k * v.x, k * v.y, k * v.z);
	}

#ifdef EQX_DEBUG
	void Print(const Vector3& vec)
	{
		cout << vec.x << " " << vec.y << " " << vec.z << " " << endl;
	}
#endif
}
