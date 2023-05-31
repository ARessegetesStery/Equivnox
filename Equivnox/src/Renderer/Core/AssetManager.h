#pragma once

#include "eqxpch.h"

#include "Renderer/Geometry/Geometry.h"
#include "Renderer/Landscape/Landscape.h"

/* When passing parameters, pass only Infos (SceneInfo/EntityInfo) to avoid null pointer in case of reallocation
 *
 *  
 * */

namespace EQX {

	class AssetManager
	{
	public:
		// returns false if the name is currently occupied. 
		SceneInfo _createEmptyScene(std::string sceneName);
		EntityInfo _createEmptyEntityUnderScene(SceneInfo scene, std::string entityName);
		EntityInfo _duplicateEntity(SceneInfo curScene, std::string from, std::string to);
		EntityInfo _duplicateEntity(SceneInfo curScene, std::string from, const Entity& ent);
		EntityInfo _duplicateEntityWithTransform(SceneInfo curScene, std::string originalName, std::string entityName, const MeshTransform& trans);
		EntityInfo _duplicateEntityWithTransform(SceneInfo curScene, const Entity& ent, const MeshTransform& trans);

		static AssetManager& _init();

		/*  Returning non-Info type should be forbidden to users outside of Renderer  */
		Scene& _scene(std::string sceneName);
		Scene& _scene(SceneInfo sceneInfo);

		EntityConfig& _configUnderScene(SceneInfo scene, std::string entName);

		Entity& _entityUnderScene(SceneInfo sceneInfo, EntityID id);
	private:
		AssetManager();

		std::vector<Scene> scenes;
	};

}

