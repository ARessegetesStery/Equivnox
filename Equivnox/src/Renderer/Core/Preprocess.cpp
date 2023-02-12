#include "eqxpch.h"

#include "Preprocess.h"

namespace EQX {

	void floorVertexXYPos(Vertex& v)
	{
		v.pos.x = floor(v.pos.x);
		v.pos.y = floor(v.pos.y);
	}

	void TransformVertexPos(const Mat4& projection, EQX_OUT Vertex& v)
	{
		v.pos = projection * v.pos;
		v.pos.Normalize();
	}

	void TransformFace(EQX_OUT Face& fTransform, const Mat4 transform)
	{
		TransformVertexPos(transform, fTransform.l);
		TransformVertexPos(transform, fTransform.m);
		TransformVertexPos(transform, fTransform.r);
		fTransform.ValidateSeq();
	}
	
	bool FrustumClipping(const Face& inFace, EQX_OUT std::vector<Face>& outFaces)
	{
		outFaces.clear();
		auto IsCanonic = [=](float x) -> bool { return x >= -1 && x <= 1; };
		auto IsVec4InCanonic = [=](const Vec3 v) -> bool { return IsCanonic(v.x) && IsCanonic(v.y) && IsCanonic(v.z); };

		/*  Special Judgement for triangle within cube  */
		if (IsVec4InCanonic(inFace.l.pos) && IsVec4InCanonic(inFace.m.pos) && IsVec4InCanonic(inFace.r.pos))
		{
			outFaces.push_back(inFace);
			return true;
		}

		/*  Detect intersection with unit box  */
		std::vector<Vertex> UnitBoxIntersection;
		std::vector<Vertex> UnitBoxInside;
		for (size_t i = 0; i != 3; ++i)
		{
			for (size_t j = i + 1; j < 3; ++j)
			{
				Vec3 start = inFace[i].pos;
				Vec3 end = inFace[j].pos;
				for (size_t ind = 0; ind != 3; ++ind)
				{
					// ind = 0: x, 1: y, 2: z
					Axis axis = Axis::AXIS_X;
					if (ind == 1)
						axis = Axis::AXIS_Y;
					else if (ind == 2)
						axis = Axis::AXIS_Z;
					if (IsInInterval(0, 1, LerpCoeff(start, end, 1.0f, axis)))
					{
						Vec3 interpPos = Lerp(start, end, 1.0f, axis);
						if (IsVec4InCanonic(interpPos))
						{
							bool repFound = false;
							for (const auto& i : UnitBoxIntersection)
								if (i.pos.ToVec3() == interpPos)
									repFound = true;
							if (!repFound)
								UnitBoxIntersection.emplace_back(Lerp(start, end, 1.0f, axis));
						}
					}
					if (IsInInterval(0, 1, LerpCoeff(start, end, -1.0f, axis)))
					{
						Vec3 interpPos = Lerp(start, end, 1.0f, axis);
						if (IsVec4InCanonic(interpPos))
						{
							bool repFound = false;
							for (const auto& i : UnitBoxIntersection)
								if (i.pos.ToVec3() == interpPos)
									repFound = true;
							if (!repFound)
								UnitBoxIntersection.emplace_back(Lerp(start, end, 1.0f, axis));
						}
					}
				}
			}
		}
		for (size_t ind = 0; ind != 3; ++ind)
			if (IsVec4InCanonic(inFace[ind].pos))
				UnitBoxInside.push_back(inFace[ind].pos);

		/*  Include corner points  */
		Vec3 intersection = Vec3::ZERO;
		Line potEdge;
		if (CornerEdge(UnitBoxIntersection, EQX_OUT potEdge))
			if (FaceIntersectiWithLine(inFace, potEdge, EQX_OUT intersection))
				UnitBoxIntersection.push_back(intersection);

		/*  Complete attributes for the vertices  */
		for (auto& v : UnitBoxIntersection)
			CompleteAttribInFace(v, inFace);
		for (auto& v : UnitBoxInside)
			CompleteAttribInFace(v, inFace);

		/*  Add appropriate faces for substitution  */
		if (UnitBoxIntersection.size() == 0)
			return false;

		if (UnitBoxInside.size() == 0)
		{
			TriangularizeHull(UnitBoxIntersection, outFaces);
			return true;
		}
		else if (UnitBoxInside.size() == 1)
		{
			UnitBoxIntersection.push_back(UnitBoxInside[0]);
			TriangularizeHull(UnitBoxIntersection, outFaces);
			return true;
		}
		else if (UnitBoxInside.size() == 2)
		{
			UnitBoxIntersection.push_back(UnitBoxInside[0]);
			UnitBoxIntersection.push_back(UnitBoxInside[1]);
			TriangularizeHull(UnitBoxIntersection, outFaces);
			return true;
		}
		return false;
	}

