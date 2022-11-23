#pragma once

#include "eqxpch.h"

#include "LineSeg.h"
#include "Vertex.h"
#include "vendor/tgaimage.h"

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

		void Render(TGAImage&);
	};

}
