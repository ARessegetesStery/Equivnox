#include "eqxpch.h"

#include "AssetManager.h"

namespace EQX {

	AssetManager::AssetManager()
	{
		this->scenes = {};
		this->_createEmptyScene(Scene::s_defaultSceneName);   // serve as a placeholder
		this->_createEmptyScene(Scene::s_rendererSceneName);
	}

	SceneInfo AssetManager::_createEmptyScene(std::string sceneName)
	{
		bool existDuplicate = false;
		for (auto iter = scenes.begin(); iter != scenes.cend(); ++iter)
			if (iter->name == sceneName)
				existDuplicate = true;
		if (existDuplicate)
		{
			Scene& defaultScene = *this->scenes.begin();
			return SceneInfo(defaultScene.GetName(), defaultScene.GetSceneID());
		}
		else
		{
			scenes.push_back(Scene(sceneName));
			Scene& result = *(scenes.end() - 1);
			return SceneInfo(result.GetName(), result.GetSceneID());
		}
	}

	EntityInfo AssetManager::_createEmptyEntityUnderScene(SceneInfo sceneInfo, std::string entityName)
	{
		Scene& scene = _scene(sceneInfo.name);
		for (auto iter = scene.Renderables().begin(); iter != scene.Renderables().cend(); ++iter)
			if (iter->GetName() == entityName)
			{
				EntityConfig& targetConfig = _configUnderScene(sceneInfo, EntityConfig::s_defaultEntityName);
				return EntityInfo(targetConfig.name, targetConfig.configID, targetConfig.entityID, sceneInfo);
			}
		const EntityConfig& res = scene.AddDefaultEntity(Entity(), entityName);
		return EntityInfo(entityName, res.configID, res.entityID, sceneInfo);
	}

	EntityInfo AssetManager::_duplicateEntity(SceneInfo sceneInfo, std::string from, std::string to)
	{
		return _duplicateEntityWithTransform(sceneInfo, from, to, MeshTransform());
	}

	EntityInfo AssetManager::_duplicateEntity(SceneInfo curScene, std::string from, EntityInfo ent)
	{
		return _duplicateEntity(curScene, from, ent.name);
	}

	EntityInfo AssetManager::_duplicateEntityWithTransform(SceneInfo sceneInfo, 
		std::string from, std::string to, const MeshTransform& trans)
	{
		Scene& curScene = _scene(sceneInfo);
		const EntityConfig& original = _configUnderScene(sceneInfo, from);
		if (original.GetName() == EntityConfig::s_defaultEntityName)
		{
			const EntityConfig& defaultConfig = _configUnderScene(sceneInfo, EntityConfig::s_defaultEntityName);
			return EntityInfo(defaultConfig.name, defaultConfig.configID, defaultConfig.entityID, sceneInfo);
		}
		const Entity& originalEntity = curScene.FindEntityWithUID(original.GetBoundUID());
		const EntityConfig& res = curScene.AddEntityWithTransform(originalEntity, to, trans);
		return EntityInfo(res.name, res.configID, res.entityID, sceneInfo);
	}

	EntityInfo AssetManager::_duplicateEntityWithTransform(SceneInfo curScene, 
		EntityInfo from, EntityInfo to, const MeshTransform& trans)
	{
		return _duplicateEntityWithTransform(curScene, from.name, to.name, trans);
	}

	AssetManager& AssetManager::_init()
	{
		static AssetManager _assetManager;
		return _assetManager;
	}

	// TODO test whether __nil__/__world__ is always the first element of vector
	Scene& AssetManager::_scene(std::string sceneName)
	{
		for (auto iter = scenes.begin(); iter != scenes.cend(); ++iter)
			if (iter->name == sceneName)
				return *iter;
		return *this->scenes.begin();
	}

	Scene& AssetManager::_scene(SceneInfo sceneInfo)
	{
		return this->_scene(sceneInfo.name);
	}

	EntityConfig& AssetManager::_configUnderScene(SceneInfo sceneInfo, std::string entName)
	{
		Scene& scene = _scene(sceneInfo.name);
		auto world = scene.Renderables().begin();
		for (auto iter = scene.Renderables().begin(); iter != scene.Renderables().cend(); ++iter)
		{
			if (iter->GetName() == entName)
				return *iter;
			else if (iter->GetName() == EntityConfig::s_defaultEntityName)
				world = iter;
		}
		return *world;
	}

	Entity& AssetManager::_entityUnderScene(SceneInfo sceneInfo, EntityID id)
	{
		Scene& scene = _scene(sceneInfo.name);
		auto world = scene.entityPool.begin();
		for (auto iter = scene.entityPool.begin(); iter != scene.entityPool.cend(); ++iter)
			if (iter->GetUID() == id)
				return *iter;
		return *world;
	}
	
}
