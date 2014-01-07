#ifndef DOUBLE4_H
#define DOUBLE4_H

#include "Common/ZHSTD.h"
#include <assert.h>

namespace ZH{
    namespace Math{

        class ZH_DLL double4{
        public:
            union {
                struct{double x;double y;double z;double w;};
                struct{double r;double g;double b;double a;};
                double val[4];
            };
        public:
            double4();
            double4( double, double, double, double);
            double4( const double4& );
            double4( const double* const );
            double operator [] ( unsigned int )const;
            double& operator [] ( unsigned int );
            double4& operator = ( const double4& );
            double4& operator = ( const double* const );

            bool operator == ( const double4& ) const;
            bool operator == ( const double* const ) const;
            bool operator != ( const double4& ) const;
            bool operator != ( const double* const ) const;
            double4 operator + ( const double4& ) const;
            double4 operator + ( const double* const ) const;
            double4 operator - ( const double4& ) const;
            double4 operator - ( const double* const ) const;
            double4 operator * ( const double4& ) const;
            double4 operator * ( const double* const ) const;
            double4 operator * ( const double& ) const;
            double4 operator / ( const double4& ) const;
            double4 operator / ( const double* const ) const;
            double4 operator / ( const double& ) const;
            void operator += ( const double4& );
            void operator += ( const double* const );
            void operator -= ( const double4& );
            void operator -= ( const double* const );
            void operator *= ( const double4& );
            void operator *= ( const double* const );
            void operator *= ( const double& );
            void operator /= ( const double4& );
            void operator /= ( const double* const );
            void operator /= ( const double& );


            void normalizeIt();
            double4 normalize() const;
            double length() const;
            double squaredLength() const;
            double dot( const double4& ) const;

        }; // class double4

        // ==
        ZH_DLL bool operator == ( const double* const, const double4& );

        // !=
        ZH_DLL bool operator != ( const double* const, const double4& );

        // +
        ZH_DLL double4 operator + ( const double* const, const double4& );

        ZH_DLL double4 operator + ( const double&, const double4& );

        // -
        ZH_DLL double4 operator - ( const double* const, const double4& );

        ZH_DLL double4 operator - ( const double&, const double4& );

        // *
        ZH_DLL double4 operator * ( const double* const, const double4& );

        ZH_DLL double4 operator * ( const double&, const double4& );

        // /
        ZH_DLL double4 operator / ( const double* const, const double4& );

        ZH_DLL double4 operator / ( const double&, const double4& );

        // +=
        ZH_DLL void operator += ( double* const, const double4& );

        // -=
        ZH_DLL void operator -= ( double* const, const double4& );

        // *=
        ZH_DLL void operator *= ( double* const, const double4& );

        // /=
        ZH_DLL void operator /= ( double* const, const double4& );


    } // namespace Math
} // namespace ZH

#endif



