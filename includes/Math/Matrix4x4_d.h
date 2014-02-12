#ifndef MATRIX4D_H
#define MATRIX4D_H

#include "Common/ZHSTD.h"

namespace ZH{
    namespace Math{

        // Forwards
        class double4;

        class ZH_MATH_DLL matrix4x4_d{
        public:
            ~matrix4x4_d(){}
            matrix4x4_d();
            matrix4x4_d( const matrix4x4_d& );
            matrix4x4_d( const double* const );
            matrix4x4_d( double );
            matrix4x4_d(
                    double,double,double,double,
                    double,double,double,double,
                    double,double,double,double,
                    double,double,double,double);

            // inline
            inline double& operator [](unsigned int i){ return v[i];}
            inline double  operator [](unsigned int i)const{ return v[i];}

            // =
            matrix4x4_d& operator = ( const matrix4x4_d& );
            matrix4x4_d& operator = ( const double* const );
            matrix4x4_d& operator = ( double );

            // ==
            bool operator == ( const matrix4x4_d& ) const;
            bool operator == ( const double* const ) const;
            bool operator == ( double ) const;

            // !=
            bool operator != ( const matrix4x4_d& ) const;
            bool operator != ( const double* const ) const;
            bool operator != ( double ) const;

            // +
            matrix4x4_d operator+(const matrix4x4_d&) const;
            matrix4x4_d operator+(const double* const) const;
            matrix4x4_d operator+(double)const;

            // -
            matrix4x4_d operator-(const matrix4x4_d&) const;
            matrix4x4_d operator-(const double* const) const;
            matrix4x4_d operator-(double) const;

            // *
            matrix4x4_d operator*(const matrix4x4_d&) const;
            matrix4x4_d operator*(const double* const) const;
            matrix4x4_d operator*(double) const;

            // /
            matrix4x4_d operator/(const matrix4x4_d&) const;
            matrix4x4_d operator/(const double* const) const;
            matrix4x4_d operator/(double) const;

            // http://en.wikipedia.org/wiki/Matrix_(mathematics)
            //
            void identityIt();
            void transposeIt();
            bool inverseIt();
            matrix4x4_d transposeMatrix()const;
            matrix4x4_d inverseMatrix()const;    
            bool isIdentity()const;
            matrix4x4_d matrix4x4_d::adjoint() const;
            double determinant() const;

        public:
            static const matrix4x4_d ZERO;
            static const matrix4x4_d IDENTITY;
            union{
                double v[16];
                double _v[4][4];
            };

        static double MINOR(const matrix4x4_d&, int,int,int,int,int,int);
            
        };

        // ==
        ZH_MATH_DLL bool operator == ( const double* const, const matrix4x4_d& );
        ZH_MATH_DLL bool operator == ( double, const matrix4x4_d& );
        // !=
        ZH_MATH_DLL bool operator != ( const double* const, const matrix4x4_d& );
        ZH_MATH_DLL bool operator != ( double, const matrix4x4_d& );
 
        // matrix4x4_d * double4
        ZH_MATH_DLL double4 operator * (const matrix4x4_d&, const double4&);
    }
} // namespace ZH


#endif