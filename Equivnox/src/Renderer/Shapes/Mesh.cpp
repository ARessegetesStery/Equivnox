#include "eqxpch.h"

#include "Mesh.h"

namespace EQX {

	void Mesh::AddFace(Vertex a, Vertex b, Vertex c)
	{
		bool ARep = false, BRep = false, CRep = false;
		unsigned int AIndex = 0, BIndex = 0, CIndex = 0;
		unsigned int CurIndex = this->vertices.size();
		std::array<unsigned int, 3> indices;
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			if (!ARep && *iter == a)
			{
				AIndex = iter - vertices.cbegin();
				ARep = true;
			}
			if (!BRep && *iter == b)
			{
				BIndex = iter - vertices.cbegin();
				BRep = true;
			}
			if (!CRep && *iter == c)
			{
				CIndex = iter - vertices.cbegin();
				CRep = true;
			}
			if (ARep && BRep && CRep)
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
		if (CRep)
			indices[2] = CIndex;
		else
		{
			this->vertices.push_back(c);
			indices[2] = CurIndex;
			++CurIndex;
		}

		this->faceIndices.push_back(indices);
	}

	void Mesh::AddFace(Face& f)
	{
		Mesh::AddFace(f.l, f.m, f.r);
	}

	void Mesh::AddFace(std::vector<Face>& ls)
	{
		for (auto iter = ls.begin(); iter != ls.cend(); ++iter)
			Mesh::AddFace(*iter);
	}

	void Mesh::AddFace(const std::initializer_list<Face>& ls)
	{
		for (auto iter : ls)
			Mesh::AddFace(iter);
	}

	void Mesh::AddLine(Vertex a, Vertex b)
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

	void Mesh::AddLine(LineSeg& l)
	{
		Mesh::AddLine(l.start, l.end);
	}

	void Mesh::AddLine(std::vector<LineSeg>& ls)
	{
		for (auto iter = ls.begin(); iter != ls.cend(); ++iter)
			Mesh::AddLine(*iter);
	}

	void Mesh::AddLine(const std::initializer_list<LineSeg>& ls)
	{
		for (auto iter : ls)
			Mesh::AddLine(iter);
	}

	void Mesh::Scale(float s)
	{
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			// cout << iter->pos.z;
			iter->pos = iter->pos * s;
			// cout << iter->pos.z << endl;;
		}
	}

	void Mesh::Shift(Vector3 delta)
	{
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			iter->pos = iter->pos + delta;
		}
	}

	/**
	 * Floor each index in v so that the renderLineSmooth could function normally
	 * 
	 * @param v the vertex; Note it will be modified
	 */
	

}
