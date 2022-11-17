#include "eqxpch.h"

#include "Mesh.h"

namespace EQX {

	// TODO encode
	void Mesh::addLine(Vertex a, Vertex b)
	{
		bool ARep = false;
		bool BRep = false;
		unsigned int AIndex;
		unsigned int BIndex;
		unsigned int CurIndex = this->vertices.size();
		std::array<unsigned int, 2> indices;
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			if (!ARep)
			{
				if (*iter == a)
				{
					ARep = true;
					AIndex = iter - vertices.cbegin();
				}
			}
			if (!BRep)
			{
				if (*iter == b)
				{
					BRep = true;
					BIndex = iter - vertices.cbegin();
				}
			}
			if (ARep && BRep)
				break;
		}
		if (ARep)
			indices[0] = AIndex;
		else
		{
			this->vertices.push_back(a);
			indices[0] = CurIndex;
			++CurIndex;
		}

		if (BRep)
			indices[1] = BIndex;
		else
		{
			this->vertices.push_back(b);
			indices[1] = CurIndex;
			++CurIndex;
		}

		this->lineIndices.push_back(indices);
	}

	void Mesh::addLine(LineSeg& l)
	{
		Mesh::addLine(l.start, l.end);
	}

	void Mesh::addLine(std::vector<LineSeg>& ls)
	{
		for (auto iter = ls.begin(); iter != ls.cend(); ++iter)
			Mesh::addLine(*iter);
	}

	void Mesh::addLine(const std::initializer_list<LineSeg>& ls)
	{
		for (auto iter : ls)
			Mesh::addLine(iter);
	}

}
