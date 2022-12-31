#include "eqxpch.h"

#include "Preprocess.h"

namespace EQX {

	void floorVertexXYPos(Vertex& v)
	{
		v.pos.x = floor(v.pos.x);
		v.pos.y = floor(v.pos.y);
	}

	Vertex TransformVertexPos(const Mat4& projection, const Vertex& v)
	{
		Vertex ans = v;
		ans.pos = projection * v.pos;
		ans.pos.Normalize();
		floorVertexXYPos(ans);
		return ans;
	}
}
