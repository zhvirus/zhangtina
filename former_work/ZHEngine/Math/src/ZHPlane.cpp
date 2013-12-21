
#ifndef _ZH_MATH_DLL_
#define _ZH_MATH_DLL_
#endif


#include "ZHPlane.h"


namespace ZH {
    //-----------------------------------------------------------------------
    Plane::Plane ()
    {
        normal = Vector3::ZERO;
        d = 0.0;
    }
    //-----------------------------------------------------------------------
    Plane::Plane (const Plane& rhs)
    {
        normal = rhs.normal;
        d = rhs.d;
    }
    //-----------------------------------------------------------------------
    Plane::Plane (const Vector3& rkNormal, Real fConstant)
    {
        normal = rkNormal;
        d = -fConstant;
    }
    //-----------------------------------------------------------------------
    Plane::Plane (const Vector3& rkNormal, const Vector3& rkPoint)
    {
        normal = rkNormal;
        d = -rkNormal.dotProduct(rkPoint);
    }
    //-----------------------------------------------------------------------
    Plane::Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
        const Vector3& rkPoint2)
    {
        redefine(rkPoint0, rkPoint1, rkPoint2);
    }
    //-----------------------------------------------------------------------
    Real Plane::getDistance (const Vector3& rkPoint) const
    {
        return normal.dotProduct(rkPoint) + d;
    }
    //-----------------------------------------------------------------------
    Plane::Side Plane::getSide (const Vector3& rkPoint) const
    {
        Real fDistance = getDistance(rkPoint);

        if ( fDistance < 0.0 )
            return Plane::NEGATIVE_SIDE;

        if ( fDistance > 0.0 )
            return Plane::POSITIVE_SIDE;

        return Plane::NO_SIDE;
    }
    //-----------------------------------------------------------------------
    void Plane::redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
        const Vector3& rkPoint2)
    {
        Vector3 kEdge1 = rkPoint1 - rkPoint0;
        Vector3 kEdge2 = rkPoint2 - rkPoint0;
        normal = kEdge1.crossProduct(kEdge2);
        normal.normalise();
        d = -normal.dotProduct(rkPoint0);
    }
    //-----------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& o, Plane& p)
    {
        o << "Plane(normal=" << p.normal << ", d=" << p.d << ")";
        return o;
    }
} 