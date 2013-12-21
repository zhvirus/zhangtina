#ifndef __VTL_VECTOR_HPP__
#define __VTL_VECTOR_HPP__


/*************************************************************************\
Microsoft Research Asia
Copyright (c) 2001 Microsoft Corporation

Module Name:
  VTL Vector defination and implementation
  
Abstract:
    1. Vector can be accessed as vec[k].
    2. A number of geometry function is defined on vector
    3. All C++ operators defined on Vecter are applied on each element
    4. VectorStatic is simple copied type, as an extension of C type array.
       It can be allocated in stack. 
    5. VectorDynamic is complex copied type, as an extension of std::vector
       with geometry and numerical operations
    
Notes:
    1. VectorTraitStatic can be implicitly converted into T*, or const T*.
       However, VectorTraitDynamic must explicitly converted by function ptr().
    2. begin() end() iterator are defined to access vector as 1D iteratable.
    3. iterator can be used on many stl functions, like copy, fill, find
     
Usage:
    1. Ignore VectorTraits, VectorDescription.
    2. Use vectorStatic<n, T> and vectorDynamic<T>
       VectorDynamic<T> vec; must use resize() to allocate memory
    3. Use Vector2<> Vector3<> Vector4<> for fixed size operation
        
History:
    Created  on 2003 Feb 14 by liyin@ust.hk
    Modified on 2003 Apr 16 by liyin@ust.hk
          
\*************************************************************************/

#include <iostream>
#include <vector>
#include <numeric>

#include "vtl_platform.h"

namespace vtl{
    
    //////////////////////////////////////////////////////////////////
    ///  vtl::vector<sizetype DIM, class T>
    ///  Usage:  use it as a static size array
    ///      It uses continuous storage
    ///      It requires DIM * sizeof (T) byte memory size
    ///      It can be used as a pointer of T
    //////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////
    /// Constructor for assignment from ParentType
    /// It's useful for operators as implicit convertion of return value
    /// Therefore, it cannot be explicit.
#define CONSTRUCTOR_FROM_PARENT(ClassName, ParentType)                  \
    vtl_inline ClassName (const ParentType& v) : ParentType (v) {}
    
    //////////////////////////////////////////////////////////////////
    /// Constructor to fill scalar type:  Vector v = 6, Vector v(float(4))
#define FILL_CONSTRUCTOR(ClassName, ScalarType)                         \
    vtl_inline explicit ClassName(const ScalarType& c) { Fill(c);}

    //////////////////////////////////////////////////////////////////
    /// define macro constructor for vtl::Vector and vtl::Matrix
#define CONSTRUCTOR_VECTOR_DEFAULT(ClassName, Type)     \
    vtl_inline ClassName() {}                           \
    FILL_CONSTRUCTOR(ClassName, Type)

    //////////////////////////////////////////////////////////////////
    /// Vector Traits
    ///     magic traits, compatibal with both static and dynamic
    ///     vector defination.  Oliver Liyin by May, 29, 2003
    /// a vector core must define:
    ///      1. memory function, resize and clear
    ///      2. define a member access function, named at(int k)
    ///      3. define size() return number of elements
    ///      4. define a bool function empty()
    /// It must manage the memery by it's self
    /// These interfaces must be compatible to std::vector
    ///////////////////////////////////////////////////////////////////
    

    /// define static vector size
#define VECTOR_TRAITS_STATIC_SIZE(N)                                          \
        vtl_inline void resize (sizetype _size) const { assert(N == _size);}  \
        vtl_inline void clear () const {}                                     \
        vtl_inline sizetype size() const { return N;}                         \
        vtl_inline bool empty () const { return false;}                       \
        enum {NDIM = N};


    /// access 1D vector using at(k) and ptr()
#define VECTOR_TRAITS_ACCESS_DATA(T, Ptr)                                     \
        vtl_inline T & at (sizetype k)                                        \
            { assert(k < size()); return (Ptr)[k];}                           \
        vtl_inline const T & at (sizetype k) const                            \
            { assert(k < size()); return (Ptr)[k];}                           \

