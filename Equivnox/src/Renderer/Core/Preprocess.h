#pragma once

#include "eqxpch.h"

#include "Renderer/Shapes/Vertex.h"

namespace EQX {

	void floorVertexXYPos(Vertex& v);

	Vertex TransformVertexPos(const Mat4& projection, const Vertex& v);

}
