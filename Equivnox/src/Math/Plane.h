#pragma once

#include "eqxpch.h"

#include "Line.h"

namespace EQX {

	/**
	 * The normal needs to be of unit length
	 * Default set to be the XY plane
	 * Note that the normal may be parallel but opposite
	 */
	class Plane
	{
	private:
		Vector3 point;
		Vector3 normal;
		
	public:
		Plane();
		Plane(Vector3 point, Vector3 normal);
		Plane(Vector3 point1, Vector3 point2, Vector3 point3);
		Plane(Vector4 coeffs);
		Plane(const Plane&);

		inline Vector3 GetNormal() const { return normal; }
		inline Vector3 GetPoint() const { return point; }
		inline void SetPoint(Vector3 p) { point = p; }
		inline void SetNormal(Vector3 n) { normal = n.Normalize(); }

		Vector4 ToFormula() const;

		Vector3 TanVec() const;

		bool operator== (const Plane&) const;
	};

	bool IsPointOnPlane(const Vector3&, const Plane&);

	/**
	 * @param p - the plane
	 * @param l - the line
	 * @param pos - [OUT]the point of intersection; does not change value if no intersection detected
	 * @return true if has intersection; false if parallel, i.e. no intersection
	 */
	bool PlaneIntersectWithLine(const Plane& p, const Line& l,Vector3& pos);
}

