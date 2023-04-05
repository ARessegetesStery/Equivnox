#pragma once

#include "eqxpch.h"

#include "Face.h"

namespace EQX {

	class Mesh;
	class MeshTransform;

	class Mesh
	{
	public:
		friend class Renderer;

		void AddFace(const Vertex&, const Vertex&, const Vertex&);
		void AddFace(Face&);
		void AddFace(std::vector<Face>&);
		void AddFace(const std::initializer_list<Face>&);

		void AddLine(Vertex, Vertex);
		void AddLine(LineSeg&);
		void AddLine(std::vector<LineSeg>&);
		void AddLine(const std::initializer_list<LineSeg>&);

		void Scale(float s);
		void Scale(float s, Vector3 center);
		void Scale(Vector3 s);
		void Scale(Vector3 s, Vector3 center);
		void Shift(Vector3 delta);

		void Transform(const MeshTransform&);

		inline bool Empty() { return vertices.empty(); }

		Mesh() = default;
		Mesh(const Mesh& m);

		void operator= (const Mesh& m);

		static const Mesh EmptyMesh;

	public:
		std::vector<Vertex> vertices;
		std::vector<std::array<unsigned int, 3>> faceIndices;
		std::vector<std::array<unsigned int, 2>> lineIndices;

	};

	// Finalized mesh only to be created in the rendering process
	class FMesh
	{
		friend class Renderer;
	public:
		FMesh() = default;

		inline void AddFace(const Face& f) { this->faces.emplace_back(f); }
		inline const std::vector<Face>& const Trigs() { return faces; }

	private:
		std::vector<Face> faces;
	};

	/**
	 * The sequence of applying the changes is
	 * Translation -> Scaling -> Rotation
	 */
	class MeshTransform
	{
	public:
		Vec3 displacement;
		Vec3 scaleCoeff;
		Vec3 scaleRef;
		float rotAngle;
		Vec3 rotAxis;

	public: 
		MeshTransform();
	};
}