    //////////////////////////////////////////////////////////////////
    ///  It's used as static vector template parameter
    ///  User can define his/her own VectorDescriptor
    //////////////////////////////////////////////////////////////////
    template <sizetype NDIM, typename T>
        struct VectorTraitsStatic
    {
    public:
        VECTOR_TRAITS_STATIC_SIZE(NDIM);
        VECTOR_TRAITS_ACCESS_DATA(T, m_pData);

    protected: 
        T m_pData[NDIM];
    };

    ///////////////////////////////////////////////////////////////////
    /// VectorTraitsDynamic is the basic of all dynamic vector
    /// it's designed for dynamically resizable vector
    /// It's size is run-time changable
    /// It is inherited from std::vector, while can be 
    /// used in math and geometry functions
    ///
    template <typename T> 
        class VectorTraitsDynamic
    {
    public:
        VECTOR_TRAITS_ACCESS_DATA(T, m_rgData);

        vtl_inline void resize (sizetype _size) { m_rgData.resize(_size);}
        vtl_inline void clear () { m_rgData.clear ();}

        vtl_inline sizetype size() const { return m_rgData.size ();}
        vtl_inline bool empty () const { return m_rgData.empty();}

    private:
        std::vector<T> m_rgData;
    };
    
    //////////////////////////////////////////////////////////////////
    /// class vtl::Vector
    /// std::vector for geometry and math
    /// VectorTraits is either VectorTraitsStatic or VectorTraitsDynamic
    //////////////////////////////////////////////////////////////////
    template <class T, class VectorTraits>
        class Vector : public VectorTraits
    {
    public:
        CONSTRUCTOR_VECTOR_DEFAULT(Vector, T)
        CONSTRUCTOR_FROM_PARENT(Vector, VectorTraits)

    public:
        /// access as 1D pointer, vector[k], (T*)
        /// therefore, we do not need operator []
        /// T & operator[] (sizetype k) { return at(k);}
        /// const T & operator[] (sizetype k) const { return at(k);}
        vtl_inline operator T* () { return &at(0);}
        vtl_inline operator const T* () const { return &at(0);}

        ///////////////////////////////////////////////////////////////
        /// ge the pointer of the head of vector
        vtl_inline T * ptr () { return &at(0);}
        vtl_inline const T* ptr () const { return &at(0);}

        //////////////////////////////////////////////////////////////////////
        /// Iterator for STL compatibility, ref std::algorithm
        typedef T* iterator;
        typedef const T* const_iterator;
    
        vtl_inline iterator begin()    { return &at(0);}
        vtl_inline iterator end()      { return &at(0)+size();}
    
        vtl_inline const_iterator begin()  const { return &at(0);}
        vtl_inline const_iterator end()    const { return &at(0) + size();}

        vtl_inline T& front ()  { return at(0);}
        vtl_inline const T& front () const { return at(0);}

        vtl_inline T& back ()  { return at(size()-1);}
        vtl_inline const T& back () const { return at(size()-1);}

        //////////////////////////////////////////////////////////////////
        ///  Helper functions other than std::vector provides
    public:
        /// copy from other type vector, one by one
        /// InVector can be any std::vector compatible vector
        template <class InVector>
            Vector& CopyFrom(const InVector& src)
        {
            resize(src.size ());
            InVector::const_iterator is = src.begin();
            for(iterator it = begin(); it != end(); ++it, ++is)
            {
                (*it) = static_cast<T>(*is);
            }
            return *this;
        }

        /// fill in a value to vector
        Vector& Fill(const T& value)
        {
            if(!empty ())
            {
                std::fill(begin(), end(), value);
            }
            return *this;
        }

        /// quick fill zero to vector
        Vector& SetZero()
        {
            if(!empty ())
            {
                ::memset(ptr(), 0, size() * sizeof(T));
            }
            return *this;
        }
    
        ///check if *this is a zero vector
        bool IsZero(T threshold = std::numeric_limits<T>::epsilon()) const
        {
            for (sizetype i = 0; i < size(); i++)
            {
                /// using bounding box
                if(!vtl::IsZero(at(i), threshold)) return false;
            }
            return true;
        }
    
