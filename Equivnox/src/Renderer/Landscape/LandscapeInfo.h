#pragma once

#include "eqxpch.h"

#include "Renderer/Geometry/Mesh.h"

namespace EQX {

	/**
	 * Contains one or several meshes; allows hierarchial manipulation
	 * Should not be directly manipulated with.
	 */
	class Entity;

	/**
	 * Contains a pointer `binded` to the entity prototype and a
	 * `MeshTransform` describing the transformations done to it.
	 */
	class EntityConfig;

	class Scene;

	class EntityInfo;

	class SceneInfo;

	using EntityID = unsigned int;
	using ConfigID = unsigned int;
	using SceneID = unsigned int;

	struct SceneInfo
	{
	public:
		std::string name;
		SceneID sceneID;

		SceneInfo(std::string, SceneID);
	};

	struct EntityInfo
	{
	public:
		std::string name;
		ConfigID configID;
		EntityID entityID;

		SceneInfo parent;

		EntityInfo(std::string, ConfigID, EntityID, SceneInfo);
	};

}
