#include "eqxpch.h"

#include "Preprocess.h"

namespace EQX {

	void floorVertexXYPos(Vertex& v)
	{
		v.pos.x = floor(v.pos.x);
		v.pos.y = floor(v.pos.y);
	}

	void FrustumClipping(const Face& inFace, EQX_OUT Mesh& clippedMesh)
	{
		auto IsCanonic = [=](XFloat x) -> XBool { return x >= -1.f && x <= 1.f; };
		auto IsVec4InCanonic = [=](const Vec3 v) -> XBool { return IsCanonic(v.x) && IsCanonic(v.y) && IsCanonic(v.z); };

		/*  Special Judgement for triangle within cube  */
		if (IsVec4InCanonic(inFace.L().pos) && IsVec4InCanonic(inFace.M().pos) && IsVec4InCanonic(inFace.R().pos))
		{
			clippedMesh.AddFace(inFace);
#ifdef EQX_PRINT_TRIG_CLIPPING
			cout << "-- Face Inside Screen --" << endl;
			Print(inFace.L().pos);
			Print(inFace.M().pos);
			Print(inFace.R().pos);
#endif
			return;
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
							XBool repFound = false;
							for (const auto& i : UnitBoxIntersection)
								if (Distance(i.pos, interpPos) <= 1e-5)
									repFound = true;
							if (!repFound)
								UnitBoxIntersection.emplace_back(interpPos);
						}
					}
					if (IsInInterval(0, 1, LerpCoeff(start, end, -1.0f, axis)))
					{
						Vec3 interpPos = Lerp(start, end, -1.0f, axis);
						if (IsVec4InCanonic(interpPos))
						{
							XBool repFound = false;
							for (const auto& i : UnitBoxIntersection)
								if (Distance(i.pos, interpPos) <= 1e-5)
									repFound = true;
							if (!repFound)
								UnitBoxIntersection.emplace_back(interpPos);
						}
					}
				}
			}
		}
		for (size_t ind = 0; ind != 3; ++ind)
			if (IsVec4InCanonic(inFace[ind].pos))
				UnitBoxInside.push_back(inFace[ind].pos);
#ifdef EQX_PRINT_TRIG_CLIPPING
		cout << "-- Face Partially Outside --" << endl;
		cout << ">> Inside Vertices:" << endl;
		for (const auto& i : UnitBoxInside)
			Print(i.pos);
		cout << ">> Intersection Vertices:" << endl;
		for (const auto& i : UnitBoxIntersection)
			Print(i.pos);