        bool EqualsTo(const Vector& v, 
            T threshold = std::numeric_limits<T>::epsilon()) const
        {
            if(size () != v.size ()) return false;
            

            for (sizetype i = 0; i < size(); i++)
            {
                /// using bounding box
                if(!vtl::IsZero(at(i) - v[i], threshold)) return false;
            }
            return true;
        }    
           
        /////////////////////////////////////////////////////////////////////
        /// geometry helper functions
        ////////////////////////////////////////////////////////////////////

        /// dot product, v.dot(u)
        template<typename _T, typename _Traits>
            vtl_inline const T Dot (const Vector<_T, _Traits>& u) const
        {
            return vtl::DotProduct (*this, u);
        }
        
        /// return a cross product, only works for size()==3
        template<typename _T, typename _Traits>
            vtl_inline const Vector Cross(const Vector<_T, _Traits>& u) const
        {
            Vector r;
            vtl::CrossProduct (r, *this, u);
            return r;
        }

        /// vector square length, dot(v, v)
        vtl_inline const T SquareLength() const
        {
            return vtl::SquareLength (*this);
        }
        
        /// vector length, sqrt(dot(v,v))
        vtl_inline const T Length() const
        {
            return vtl::Length(*this);
        }
        
        /// set the vector length
        const Vector ApplyLength (T newLength)  const
        {
            const T len = Length ();
            if (vtl::IsZero (len))
            {
                return *this;   /// return the zero lenth vector
            }
            else
            {
                return *this * newLength / len;
            }
        }
        
        /// vector normalize, v /= v.Length
        vtl_inline void Normalize()
        {
            vtl::Normalize (*this);
        }
        
        /// vector normalize, const this
        vtl_inline const Vector Normalized() const
        {
            Vector v = *this;
            vtl::Normalize (v);
            return v;
        }
        
        /// Sum of items
        vtl_inline const T Sum () const
        {
            return vtl::Sum (*this);
        }
        
        /// make the vector sum to one
        vtl_inline Vector& SumToOne ()
        {
            return vtl::SumToOne (*this);
        }
        
        /// Lerp, alpha=0  =>  return *this; alpha=1 => return v;
        vtl_inline const Vector Lerp (const Vector& u, T alpha) const
        {   
            return vtl::Lerp(*this, u, alpha);
        }

        ///  Clamp this vector (*this) within the bouding-box (vMin, vMax)
        Vector& Clamp (const Vector& vMin, const Vector& vMax)
        { 
            assert (size () == vMin.size ());
            assert (size () == vMax.size ());
            for (sizetype i = 0; i < size(); i++)     
            {
                if  (at(i) < vMin[i])   at(i) = vMin[i];
                if  (at(i) > vMax[i])   at(i) = vMax[i];
            }
            return *this;
        }

        /////////////////////////////////////////////////////////////
        /// Advanced geometry operation

        ///  Projecttion on a normalized axis vector
        vtl_inline const Vector ProjectTo (const Vector& axis) const
        {
            return axis.ApplyLength (Dot (axis));
        }
        
        ///  Projecttion on a normalized axis vector
        vtl_inline const Vector ProjectTo_Remain (const Vector& axis) const
        {
            return  *this - ProjectTo (axis);
        }

        /////////////////////////////////////////////////////////////
        /// Homogenous coordinate related

        /// assign one dimention lower vector, *this = [v, 1]
        template<class InVector>
            Vector& assignPoint(const InVector& v)
        {
            resize(v.size () + 1);
            for(sizetype k = 0; k < size () - 1; k ++)
            {
                at (k) = v[k];
            }
            at (k) = 1;
            return *this;
        }
        
        /// assign one dimention lower vector, *this = [v, 0]
        template<class InVector>
            Vector& assignVector(const InVector& v)
        {
            resize(v.size () + 1);
            for(sizetype k = 0; k < size () - 1; k ++)
            {
                at (k) = v[k];
            }
            at (k) = 0;
            return *this;
        }
        
