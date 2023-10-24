#pragma 

#include "CoreMinimal.h"

#include "Loader/Parsing.h"
#include "Runtime/Renderer/Renderer.h"
#include "Runtime/Landscape/Landscape.h"

namespace EQX {

    // parses into the binded entity; erases the original mesh info in it
    class ObjParser
    {
    public:
        ObjParser(Renderer& renderer, EntityInfo ent, EQXString filename)
        {
            if (ent.name == EntityConfig::s_defaultEntityName)
            {
#ifdef EQX_DEBUG
                Print("[ERROR] Invalid Entity to parse into!");
                Print("The EntityInfo passed in is:");
                EQX_LOG(ent)
#endif
                p_ent = nullptr;
            }
            else
                this->p_ent = &renderer._entityUnderScene(ent.parent, ent.entityID);
            this->filename = filename;
        }
        void ParseWith(MeshTransform&);
        void Parse();

    private:
        Entity* p_ent;
        EQXString filename;
    };

}
