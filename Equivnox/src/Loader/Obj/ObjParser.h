#pragma 

#include "eqxpch.h"

#include "Renderer/Shapes/Shapes.h"
#include "Math/MathHeader.h"

namespace EQX {

    class Parser
    {
    public:
        Parser(Mesh* m, std::string filename)
        {
            this->p_mesh = m;
            this->filename = filename;
        }
        void parse();

    private:
        Mesh* p_mesh;
        std::string filename;
    };

}
