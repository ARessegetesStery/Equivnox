#include "eqxpch.h"

#include "ObjParser.h"

#pragma warning(push)
#pragma warning(disable:4996)

namespace EQX {

    void ObjParser::Parse()
    {
        this->ParseWith(MeshTransform());
    }

    void ObjParser::ParseWith(MeshTransform& trans)
    {
        if (this->p_ent == nullptr)
            return;

        /*  Parsing  */
        std::ifstream infile;
        infile.open((filename + ".obj"), std::ios::in);

        p_ent->ClearMesh();

        if (!infile.is_open())
        {
#ifdef EQX_DEBUG
        cout << "Cannot open file \"" << filename << "\"" << endl;
#endif
            return;
        }
        
        XChar linebuf[1024];
        std::vector<Vector3> v;
        std::vector<Vector2> vt;
        std::vector<Vector3> vn;
        std::vector<EQX_PARSING::face> fs;
        XChar curObjName[128];
        Vector3 v3;
        Vector2 v2;
        v.push_back(v3);
        vt.push_back(v2);
        vn.push_back(v3);
        XBool isFirstObj = true;

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
                        p_ent->AddFaceToMesh({ {v[iter->f[0].x], v[iter->f[1].x], v[iter->f[2].x]},
                            {v[iter->f[2].x], v[iter->f[0].x], v[iter->f[3].x]} });
                    }
                    fs.clear();
                }
                if (std::sscanf(linebuf, "o %s", curObjName))
                {
                    if (curObjName[0] > 64 && curObjName[0] < 91)
                    {
                        curObjName[0] = curObjName[0] + 32;
                    }
                    for (XInt i = 0; i < strlen(curObjName); ++i)
                    {
                        XChar c = curObjName[i];
                        if (c < 48 || (c > 57 && c < 65) || (c > 90 && c < 97) || c > 122)
                        {
                            curObjName[i] = '_';
                        }
                    }
                }
            }
            else if (linebuf[0] == 'v' && linebuf[1] == ' ')
            {
                if (std::sscanf(linebuf, "v %f %f %f", &v3.x, &v3.y, &v3.z))
                    v.push_back(v3);
            }
            else if (linebuf[0] == 'v' && linebuf[1] == 't')
            {
                if (std::sscanf(linebuf, "vt %f %f", &v2.x, &v2.y))
                    vt.push_back(v2);
            }
            else if (linebuf[0] == 'v' && linebuf[1] == 'n')
            {
                if (std::sscanf(linebuf, "vn %f %f %f", &v3.x, &v3.y, &v3.z))
                    vn.push_back(v3);
            }
            else if (linebuf[0] == 'f' && linebuf[1] == ' ')
            {
                EQX_PARSING::face F;
                if (std::sscanf(linebuf, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                    &F.f[0].x, &F.f[0].y, &F.f[0].z,
                    &F.f[1].x, &F.f[1].y, &F.f[1].z,
                    &F.f[2].x, &F.f[2].y, &F.f[2].z,
                    &F.f[3].x, &F.f[3].y, &F.f[3].z))
                    fs.push_back(F);
            }
        }

        // writing to mesh
        for (auto iter = fs.begin(); iter != fs.cend(); ++iter)
        {
            p_ent->AddFaceToMesh(Vertex(v[iter->f[0].x], vn[iter->f[0].z], vt[iter->f[0].y]),
                Vertex(v[iter->f[1].x], vn[iter->f[1].z], vt[iter->f[1].y]), 
                Vertex(v[iter->f[2].x], vn[iter->f[2].z], vt[iter->f[2].y]));
            p_ent->AddFaceToMesh(Vertex(v[iter->f[2].x], vn[iter->f[2].z], vt[iter->f[2].y]),
                Vertex(v[iter->f[0].x], vn[iter->f[0].z], vt[iter->f[0].y]),
                Vertex(v[iter->f[3].x], vn[iter->f[3].z], vt[iter->f[3].y]));
        }

        p_ent->TransformMesh(trans);

        infile.close();
    }

}

#pragma warning(pop)
