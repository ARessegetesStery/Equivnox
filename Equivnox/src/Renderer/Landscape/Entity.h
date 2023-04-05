#pragma once

#include "eqxpch.h"

#include "Renderer/Geometry/Mesh.h"

namespace EQX {

	extern class Scene;

	/**
	 * Contains one or several meshes; allows hierarchial manipulation
	 */
	class Entity;

	/**
	 * Contains a pointer `binded` to the entity prototype and a 
	 * `MeshTransform` describing the transformations done to it.
	 */
	class EntityConfig;

	class Entity
	{
	public:
		Entity();
		Entity(const Mesh&);
		Entity(const Entity&);

		inline int GetUID() const { return this->UID; }
		inline const Mesh& GetMesh() const { return this->mesh; }
		inline const std::vector<Entity*>& GetChildren() const { return this->children; }

		inline void AddChild(Entity* c) { this->children.push_back(c); };

	private:
		int UID;
		Mesh mesh;
		std::vector<Entity*> children;

		static int UIDCounter;
	};

	inline bool EqualID(const Entity& e1, const Entity& e2) { return e1.GetUID() == e2.GetUID(); }

	class EntityConfig
	{
	public:
		EntityConfig(int);
		EntityConfig(int, const MeshTransform&);
		EntityConfig(const Entity&);
		EntityConfig(const Entity&, const MeshTransform&);

		inline void Translate(const Vec3 v) { transform.displacement = transform.displacement + v; };
		inline void ScaleCoeff(float f) { this->transform.scaleCoeff = Vec3(f, f, f); }
		inline void ScaleCoeff(const Vec3& coeff) { this->transform.scaleCoeff = coeff; }
		inline void ScaleCenter(const Vec3& point) { this->transform.scaleRef = point; }
		inline void RotAxis(const Vec3& axis) { this->transform.rotAxis = axis; }
		inline void RotAngle(float angle) { this->transform.rotAngle = angle; }
		inline int GetUID() const { return this->UID; }
		inline const MeshTransform& GetTransform() const { return this->transform; }

	private:
		int UID;
		MeshTransform transform;
	};
	
}
