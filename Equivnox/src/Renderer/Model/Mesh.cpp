#include "eqxpch.h"

#include "Mesh.h"

namespace EQX {

	// TODO encode
	void Mesh::addLine(Vertex a, Vertex b)
	{
		this->vertices.push_back(a);
		this->vertices.push_back(b);

		this->lineIndices.push_back(std::array<unsigned int, 2>{0, 1});
	}

	void Mesh::addLine(LineSeg& l)
	{
		this->vertices.push_back(l.start);
		this->vertices.push_back(l.end);

		this->lineIndices.push_back(std::array<unsigned int, 2>{0, 1});
	}

}
