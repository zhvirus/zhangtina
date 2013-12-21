#ifndef __VTL_GEOMETRY_HPP__
#define __VTL_GEOMETRY_HPP__

/*************************************************************************\
Microsoft Research Asia
Copyright (c) 2001 Microsoft Corporation

Module Name:
Geometry related class and functions

Abstract:
A. Geometry helper functions
1. MixedProduct
2. TransposeInvert
3. BuildLeftHandNormal
4. BuildRightHandNormal
5. VectorAngleCosine

B. class CubicSpline represent Cubic curve with 3 control points.
Control points can be float, PointF, Vector3 etc.
1. P() for position
2. DP() for differential
3. SetControl() to modify control points position. However, at this 
function, the curve has not been realized
4. Build_Bezier() to realize the curve into a Bezier segment
5. Build_CatmullRom() to realize the curve into a Catmull Rom segment
6. Evaluate() to sample the curve into polygon lines
7. Length() to compute the length of the curve

C. class Qunternion for possible arcball, rotation, expotential map
1. An extension of VectorStatic<4>
2. Reset for identity rotation
3. Conjugate for negative rotation
4. Get rotation angle and axis
5. Set rotation by angle and axis
6. Set rotation by two vector, rotate from one to another
7. Slerp for spherical linear interpolation
8. Exp and Log for exponentail map

Notes:
TODO: finish Quaternion,
Rotation Matrix

Usage:

History:
Created  on 2003 Feb 14 by liyin@ust.hk
Modified on 2003 Apr 12 by liyin@ust.hk

\*************************************************************************/

#include <math.h>
#include "vtl_vector.h"
#include "vtl_matrix.h"

namespace vtl{

    const double PI = 3.14159265358979323846264338327950288L;
    const double RAD_TO_DEG = 180 / PI;
    const double DEG_TO_RAD = PI / 180;

    //////////////////////////////////////////////////////////////////
    // Geometry functions
    //////////////////////////////////////////////////////////////////

    // mixed triple product of three 3D vector, return scalar
    template <class T, class D>
        T MixedProduct ( const Vector3<T, D>& u, 
        const Vector3<T, D>& v, 
        const Vector3<T, D>& w)
    {
        Vector3<T, D> r;
        CrossProduct (r, v, w);
        return DotProduct (u, r);
    }

    // screw matrix from vector
    template<class T, class D>
        void TransposeInvert (Matrix3x3<T>, const Vector3<T, D> & v)
    {
        t.SetZero ();
        t[0][1] = -v[2];    t[1][0] =  v[2];
        t[0][2] =  v[1];    t[2][0] = -v[1];
        t[1][2] = -v[0];    t[2][1] =  v[0];
    }

    // Build the left hand normal vector from three 3D vector
    template <class T, class D>
        Vector3<T, D> BuildLeftHandNormal (
        const Vector3<T, D>& v0,
        const Vector3<T, D>& v1,
        const Vector3<T, D>& v2)
    {
        Vector3<T, D> dst;
        CrossProduct (dst, v1 - v0, v2 - v0);
        return dst;
    }

    // Build the right hand normal vector from three 3D vector
    template <class T, class D>
        Vector3<T, D> BuildRightHandNormal (
        const Vector3<T, D>& v0,
        const Vector3<T, D>& v1,
        const Vector3<T, D>& v2)
    {
        Vector3<T, D> dst;
        CrossProduct (dst, v2 - v0, v1 - v0);
        return dst;
    }

    // compute the radian angle of two vector, by nomalized dot product
    template <class T, class Core>
        T VectorAngleCosine (
        const Vector<T, Core>& a, 
        const Vector<T, Core>& b)
    {
        return DotProduct (a.Normalized (), b.Normalized ());
    }

    //////////////////////////////////////////////////////////////////
    // Quaternion, Exponetial mapping
    //////////////////////////////////////////////////////////////////

    template<class T, class V3 = Vector3<T> >
    class Quaternion
    {
    protected:
        T w;    // this is not angle
        V3 v;   // this is not axis, we use its x, y, z here

    public:
        // If you got a compile error at this line,
        //  you are using a unsupported V3 type.
        // you can either try Vector3<T>,
        //  or define x, y, z and constructor(_x, _y, _z)
        Quaternion() : w(1), v(0, 0, 0)  {}
        Quaternion(const T _w, const V3& _v) : w(_w), v(_v) {}

