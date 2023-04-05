#pragma once

#include "eqxpch.h"

#include "Entity.h"

namespace EQX {

	class Scene;
	class FScene;

	class Scene
	{
	public:
		Scene() = default;
		
		inline const std::vector<EntityConfig>& Renderables() const { return renderables; }
		const Entity& FindUID(int UID) const;

		inline void AddRawEntity(const Entity& ent) { this->entityPool.emplace_back(ent); }
		// the corresponding EntityConfig is default 
		void AddDefaultEntity(const Entity& ent);
		void AddEntityWithTransform(const Entity& ent, const MeshTransform& transform);
		
	private:
		std::vector<EntityConfig> renderables;
		std::vector<Entity> entityPool;
	};

	/**
	 * Can be only initialized from class Scene in the final rendering stage
	 * Expands all the renderables in the EntityConfig
	 */
	class FScene
	{
	public:
		FScene() = default;
		FScene(const Scene&);
		
		// when loading a new scene, existing entities are cleared
		void LoadScene(const Scene&);
		inline const std::vector<Entity>* Renderables() const { return &entities; }

	private:
		std::vector<Entity> entities;
	};

}