        Vector& Homogenize ()
        {
            T & denorminator = at(size() - 1);
            if(denorminator != 0)
            {
                for(sizetype k = 0; k < size() - 1; k ++)
                {
                    at (k) /= denorminator;
                }
                denorminator = 1;
            }
            return *this;
        }

        /// assign one dimention higher vector, [*this, 1] = v
        template<class InVector>
            Vector& assignHomogenized (const InVector& v)
        {
            resize(v.size () - 1);

            InVector dst = v;
            dst.Homogenize ();

            for(sizetype k = 0; k < size (); k ++)
            {
                at (k) = dst[k];
            }
            return *this;
        }

        //////////////////////////////////////////////////////////////////////
        /// IO stream functions
        /// there is no allocation for input and no size output. 
        /// assume caller do it
        //////////////////////////////////////////////////////////////////////
        friend std::istream& operator >> (std::istream& is, Vector& v)
        {
            for (sizetype i = 0; i < v.size (); i ++)
            {
                is >> v[i];
            }
            return is;
        }

        friend std::ostream& operator << (std::ostream& os, const Vector& v)
        {
            for (sizetype i = 0; i < v.size (); i ++)
            {
                os << v[i] << " ";
            }
            return os;
        }
    };

    //////////////////////////////////////////////////////////////////////
    // Operator Functions
    //////////////////////////////////////////////////////////////////////


    // TODO: make the operator faster for Vector Static
    // 
    // TODO: V& OP (V&, const V&)             // for V += V
    // TODO: const V OP(const V&, const V&)   // for V + V
    // TODO: bool OP(const V&, const V&)      // For V < V
    // TODO: != and ==                        // for V == V
    // TODO: V& OP (V&, scalar) for +=        // for V += scalar
    // TODO: const V OP (scalar, const V&)    // for scalar + V
    // TODO: const V OP (const V&, scalar)    // for V + scalar
    // TODO: const V OP(const V&) for -       // for -V
    // TODO: void OP(V&) for ++               // for ++ V
    // TODO: const V OP(const V&)(int)        // for V ++
    //
    // 

    // -----Vector .vs. Vector -----------------------------
    // ?= operator

    #define Vector_OPU_Vector(OPU)                                  \
        template<class TD, class TS, class CoreD, class CoreS>      \
        Vector<TD, CoreD>& operator OPU ( Vector<TD, CoreD> & dst,  \
            const Vector<TS, CoreS> & src)                          \
    {                                                               \
        assert(dst.size () == src.size ());                         \
        Vector<TD, CoreD>::iterator id = dst.begin();               \
        Vector<TS, CoreS>::const_iterator is = src.begin();         \
        for(; id != dst.end(); ++id, ++is)                          \
        {                                                           \
            (*id) OPU (*is);                                        \
        }                                                           \
        return dst;                                                 \
    }

    Vector_OPU_Vector( += )
    Vector_OPU_Vector( -= )
    Vector_OPU_Vector( *= )
    Vector_OPU_Vector( /= )
    Vector_OPU_Vector( &= )
    Vector_OPU_Vector( |= )
    Vector_OPU_Vector( ^= )
    Vector_OPU_Vector(>>= )
    Vector_OPU_Vector(<<= )

    // Binocular operation
    #define Vector_OPB_Vector(OPB, OPU)                         \
        template<class TL, class TR, class CoreL, class CoreR>  \
        const Vector<TL, CoreL> operator OPB (                  \
            const Vector<TL, CoreL>& left,                      \
            const Vector<TR, CoreR>& right)                     \
    {                                                           \
        Vector<TL, CoreL> dst (left);                           \
        dst OPU right;                                          \
        return dst;                                             \
    }

    Vector_OPB_Vector( +, += )
    Vector_OPB_Vector( -, -= )
    Vector_OPB_Vector( *, *= )
    Vector_OPB_Vector( /, /= )
    Vector_OPB_Vector( &, &= )
    Vector_OPB_Vector( |, |= )
    Vector_OPB_Vector( ^, ^= )
    Vector_OPB_Vector(<<,<<= )
    Vector_OPB_Vector(>>,>>= )


