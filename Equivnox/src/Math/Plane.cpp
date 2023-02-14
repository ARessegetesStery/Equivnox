#include "eqxpch.h"

#include "Plane.h"

namespace EQX {

	Plane::Plane()
	{
		point = Vector3::ZERO;
		normal = Vector3::UNIT_Z;
	}

	Plane::Plane(Vector3 point, Vector3 normal)
	{
		this->point = point;
		if (normal == Vector3::ZERO)
			normal = Vector3::UNIT_Z;
		this->normal = normal.Normalize();
	}

	Plane::Plane(Vector3 point1, Vector3 point2, Vector3 point3)
	{
		this->point = point1;
		this->normal = Cross(point2 - point1, point3 - point1).Normalize();
	}

	Plane::Plane(Vector4 coeffs)
	{
		if (coeffs[0] == 0 && coeffs[1] == 0 && coeffs[2] == 0)
		{
			point = Vector3::ZERO;
			normal = Vector3::UNIT_Z;
		}
		else
		{
			normal = Vector3(coeffs[0], coeffs[1], coeffs[2]).Normalize();
			if (coeffs[0] != 0)
				point = Vector3(-coeffs[3] / coeffs[0], 0, 0);
			else if (coeffs[1] != 0)
				point = Vector3(0, -coeffs[1] / coeffs[3], 0);
			else
				point = Vector3(0, 0, -coeffs[2] / coeffs[3]);
		}
	}

	Plane::Plane(const Plane& p)
	{
		this->point = p.GetPoint();
		this->normal = p.GetNormal();
	}

	Vector4 Plane::ToFormula() const
	{
		float h = -Dot(point, normal);
		return Vector4(normal.x, normal.y, normal.z, h);
	}

	Vector3 Plane::TanVec() const
	{
		if (normal.z == 0)
			return Vector3(normal.y, -normal.x, 0);
		else
			return Vector3(normal.y, normal.z, -(normal.x * normal.y + normal.y * normal.z) / normal.z);
	}

	bool Plane::operator==(const Plane& alpha) const
	{
		return Dot(this->normal, alpha.GetNormal()) == 1 && IsPointOnPlane(this->point, alpha);
	}

	bool IsPointOnPlane(const Vector3& p, const Plane& alpha)
	{
		return Dot(alpha.GetNormal(), p) == 0;
	}
}
