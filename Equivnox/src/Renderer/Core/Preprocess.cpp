#include "eqxpch.h"

#include "Preprocess.h"

namespace EQX {

	void floorVertexPos(Vertex& v)
	{
		v.pos.x = floor(v.pos.x);
		v.pos.y = floor(v.pos.y);
		v.pos.z = floor(v.pos.z);
	}

}
