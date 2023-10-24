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
		Scene(XString);

		inline const XString GetName() const { return this->name; }
		inline void SetName(XString& name) { this->name = name; }
		
		inline std::vector<EntityConfig>& Renderables() { return renderables; }

		Entity& FindEntityWithUID(XInt UID);
		const Entity& FindEntityWithUID(XInt UID) const;

		inline const SceneID GetSceneID() const { return this->sceneID; }

		static const XString s_defaultSceneName;
		static const XString s_rendererSceneName;

	private:
		// the corresponding EntityConfig is default 
		EntityConfig& AddDefaultEntity(const Entity& ent, XString name);
		EntityConfig& AddEntityWithTransform(const Entity& ent, XString name, const MeshTransform& transform);
		
		XString name;
		SceneID sceneID;

		std::vector<EntityConfig> renderables;
		std::vector<Entity> entityPool;

		static SceneID UIDCounter;
	};

}
