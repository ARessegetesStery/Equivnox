#pragma 

#include "eqxpch.h"

#include "Loader/Parsing.h"

namespace EQX {

    class ObjParser
    {
    public:
        ObjParser(Mesh* m, std::string filename)
        {
            this->p_mesh = m;
            this->filename = filename;
        }
        void Parse();

    private:
        Mesh* p_mesh;
        std::string filename;
    };

}
