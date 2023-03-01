#pragma once

#include "eqxpch.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace EQX {

	/**
	 * Indexing:
	 * [0][0] [0][1] [0][2] [0][3]
	 * [1][0] [1][1] [1][2] [1][3]
	 * [2][0] [2][1] [2][2] [2][3]
	 * [3][0] [3][1] [3][2] [3][3]
	 */
	class Matrix4x4
	{
	public:
		float Mat[4][4];

	public:
		Matrix4x4();
		Matrix4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		Matrix4x4(Vector4 c1, Vector4 c2, Vector4 c3, Vector4 c4);		// Init by rows
		Matrix4x4(const Matrix4x4&);
		Matrix4x4(Matrix4x4&&) = default;

		// concatenate/add gives post-operations
		Matrix4x4 Concatenate(const Matrix4x4& r) const;
		Matrix4x4 Add(const Matrix4x4& r) const;

		// If the matrix is singular, return the zero matrix
		Matrix4x4 Inverse() const;

		Matrix4x4 operator+ (const Matrix4x4&);
		Matrix4x4 operator= (const Matrix4x4&);
		Vector4 operator* (const Vector4&) const;
		Matrix4x4 operator* (const Matrix4x4&);

		float& operator[] (size_t index);
		const float operator[] (size_t index) const;

		static const Matrix4x4 Identity;
		static const Matrix4x4 Zeros;
	};

	/**
	 * Compute the View Matrix
	 * Position: M_ortho * M_persp * [M_view]
	 *
	 * @param CameraPos Position of camera
	 * @param LookAt The direction the camera aims at
	 * @param UpDir The upward orthogonal direction of camera
	 * @return 4*4 View Matrix
	 */
	Matrix4x4 MakeView(Vector4 CameraPos, Vector4 LookAt, Vector4 UpDir);
	Matrix4x4 MakeView(Vector4 CameraPos, Vector4 LookAt);

	/**
	 * Creates a matrix for orthogonal projection
	 * Position: [M_ortho] * M_persp * M_view
	 *
	 * @param ViewportWidth Width of target viewport
	 * @param ViewportHeight Height of target viewport
	 * @param NearClip Near clipping distance
	 * @param FarClip Far clipping distance
	 * @return Orthogonal Projection Matrix
	 */
	Matrix4x4 makeOrtho(float ViewportWidth, float ViewportHeight,
		float NearClip = -0.1f, float FarClip = -100.f);

	/**
	 * Creates a matrix for perspective projection
	 * Position: [M_ortho * M_persp] * M_view
	 *
	 * @param ViewportWidth Width of target viewport
	 * @param ViewportHeight Height of target viewport
	 * @param NearClip Near clipping distance
	 * @param FarClip Far clipping distance
	 * @return Perspective Projection Matrix
	 */
	Matrix4x4 MakePersp(float ViewportWidth, float ViewportHeight,
		float NearClip = -0.1f, float FarClip = -100.f);

	Matrix4x4 MakeScreenSpace(float Width, float Height);

}