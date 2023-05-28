#pragma once

#include "eqxpch.h"

#include "Entity.h"

namespace EQX {

	class Scene;

	class Scene
	{
	public:
		Scene() = default;
		
		inline const std::vector<EntityConfig>& Renderables() const { return renderables; }
		Entity& FindEntityWithUID(int UID);
		const Entity& FindEntityWithUID(int UID) const;

		inline void AddRawEntity(const Entity& ent) { this->entityPool.emplace_back(ent); }
		// the corresponding EntityConfig is default 
		void AddDefaultEntity(const Entity& ent);
		void AddEntityWithTransform(const Entity& ent, const MeshTransform& transform);
		
	private:
		std::vector<EntityConfig> renderables;
		std::vector<Entity> entityPool;
	};

}
