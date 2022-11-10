#include "eqxpch.h"

#include "Vector3.h"

namespace EQX
{
	const Vector3 Vector3::ZERO(0, 0, 0);
	const Vector3 Vector3::UNIT_X(1, 0, 0);
	const Vector3 Vector3::UNIT_Y(0, 1, 0);
	const Vector3 Vector3::UNIT_Z(0, 0, 1);

	Vector3::Vector3() : x(0), y(0), z(0) {  }

	Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {  }

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

	Vector3& Vector3::operator= (const Vector3& p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		return *this;
	}
}