	bool CornerEdge(const std::vector<Vertex>& intersections, EQX_OUT Line& l)
	{
		bool up = false, down = false, left = false, right = false, front = false, behind = false;
		for (const auto& i : intersections)
		{
			if (i.pos.x == -1)
				left = true;
			else if (i.pos.x == 1)
				right = true;
			else if (i.pos.y == -1)
				down = true;
			else if (i.pos.y == 1)
				up = true;
			else if (i.pos.z == -1)
				behind = true;
			else if (i.pos.z == 1)
				front = true;
		}

		bool exists = true;
		if (left && up)
			l = Line(Vec3(-1, 1, 1), Vec3(0, 0, 1));
		else if (right && up)
			l = Line(Vec3(1, 1, 1), Vec3(0, 0, 1));
		else if (front && up)
			l = Line(Vec3(1, 1, 1), Vec3(1, 0, 0));
		else if (behind && up)
			l = Line(Vec3(-1, 1, -1), Vec3(1, 0, 0));
		else if (front && left)
			l = Line(Vec3(-1, 1, 1), Vec3(0, 1, 0));
		else if (front && right)
			l = Line(Vec3(1, 1, 1), Vec3(0, 1, 0));
		else if (right && behind)
			l = Line(Vec3(1, 1, -1), Vec3(0, 1, 0));
		else if (left && behind)
			l = Line(Vec3(-1, -1, -1), Vec3(0, 1, 0));
		else if (front && down)
			l = Line(Vec3(1, -1, 1), Vec3(1, 0, 0));
		else if (right && down)
			l = Line(Vec3(1, -1, 1), Vec3(0, 0, 1));
		else if (left && down)
			l = Line(Vec3(-1, -1, -1), Vec3(0, 0, 1));
		else if (down && behind)
			l = Line(Vec3(-1, -1, -1), Vec3(1, 0, 0));
		else exists = false;

		return exists;
	}

	void TriangularizeHull(std::vector<Vertex>& vertices, EQX_OUT std::vector<Face>& trigs)
	{
		// TODO test
		if (vertices.size() <= 2)
			return;
		Plane trigPlane = Plane(vertices[0].pos, vertices[1].pos, vertices[2].pos);
		Vec3 tanVec = trigPlane.TanVec();
		Vec3 binormalVec = Cross(tanVec, trigPlane.GetNormal());
		auto LessAlongTan = [&](const Vertex& v1, const Vertex& v2)
			-> bool { return Dot((v1.pos - trigPlane.GetPoint()), tanVec) < Dot((v2.pos - trigPlane.GetPoint()), tanVec); };
		auto BinormalMeasure = [&](const Vertex& v)
			-> float { return Dot((v.pos - trigPlane.GetPoint()), binormalVec); };
		std::sort(vertices.begin(), vertices.end(), LessAlongTan);

		/*  Divide into ABOVE and BELOW vertices on a plane  */
		Vertex& lowerEnd = *vertices.begin();
		Vertex& upperEnd = *(vertices.end() - 1);
		std::vector<Vertex> aboveVerts, belowVerts;
		for (auto& i : vertices)
		{
			if (i == lowerEnd || i == upperEnd)
				continue;
			if (BinormalMeasure(i) >= 0)
				aboveVerts.push_back(i);
			else if (BinormalMeasure(i) < 0)
				belowVerts.push_back(i);
		}
		// preserve tangential sequence
		std::sort(aboveVerts.begin(), aboveVerts.end(), LessAlongTan);
		std::sort(belowVerts.begin(), belowVerts.end(), LessAlongTan);

		/*  Construct triangle chain (Clockwise)  */
		if (aboveVerts.size() == 1)
			trigs.push_back(Face(lowerEnd, upperEnd, aboveVerts[0]));
		else if (aboveVerts.size() >= 2)
		{
			for (auto iter = aboveVerts.begin(); iter != aboveVerts.cend() - 1; ++iter)
			{
				trigs.push_back(Face(lowerEnd, *iter, *(iter + 1)));
				if (iter == aboveVerts.cend() - 1)
					trigs.push_back(Face(lowerEnd, upperEnd, *(iter + 1)));
			}
		}
		if (belowVerts.size() == 1)
			trigs.push_back(Face(lowerEnd, upperEnd, belowVerts[0]));
		else if (belowVerts.size() >= 2)
		{
			for (auto iter = aboveVerts.end() - 1; iter != aboveVerts.cbegin(); ++iter)
			{
				trigs.push_back(Face(lowerEnd, *iter, *(iter - 1)));
				if (iter == aboveVerts.cbegin() + 1)
					trigs.push_back(Face(lowerEnd, upperEnd, *(iter - 1)));
			}
		}
	}
}
