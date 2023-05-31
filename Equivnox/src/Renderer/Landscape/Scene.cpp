#include "eqxpch.h"

#include "Scene.h"

namespace EQX {

    const std::string Scene::s_defaultSceneName = "__nil__";
    const std::string Scene::s_rendererSceneName = "__renderer__";

    // TODO Maintain searching structure
    const Entity& Scene::FindEntityWithUID(int UID) const
    {
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
        {
            if (iter->GetUID() == UID)
                return *iter;
        }
    }

    Scene::Scene(std::string str)
    {
        this->name = str;
        this->renderables = {};
        this->entityPool = {};
        this->AddDefaultEntity(Entity(), EntityConfig::s_defaultEntityName); // serve as a placeholder
    }

    Entity& Scene::FindEntityWithUID(int UID)
    {
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                return *iter;
        return *entityPool.begin();
    }

    EntityConfig& Scene::AddDefaultEntity(const Entity& ent, std::string name)
    {
        int UID = ent.GetUID();
        bool existDuplicate = false;
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                existDuplicate = true;
        if (!existDuplicate)
            this->entityPool.emplace_back(ent);
        this->renderables.push_back(EntityConfig(UID, name));
        return *(this->renderables.end() - 1);
    }

    EntityConfig& Scene::AddEntityWithTransform(const Entity& ent, std::string name, const MeshTransform& transform)
    {
        int UID = ent.GetUID();
        bool existDuplicate = false;
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                existDuplicate = true;
        if (!existDuplicate)
            this->entityPool.emplace_back(ent);
        this->renderables.push_back(EntityConfig(UID, name, transform));
        return *(this->renderables.end() - 1);
    }

    SceneInfo::SceneInfo(std::string name, SceneID ID) : name(name), sceneID(ID)  {  }

}
