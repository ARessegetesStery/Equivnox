#pragma once

#include <CoreMinimal.h>

#include "Entity.h"
#include "LandscapeInfo.h"

namespace EQX {

	class Scene
	{
		friend class AssetManager;
	public:
		Scene() = default;
		Scene(std::string);

		inline const std::string GetName() const { return this->name; }
		inline void SetName(std::string& name) { this->name = name; }
		
		inline std::vector<EntityConfig>& Renderables() { return renderables; }

		Entity& FindEntityWithUID(int UID);
		const Entity& FindEntityWithUID(int UID) const;

		inline const SceneID GetSceneID() const { return this->sceneID; }

		static const std::string s_defaultSceneName;
		static const std::string s_rendererSceneName;

	private:
		// the corresponding EntityConfig is default 
		EntityConfig& AddDefaultEntity(const Entity& ent, std::string name);
		EntityConfig& AddEntityWithTransform(const Entity& ent, std::string name, const MeshTransform& transform);
		
		std::string name;
		SceneID sceneID;

		std::vector<EntityConfig> renderables;
		std::vector<Entity> entityPool;

		static SceneID UIDCounter;
	};

}
