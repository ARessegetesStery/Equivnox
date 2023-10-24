#include "eqxpch.h"

#include "Mesh.h"

namespace EQX {

	void Mesh::AddFace(const Vertex& a, const Vertex& b, const Vertex& c)
	{
		XBool ARep = false, BRep = false, CRep = false;
		XUInt AIndex = 0, BIndex = 0, CIndex = 0;
		XUInt CurIndex = static_cast<XUInt>(this->vertices.size());
		std::array<XUInt, 3> indices;
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			if (!ARep && *iter == a)
			{
				AIndex = static_cast<XUInt>(iter - vertices.cbegin());
				ARep = true;
			}
			if (!BRep && *iter == b)
			{
				BIndex = static_cast<XUInt>(iter - vertices.cbegin());
				BRep = true;
			}
			if (!CRep && *iter == c)
			{
				CIndex = static_cast<XUInt>(iter - vertices.cbegin());
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

	void Mesh::AddFace(const Face& f)
	{
		Mesh::AddFace(f.L(), f.M(), f.R());
	}

	void Mesh::AddFace(std::vector<Face>& ls)
	{
		for (auto iter = ls.begin(); iter != ls.cend(); ++iter)
			Mesh::AddFace(*iter);
	}

	void Mesh::AddFace(const std::initializer_list<Face>& ls)
	{
		for (const auto& iter : ls)
			Mesh::AddFace(iter);
	}

	void Mesh::AddLine(Vertex a, Vertex b)
	{
		XBool ARep = false;
		XBool BRep = false;
		XUInt AIndex;
		XUInt BIndex;
		XUInt CurIndex = static_cast<XUInt>(this->vertices.size());
		std::array<XUInt, 2> indices;
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			if (!ARep)
			{
				if (*iter == a)
				{
					ARep = true;
					AIndex = static_cast<XUInt>(iter - vertices.cbegin());
				}
			}
			if (!BRep)
			{
				if (*iter == b)
				{
					BRep = true;
					BIndex = static_cast<XUInt>(iter - vertices.cbegin());
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

	void Mesh::Scale(XFloat s)
	{
		this->Scale(Vec3(s, s, s), Vec3::Zero);
	}

	void Mesh::Scale(XFloat s, Vector3 center)
	{
		this->Scale(Vec3(s, s, s),center);
	}

	void Mesh::Scale(Vector3 s)
	{
		this->Scale(s, Vec3::Zero);
	}

	void Mesh::Scale(Vector3 s, Vector3 center)
	{
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			Vec3 diff = iter->pos - center;
			iter->pos = center + TermWiseProduct(diff, s);
		}
	}

	void Mesh::Shift(Vector3 delta)
	{
		for (auto iter = vertices.begin(); iter != vertices.cend(); ++iter)
		{
			iter->pos = iter->pos + delta;
		}
	}

	void Mesh::Transform(const MeshTransform& transform)
	{
		this->Shift(transform.displacement);
		if (transform.scaleCoeff != Vec3::One)
			this->Scale(transform.scaleCoeff, transform.scaleRef);
		// TODO implement rotation
	}

	void Mesh::Clear()
	{
		this->faceIndices.clear();
		this->faceIndices.clear();
		this->lineIndices.clear();
	}

	Mesh::Mesh()
	{
		this->vertices = {};
		this->faceIndices = {};
		this->lineIndices = {};
	}

	Mesh::Mesh(const Mesh& m)
	{
		this->Clear();
		for (auto iter = m.vertices.begin(); iter != m.vertices.cend(); ++iter)
			this->vertices.push_back(*iter);
		for (auto iter = m.faceIndices.begin(); iter != m.faceIndices.cend(); ++iter)
			this->faceIndices.push_back(*iter);
		for (auto iter = m.lineIndices.begin(); iter != m.lineIndices.cend(); ++iter)
			this->lineIndices.push_back(*iter);
	}

	void Mesh::operator=(const Mesh& m)
	{
		this->Clear();
		for (auto iter = m.vertices.begin(); iter != m.vertices.cend(); ++iter)
			this->vertices.push_back(*iter);
		for (auto iter = m.faceIndices.begin(); iter != m.faceIndices.cend(); ++iter)
			this->faceIndices.push_back(*iter);
		for (auto iter = m.lineIndices.begin(); iter != m.lineIndices.cend(); ++iter)
			this->lineIndices.push_back(*iter);
	}

}
