#pragma once

#include <CoreMinimal.h>

#include "LandscapeFwd.h"
#include "Runtime/Geometry/Mesh.h"

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

	using EntityID = XUInt;
	using ConfigID = XUInt;
	using SceneID = XUInt;

	class SceneInfo
	{
	public:
		XString name;
		SceneID sceneID;

		SceneInfo(XString, SceneID);
	};

	class EntityInfo
	{
	public:
		XString name;
		ConfigID configID;
		EntityID entityID;

		SceneInfo parent;

		EntityInfo(XString, ConfigID, EntityID, SceneInfo);
	};

}
