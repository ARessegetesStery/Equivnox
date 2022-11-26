#include "Face.h"

namespace EQX {
	

	Face::Face() 
		: l(Vector2(0, 0)), m(Vector2(0, 0)), r(Vector2(0, 0)), 
			kLM(0), kLR(0), kMR(0) {   }

	Face::Face(LineSeg& l1, LineSeg& l2, LineSeg& l3) : 
		Face(l1.start, l1.end, (l3.start.pos == l1.start.pos || l3.start.pos == l1.end.pos) ?
		l3.end : l3.start)
	{	}

	Face::Face(std::array<Vertex, 3>& vertices)
	{
		std::sort(vertices.begin(), vertices.end(), LefterVertex);
		this->l = vertices[0];
		this->m = vertices[1];
		this->r = vertices[2];
		this->kLM = GetSlope(l, m);
		this->kLR = GetSlope(l, r);
		this->kMR = GetSlope(m, r);
		if (std::abs(kLM) > SLOPE_MAX)
		{
			l = std::min(l, m, LowerVertex);
			m = std::max(l, m, LowerVertex);
		}
		if (std::abs(kMR) > SLOPE_MAX)
		{
			m = std::min(r, m, LowerVertex);
			r = std::max(r, m, LowerVertex);
		}
	}

	Face::Face(Vertex v1, Vertex v2, Vertex v3) 
		: Face(std::array<Vertex, 3>{v1, v2, v3})  {	}

	Face::Face(const Face& f) : 
		l(f.l), m(f.m), r(f.r), kLM(f.kLM), kLR(f.kLR), kMR(f.kMR)  {	}

}
