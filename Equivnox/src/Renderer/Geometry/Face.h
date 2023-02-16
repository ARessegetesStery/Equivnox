#pragma once

#include "eqxpch.h"

#include "Math/Line.h"
#include "LineSeg.h"
#include "Vertex.h"

namespace EQX {

	// TODO Encapsulate

	/* l.x <= m.x <= r.x
	 * if any of the equality is satisfied, the inequality transfers to y direction
	 */
	class Face
	{
	private:
		Vertex l, m, r;
		float kLM, kLR, kMR;

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
		inline const float SlopeLM() const { return this->kLM; }
		inline const float SlopeLR() const { return this->kLR; }
		inline const float SlopeMR() const { return this->kMR; }

		Face& operator= (const Face& f);
		Vertex& operator[] (size_t index);
		const Vertex& operator[] (size_t index) const;

		/**
		 * output: v = ans[0] * l + ans[1] * m + ans[2] * r;
		 * Only considers xy
		 */
		Vector3 baryCoord(Vector2) const;
		Vector3 baryCoord(float, float) const;

		// Returns -Z_MAX if the point falls outside the triangle
		float ZAtXYFace(Vector2) const;
		float ZAtXYFace(float, float) const;

		// Returns z value on the spanned surface of the face
		float ZAtXYPlane(Vector2) const;
		float ZAtXYPlane(float, float) const;

		void Transform(const Mat4& projection);
		void DiscardZ();
	};

	/**
	 * If a point is on the edge or at the vertex, return true.
	 * Only works for 2D case: the z info of vertex and face is discarded
	 */
	bool IsPointInTriangle(Vertex, Face);

	void CompleteAttribInFace(EQX_OUT Vertex& v, const Face& f);

	/**
	 * @param p - the plane
	 * @param l - the line
	 * @param pos - [OUT]the point of intersection; does not change value if no intersection detected
	 * @return true if has intersection; false if parallel, i.e. no intersection
	 */
	bool FaceIntersectWithLine(const Face& f, const Line& l, EQX_OUT Vec3& pos);

	/**
	 * @param p - the plane
	 * @param l - the line
	 * @param pos - [OUT]the point of intersection; does not change value if no intersection detected
	 * @return true if has intersection; false if parallel, i.e. no intersection
	 */
	bool PlaneIntersectWithLine(const Plane& p, const Line& l, EQX_OUT Vector3& pos);

#ifdef EQX_DEBUG
	void Print(const Face&);
#endif
}
