#pragma once

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

	bool operator== (Vertex& v1, Vertex& v2)
	{
		return (v1.color == v2.color) && (v1.uv == v2.uv)
			&& (v1.normal == v2.color) && (v1.pos == v2.pos);
	}

	bool operator== (Vertex& v1, const Vertex& v2)
	{
		return (v1.color == v2.color) && (v1.uv == v2.uv)
			&& (v1.normal == v2.color) && (v1.pos == v2.pos);
	}

	bool LefterVertex(const Vertex& l, const Vertex& r)
	{
		return l.pos.x < r.pos.x;
	}

	bool LowerVertex(const Vertex& l, const Vertex& u)
	{
		return l.pos.y < u.pos.y;
	}

	bool LefterLowerBehindVertex(const Vertex& l, const Vertex& r)
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
