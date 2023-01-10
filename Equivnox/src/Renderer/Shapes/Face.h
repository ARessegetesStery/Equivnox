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

		float ZatXY(Vector2) const;

		void ValidateSeq();
	};

}
