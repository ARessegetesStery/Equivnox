#pragma once

#include <CoreMinimal.h>

#include "LineSeg.h"

namespace EQX {

	/* l.x <= m.x <= r.x
	 * if any of the equality is satisfied, the inequality transfers to y direction
	 */
	class Face
	{
	private:
		Vertex l, m, r;
		XFloat kLM, kLR, kMR;

	public:
		Face();
		Face(std::array<Vertex, 3>& vertices);
		Face(Vertex, Vertex, Vertex);
		Face(LineSeg&, LineSeg&, LineSeg&);		// make sure the line segments connects each other
		Face(const Face&);
		Face(Face&&) = default;

		void ValidateSeq();

		inline const Vertex& L() const { return this->l; }
		inline const Vertex& M() const { return this->m; }
		inline const Vertex& R() const { return this->r; }
		inline const XFloat SlopeLM() const { return this->kLM; }
		inline const XFloat SlopeLR() const { return this->kLR; }
		inline const XFloat SlopeMR() const { return this->kMR; }

		Face& operator= (const Face& f);
		const Vertex& operator[] (size_t index) const;

		/**
		 * output: v = ans[0] * l + ans[1] * m + ans[2] * r;
		 * Only considers xy
		 */
		Vector3 baryCoord(Vector2) const;
		Vector3 baryCoord(XFloat, XFloat) const;

		// Returns -Z_MAX if the point falls outside the triangle
		XFloat ZAtXYFace(Vector2) const;
		XFloat ZAtXYFace(XFloat, XFloat) const;

		// Returns z value on the spanned surface of the face
		XFloat ZAtXYPlane(Vector2) const;
		XFloat ZAtXYPlane(XFloat, XFloat) const;

		void MatTransform(const Mat4& projection);
		void MeshTransform(const MeshTransform& trans);
		void DiscardZ();
	};

	/**
	 * If a point is on the edge or at the vertex, return true.
	 * Only works for 2D case: the z info of vertex and face is discarded
	 */
	XBool IsPointInTriangle(Vertex, Face);

	void CompleteAttribInFace(EQX_OUT Vertex& v, const Face& f);

	/**
	 * @param p - the plane
	 * @param l - the line
	 * @param pos - [OUT]the point of intersection; does not change value if no intersection detected
	 * @return true if has intersection; false if parallel, i.e. no intersection
	 */
	XBool FaceIntersectWithLine(const Face& f, const Line& l, EQX_OUT Vec3& pos);

	/**
	 * @param p - the plane
	 * @param l - the line
	 * @param pos - [OUT]the point of intersection; does not change value if no intersection detected
	 * @return true if has intersection; false if parallel, i.e. no intersection
	 */
	XBool PlaneIntersectWithLine(const Plane& p, const Line& l, EQX_OUT Vector3& pos);
}
