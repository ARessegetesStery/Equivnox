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
		// TODO simplify
		this->l = vertices[0];
		this->m = vertices[1];
		this->r = vertices[2];
		this->ValidateSeq();
	}

	Face::Face(Vertex v1, Vertex v2, Vertex v3) 
		: Face(std::array<Vertex, 3>{v1, v2, v3})  {	}

	Face::Face(const Face& f) : 
		l(f.l), m(f.m), r(f.r), kLM(f.kLM), kLR(f.kLR), kMR(f.kMR)  {	}

	Vector3 Face::baryCoord(Vector2 xy) const
	{
		float D = (l.pos.x - r.pos.x) * (m.pos.y - r.pos.y) - 
			(m.pos.x - r.pos.x) * (l.pos.y - r.pos.y);
		float DAlpha = (xy.x - r.pos.x) * (m.pos.y - r.pos.y) -
			(m.pos.x - r.pos.x) * (xy.y - r.pos.y);
		float DBeta = (l.pos.x - r.pos.x) * (xy.y - r.pos.y) -
			(xy.x - r.pos.x) * (l.pos.y - r.pos.y);
		float alpha = DAlpha / D;
		float beta = DBeta / D;
		return Vec3(alpha, beta, 1 - alpha - beta);
	}

	Vector3 Face::baryCoord(float x, float y) const
	{
		return baryCoord(Vec2(x, y));
	}

	float Face::ZatXY(Vector2 xy) const
	{
		Vec3 baryCoord = this->baryCoord(xy);
		return baryCoord[0] * l.pos.z + baryCoord[1] * m.pos.z + baryCoord[2] * r.pos.z;
	}

	void Face::ValidateSeq()
	{
		std::array<Vertex, 3> vertices = { l, m, r };
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
}
