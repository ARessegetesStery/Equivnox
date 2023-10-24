#pragma once

#include <CoreMinimal.h>

#include "Runtime/Geometry/Geometry.h"
#include "Runtime/Landscape/Landscape.h"

/* EQX::AssetManager is constructed in a three-layer structure
 *	Mesh -> EntityConfig/Entity -> Scene
 * while the AssetManager takes control only of the collection of scenes.
 * 
 * Entity stores all the information on mesh, with EntityConfig stores only 
 * transformations of the corresponding Entity. 
 * 
 * The user of the renderer is then unable to use any structure that is created
 * outside the renderer even though they can be created. 
 * 
 * When passing parameters, pass only Infos (SceneInfo/EntityInfo) to avoid 
 * null pointer in case of reallocation 
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
		EntityInfo _duplicateEntity(SceneInfo curScene, std::string from, EntityInfo ent);
		EntityInfo _duplicateEntityWithTransform(SceneInfo curScene, std::string originalName, 
			std::string entityName, const MeshTransform& trans);
		EntityInfo _duplicateEntityWithTransform(SceneInfo curScene, EntityInfo from,  
			EntityInfo to, const MeshTransform& trans);

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

