#include "eqxpch.h"

#include "Mesh.h"

namespace EQX {

	const Mesh Mesh::EmptyMesh;

	void Mesh::AddFace(const Vertex& a, const Vertex& b, const Vertex& c)
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
		Mesh::AddFace(f.L(), f.M(), f.R());
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
		this->Scale(Vec3(s, s, s), Vec3::Zero);
	}

	void Mesh::Scale(float s, Vector3 center)
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
			iter->pos = center + Vec3(diff.x * s.x, diff.y * s.y, diff.z * s.z);
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
		// TODO Apply rotation
	}

	Mesh::Mesh(const Mesh& m)
	{
		for (auto iter = m.vertices.begin(); iter != m.vertices.cend(); ++iter)
			this->vertices.push_back(*iter);
		for (auto iter = m.faceIndices.begin(); iter != m.faceIndices.cend(); ++iter)
			this->faceIndices.push_back(*iter);
		for (auto iter = m.lineIndices.begin(); iter != m.lineIndices.cend(); ++iter)
			this->lineIndices.push_back(*iter);
	}

	void Mesh::operator=(const Mesh& m)
	{
		for (auto iter = m.vertices.begin(); iter != m.vertices.cend(); ++iter)
			this->vertices.push_back(*iter);
		for (auto iter = m.faceIndices.begin(); iter != m.faceIndices.cend(); ++iter)
			this->faceIndices.push_back(*iter);
		for (auto iter = m.lineIndices.begin(); iter != m.lineIndices.cend(); ++iter)
			this->lineIndices.push_back(*iter);
	}

	MeshTransform::MeshTransform()
	{
		this->displacement = Vec3::Zero;
		this->scaleCoeff = Vec3::One;
		this->scaleRef = Vec3::Zero;
		this->rotAxis = Vec3::UnitZ;
		this->rotAngle = 0.f;
	}
}