    // Bool operator, lexicographical_compare
    #define Vector_LESS_Vector(OP, OPS, EQUAL)                  \
        template<class TL, class TR, class CoreL, class CoreR>  \
        bool operator OP ( const Vector<TL, CoreL> & left,      \
                           const Vector<TR, CoreR> & right)     \
    {                                                           \
        Vector<TL, CoreL>::const_iterator il = left.begin ();   \
        Vector<TR, CoreR>::const_iterator ir = right.begin ();  \
        for(; il != left.end() && ir != right.end(); ++il, ++ir)\
        {                                                       \
            if((*il) OPS (*ir)) return true;                    \
            if((*ir) OPS (*il)) return false;                   \
        }                                                       \
        if(left.size () OPS right.size()) return true;          \
        if(right.size () OPS left.size()) return false;         \
        return EQUAL;                                           \
    }

    Vector_LESS_Vector( >  , > , false)
    Vector_LESS_Vector( <  , < , false)
    Vector_LESS_Vector( >= , > , true)
    Vector_LESS_Vector( <= , < , true)

    template<class TL, class TR, class CoreL, class CoreR>
        bool operator == (const Vector<TL, CoreL> & left,
                          const Vector<TR, CoreR> & right)
    {
        Vector<TL, CoreL>::const_iterator il = left.begin ();
        Vector<TR, CoreR>::const_iterator ir = right.begin ();
        for(; il != left.end() && ir != right.end(); ++il, ++ir)
        {
            if((*il) != (*ir)) return false;
        }
        return (left.size () == right.size ());
    }

    template<class TL, class TR, class CoreL, class CoreR>
        bool operator != (const Vector<TL, CoreL> & left,
                          const Vector<TR, CoreR> & right)
    {
        return !(left == right);
    }

    // ----- Vector .vs. Scalar ---------------------------
    #define Vector_OPE_Scalar_ST(OP, ST)                \
        template<class T, class Core>                   \
        Vector<T, Core>& operator OP (                  \
            Vector<T, Core> & dst, const ST scalar)     \
    {                                                   \
        Vector<T, Core>::iterator id = dst.begin ();    \
        for(; id != dst.end (); ++id)                   \
        {                                               \
            (*id) OP static_cast<T>(scalar);            \
        }                                               \
        return dst;                                     \
    }

    #define Vector_OPE_Scalar(OP)               \
        Vector_OPE_Scalar_ST(OP, long double)   \
        Vector_OPE_Scalar_ST(OP, double)        \
        Vector_OPE_Scalar_ST(OP, float)         \
        Vector_OPE_Scalar_ST(OP, int)           \
        Vector_OPE_Scalar_ST(OP, unsigned int)

    Vector_OPE_Scalar( += )
    Vector_OPE_Scalar( -= )
    Vector_OPE_Scalar( *= )
    Vector_OPE_Scalar( /= )
    Vector_OPE_Scalar( &= )
    Vector_OPE_Scalar( |= )
    Vector_OPE_Scalar( ^= )

    #define Scalar_OP_Vector_ST(OP, ST)                     \
        template<class T, class Core>                       \
        const Vector<T, Core> operator OP (                 \
            const ST scalar, const Vector<T, Core> & src)   \
    {                                                       \
        Vector<T, Core> dst;                                \
        dst.resize (src.size ());                           \
        Vector<T, Core>::iterator id = dst.begin ();        \
        Vector<T, Core>::const_iterator is = src.begin ();  \
        for(; id != dst.end(); ++id, ++is)                  \
        {                                                   \
            (*id) =  static_cast<T>(scalar OP (*is));       \
        }                                                   \
        return dst;                                         \
    }

    #define Scalar_OP_Vector(OP)                \
        Scalar_OP_Vector_ST(OP, long double)    \
        Scalar_OP_Vector_ST(OP, double)         \
        Scalar_OP_Vector_ST(OP, float)          \
        Scalar_OP_Vector_ST(OP, int)            \
        Scalar_OP_Vector_ST(OP, unsigned int)

