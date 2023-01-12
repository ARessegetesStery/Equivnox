#pragma once

#include "eqxpch.h"

#include "LineSeg.h"
#include "Vertex.h"

namespace EQX {

	class Face
	{
	public:
		Vertex l, m, r;
		float kLM, kLR, kMR;

	public:
		Face();
		Face(std::array<Vertex, 3>& vertices);
		Face(Vertex, Vertex, Vertex);
		Face(LineSeg&, LineSeg&, LineSeg&);		// make sure the line segments connects each other
		Face(const Face&);
		Face(Face&&) = default;

		/**
		 * output: v = ans[0] * l + ans[1] * m + ans[2] * r;
		 * Only considers xy
		 */
		Vector3 baryCoord(Vector2) const;
		Vector3 baryCoord(float, float) const;

		// Returns -Z_MAX if the point falls outside the triangle
		float ZatXYFace(Vector2) const;
		float ZatXYFace(float, float) const;

		// Returns z value on the spanned surface of the face
		float ZatXYPlane(Vector2) const;
		float ZatXYPlane(float, float) const;

		void ValidateSeq();
	};

	/**
	 * If a point is on the edge or at the vertex, return true.
	 * Only works for 2D case: the z info of vertex and face is discarded
	 */
	bool IsPointInTriangle(Vertex, Face);

}
