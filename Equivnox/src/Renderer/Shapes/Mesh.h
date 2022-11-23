#pragma once

#include "eqxpch.h"

#include "Renderer/Core/RenderConfig.h"
#include "LineSeg.h"
#include "Face.h"
#include "Vertex.h"

namespace EQX {

	class Mesh
	{
	public:
		friend class Renderer;

		void AddFace(Vertex, Vertex, Vertex);
		void AddFace(Face&);
		void AddFace(std::vector<Face>&);
		void AddFace(const std::initializer_list<Face>&);

		void AddLine(Vertex, Vertex);
		void AddLine(LineSeg&);
		void AddLine(std::vector<LineSeg>&);
		void AddLine(const std::initializer_list<LineSeg>&);

		void Scale(float s);
		void Shift(Vector3 delta);

		Mesh() = default;
		Mesh(const Mesh& m) = delete; // should not copy meshes

	public:
		std::vector<Vertex> vertices;
		std::vector<std::array<unsigned int, 3>> faceIndices;
		std::vector<std::array<unsigned int, 2>> lineIndices;

	};

	void floorVertexPos(Vertex& v);
}

