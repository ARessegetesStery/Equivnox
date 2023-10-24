#include "eqxpch.h"

#include "Scene.h"

namespace EQX {

    const XString Scene::s_defaultSceneName = "__nil__";
    const XString Scene::s_rendererSceneName = "__renderer__";

    Scene::Scene(XString str)
    {
        this->name = str;
        this->renderables = {};
        this->entityPool = {};
        this->AddDefaultEntity(Entity(), EntityConfig::s_defaultEntityName); // serve as a placeholder
    }

    // TODO Maintain searching structure
    const Entity& Scene::FindEntityWithUID(XInt UID) const
    {
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                return *iter;
        return *entityPool.begin();
    }

    Entity& Scene::FindEntityWithUID(XInt UID)
    {
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                return *iter;
        return *entityPool.begin();
    }

    EntityConfig& Scene::AddDefaultEntity(const Entity& ent, XString name)
    {
        XInt UID = ent.GetUID();
        XBool existDuplicate = false;
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                existDuplicate = true;
        if (!existDuplicate)
            this->entityPool.emplace_back(ent);
        this->renderables.push_back(EntityConfig(UID, name));
        return *(this->renderables.end() - 1);
    }

    EntityConfig& Scene::AddEntityWithTransform(const Entity& ent, XString name, const MeshTransform& transform)
    {
        XInt UID = ent.GetUID();
        XBool existDuplicate = false;
        for (auto iter = entityPool.begin(); iter != entityPool.cend(); ++iter)
            if (iter->GetUID() == UID)
                existDuplicate = true;
        if (!existDuplicate)
            this->entityPool.emplace_back(ent);
        this->renderables.push_back(EntityConfig(UID, name, transform));
        return *(this->renderables.end() - 1);
    }

    SceneInfo::SceneInfo(XString name, SceneID ID) : name(name), sceneID(ID)  {  }

}
