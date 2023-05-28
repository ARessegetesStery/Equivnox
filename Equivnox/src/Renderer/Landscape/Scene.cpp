#include "eqxpch.h"

#include "Scene.h"

namespace EQX {

    // If there are lots of entities, maintain a binary tree
    const Entity& Scene::FindEntityWithUID(int UID) const
    {
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
        {
            if (iter->GetUID() == UID)
                return *iter;
        }
    }

    Entity& Scene::FindEntityWithUID(int UID)
    {
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
        {
            if (iter->GetUID() == UID)
                return *iter;
        }
    }

    void Scene::AddDefaultEntity(const Entity& ent)
    {
        int UID = ent.GetUID();
        this->entityPool.push_back(ent);
        this->renderables.push_back(EntityConfig(UID));
    }

    void Scene::AddEntityWithTransform(const Entity& ent, const MeshTransform& transform)
    {
        int UID = ent.GetUID();
        this->entityPool.push_back(ent);
        this->renderables.push_back(EntityConfig(UID, transform));
    }

}
