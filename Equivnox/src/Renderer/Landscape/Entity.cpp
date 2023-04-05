#include "eqxpch.h"

#include "Entity.h"

namespace EQX {

	int Entity::UIDCounter = 0;

	Entity::Entity()
	{
		this->UID = UIDCounter;
		++UIDCounter;
		this->mesh = Mesh::EmptyMesh;
	}

	Entity::Entity(const Mesh& mesh)
	{
		this->UID = UIDCounter;
		++UIDCounter;
		this->mesh = mesh;
	}

	Entity::Entity(const Entity& ent)
	{
		for (auto iter = ent.GetChildren().begin(); iter != ent.GetChildren().cend(); ++iter)
			this->children.push_back(*iter);
		this->mesh = ent.GetMesh();
		this->UID = ent.GetUID();
	}

	EntityConfig::EntityConfig(int UID) : UID(UID) { }

	EntityConfig::EntityConfig(int UID, const MeshTransform& trans) : 
		UID(UID), transform(trans) { }

	EntityConfig::EntityConfig(const Entity& ent) : UID(ent.GetUID())  {	}

	EntityConfig::EntityConfig(const Entity& ent, const MeshTransform& trans) : 
		UID(ent.GetUID()), transform(trans) { }

}