#endif

		/*  Include corner points  */
		Vec3 intersection = Vec3::Zero;
		std::vector<Line> potEdges;
		if (CornerEdge(UnitBoxIntersection, EQX_OUT potEdges))
		{
			for (const Line& l : potEdges)
			{
				if (FaceIntersectWithLine(inFace, l, EQX_OUT intersection))
				{
					UnitBoxIntersection.push_back(intersection);
#ifdef EQX_PRINT_TRIG_CLIPPING
					cout << ">> Edge Intersection:" << endl;
					Print(intersection);
#endif
				}
			}
		}

		/*  Complete attributes for the vertices  */
		for (auto& v : UnitBoxIntersection)
			CompleteAttribInFace(v, inFace);
		for (auto& v : UnitBoxInside)
			CompleteAttribInFace(v, inFace);

		/*  Add appropriate faces for substitution  */
		if (UnitBoxIntersection.size() == 0)
			return;

		if (UnitBoxInside.size() == 0)
			TriangularizeHull(UnitBoxIntersection, clippedMesh);

		else if (UnitBoxInside.size() == 1)
		{
			UnitBoxIntersection.push_back(UnitBoxInside[0]);
			TriangularizeHull(UnitBoxIntersection, clippedMesh);
		}
		else if (UnitBoxInside.size() == 2)
		{
			UnitBoxIntersection.push_back(UnitBoxInside[0]);
			UnitBoxIntersection.push_back(UnitBoxInside[1]);
			TriangularizeHull(UnitBoxIntersection, clippedMesh);
		}
	}

	XBool CornerEdge(const std::vector<Vertex>& intersections, EQX_OUT std::vector<Line>& lines)
	{
		XBool up = false, down = false, left = false, right = false, front = false, behind = false;
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

		XBool exists = true;
		if (left && up)
			lines.emplace_back(Line(Vec3(-1, 1, 1), Vec3(0, 0, 1)));
		if (right && up)
			lines.emplace_back(Line(Vec3(1, 1, 1), Vec3(0, 0, 1)));
		if (front && up)
			lines.emplace_back(Line(Vec3(1, 1, 1), Vec3(1, 0, 0)));
		if (behind && up)
			lines.emplace_back(Line(Vec3(-1, 1, -1), Vec3(1, 0, 0)));
		if (front && left)
			lines.emplace_back(Line(Vec3(-1, 1, 1), Vec3(0, 1, 0)));
		if (front && right)
			lines.emplace_back(Line(Vec3(1, 1, 1), Vec3(0, 1, 0)));
		if (right && behind)
			lines.emplace_back(Line(Vec3(1, 1, -1), Vec3(0, 1, 0)));
		if (left && behind)
			lines.emplace_back(Line(Vec3(-1, -1, -1), Vec3(0, 1, 0)));
		if (front && down)
			lines.emplace_back(Line(Vec3(1, -1, 1), Vec3(1, 0, 0)));
		if (right && down)
			lines.emplace_back(Line(Vec3(1, -1, 1), Vec3(0, 0, 1)));
		if (left && down)
			lines.emplace_back(Line(Vec3(-1, -1, -1), Vec3(0, 0, 1)));
		if (down && behind)
			lines.emplace_back(Line(Vec3(-1, -1, -1), Vec3(1, 0, 0)));
		if (lines.empty())
			exists = false;

		return exists;
	}

	void TriangularizeHull(std::vector<Vertex>& vertices, EQX_OUT Mesh& clippedMesh)
	{
		if (vertices.size() <= 2)
			return;
		Plane trigPlane = Plane(vertices[0].pos, vertices[1].pos, vertices[2].pos);
		Vec3 tanVec = trigPlane.TanVec();
		auto LessAlongTan = [&](const Vertex& v1, const Vertex& v2)
			-> XBool { return Dot((v1.pos - trigPlane.GetPoint()), tanVec) < Dot((v2.pos - trigPlane.GetPoint()), tanVec); };
		std::sort(vertices.begin(), vertices.end(), LessAlongTan);

		/*  Change {tanVec} into measure along the extrema and evaluate binormal  */
		Vertex& lowerEnd = *vertices.begin();
		Vertex& upperEnd = *(vertices.end() - 1);
		tanVec = (upperEnd.pos - lowerEnd.pos);
		tanVec.Normalize();
		Vec3 binormalVec = Cross(tanVec, trigPlane.GetNormal());
		// Binormal measure is required to be compared with the ends with extreme tangential measure
		auto BinormalMeasure = [&](const Vertex& v)
			-> XFloat { return Dot((v.pos - upperEnd.pos), binormalVec); }; 

		/*  Divide into ABOVE and BELOW vertices on a plane  */
		std::vector<Vertex> aboveVerts, belowVerts;
		for (auto& i : vertices)
		{
			if (i.pos == lowerEnd.pos || i.pos == upperEnd.pos)
				continue;
			if (BinormalMeasure(i) >= 0)
				aboveVerts.push_back(i);
			else if (BinormalMeasure(i) < 0)
				belowVerts.push_back(i);
		}
		// preserve tangential sequence
		std::sort(aboveVerts.begin(), aboveVerts.end(), LessAlongTan);
		std::sort(belowVerts.begin(), belowVerts.end(), LessAlongTan);

#ifdef EQX_PRINT_TRIG_CLIPPING
		cout << ">> Low End:" << endl;
		Print(lowerEnd.pos);
		cout << ">> High End:" << endl;
		Print(upperEnd.pos);
		cout << ">> Vertices Above:" << endl;
		for (const auto& i : aboveVerts)
			Print(i.pos);
		cout << ">> Vertices Below:" << endl;
		for (const auto& i : belowVerts)
			Print(i.pos);
#endif

		/*  Construct triangle chain (Clockwise)  */
		if (aboveVerts.size() == 1)
			clippedMesh.AddFace(Face(lowerEnd, upperEnd, aboveVerts[0]));
		else if (aboveVerts.size() >= 2)
		{
			for (auto iter = aboveVerts.begin(); iter != aboveVerts.cend() - 1; ++iter)
			{
				clippedMesh.AddFace(Face(lowerEnd, *iter, *(iter + 1)));
				if (iter == aboveVerts.cend() - 2)
					clippedMesh.AddFace(Face(lowerEnd, upperEnd, *(iter + 1)));
			}
		}
		if (belowVerts.size() == 1)
			clippedMesh.AddFace(Face(lowerEnd, upperEnd, belowVerts[0]));
		else if (belowVerts.size() >= 2)
		{
			for (auto iter = belowVerts.begin(); iter != belowVerts.cend() - 1; ++iter)
			{
				clippedMesh.AddFace(Face(lowerEnd, *iter, *(iter + 1)));
				if (iter == belowVerts.cend() - 2)
					clippedMesh.AddFace(Face(lowerEnd, upperEnd, *(iter + 1)));
			}
		}

#ifdef EQX_PRINT_TRIG_CLIPPING
		for (const auto& trig : trigs)
			Print(trig);
#endif

	}
}
