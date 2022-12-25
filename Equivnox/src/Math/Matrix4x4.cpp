#include "eqxpch.h"

#include "Matrix4x4.h"

namespace EQX {

	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		Vector4(1, 0, 0, 0),
		Vector4(0, 1, 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(0, 0, 0, 1)
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

	Matrix4x4::Matrix4x4(Vector4 c1, Vector4 c2, Vector4 c3, Vector4 c4)
	{
		this->Mat[0][0] = c1.x;
		this->Mat[1][0] = c1.y;
		this->Mat[2][0] = c1.z;
		this->Mat[3][0] = c1.w;
		this->Mat[0][1] = c2.x;
		this->Mat[1][1] = c2.y;
		this->Mat[2][1] = c2.z;
		this->Mat[3][1] = c2.w;
		this->Mat[0][2] = c3.x;
		this->Mat[1][2] = c3.y;
		this->Mat[2][2] = c3.z;
		this->Mat[3][2] = c3.w;
		this->Mat[0][3] = c4.x;
		this->Mat[1][3] = c4.y;
		this->Mat[2][3] = c4.z;
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

	Matrix4x4 Matrix4x4::Concatenate(const Matrix4x4& m2)
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

	Matrix4x4 Matrix4x4::Add(const Matrix4x4& r)
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

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& r)
	{
		return this->Add(r);
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

	Matrix4x4 makeView(Vector4 CameraPos, Vector4 LookAt, Vector4 UpDir)
	{
		Vector3 look = LookAt.ToVec3();
		Vector3 up = UpDir.ToVec3();
		Vector3 binormal = Cross(look, up).Normalize();
		look = look.Neg();

		Matrix4x4 rotation(Vector4(binormal.x, up.x, look.x, 0), Vector4(binormal.y, up.y, look.y, 0), 
			Vector4(binormal.z, up.z, look.z, 0), Vector4(0, 0, 0, 1));
		Matrix4x4 trans(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), 
			Vector4(0, 0, 1, 0), Vector4(CameraPos).Neg());

		return rotation * trans;
	}

	Matrix4x4 makeView(Vector4 CameraPos, Vector4 LookAt)
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
		return makeView(CameraPos, LookAt, Updir);
	}

	
	Matrix4x4 makeOrtho(float NearWidth, float NearHeight,
		float NearClip, float FarClip)
	{
		return Matrix4x4(Vector4(2.0 / NearWidth, 0, 0, 0), 
			Vector4(0, 2.0 / NearHeight, 0, 0),
			Vector4(0, 0, 2 / FarClip - NearClip, 0), 
			Vector4(0, 0, -(FarClip + NearClip) / 2, 1));
	}

	Matrix4x4 makePersp(float NearWidth, float NearHeight,
		float NearClip, float FarClip)
	{
		Matrix4x4 Ortho = makeOrtho(NearWidth, NearHeight, NearClip, FarClip);
		Matrix4x4 Persp = Matrix4x4(Vector4(NearClip, 0, 0, 0), 
			Vector4(0, NearClip, 0, 0),
			Vector4(0, 0, NearClip + FarClip, 1), 
			Vector4(0, 0, -NearClip * FarClip, 0));
		return Ortho * Persp;
	}

	Matrix4x4 makeScreenSpace(float Width, float Height)
	{
		return Matrix4x4(Vector4(Width / 2.0f, 0, 0, 0), 
			Vector4(0, Height / 2.0f, 0, 0), 
			Vector4(0, 0, 1, 0), 
			Vector4(Width / 2.0f, Height / 2.0f, 0, 1));
	}

}
