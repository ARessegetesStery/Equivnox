#include "eqxpch.h"

#include "Matrix4x4.h"

namespace EQX {

	const Matrix4x4 Matrix4x4::Identity = Matrix4x4(
		Vector4(1, 0, 0, 0),
		Vector4(0, 1, 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(0, 0, 0, 1)
	);

	const Matrix4x4 Matrix4x4::ZEROS = Matrix4x4(
		Vector4(0, 0, 0, 0),
		Vector4(0, 0, 0, 0),
		Vector4(0, 0, 0, 0),
		Vector4(0, 0, 0, 0)
	);

	Matrix4x4::Matrix4x4()
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->Mat[i][j] = 0;
			}
		}
	}

	Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		Mat[0][0] = m00;
		Mat[0][1] = m01;
		Mat[0][2] = m02;
		Mat[0][3] = m03;
		Mat[1][0] = m10;
		Mat[1][1] = m11;
		Mat[1][2] = m12;
		Mat[1][3] = m13;
		Mat[2][0] = m20;
		Mat[2][1] = m21;
		Mat[2][2] = m22;
		Mat[2][3] = m23;
		Mat[3][0] = m30;
		Mat[3][1] = m31;
		Mat[3][2] = m32;
		Mat[3][3] = m33;
	}

	Matrix4x4::Matrix4x4(Vector4 c1, Vector4 c2, Vector4 c3, Vector4 c4)
	{
		this->Mat[0][0] = c1.x;
		this->Mat[0][1] = c1.y;
		this->Mat[0][2] = c1.z;
		this->Mat[0][3] = c1.w;
		this->Mat[1][0] = c2.x;
		this->Mat[1][1] = c2.y;
		this->Mat[1][2] = c2.z;
		this->Mat[1][3] = c2.w;
		this->Mat[2][0] = c3.x;
		this->Mat[2][1] = c3.y;
		this->Mat[2][2] = c3.z;
		this->Mat[2][3] = c3.w;
		this->Mat[3][0] = c4.x;
		this->Mat[3][1] = c4.y;
		this->Mat[3][2] = c4.z;
		this->Mat[3][3] = c4.w;
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& r)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->Mat[i][j] = r.Mat[i][j];
			}
		}
	}

	Matrix4x4 Matrix4x4::Concatenate(const Matrix4x4& m2) const
	{
		Matrix4x4 r;
		r.Mat[0][0] = Mat[0][0] * m2.Mat[0][0] + Mat[0][1] * m2.Mat[1][0] + 
			Mat[0][2] * m2.Mat[2][0] + Mat[0][3] * m2.Mat[3][0];
		r.Mat[0][1] = Mat[0][0] * m2.Mat[0][1] + Mat[0][1] * m2.Mat[1][1] + 
			Mat[0][2] * m2.Mat[2][1] + Mat[0][3] * m2.Mat[3][1];
		r.Mat[0][2] = Mat[0][0] * m2.Mat[0][2] + Mat[0][1] * m2.Mat[1][2] + 
			Mat[0][2] * m2.Mat[2][2] + Mat[0][3] * m2.Mat[3][2];
		r.Mat[0][3] = Mat[0][0] * m2.Mat[0][3] + Mat[0][1] * m2.Mat[1][3] + 
			Mat[0][2] * m2.Mat[2][3] + Mat[0][3] * m2.Mat[3][3];

		r.Mat[1][0] = Mat[1][0] * m2.Mat[0][0] + Mat[1][1] * m2.Mat[1][0] + 
			Mat[1][2] * m2.Mat[2][0] + Mat[1][3] * m2.Mat[3][0];
		r.Mat[1][1] = Mat[1][0] * m2.Mat[0][1] + Mat[1][1] * m2.Mat[1][1] + 
			Mat[1][2] * m2.Mat[2][1] + Mat[1][3] * m2.Mat[3][1];
		r.Mat[1][2] = Mat[1][0] * m2.Mat[0][2] + Mat[1][1] * m2.Mat[1][2] + 
			Mat[1][2] * m2.Mat[2][2] + Mat[1][3] * m2.Mat[3][2];
		r.Mat[1][3] = Mat[1][0] * m2.Mat[0][3] + Mat[1][1] * m2.Mat[1][3] + 
			Mat[1][2] * m2.Mat[2][3] + Mat[1][3] * m2.Mat[3][3];

		r.Mat[2][0] = Mat[2][0] * m2.Mat[0][0] + Mat[2][1] * m2.Mat[1][0] + 
			Mat[2][2] * m2.Mat[2][0] + Mat[2][3] * m2.Mat[3][0];
		r.Mat[2][1] = Mat[2][0] * m2.Mat[0][1] + Mat[2][1] * m2.Mat[1][1] + 
			Mat[2][2] * m2.Mat[2][1] + Mat[2][3] * m2.Mat[3][1];
		r.Mat[2][2] = Mat[2][0] * m2.Mat[0][2] + Mat[2][1] * m2.Mat[1][2] + 
			Mat[2][2] * m2.Mat[2][2] + Mat[2][3] * m2.Mat[3][2];
		r.Mat[2][3] = Mat[2][0] * m2.Mat[0][3] + Mat[2][1] * m2.Mat[1][3] + 
			Mat[2][2] * m2.Mat[2][3] + Mat[2][3] * m2.Mat[3][3];

		r.Mat[3][0] = Mat[3][0] * m2.Mat[0][0] + Mat[3][1] * m2.Mat[1][0] + 
			Mat[3][2] * m2.Mat[2][0] + Mat[3][3] * m2.Mat[3][0];
		r.Mat[3][1] = Mat[3][0] * m2.Mat[0][1] + Mat[3][1] * m2.Mat[1][1] + 
			Mat[3][2] * m2.Mat[2][1] + Mat[3][3] * m2.Mat[3][1];
		r.Mat[3][2] = Mat[3][0] * m2.Mat[0][2] + Mat[3][1] * m2.Mat[1][2] + 
			Mat[3][2] * m2.Mat[2][2] + Mat[3][3] * m2.Mat[3][2];
		r.Mat[3][3] = Mat[3][0] * m2.Mat[0][3] + Mat[3][1] * m2.Mat[1][3] + 
			Mat[3][2] * m2.Mat[2][3] + Mat[3][3] * m2.Mat[3][3];

		return r;
	}

	Matrix4x4 Matrix4x4::Add(const Matrix4x4& r) const
	{
		Matrix4x4 a;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				a.Mat[i][j] = this->Mat[i][j] + r.Mat[i][j];
			}
		}
		return a;
	}

	Matrix4x4 Matrix4x4::Inverse() const
	{
		float m00 = Mat[0][0], m01 = Mat[0][1], m02 = Mat[0][2], m03 = Mat[0][3];
		float m10 = Mat[1][0], m11 = Mat[1][1], m12 = Mat[1][2], m13 = Mat[1][3];
		float m20 = Mat[2][0], m21 = Mat[2][1], m22 = Mat[2][2], m23 = Mat[2][3];
		float m30 = Mat[3][0], m31 = Mat[3][1], m32 = Mat[3][2], m33 = Mat[3][3];

		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;

		float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

		float det = (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		if (det == 0)
			return Mat4::ZEROS;

		float invDet = 1 / det;

		float d00 = t00 * invDet;
		float d10 = t10 * invDet;
		float d20 = t20 * invDet;
		float d30 = t30 * invDet;

		float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return Matrix4x4(d00, d01, d02, d03, d10, d11, d12, d13, d20, d21, d22, d23, d30, d31, d32, d33);
	}

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& r)
	{
		return this->Add(r);
	}

	Matrix4x4 Matrix4x4::operator=(const Matrix4x4& r)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				this->Mat[i][j] = r.Mat[i][j];
			}
		}
		return *this;
	}

	Vector4 Matrix4x4::operator*(const Vector4& v) const
	{
		Vector4 vec4;
		vec4.x = Mat[0][0] * v.x + Mat[0][1] * v.y +
			Mat[0][2] * v.z + Mat[0][3] * v.w;
		vec4.y = Mat[1][0] * v.x + Mat[1][1] * v.y +
			Mat[1][2] * v.z + Mat[1][3] * v.w;
		vec4.z = Mat[2][0] * v.x + Mat[2][1] * v.y +
			Mat[2][2] * v.z + Mat[2][3] * v.w;
		vec4.w = Mat[3][0] * v.x + Mat[3][1] * v.y +
			Mat[3][2] * v.z + Mat[3][3] * v.w;

		return vec4;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& r)
	{
		return this->Concatenate(r);
	}

	float& Matrix4x4::operator[](size_t index)
	{
		if (index > 15)
			index = 0;
		return this->Mat[index / 4][index % 4];
	}

	const float Matrix4x4::operator[](size_t index) const
	{
		if (index > 15)
			index = 0;
		return this->Mat[index / 4][index % 4];
	}

	Matrix4x4 MakeView(Vector4 CameraPos, Vector4 LookAt, Vector4 UpDir)
	{
		Vector3 look = LookAt.ToVec3();
		Vector3 up = UpDir.ToVec3();
		Vector3 binormal = Cross(look, up).Normalize();
		look.Normalize();
		up.Normalize();
		look = look.Neg();

		Matrix4x4 rotation(
			Vector4(binormal.x, binormal.y, binormal.z, 0),
			Vector4(up.x	  , up.y	  , up.z	  , 0), 
			Vector4(look.x	  , look.y    , look.z    , 0), 
			Vector4(0		  , 0		  , 0		  , 1));
		Matrix4x4 trans(
			Vector4(1, 0, 0, -CameraPos.x), 
			Vector4(0, 1, 0, -CameraPos.y),
			Vector4(0, 0, 1, -CameraPos.z),
			Vector4(0, 0, 0, 1));

		return rotation * trans;
	}

	Matrix4x4 MakeView(Vector4 CameraPos, Vector4 LookAt)
	{
		Vector3 Updir3;
		Vector3 look = LookAt.ToVec3();
		if (look.x == 0 && look.z == 0)
		{
			Updir3 = Vector3(look.y, 0, 0);
		}
		else
		{
			float xzLength = (look - Vector3(0, look.y, 0)).Norm();
			Updir3 = (look - Vector3(0, look.y, 0)) * (-look.y / xzLength) +
				Vector3(0, xzLength, 0);
		}
		Updir3 = Updir3.Normalize();
		Vector4 Updir(Updir3);
		return MakeView(CameraPos, LookAt, Updir);
	}

	Matrix4x4 makeOrtho(float NearWidth, float NearHeight,
		float NearClip, float FarClip)
	{
		Matrix4x4 scale = Matrix4x4(
			Vector4(2.0 / NearWidth, 0               , 0                         , 0),
			Vector4(0              , 2.0 / NearHeight, 0                         , 0),
			Vector4(0              , 0               , 2.0 / (NearClip - FarClip), 0),
			Vector4(0              , 0               , 0                         , 1));
		Matrix4x4 trans = Matrix4x4(
			Vector4(1, 0, 0, 0),
			Vector4(0, 1, 0, 0),
			Vector4(0, 0, 1, -0.5f * (NearClip + FarClip)),
			Vector4(0, 0, 0, 1));
		return scale * trans;
	}

	Matrix4x4 MakePersp(float NearWidth, float NearHeight,
		float NearClip, float FarClip)
	{
		Matrix4x4 Ortho = makeOrtho(NearWidth, NearHeight, NearClip, FarClip);
		Matrix4x4 Persp = Matrix4x4(
			Vector4(NearClip, 0       , 0                   , 0), 
			Vector4(0       , NearClip, 0                   , 0),
			Vector4(0       , 0       , (NearClip + FarClip), -NearClip * FarClip),
			Vector4(0       , 0       , 1                   , 0));
		return Ortho * Persp;
	}

	Matrix4x4 MakeScreenSpace(float Width, float Height)
	{
		return Matrix4x4(
			Vector4(Width / 2.0f, 0            , 0, Width / 2.0f),
			Vector4(0           , Height / 2.0f, 0, Height / 2.0f),
			Vector4(0           , 0            , 1, 0), 
			Vector4(0           , 0            , 0, 1));
	}

}
