#ifndef DOUBLE2_H
#define DOUBLE2_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_DLL double2{
        public:
#pragma warning( push )
#pragma warning( disable : 4201 )
            union {
                struct{double x;double y;};
                struct{double r;double g;};
                double val[2];
            };
#pragma warning( pop ) 
        public:
            double2();
            double2( double, double);
            double2( const double2& );
            double2( const double* const );
            double operator [] ( unsigned int )const;
            double& operator [] ( unsigned int );
            double2& operator = ( const double2& );
            double2& operator = ( const double* const );

            bool operator == ( const double2& ) const;
            bool operator == ( const double* const ) const;
            bool operator != ( const double2& ) const;
            bool operator != ( const double* const ) const;
            double2 operator + ( const double2& ) const;
            double2 operator + ( const double* const ) const;
            double2 operator - ( const double2& ) const;
            double2 operator - ( const double* const ) const;
            double2 operator * ( const double2& ) const;
            double2 operator * ( const double* const ) const;
            double2 operator * ( const double& ) const;
            double2 operator / ( const double2& ) const;
            double2 operator / ( const double* const ) const;
            double2 operator / ( const double& ) const;
            void operator += ( const double2& );
            void operator += ( const double* const );
            void operator -= ( const double2& );
            void operator -= ( const double* const );
            void operator *= ( const double2& );
            void operator *= ( const double* const );
            void operator *= ( const double& );
            void operator /= ( const double2& );
            void operator /= ( const double* const );
            void operator /= ( const double& );

            double length() const;
            double squaredLength() const;
            double dot( const double2& ) const;
        };

        // ==
        ZH_DLL bool operator == ( const double* const, const double2& );
        // !=
        ZH_DLL bool operator != ( const double* const, const double2& );
        // +
        ZH_DLL double2 operator + ( const double* const, const double2& );
        ZH_DLL double2 operator + ( const double&, const double2& );
        // -
        ZH_DLL double2 operator - ( const double* const, const double2& );
        ZH_DLL double2 operator - ( const double&, const double2& );
        // *
        ZH_DLL double2 operator * ( const double* const, const double2& );
        ZH_DLL double2 operator * ( const double&, const double2& );
        // /
        ZH_DLL double2 operator / ( const double* const, const double2& );
        ZH_DLL double2 operator / ( const double&, const double2& );
        // +=
        ZH_DLL void operator += ( double* const, const double2& );
        // -=
        ZH_DLL void operator -= ( double* const, const double2& );
        // *=
        ZH_DLL void operator *= ( double* const, const double2& );
        // /=
        ZH_DLL void operator /= ( double* const, const double2& );

    }
} // namespace ZH


#endif
