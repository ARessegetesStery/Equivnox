#include "eqxpch.h"

#include "Entity.h"

namespace EQX {

	const std::string EntityConfig::s_defaultEntityName = "__world__";

	EntityID Entity::UIDCounter = 0;
	ConfigID EntityConfig::UIDCounter = 0;

	Entity::Entity() : UID(UIDCounter)
	{
		++UIDCounter;
		this->mesh = Mesh();
	}

	Entity::Entity(const Mesh& mesh) : UID(UIDCounter)
	{
		++UIDCounter;
		this->mesh = mesh;
	}

	Entity::Entity(const Entity& ent) : UID(ent.GetUID())
	{
		for (auto iter = ent.GetChildren().begin(); iter != ent.GetChildren().cend(); ++iter)
			this->children.push_back(*iter);
		this->mesh = ent.GetMesh();
	}

	Entity& Entity::operator=(const Entity& ent)
	{
		this->UID = ent.GetUID();
		for (auto iter = ent.GetChildren().begin(); iter != ent.GetChildren().cend(); ++iter)
			this->children.push_back(*iter);
		this->mesh = ent.GetMesh();
		return *this;
	}

	void Entity::ClearMesh()
	{
		this->mesh.Clear();
	}

	void Entity::AddFaceToMesh(const Vertex& a, const Vertex& b, const Vertex& c)
	{
		this->mesh.AddFace(a, b, c);
	}

	void Entity::AddFaceToMesh(const Face& f)
	{
		this->mesh.AddFace(f);
	}

	void Entity::AddFaceToMesh(std::vector<Face>& ls)
	{
		this->mesh.AddFace(ls);
	}

	void Entity::AddFaceToMesh(const std::initializer_list<Face>& ls)
	{
		this->mesh.AddFace(ls);
	}

	EntityConfig::EntityConfig(int UID, std::string name) : 
		entityID(UID), name(name), transform(MeshTransform()) 
	{
		configID = UIDCounter;
		++UIDCounter;
	}

	EntityConfig::EntityConfig(Entity& ent, std::string name) : 
		entityID(ent.GetUID()), name(name), transform(MeshTransform()) 
	{
		configID = UIDCounter;
		++UIDCounter;
	}

	EntityConfig::EntityConfig(int UID, std::string name, const MeshTransform& trans) :
		entityID(UID), name(name), transform(trans) 
	{
		configID = UIDCounter;
		++UIDCounter;
	}

	EntityConfig::EntityConfig(Entity& ent, std::string name, const MeshTransform& trans) :
		entityID(ent.GetUID()), name(name), transform(trans) 
	{
		configID = UIDCounter;
		++UIDCounter;
	}

	EntityConfig& EntityConfig::operator=(const EntityConfig& entConfig)
	{
		this->configID = entConfig.configID;
		this->entityID= entConfig.GetBoundUID();
		this->name = entConfig.GetName();
		this->transform = entConfig.GetTransform();
		return *this;
	}

	EntityInfo::EntityInfo(std::string name, ConfigID CID, EntityID EID, SceneInfo parent) : 
		name(name), configID(CID), entityID(EID), parent(parent)	{	}
}
