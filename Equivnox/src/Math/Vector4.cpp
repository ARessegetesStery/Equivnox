#include "eqxpch.h"

#include "Vector3.h"
#include "Vector4.h"

namespace EQX
{
	const Vector4 Vector4::Zero(0, 0, 0, 1);

	Vector4::Vector4() : x(0), y(0), z(0), w(1) {  }

	Vector4::Vector4(const float x, const float y, const float z, const float w) 
		: x(x), y(y), z(z), w(w) {  }

	Vector4::Vector4(const Vector3& v3)
		: x(v3.x), y(v3.y), z(v3.z), w(1) {  }

	Vector4::Vector4(const Vector4& v)
		: x(v.x), y(v.y), z(v.z), w(v.w) {	}

	Vector4::Vector4(Vector4&& v) noexcept
		: x(v.x), y(v.y), z(v.z), w(v.w) {	}

	Vector4 Vector4::operator- (const Vector4& p) const
	{
		return Vector4(this->x - p.x, this->y - p.y, this->z - p.z, this-> w - p.w);
	}

	Vector4 Vector4::operator+ (const Vector4& p) const
	{
		return Vector4(this->x + p.x, this->y + p.y, this->z + p.z, this-> w + p.w);
	}

	Vector4 Vector4::operator+ (const Vector3& p) const 
	{
		return Vector4(this->x + p.x, this->y + p.y, this->z + p.z, this->w);
	}


	Vector4& Vector4::operator= (const Vector4& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		w = p.w;
		return *this;
	}

	Vector4& Vector4::operator-= (const Vector4& p)
	{
		*this = *this - p;
		return *this;
	}

	Vector4& Vector4::operator+= (const Vector4& p)
	{
		*this = *this + p;
		return *this;
	}

	Vector4& Vector4::operator+= (const Vector3& p)
	{
		*this = *this + p;
		return *this;
	}

	/**
	 * Multiply the distance between the point and the origin
	 * w is not multiplied here or nothing is changed
	 * 	  
	 * @param s scale of multiplication
	 * @return 
	 */
	Vector4& Vector4::operator*= (float s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		return *this;
	}

	Vector3 Vector4::ToVec3() const
	{
		if (w == 0)
			return Vector3(x, y, z);
		else
			return Vector3(x / w, y / w, z / w);
	}

	void Vector4::Normalize()
	{
		if (w == 0)
			return;

		x /= w;
		y /= w;
		z /= w; 
		w = 1;
	}

	Vector4 Vector4::Neg() const
	{
		return Vector4(-x, -y, -z, w);
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

	bool operator== (const Vector4& v1, const Vector4& v2)
	{
		return (v1.x/v1.w == v2.x/v2.w) 
			&& (v1.y/v1.w == v2.y/v2.w) 
			&& (v1.z/v1.w == v2.z/v2.w);
	}

	Vector4 operator*(const Vector4& v4, const float coeff)
	{
		return Vector4(v4.x * coeff, v4.y * coeff, v4.z * coeff, v4.w);
	}

	Vector4 operator*(const float coeff, const Vector4& v4)
	{
		return v4 * coeff;
	}

	Vector4 Cross(const Vector4& v1, const Vector4& v2)
	{
		Vector3 t3(v1.x, v1.y, v1.z);
		Vector3 v3(v2.x, v2.y, v2.z);
		Vector3 ans3 = Cross(t3, v3);
		return Vector4(ans3);
	}

}