    Scalar_OP_Vector( + )
    Scalar_OP_Vector( - )
    Scalar_OP_Vector( * )
    Scalar_OP_Vector( / )
    Scalar_OP_Vector( & )
    Scalar_OP_Vector( | )
    Scalar_OP_Vector( ^ )

    #define Vector_OP_Scalar_ST(OP, ST)                     \
        template<class T, class Core>                       \
        const Vector<T, Core> operator OP (                 \
            const Vector<T, Core> & src, const ST scalar)   \
    {                                                       \
        Vector<T, Core> dst;                                \
        dst.resize (src.size ());                           \
        Vector<T, Core>::iterator id = dst.begin ();        \
        Vector<T, Core>::const_iterator is = src.begin ();  \
        for(; id != dst.end(); ++id, ++is)                  \
        {                                                   \
            (*id) =  static_cast<T>((*is) OP scalar);       \
        }                                                   \
        return dst;                                         \
    }
    
    #define Vector_OP_Scalar(OP)                \
        Vector_OP_Scalar_ST(OP, long double)    \
        Vector_OP_Scalar_ST(OP, double)         \
        Vector_OP_Scalar_ST(OP, float)          \
        Vector_OP_Scalar_ST(OP, int)            \
        Vector_OP_Scalar_ST(OP, unsigned int)

    Vector_OP_Scalar( + )
    Vector_OP_Scalar( - )
    Vector_OP_Scalar( * )
    Vector_OP_Scalar( / )
    Vector_OP_Scalar( & )
    Vector_OP_Scalar( | )
    Vector_OP_Scalar( ^ )

    // -------  Vector Self --------------------------
    // Single prefix Operand operator
    #define Vector_OPS_Vector(OP)                       \
        template<class T, class Core>                   \
        const Vector<T, Core> operator OP (             \
            const Vector<T, Core> & src)                \
    {                                                   \
        Vector<T, Core> dst = src;                      \
        Vector<T, Core>::iterator id = dst.begin ();    \
        for(; id != dst.end(); ++id)                    \
        {                                               \
            (*id) = OP (*id);                           \
        }                                               \
        return dst;                                     \
    }

    Vector_OPS_Vector( - )
    Vector_OPS_Vector( ! )
    Vector_OPS_Vector( ~ )

    #define Vector_OPSS_Vector(OP)                      \
        template<class T, class Core>                   \
        void operator OP (Vector<T, Core> & dst)        \
    {                                                   \
        Vector<T, Core>::iterator id = dst.begin ();    \
        for(; id != dst.end(); ++id)                    \
        {                                               \
            OP (*id);                                   \
        }                                               \
    }

    Vector_OPSS_Vector( ++ )
    Vector_OPSS_Vector( -- )

    // no return value

    //////////////////////////////////////////////////////////////////////
    // Helper Functions
    //////////////////////////////////////////////////////////////////////

    /// Linear interpolation, return u*(1-alpha) + v*alpha 
    /// alpha=0  =>  return u; alpha=1 => return v;
    template <class U, class V, class CoreU, class CoreV, class W>
        const Vector<U, CoreU> Lerp (
            const Vector<U, CoreU>& u, const Vector<V, CoreV>& v, W alpha)
    {   
        if(EqualsTo(alpha, static_cast<W>(0)))
            return Vector<U, CoreU>(v);
        else if(EqualsTo(alpha, static_cast<W>(1)))
            return u;
        else
            return (u - v) * static_cast<U>(alpha) + v;
    }

    /// square of Euclidean length
    template <class T, class Core>
        const T SquareLength (const Vector<T, Core> & v)
    {
        return DotProduct (v, v);
    }

    /// Euclidean length
    template <class T, class Core>
        const T Length (const Vector<T, Core> & v)
    {
        return static_cast<T>(sqrt(SquareLength (v)));
    }

    /// normalize vector v so that length = 1
    template <class T, class Core>
        Vector<T, Core>& Normalize (Vector<T, Core> & v)
    {
        T mag = Length (v);
        IsZero(mag) ? v.SetZero () : (v /= mag) ;
        return v;
    }

