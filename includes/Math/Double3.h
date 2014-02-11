#ifndef DOUBLE3_H
#define DOUBLE3_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_DLL double3{
        public:
#pragma warning( push )
#pragma warning( disable : 4201 )
            union {
                struct{double x;double y;double z;};
                struct{double r;double g;double b;};
                double val[3];
            };
#pragma warning( pop ) 
        public:
            double3();
            double3( double, double, double);
            double3( const double3& );
            double3( const double* const );
            double operator [] ( unsigned int )const;
            double& operator [] ( unsigned int );
            double3& operator = ( const double3& );
            double3& operator = ( const double* const );

            bool operator == ( const double3& ) const;
            bool operator == ( const double* const ) const;
            bool operator != ( const double3& ) const;
            bool operator != ( const double* const ) const;
            double3 operator + ( const double3& ) const;
            double3 operator + ( const double* const ) const;
            double3 operator - ( const double3& ) const;
            double3 operator - ( const double* const ) const;
            double3 operator * ( const double3& ) const;
            double3 operator * ( const double* const ) const;
            double3 operator * ( const double& ) const;
            double3 operator / ( const double3& ) const;
            double3 operator / ( const double* const ) const;
            double3 operator / ( const double& ) const;
            void operator += ( const double3& );
            void operator += ( const double* const );
            void operator -= ( const double3& );
            void operator -= ( const double* const );
            void operator *= ( const double3& );
            void operator *= ( const double* const );
            void operator *= ( const double& );
            void operator /= ( const double3& );
            void operator /= ( const double* const );
            void operator /= ( const double& );


            void normalizeIt();
            double3 normalize() const;
            double length() const;
            double squaredLength() const;
            double dot( const double3& ) const;
            double3 cross( const double3& ) const;

        }; // class double3

        // ==
        ZH_DLL bool operator == ( const double* const, const double3& );
        // !=
        ZH_DLL bool operator != ( const double* const, const double3& );
        // +
        ZH_DLL double3 operator + ( const double* const, const double3& );
        ZH_DLL double3 operator + ( const double&, const double3& );
        // -
        ZH_DLL double3 operator - ( const double* const, const double3& );
        ZH_DLL double3 operator - ( const double&, const double3& );
        // *
        ZH_DLL double3 operator * ( const double* const, const double3& );
        ZH_DLL double3 operator * ( const double&, const double3& );
        // /
        ZH_DLL double3 operator / ( const double* const, const double3& );
        ZH_DLL double3 operator / ( const double&, const double3& );
        // +=
        ZH_DLL void operator += ( double* const, const double3& );
        // -=
        ZH_DLL void operator -= ( double* const, const double3& );
        // *=
        ZH_DLL void operator *= ( double* const, const double3& );
        // /=
        ZH_DLL void operator /= ( double* const, const double3& );

    } // namespace Math
} // namespace ZH

#endif


