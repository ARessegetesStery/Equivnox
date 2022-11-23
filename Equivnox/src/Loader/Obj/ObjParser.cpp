#include "eqxpch.h"

#include "ObjParser.h"

namespace EQX {

    namespace EQX_PARSING {

        struct face
        {
            Vector3 f[4];
            face(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
            {
                f[0] = a;
                f[1] = b;
                f[2] = c;
                f[3] = d;
            }
            face()
            {
                Vector3 zero(0, 0, 0);
                f[0] = Vector3(zero);
                f[1] = Vector3(zero);
                f[2] = Vector3(zero);
                f[3] = Vector3(zero);
            }
        };

    }


// TODO test
void Parser::parse()
{
    std::ifstream infile;
    infile.open((filename + ".obj"), std::ios::in);
    char linebuf[1024];
    std::vector<Vector3> v;
    std::vector<Vector2> vt;
    std::vector<Vector3> vn;
    std::vector<EQX_PARSING::face> fs;
    char curObjName[128];
    Vector3 v3;
    Vector2 v2;
    v.push_back(v3);
    vt.push_back(v2);
    vn.push_back(v3);
    bool isFirstObj = true;

    while (infile.getline(linebuf, 1024, '\n'))
    {
        // std::cout << linebuf << std::endl;
        if (linebuf[0] == '#')
            continue;
        else if (linebuf[0] == 'o' && linebuf[1] == ' ')
        {
            if (isFirstObj == true)
                isFirstObj = false;
            else
            {
                for (auto iter = fs.begin(); iter != fs.cend(); ++iter)
                {
                    p_mesh->addLine({ LineSeg(iter->f[0], iter->f[1]),
                        LineSeg(iter->f[0], iter->f[2]),
                        LineSeg(iter->f[2], iter->f[1]),
                        LineSeg(iter->f[0], iter->f[3]),
                        LineSeg(iter->f[2], iter->f[3]) });
                }
                fs.clear();
            }
            std::sscanf(linebuf, "o %s", curObjName);
            if (curObjName[0] > 64 && curObjName[0] < 91)
            {
                curObjName[0] = curObjName[0] + 32;
            }
            for (int i = 0; i < strlen(curObjName); ++i)
            {
                char c = curObjName[i];
                if (c < 48 || (c > 57 && c < 65) || (c > 90 && c < 97) || c > 122)
                {
                    curObjName[i] = '_';
                }
            }
        }
        else if (linebuf[0] == 'v' && linebuf[1] == ' ')
        {
            std::sscanf(linebuf, "v %f %f %f", &v3.x, &v3.y, &v3.z);
            v.push_back(v3);
        }
        else if (linebuf[0] == 'v' && linebuf[1] == 't')
        {
            std::sscanf(linebuf, "vt %f %f", &v2.x, &v2.y);
            vt.push_back(v2);
        }
        else if (linebuf[0] == 'v' && linebuf[1] == 'n')
        {
            std::sscanf(linebuf, "vn %f %f %f", &v3.x, &v3.y, &v3.z);
            vn.push_back(v3);
        }
        else if (linebuf[0] == 'f' && linebuf[1] == ' ')
        {
            std::cout << linebuf << std::endl;
            EQX_PARSING::face F;
            std::sscanf(linebuf, "f %f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f",
                &F.f[0].x, &F.f[0].y, &F.f[0].z,
                &F.f[1].x, &F.f[1].y, &F.f[1].z,
                &F.f[2].x, &F.f[2].y, &F.f[2].z,
                &F.f[3].x, &F.f[3].y, &F.f[3].z);
            fs.push_back(F);
        }
    }
    for (auto iter = fs.begin(); iter != fs.cend(); ++iter)
    {
        p_mesh->addLine({ LineSeg(iter->f[0], iter->f[1]),
            LineSeg(iter->f[0], iter->f[2]),
            LineSeg(iter->f[2], iter->f[1]),
            LineSeg(iter->f[0], iter->f[3]),
            LineSeg(iter->f[2], iter->f[3]) });
    }

    

    infile.close();
}

}