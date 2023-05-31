#pragma 

#include "eqxpch.h"

#include "Loader/Parsing.h"
#include "Renderer/Core/Renderer.h"
#include "Renderer/Landscape/Landscape.h"

namespace EQX {

    // parses into the binded entity; erases the original mesh info in it
    class ObjParser
    {
    public:
        ObjParser(Renderer& renderer, EntityInfo ent, std::string filename)
        {
            this->p_ent = &renderer._entityUnderScene(ent.parent, ent.entityID);
            this->filename = filename;
        }
        void ParseWith(MeshTransform&);
        void Parse();

    private:
        Entity* p_ent;
        std::string filename;
    };

}
