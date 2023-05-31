#pragma once

#include "eqxpch.h"

#include "LandscapeInfo.h"

namespace EQX {

	class Entity
	{
		friend class Renderer;
		friend class AssetManager;
	public:
		Entity();
		Entity(const Mesh&);
		Entity(const Entity&);

		Entity& operator= (const Entity&);

		inline const EntityID GetUID() const { return this->UID; }
		inline const Mesh& GetMesh() const { return this->mesh; }
		inline const std::vector<Entity*>& GetChildren() const { return this->children; }

		// TODO change the way children relationships are managed in Entity s
		inline void AddChild(Entity* c) { this->children.push_back(c); };

		// Directly acts on mesh; call corresponding functions of class {Mesh}
		// inline void ScaleOnMesh(float s) { return mesh.Scale(s); };
		// inline void ScaleOnMesh(float s, Vector3 center) { return mesh.Scale(s, center); };
		// inline void ScaleOnMesh(Vector3 s) { return mesh.Scale(s); };
		// inline void ScaleOnMesh(Vector3 s, Vector3 center) { return mesh.Scale(s, center); };
		// inline void ShiftOnMesh(Vector3 delta) { return mesh.Shift(delta); };

		inline void TransformMesh(MeshTransform& trans) { this->mesh.Transform(trans); }

		void ClearMesh();
		void AddFaceToMesh(const Vertex&, const Vertex&, const Vertex&);
		void AddFaceToMesh(const Face&);
		void AddFaceToMesh(std::vector<Face>&);
		void AddFaceToMesh(const std::initializer_list<Face>&);

	private:
		// UID of Entity is generated (assigned) upon construction or copy; and cannot be changed by outside
		EntityID UID;
		Mesh mesh;
		std::vector<Entity*> children;

		static EntityID UIDCounter;
	};

	inline bool EqualID(const Entity& e1, const Entity& e2) { return e1.GetUID() == e2.GetUID(); }

	/* An EntityConfig should be bound to a unique Entity and could not be changed.
	 * If one Entity shall be excluded, the corresponding EntityConfig should be removed from Renderables
	 * */
	class EntityConfig
	{
		friend class Renderer;
		friend class AssetManager;
	public:
		EntityConfig(int, std::string);
		EntityConfig(Entity&, std::string);
		EntityConfig(int, std::string, const MeshTransform&);
		EntityConfig(Entity&, std::string, const MeshTransform&);

		EntityConfig& operator= (const EntityConfig&);

		inline void Translate(const Vec3 v) { transform.displacement = transform.displacement + v; };
		inline void ScaleCoeff(float f) { this->transform.scaleCoeff = Vec3(f, f, f); }
		inline void ScaleCoeff(const Vec3& coeff) { this->transform.scaleCoeff = coeff; }
		inline void ScaleCenter(const Vec3& point) { this->transform.scaleRef = point; }
		inline void RotAxis(const Vec3& axis) { this->transform.rotAxis = axis; }
		inline void RotAngle(float angle) { this->transform.rotAngle = angle; }
		inline std::string GetName() const { return this->name; }
		inline EntityID GetBoundUID() const { return this->entityID; }
		inline ConfigID GetID() const { return this->configID; }
		inline const MeshTransform& GetTransform() const { return this->transform; }

		static const std::string s_defaultEntityName;

	private:
		ConfigID configID;
		EntityID entityID; 
		std::string name;
		MeshTransform transform;

		static ConfigID UIDCounter;
	};

}
