#include "eqxpch.h"

#include "Scene.h"

namespace EQX {

    // If there are lots of entities, maintain a binary tree
    const Entity& Scene::FindUID(int UID) const
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

    FScene::FScene(const Scene& scene)
    {
        for (auto config = scene.Renderables().begin();
            config != scene.Renderables().cend(); ++config)
        {
            Mesh m = scene.FindUID(config->GetUID()).GetMesh();
            m.Transform(config->GetTransform());
            this->entities.emplace_back(m);
        }
    }

    void FScene::LoadScene(const Scene& scene)
    {
        this->entities.clear();
        if (scene.Renderables().empty() == true)
            return;
        for (auto config = scene.Renderables().begin(); 
            config != scene.Renderables().cend(); ++config)
        {
            Mesh m = scene.FindUID(config->GetUID()).GetMesh();
            m.Transform(config->GetTransform());
            this->entities.emplace_back(m);
        }
    }

}