    /// sum of the vector
    template <class T, class Core>
        const T AbsSum (const Vector<T, Core> & v)
    {
        T sum = 0;
        Vector<T, Core>::const_iterator it;
        for( it = v.begin(); it != v.end(); ++it)
        {
            sum += abs(*it);
        }
        return sum;
    }

    /// sum of the vector
    template <class T, class Core>
        const T Sum (const Vector<T, Core> & v)
    {
        return std::accumulate(v.begin(), v.end(), T(0));
    }

    /// normalize vector v so that sum = 1
    template <class T, class Core>
        Vector<T, Core>& SumToOne (Vector<T, Core>& v)
    {
        if(v.size() > 0)
        {
            const T sum = Sum (v);
            IsZero(sum) ? (v /= sum) : 
                v.Fill (static_cast<T>(1.0 / v.size ()));
        }
        return v;
    }

    /// dot product of two vector, return scalar
    template <class U, class V, class CoreU, class CoreV>
        const U DotProduct(const Vector<U, CoreU>& u, const Vector<V, CoreV>& v)
    {
        return std::inner_product(u.begin(), u.end(), v.begin(), U(0));
    }

    /// cross product of two 3D vector, return 3D vector
    template <class R, class U, class V, class CoreR, class CoreU, class CoreV>
    void CrossProduct (Vector<R, CoreR>& r, 
                    const Vector<U, CoreU>& u, 
                    const Vector<V, CoreV>& v)
    {
        assert(r.size () == 3);
        assert(u.size () == 3);
        assert(v.size () == 3);
        r[0] = static_cast<R>(u[1] * v[2] - u[2] * v[1]);
        r[1] = static_cast<R>(u[2] * v[0] - u[0] * v[2]);
        r[2] = static_cast<R>(u[0] * v[1] - u[1] * v[0]);
    }

    /////////////////////////////////////////////////////////////////////
    /// Vector Descriptors
    /// for specific fixed size vector
    /// with specific accessing descriptor
    /////////////////////////////////////////////////////////////////////
    #pragma warning (push)

    ///nonstandard extension used : nameless struct/union
    #pragma warning (disable:4201)
        
    template <class T>
        struct VectorTraits_2
    {
        VECTOR_TRAITS_STATIC_SIZE(2);
        VECTOR_TRAITS_ACCESS_DATA(T, m_pData);

        union 
        {
            struct { T x, y; };
            T m_pData[2];
        };
    };

    template <class T>
        struct VectorTraits_3
    {
        VECTOR_TRAITS_STATIC_SIZE(3);
        VECTOR_TRAITS_ACCESS_DATA(T, m_pData);

        union 
        {
            struct { T x, y, z; };
            T m_pData[3];
        };
    };

    template <class T>
        struct VectorTraits_3_EX
    {
        VECTOR_TRAITS_STATIC_SIZE(3);
        VECTOR_TRAITS_ACCESS_DATA(T, m_pData);

        vtl_inline T& x () { return m_pData[0];}
        vtl_inline T& y () { return m_pData[1];}
        vtl_inline T& z () { return m_pData[2];}

        vtl_inline const T& x () const { return m_pData[0];}
        vtl_inline const T& y () const { return m_pData[1];}
        vtl_inline const T& z () const { return m_pData[2];}

    protected: T m_pData[3];
    };

    template <class T>
        struct VectorTraits_4
    {
        VECTOR_TRAITS_STATIC_SIZE(4);
        VECTOR_TRAITS_ACCESS_DATA(T, m_pData);

        union 
        {
            struct { T x, y, z, w; };
            T m_pData[4];
        };
    };

    #pragma warning (pop)

    /////////////////////////////////////////////////////////////////////
    /// Vector2, Vector2f, Vector2d
    /////////////////////////////////////////////////////////////////////
    template<class T, class VectorTraits = VectorTraits_2<T> >
        class Vector2 : public Vector<T, VectorTraits>
    {
        typedef Vector<T, VectorTraits> ParentType;
    public:
        CONSTRUCTOR_VECTOR_DEFAULT(Vector2, T)
        CONSTRUCTOR_FROM_PARENT(Vector2, ParentType)

