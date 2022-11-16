#pragma once

#include "eqxpch.h"

#include "Renderer/Core/RenderConfig.h"
#include "LineSeg.h"

namespace EQX {

	class Mesh
	{
	public:
		friend class Renderer;

		// void addFace(Vertex, Vertex, Vertex);
		void addLine(Vertex, Vertex);
		void addLine(LineSeg&);

		Mesh() = default;
		Mesh(const Mesh& m) = delete; // should not copy meshes

	private:
		std::vector<Vertex> vertices;
		std::vector<std::array<unsigned int, 3>> faceIndices;
		std::vector<std::array<unsigned int, 2>> lineIndices;

	};
}

