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

		void addFace(Vertex, Vertex, Vertex);
		void addFace(Face&);
		void addFace(std::vector<Face>&);
		void addFace(const std::initializer_list<Face>&);

		void addLine(Vertex, Vertex);
		void addLine(LineSeg&);
		void addLine(std::vector<LineSeg>&);
		void addLine(const std::initializer_list<LineSeg>&);

		void scale(float s);
		void shift(Vector3 delta);

		Mesh() = default;
		Mesh(const Mesh& m) = delete; // should not copy meshes

	private:
		std::vector<Vertex> vertices;
		std::vector<std::array<unsigned int, 3>> faceIndices;
		std::vector<std::array<unsigned int, 2>> lineIndices;

	};
}

