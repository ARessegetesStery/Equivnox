#include "eqxpch.h"

#include "Line.h"

namespace EQX {

    Line::Line()
    {
        point = Vec3::Zero;
        direction = Vec3::UnitZ;
    }

    Line::Line(Vec3 point, Vec3 direction)
    {
        this->point = point;
        if (direction == Vec3::Zero)
            direction = Vec3::UnitZ;
        this->direction = direction.Normalize();
    }

    Line::Line(const Line& l)
    {
        point = l.point;
        direction = l.direction;
    }

    EQXBool Line::operator==(const Line& l) const
    {
        return (l.GetDirection() == direction) && 
            IsPointOnLine(this->point, l);
    }

    EQXBool IsPointOnLine(const Vec3& point, const Line& l)
    {
        Vec3 diff = point - l.GetPoint();
        EQXFloat diffLengthSquared = diff.NormSquare();
        EQXFloat innerprod = Dot(diff, l.GetDirection());
        EQXFloat innerprodSquared = innerprod * innerprod;
        if (abs(diffLengthSquared) == abs(innerprodSquared))
            return true;
        else
            return false;
    }

}
