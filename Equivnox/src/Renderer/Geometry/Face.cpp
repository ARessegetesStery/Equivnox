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
		this->l = vertices[0];
		this->m = vertices[1];
		this->r = vertices[2];
		this->ValidateSeq();
	}

	Face::Face(Vertex v1, Vertex v2, Vertex v3) 
		: Face(std::array<Vertex, 3>{v1, v2, v3})  {	}

	Face::Face(const Face& f) : 
		l(f.l), m(f.m), r(f.r), kLM(f.kLM), kLR(f.kLR), kMR(f.kMR)  {	}

	Face& Face::operator=(const Face& f)
	{
		this->l = f.l;
		this->m = f.m;
		this->r = f.r;
		this->ValidateSeq();
		return *this;
	}

	Vertex& Face::operator[](size_t index)
	{
		if (index == 0)
			return this->l;
		else if (index == 1)
			return this->m;
		else if (index == 2)
			return this->r;
		else
			return this->l;
	}

	const Vertex& Face::operator[](size_t index) const
	{
		if (index == 0)
			return this->l;
		else if (index == 1)
			return this->m;
		else if (index == 2)
			return this->r;
		else
			return this->l;
	}

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

	float Face::ZAtXYFace(Vector2 xy) const
	{
		Vec3 baryCoord = this->baryCoord(xy);
		 if (baryCoord[0] < 0 || baryCoord[1] < 0 || baryCoord[2] < 0)
		 	return -Z_MAX; 
		return baryCoord[0] * l.pos.z + baryCoord[1] * m.pos.z + baryCoord[2] * r.pos.z;
	}

	float Face::ZAtXYFace(float x, float y) const
	{
		return this->ZAtXYFace(Vec2(x, y));
	}

	float Face::ZAtXYPlane(Vector2 xy) const
	{
		Vec3 baryCoord = this->baryCoord(xy);
		return baryCoord[0] * l.pos.z + baryCoord[1] * m.pos.z + baryCoord[2] * r.pos.z;
	}

	float Face::ZAtXYPlane(float x, float y) const
	{
		return this->ZAtXYPlane(Vec2(x, y));
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
		if (std::abs(kLM) > SLOPE_MAX && std::abs(kMR) > SLOPE_MAX)
		{
			std::sort(vertices.begin(), vertices.end(), LowerVertex);
			this->l = vertices[0];
			this->m = vertices[1];
			this->r = vertices[2];
			this->kLM = GetSlope(l, m);
			this->kLR = GetSlope(l, r);
			this->kMR = GetSlope(m, r);
		}
		else if (std::abs(kLM) > SLOPE_MAX)
		{
			const Vertex lMem = l;
			l = std::min(lMem, m, LowerVertex);
			m = std::max(lMem, m, LowerVertex);
		}
		else if (std::abs(kMR) > SLOPE_MAX)
		{
			const Vertex mMem = m;
			m = std::min(r, mMem, LowerVertex);
			r = std::max(r, mMem, LowerVertex);
		}
		assert(!(l == r || m == r || l == m));
	}

	bool IsPointInTriangle(Vertex v, Face f)
	{
		f.l.pos.z = 0;
		f.m.pos.z = 0;
		f.r.pos.z = 0;
		v.pos.z = 0;
		Vec3 dir1 = (Cross(f.l.pos - v.pos, f.m.pos - f.l.pos));
		Vec3 dir2 = (Cross(f.m.pos - v.pos, f.r.pos - f.m.pos));
		Vec3 dir3 = (Cross(f.r.pos - v.pos, f.l.pos - f.r.pos));
		// Need to verify three because of the presence of sgn() = 0: consider (1, 0, -1)
		return Dot(dir1, dir2) >= 0 && Dot(dir3, dir2) >= 0 && Dot(dir1, dir3) >= 0;
		// return sgn(dir1.z) * sgn(dir2.z) >= 0 && sgn(dir1.z) * sgn(dir3.z) >= 0 && sgn(dir2.z) * sgn(dir3.z) >= 0;
	}

	void CompleteAttribInFace(EQX_OUT Vertex& v, const Face& f)
	{
		Vec3 baryCoordAtV = f.baryCoord(v.pos.x, v.pos.y);
		v.normal = baryCoordAtV[0] * f.l.normal + baryCoordAtV[1] * f.m.normal + baryCoordAtV[2] * f.r.normal;
		v.uv = baryCoordAtV[0] * f.l.uv + baryCoordAtV[1] * f.m.uv + baryCoordAtV[2] * f.r.uv;
	}

	bool FaceIntersectWithLine(const Face& f, const Line& l, EQX_OUT Vec3& pos)
	{
		Vec3 originalPos = pos;
		Plane facePlane = Plane(f.l.pos, f.m.pos, f.r.pos);
		if (PlaneIntersectWithLine(facePlane, l, EQX_OUT pos))
		{
			if (IsPointInTriangle(pos, f))
				return true;
			else
			{
				pos = originalPos;
				return false;
			}
		}
		return false;
	}

	bool PlaneIntersectWithLine(const Plane& p, const Line& l, EQX_OUT Vector3& pos)
	{
		if (Dot(p.GetNormal(), l.GetDirection()) == 0)
			return false;

		float dirMult = (Dot(p.GetNormal(), p.GetPoint()) - Dot(p.GetNormal(), l.GetPoint())) /
			(Dot(p.GetNormal(), l.GetDirection()));
		pos = l.GetPoint() + dirMult * l.GetDirection();
		return true;
	}

	void Print(const Face& f)
	{
		cout << "| Face:" << endl;
		cout << "| Vertex 1: ";
		Print(f.l.pos);
		cout << "| Vertex 2: ";
		Print(f.m.pos);
		cout << "|_Vertex 3: ";
		Print(f.r.pos);
	}
}
