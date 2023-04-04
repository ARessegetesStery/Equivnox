#include "eqxpch.h"

#include "Quaternion.h"

namespace EQX {

	Quaternion::Quaternion()
	{
		this->re = 0;
		this->im = Vector3::Zero;
	}

	Quaternion::Quaternion(const Vector3& v)
	{
		this->re = 0;
		this->im = v;
	}

	Quaternion::Quaternion(const Vector4& v)
	{
		this->re = v.x;
		this->im = Vector3(v.y, v.z, v.w);
	}

	Quaternion::Quaternion(float re, const Vector3& im)
	{
		this->re = re;
		this->im = im;
	}

	Quaternion::Quaternion(const Quaternion& q)
	{
		this->re = q.re;
		this->im = q.im;
	}

	Quaternion Quaternion::Conjugate() const
	{
		return Quaternion(this->re, this->im.Neg());
	}

	float Quaternion::Norm() const
	{
		return sqrt(re * re + this->im.Norm() * this->im.Norm());
	}

	Quaternion Quaternion::Inverse() const
	{
		float normSquared = this->Norm() * this->Norm();
		return this->Conjugate() / normSquared;
	}

	Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion(q1.re + q2.re, q1.im + q2.im);
	}

	Quaternion operator*(const Quaternion& q, float coeff)
	{
		return Quaternion(coeff * q.re, coeff * q.im);
	}

	Quaternion operator*(float coeff, const Quaternion& q)
	{
		return q * coeff;
	}

	Quaternion operator/(const Quaternion& q, float coeff)
	{
		return q * (1.f / coeff);
	}

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		return Quaternion(q1.re * q2.re - Dot(q1.im, q2.im), 
			q1.re * q2.im + q2.re * q1.im + Cross(q1.im, q2.im));
	}

	Vector3 RotateQuat(const Vector3& pos, const Vector3& axis, float angle)
	{
		Vector3 rotAxis = axis.Normalize();
		Quaternion q = Quaternion(cosf(angle / 2), sin(angle / 2) * rotAxis);
		return (q * Quaternion(0, pos) * q.Conjugate()).im;
	}

}