#pragma once

#include "eqxpch.h"

#include "Vector3.h"
#include "Vector4.h"

namespace EQX {

	class Quaternion
	{
	public:
		float re;		// the real part
		Vector3 im;		// the imaginary part

	public:
		Quaternion();
		explicit Quaternion(const Vector3&);
		explicit Quaternion(const Vector4&);
		explicit Quaternion(float, const Vector3&);
		explicit Quaternion(const Quaternion&);
		Quaternion(Quaternion&&) = default;

		Quaternion Conjugate() const;
		float Norm() const;
		Quaternion Inverse() const;
	};

	Quaternion operator+ (const Quaternion& q1, const Quaternion& q2);
	Quaternion operator* (const Quaternion&, float coeff);
	Quaternion operator* (float coeff, const Quaternion&);
	Quaternion operator/ (const Quaternion&, float coeff);
	Quaternion operator* (const Quaternion&, const Quaternion&);

	Vector3 RotateQuat(const Vector3& pos, const Vector3& axis, float angle);

}