        // =============================================================
        // TODO: should support Matrix 3x3
        const Matrix4x4<T> GetMatrix() const;
        Quaternion& SetMatrix(const Matrix4x4<T>& m);

        friend const Quaternion Exp(const V3& vec);
        friend const V3 Log(const Quaternion& src);

        // =============================================================
        Quaternion&  Reset()
        {
            w = 1;
            v.Set(0, 0, 0);
            return *this;
        }

        void Conjugate()
        {
        	v = -v;
        }

        const Quaternion Conjugated()const
        { 
            return Quaternion (w, -v);
        }

        void Invert()
        {
            Conjugate();
            operator /= (SquareLength());
        }

        const Quaternion Inverted()const
        { 
            return Conjugated() / SquareLength();
        }

        T SquareLength() const
        {
            return w*w + v.SquareLength();
        }

        T Length() const
        {
            return static_cast<T>(sqrt(SquareLength()));
        }

        void Normalize()
        {
            operator /= (Length());
        }

        const Quaternion Normalized() const
        {
        	Quaternion qt = *this;
            qt.Normalize();
            return qt;
        }

        const V3 GetAxis(void) const
        { 
            T _sin = static_cast<T>(sin(acos(w))); 
            return vtl::IsZero(_sin) ? V3(1,0,0) : (v / _sin);
        }

        T GetAngle(void) const
        {
            return static_cast<T>(2 * acos(w));
        }

        T GetDegree(void) const
        {
            return GetAngle() * RAD_TO_DEG;
        }

        Quaternion& SetAxisAngle(const V3& axis, const T radian)
        {
            const T _sin= T(sin(radian * 0.5));
            const T _cos= T(cos(radian * 0.5));

            return SetQuaternion(_cos, axis * _sin);
        }

        Quaternion& SetTwoVector(const V3& from, const V3& to)
        {
            const V3 axis= from.Cross(to).Normalized();

            const T A = from.Dot(to);
            const T B = from.Length()*to.Length();

            if(vtl::IsZero(B)) return Reset();

            const T angle = T(acos(__min(1, __max(-1, A / B))));

            return SetAxisAngle(axis, angle);
        }

        Quaternion& SetQuaternion(const T _w, const V3& _v)
        {
            w = _w;
            v = _v;
            return *this;
        }

        const V3 Rotate(const V3 vec) const
        {
            Quaternion <T> qv(0, vec);
            Quaternion <T> qm = (*this) * qv * Inverted();      
            return qm.v;    	
        }

        const Quaternion Slerp(const Quaternion& to, T t)
        {
            return (*this) * Exp(t * Log(Inverted() * to)); 
        }

        friend std::ostream& operator << (std::ostream& os, Quaternion& qt)
        {
            os << "[" << qt.w << ", (" << qt.v << ")]";
            return os;
        }

        // Combine rotation of q1 and q2
        friend const Quaternion 
            operator * (const Quaternion& q1 ,const Quaternion& q2)
        {
            return Quaternion(q1.w*q2.w - q1.v.Dot(q2.v),
                q1.w * q2.v + q1.v * q2.w + q1.v.Cross(q2.v));
        }

        // eliminate rotation of q2 from q1
        friend const Quaternion 
            operator / (const Quaternion& q1 ,const Quaternion& q2)
        {
            return q1 * q2.Inversed();
        }

        Quaternion& operator *= (T scalar)
        {
            w *= scalar;
            v *= scalar;
            return *this;
        }

        Quaternion& operator /= (T scalar)
        {
            if (vtl::IsZero(scalar)) return Reset();
            else return operator *= (1 / scalar);
        }

        friend const Quaternion operator* (const Quaternion& qt ,T scalar)
        {
            Quaternion result = qt;
            return result *= scalar;
        }

        friend const Quaternion operator* (T scalar, const Quaternion& qt)
        {
            Quaternion result = qt;
            return result *= scalar;
        }

        friend const Quaternion operator/ (const Quaternion& qt ,T scalar)
        {
            Quaternion result = qt;
            return result *= (1 / scalar);
        }
    };

} // namespace vtl

#include "vtl_geometry.inl"

#endif//__VTL_GEOMETRY_HPP__