        vtl_inline Vector2(T _x, T _y)
        {
            at(0) = _x;
            at(1) = _y;
        }

        vtl_inline Vector2& Set(T _x, T _y)
        {
            at(0) = _x;
            at(1) = _y;
            return *this;
        }

        /// return a perpendicular vector
        Vector2 Perpendicular() const { return Vector2(at(1), -at(0));}

        /// rotate anti clockwise
        Vector2& Rotate(T radian)
        {
	        T cosa = static_cast<T>(cos(radian));
	        T sina = static_cast<T>(sin(radian));
	        T _x = at(0)*cosa + at(1)*sina;
	        T _y = at(1)*cosa - at(0)*sina;
            at(0) = _x;
            at(1) = _y;
            return *this;
        }

        T GetAngle() const
        {
            return static_cast<T>(atan2(at(1), at(0)));
        }
    };

    typedef Vector2<float>      Vector2F;
    typedef Vector2<double>     Vector2D;

    /////////////////////////////////////////////////////////////////////
    ///  Vector3<>, Vector3f, Vector3d
    /////////////////////////////////////////////////////////////////////
    template<class T, class VectorTraits = VectorTraits_3<T> >
        class Vector3 : public Vector<T, VectorTraits>
    {
        typedef Vector<T, VectorTraits> ParentType;
    public:
        CONSTRUCTOR_VECTOR_DEFAULT(Vector3, T)
        CONSTRUCTOR_FROM_PARENT(Vector3, ParentType)

        vtl_inline Vector3(T _x, T _y, T _z)
        {
            at(0) = _x;
            at(1) = _y;
            at(2) = _z;
        }

        vtl_inline Vector3& Set(T _x, T _y, T _z)
        {
            at(0) = _x;
            at(1) = _y;
            at(2) = _z;
            return *this;
        }
    };

    typedef Vector3<float>      Vector3F;
    typedef Vector3<double>     Vector3D;

    //////////////////////////////////////////////////////////////////
    ////  Vector4<>, Vector4f, Vector4d
    //////////////////////////////////////////////////////////////////
    template<class T, class VectorTraits = VectorTraits_4<T> >
        class Vector4 : public Vector<T, VectorTraits>
    {
        typedef Vector<T, VectorTraits> ParentType;
    public:
        CONSTRUCTOR_VECTOR_DEFAULT(Vector4, T)
        CONSTRUCTOR_FROM_PARENT(Vector4, ParentType)

        vtl_inline Vector4(T _x, T _y, T _z, T _w)
        {
            at(0) = _x;
            at(1) = _y;
            at(2) = _z;
            at(3) = _w;
        }

        vtl_inline Vector4& Set(T _x, T _y, T _z, T _w)
        {
            at(0) = _x;
            at(1) = _y;
            at(2) = _z;
            at(3) = _w;
            return *this;
        }
    };

    typedef Vector4<float>      Vector4F;
    typedef Vector4<double>     Vector4D;

    //////////////////////////////////////////////////////////////////
    /// class vtl::VectorDynamic and vtl::VectorStatic
    /// short alias for vector defination
    /// Should be typedef template
    //////////////////////////////////////////////////////////////////

    template<sizetype NDIM, class T>
        class VectorStatic : public Vector<T, VectorTraitsStatic<NDIM, T> > 
        {
            typedef Vector<T, VectorTraitsStatic<NDIM, T> > ParentType;
        public:
            CONSTRUCTOR_VECTOR_DEFAULT(VectorStatic, T)
            CONSTRUCTOR_FROM_PARENT(VectorStatic, ParentType)
        };

    template<class T>
        class VectorDynamic : public Vector<T, VectorTraitsDynamic<T> > 
        {
            typedef Vector<T, VectorTraitsDynamic<T> > ParentType;
        public:
            CONSTRUCTOR_VECTOR_DEFAULT(VectorDynamic, T)
            CONSTRUCTOR_FROM_PARENT(VectorDynamic, ParentType);
        };

} /// namespace vtl

#endif//__VTL_VECTOR_HPP__