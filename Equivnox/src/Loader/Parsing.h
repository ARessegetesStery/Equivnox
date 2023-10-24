#pragma once

#include <CoreMinimal.h>

#include "Runtime/Geometry/Geometry.h"

namespace EQX {

	namespace EQX_PARSING {

        struct IndexVector3
        {
            XInt x;
            XInt y;
            XInt z;
            IndexVector3(XInt x, XInt y, XInt z) : x(x), y(y), z(z) {}
            IndexVector3() : x(0), y(0), z(0) {}

            IndexVector3 operator = (const IndexVector3& v)
            {
                IndexVector3 res(v.x, v.y, v.z);
                return res;
            }
            IndexVector3(const IndexVector3& v)
            {
                this->x = v.x;
                this->y = v.y;
                this->z = v.z;
            }
        };

        struct face
        {
            IndexVector3 f[4];
            face(IndexVector3 a, IndexVector3 b, IndexVector3 c, IndexVector3 d)
            {
                f[0] = a;
                f[1] = b;
                f[2] = c;
                f[3] = d;
            }
            face()
            {
                IndexVector3 zero(0, 0, 0);
                f[0] = IndexVector3(zero);
                f[1] = IndexVector3(zero);
                f[2] = IndexVector3(zero);
                f[3] = IndexVector3(zero);
            }
        };

	}

}
