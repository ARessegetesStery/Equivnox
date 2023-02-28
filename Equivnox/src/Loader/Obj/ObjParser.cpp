#include "eqxpch.h"

#include "ObjParser.h"

namespace EQX {

    void ObjParser::Parse()
    {
    std::ifstream infile;
    infile.open((filename + ".obj"), std::ios::in);

    if (!infile.is_open())
    {
#ifdef EQX_DEBUG
        cout << "Cannot open file \"" << filename << "\"" << endl;
#endif
        return;
    }
    
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
                    p_mesh->AddFace({ {v[iter->f[0].x], v[iter->f[1].x], v[iter->f[2].x]},
                        {v[iter->f[2].x], v[iter->f[0].x], v[iter->f[3].x]} });
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
            EQX_PARSING::face F;
            std::sscanf(linebuf, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                &F.f[0].x, &F.f[0].y, &F.f[0].z,
                &F.f[1].x, &F.f[1].y, &F.f[1].z,
                &F.f[2].x, &F.f[2].y, &F.f[2].z,
                &F.f[3].x, &F.f[3].y, &F.f[3].z);
            fs.push_back(F);
        }
    }

    // writing to mesh
    for (auto iter = fs.begin(); iter != fs.cend(); ++iter)
    {
        p_mesh->AddFace(Vertex(v[iter->f[0].x], vn[iter->f[0].z], vt[iter->f[0].y]),
            Vertex(v[iter->f[1].x], vn[iter->f[1].z], vt[iter->f[1].y]), 
            Vertex(v[iter->f[2].x], vn[iter->f[2].z], vt[iter->f[2].y]));
        p_mesh->AddFace(Vertex(v[iter->f[2].x], vn[iter->f[2].z], vt[iter->f[2].y]),
            Vertex(v[iter->f[0].x], vn[iter->f[0].z], vt[iter->f[0].y]),
            Vertex(v[iter->f[3].x], vn[iter->f[3].z], vt[iter->f[3].y]));
    }

    infile.close();
}

}