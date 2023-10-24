#include "eqxpch.h"

#include "Vertex.h"

namespace EQX
{
	Vertex::Vertex(Vector4 pos, Vector3 normal, Vector2 uv)
		: pos(pos), normal(normal), uv(uv) {  }

	Vertex::Vertex(Vector3 pos, Vector3 normal, Vector2 uv)
		: pos(Vector4(pos.x, pos.y, pos.z, 1.0)), normal(normal), uv(uv) {  }

	Vertex::Vertex(Vector2 pos, Vector3 normal, Vector2 uv)
		: pos(Vector4(pos.x, pos.y, 0.0, 1.0)), normal(normal), uv(uv) {  }

	XBool operator== (Vertex& v1, Vertex& v2)
	{
		return (v1.color == v2.color) && (v1.uv == v2.uv)
			&& (v1.normal == v2.normal) && (v1.pos == v2.pos);
	}

	XBool operator== (Vertex& v1, const Vertex& v2)
	{
		return (v1.color == v2.color) && (v1.uv == v2.uv)
			&& (v1.normal == v2.normal) && (v1.pos == v2.pos);
	}

	void Vertex::MatTransform(const Mat4& projection)
	{
		this->pos = projection * this->pos;
		this->pos.Normalize();
	}

	void Vertex::MeshTransform(const EQX::MeshTransform& trans)
	{
		this->pos = this->pos + trans.displacement;
		// this->pos = trans.scaleRef + TermWiseProduct(trans.scaleCoeff, 
		// 	(this->pos - trans.scaleRef).ToVec3());
		// TODO implement rotation
	}

	XBool LefterVertex(const Vertex& l, const Vertex& r)
	{
		return l.pos.x < r.pos.x;
	}

	XBool LowerVertex(const Vertex& l, const Vertex& u)
	{
		return l.pos.y < u.pos.y;
	}

	XBool LefterLowerBehindVertex(const Vertex& l, const Vertex& r)
	{
		if (l.pos.x != r.pos.x)
			return l.pos.x < r.pos.x;
		else
		{
			if (l.pos.y != r.pos.y)
				return l.pos.y < r.pos.y;
			else
				return l.pos.z < r.pos.z;
		}
	}
}
