#ifndef __VTL_NUMERIC_HPP__
#define __VTL_NUMERIC_HPP__

/*************************************************************************\
Microsoft Research Asia
Copyright (c) 2001 Microsoft Corporation

Module Name:
   Numeric function, matrix related

Abstract:
    Functions adapted from Numerical Recipes, including
    1. ludcmp for LU decomposition
    2. lubksb with ludcmp to solve linear equation
    3. ludcmp for matrix inverse and determination
    4. choldc for Cholesky decomposition
    5. eigen_sys for eigen system
    6. svdcmp for singular value decomposition
    7. class SVD, SVD_Static, and SVD_Dynamic for easier usage of svd
    8. class EigenSystem for easier fixed sized eigen system

Notes:
    TODO: Make better interface for inverse, solve equation, and Least square

Usage:
    
History:
    Created  on 2003 Feb 14 by liyin@ust.hk
    Modified on 2003 Apr 17 by liyin@ust.hk

\*************************************************************************/

#include <assert.h>
#include <math.h>
#include "vtl_matrix.h"

namespace vtl {

// ---------------------------------------------------------------------------------
// These functions are recomposed from Numerical Recipes In C Second Edition
// 1. Data types are changed in to template Matrix and VectorNumeric.
// 2. Many assertions added.
// 3. make the subscript from 0 .. N-1 instead of 1 .. N
// 4. Make type more intuitive and checkable
//
// ---------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////
//template parameter, 
//   T is float, double or long double
//   MT is a matrix type, to be accessed by MT[i][j]
//      therefore, MT can be Matrix, T**, or T[][]
//   VT& is a vector type, to be accessed by VT[k]
//      therefore, VT& can be Vector, T*, or T[]
////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// LU decomposition, a is square matrix of dimension n
// indx is permutation int vector, can be vtl::Vector<> or std::vector
// d is used for matrix determination
template <class T, class MT, class VI>
    LRESULT ludcmp(int n, Matrix<T, MT>& a, VI indx[], T* d);

template <class T, class MT, class VI>
    vtl_inline LRESULT ludcmp(int n, Matrix<T, MT>& a, VI& indx, T* d)
{
    return ludcmp(n, a, indx.ptr(), d);
}

///////////////////////////////////////////////////////////////////////////
// LU back substraction, used for linear equation solver
// call ludcmp first.  n, a, indx is same as ludcmp, 
// b is the AX=b, in linear equation
template <class T, class MT, class VI, class VT>
    LRESULT lubksb(int n, Matrix<T, MT>& a, VI indx[], VT b[]);

template <class T, class MT, class VI, class VT>
    vtl_inline LRESULT lubksb(int n, Matrix<T, MT>& a, VI& indx, VT& b)
{
    return lubksb(n, a, indx.ptr(), b.ptr());
}

///////////////////////////////////////////////////////////////////////////
// Cholesky decomposition
// a is a square symetric matrix, dimension of n
// p ?????????????
template <class T, class MT, class VT>
    LRESULT choldc(int n, Matrix<T, MT>& a, VT p[]);

template <class T, class MT, class VT>
    vtl_inline LRESULT choldc(int n, Matrix<T, MT>& a, VT& p)
{
    return choldc(n, a, p.ptr());
}

///////////////////////////////////////////////////////////////////////////
// ??????????????????????????????????????????????????????????????
template <class T, class MT, class VT>
    void cholsl(int n, Matrix<T, MT>& a, VT p[], VT b[], VT x[]);

template <class T, class MT, class VT>
    void cholsl(int n, Matrix<T, MT>& a, VT& p, VT& b, VT& x)
{
    cholsl(n, a, p.ptr(), b.ptr(), x.ptr());
}

//////////////////////////////////////////////////////////////////////////////
// Solve the eigen system, a is input symetric diagonal matrix
// d is output eigen values.  no eigen vector
template<class T, class MT, class VT>
    LRESULT eigen_sys(int n, const Matrix<T, MT>& a, VT d[]);

template<class T, class MT, class VT>
    vtl_inline LRESULT eigen_sys(int n, const Matrix<T, MT>& a, VT& d)
{
    return eigen_sys(n, a, d.ptr());
}

//////////////////////////////////////////////////////////////////////////////
// Solve the eigen system, a is input symetric diagonal matrix
// d is output eigen values.  z is output eigen vector
template<class T, class MT, class VT>
    LRESULT eigen_sys(int n, const Matrix<T, MT>& a, VT d[], Matrix<T, MT>& z);

template<class T, class MT, class VT>
    vtl_inline LRESULT eigen_sys(int n, const Matrix<T, MT>& a, VT& d, Matrix<T, MT>& z)
{
    return eigen_sys(n, a, d.ptr(), z);
}

//////////////////////////////////////////////////////////////////////////////
// Fixed size eigen system, output d, and a
template <sizetype n, class T = Real, bool bNeedVector = true>
    class EigenSystem;

//////////////////////////////////////////////////////////////////////////////
// Given a matrix a, this routine computes its singular value decomposition,
// A = UWV'
// Matrix A dimention is [m][n], input A will be modified
// The diagonal matrix of singular values W is output as a vector
//      vector w size should be l = max(m, n)
// the Matrix V (not the transpose V') is output
// both U and V are column vectors, i.e. vec[k] = u[...][k];
// U is same size of A, therefore, input A in parameter u
template<class U, class T, class V>
    LRESULT svdcmp(int m, int n, U& u, T w[], V& v);

template<class A, class U, class W, class V>
    LRESULT svdcmp(int m, int n, U& u, W& w, V& v)
{    
    w.resize(n);
	svdcmp(m, n, u, w.ptr(), v);
}

//////////////////////////////////////////////////////////////////////////////
// SVD using static matrix and vectors, might be faster
template <sizetype m, sizetype n, class T>
    class SVD_Static;

//////////////////////////////////////////////////////////////////////////////
// SVD using dynamic matrix and vectors
template <class T>
    class SVD_Dynamic;

//////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------
// -- Implementation ----------------------------------------------------
// ----------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Eigen system.  make a symtric diagonal domestic matrix
// into a tridiagonal matrix
// a as input matrix with dimension n
// d is output diagonal
// e is output upper diagonal
// bNeedVector ? O(4N/3) : O(2N/3)
template<class T, class MT, class VT>
    LRESULT tred2(int n, Matrix<T, MT>& a, VT d[], VT e[], bool bNeedVector = true);

template<class T, class MT, class VT>
    vtl_inline LRESULT tred2(int n, Matrix<T, MT>& a, VT& d, VT& e, bool bNeedVector = true)
{
    tred2(n, a, d.ptr(), e.ptr(), bNeedVector);
}

////////////////////////////////////////////////////////////////////////////
// Solve the tridiagonal matrix into eigen system
// n, d, e is output of tred2
// z is output eigen vectors, in colum vectors
// d is output eigen values, unsorted
template<class T, class MT, class VT>
    LRESULT tqli(int n, VT d[], VT e[], Matrix<T, MT>& z, bool bNeedVector = true);

template<class T, class MT, class VT>
    vtl_inline LRESULT tqli(int n, VT& d, VT& e, Matrix<T, MT>& z, bool bNeedVector = true)
{
    return tqli(n, d.ptr(), e.ptr(), z, bNeedVector);
}

////////////////////////////////////////////////////////////////////////////
// w is singular value vector
// u, v is the sigular space
// usv' = X
// w is sorted from large to small
// u, and v are column vectors, i.e. vec[k] = u[...][k]
// U and input matrix are same type
// U[m][n], W[n], V[n][n]
// Use SVD_Static or SVD_Dynamic for easier interface
//
template<class U, class W, class V>
    class SVD
{
public:
    U u;
    W w;
    V v;

public:
    template<class MT> LRESULT Decompose (const MT & x)
    {
        if(x.size() == 0) return E_FAIL;
        u.CopyFrom(x);
        const sizetype m = u.rows();
        const sizetype n = u.cols();
        v.resize(n, n);
        w.resize(n);
        return svdcmp(int(m), int(n), u, w.ptr(), v);
    }

    template<class MT> LRESULT Compose (MT & x)
    {
        if(u.size() == 0) return E_FAIL;
        const sizetype m = u.rows();
        const sizetype n = u.cols();
        x.resize(m, n);
        x.SetZero();
        for(sizetype i = 0; i < m; i ++) for(sizetype j = 0; j < n; j ++)
        {
            for(sizetype k = 0; k < n; k ++)
            {
                x[i][j] += u[i][k] * w[k] * v[j][k];
            }

        }
        return S_OK;
    }


    // X = vsu'
    // Only when Matrix X is square Matrix
    template<class MT> LRESULT InverseCompose (MT & x)
    {
        if(u.size() == 0) return E_FAIL;
        const sizetype m = u.rows();
        const sizetype n = u.cols();
        assert(m == n);
        x.resize(n, m);
        x.SetZero();
        for(sizetype i = 0; i < n; i ++) for(sizetype j = 0; j < m; j ++)
        {
            for(sizetype k = 0; k < n; k ++)
            {
                x[i][j] += v[i][k] * w[k] * u[j][k];
            }

        }
        return S_OK;
    }
};

// After Decompose
template <sizetype m, sizetype n, class T>
    class SVD_Static : public SVD<
        MatrixStatic<m, n, T>, VectorStatic<n, T>, MatrixStatic<n, n, T> >
{
};

template<class T>
    class SVD_Dynamic : public SVD<
        MatrixDynamic<T>, VectorDynamic<T>, MatrixDynamic<T> >
{
};

// After Eigenvalue
// d is the eigenvalue
// a is the eigenvector, in column vector format
// Xa = ad
//
// bNeedVector = false, O(2*n^3 / 3)
// bNeedVector = ture,  O(4*n^3 / 3)
template <sizetype n, class T, bool bNeedVector>
    class EigenSystem
{
public:
    typedef VectorStatic<n, T> D;
    typedef MatrixStatic<n, n, T> A;

    D d;
    A a;

public:    
    LRESULT Eigenvalue(const A & X);

    D e;
};

template <class T, class MT, class VT>
    LRESULT SortEigenvalue(int n, VT& d, Matrix<T, MT>& a);

//  -------  Private Declaration -----------------------------------------------------

#define SIGN(a,b) ((b) >= 0 ? fabs(a) : -fabs(a))

#define SQR(a) (a * a)

//s template <class T>
//s     void SWAP(T & a, T & b)
//s {
//s     static T t;
//s     t = a;
//s     a = b;
//s     b = t;
//s }

//  -------  LU Decomposition --------------------------------------------------------
//  Given a Matrix, this routine replaces it by the LU decomposition 
//  of a rowwise permutation of itself.  
//  a is input
//  a is output, arranged as equation (2.3.14 Numerical Recipes)
//  indx[1..n] is an output vector that records the row permutation 
//      effected by the partial pivoting;
//  d is output as +1 or -1 depending on whether the number of row 
//      interchanges was even or odd. respectively
//      d will be used by Determinant()
//  This routine is used in comination with lubksb to solve 
//      linear equations or invert a matrix
// 
template <class T, class MT, class VI>
    LRESULT ludcmp(int n, Matrix<T, MT>& a, VI indx[], T* d)
{
    int i, imax, j, k;
    T big, dum, sum, temp;
    VectorDynamic<T> vv;           // vv stores the implicit scaling of each row
    vv.resize (n);

    *d = 1;                             // No row interchanges yet

    for(i = 0; i < n; i++)
    {
        big = 0;
        for(j = 0; j < n; j++)
        {
            temp = (T) fabs(a[i][j]);
            if (temp > big) big = temp;
        }

        if (big == 0)
        {   //No non zero largest element.
            PrintError ("Singular matrix in routine ludcmp\n");
            return E_FAIL;
        }

        // Save the scaling
        vv[i] = 1 / big;
    }

    // This is the loop over columns of Crout's method
    for(j = 0; j < n; j++)
    {
        for(i = 0; i < j; i++)
        {   // This is equation(2.3.12) except for i = j
            sum = a[i][j];
            for(k = 0; k < i; k++) sum -= a[i][k] * a[k][j];
            a[i][j]=sum;
        }

        // search for largest pivot element
        big=0.0;

        // init imax = j, because if so ignore exchange row
        // by Oliver Liyin, Jan 19, to clean /W4
        imax = j;

        for(i = j; i < n ; i++)
        {   //This is i = j of eqaution (2.3.12) 
            //and i = j + 1 .. N of equation(2.3.13)
            sum = a[i][j];
            for(k = 0; k < j; k++)
            {
                sum -= a[i][k] * a[k][j];
            }

            a[i][j] = sum;
            dum = vv[i] * (T) fabs(sum);
            if (dum >= big)
            {   // Is the figure of merit for the pivot better than the best so far ?
                big=dum;
                imax=i;
            }
        }

        // Do we need to inter change rows?
        if(j != imax)
        {   // Yes, do so ...
            for(k = 0; k < n; k++)
            {
                dum=a[imax][k];
                a[imax][k]=a[j][k];
                a[j][k]=dum;
            }

            *d = -*d; // ... and change the parity of d
            vv[imax]=vv[j]; // also inter change the scale factor
        }

        indx[j]=imax;

        // if the pivot element is zero, the matrix is singular
        // (at least to the precision of the algorithm).
        // For some applications on singular matrices,
        // it is desirable to substitute TINY for zero

        if (a[j][j] == 0) a[j][j] = std::numeric_limits<T>::epsilon ();
   
        if(j != n)
        {   // Now, finally, divide by the pivot element
            dum = 1 / (a[j][j]);
            for(i = j + 1; i < n; i++) a[i][j] *= dum;
        }
    }   // Go back for the next column in the reduction

    return S_OK;
}


// ------ LU Back Substitution---------------------------------------------------
// Solves the set of n linear equations AX = B
// Here a is input, not as the matrix A but rather as its LU decomposition,
// determined by the troutine ludcmp.
// index is input as the permutation vector returned by ludcmp.
// b is input as the right-hand side vector B
// and returns with the solution vector X
// This routine takes into account the possiblility that b will begin with 
// many zero elements, so it is efficient for use in matrix inversion.
// 
template <class T, class MT, class VI, class VT>
    LRESULT lubksb(int n, Matrix<T, MT>& a, VI indx[], VT b[])
{
    int i, j, ii = -1;
    T sum;
  
    // When ii is set to a positive value, it will become the index of
    // the first nonvanishing element of b.
    // We now do the forward substitution, equation (2.3.6).
    // The only new wrinkle is to unscramble the permutation as we go
    for(i = 0; i < n; i++)
    {
        int ip = (int) indx[i];
        sum = b[ip];
        b[ip] = b[i];
        if (ii >= 0)
        {
            for(j = ii; j <= i-1; j++)
            {
                sum -= a[i][j]*b[j];
            }
        }
        else if (sum != 0)
        {   // A nonzero element was encountered so from now on 
            // we will have to do the sums in the loop above.
            ii=i;
        }
        b[i]=sum;
    }

    for(i = n - 1; i >= 0; i--)
    {   // New we do the back substitution, equation (2.3.7)
        sum=b[i];
        for(j = i + 1; j < n; j++)
        {
            sum -= a[i][j]*b[j];
        }

        // Store a component of the solution vector X
        b[i] = sum / a[i][i];
    }

    return S_OK;
}

// ------------Singular Value Decomposition ------------------------------------
// Given a matrix a, this routine computes its singular value decomposition,
// A = UWV'
// Matrix A dimention is [m][n]
// The diagonal matrix of singular values W is output as a vector
//      vector w size should be l = max(m, n)
// the Matrix V (not the transpose V') is output
// both U and V are column vectors, i.e. vec[k] = u[...][k];
// 
template <class T>
    T pythag(T a, T b)
{
    T absa = static_cast<T>(fabs(a));
    T absb = static_cast<T>(fabs(b));
    if (absa > absb) 
        return absa * static_cast<T>(sqrt(1 + SQR(absb / absa)));
    else 
        return (absb == 0 ? 0 : 
            absb * static_cast<T>(sqrt(1 + SQR(absa/absb))));
}

// assume w resized to n
// assume u has been resized to (m, n)
// assume v has been resized to (n, n)
template<class U, class T, class V>
    LRESULT svdcmp(int m, int n, U& u, T w[], V& v)
{
    // a is an alias of u
    U& a = u;

    assert(u.rows() == m);
    assert(u.cols() == n);
    v.resize(n, n);

    int i, j, k, l, nm;
    T anorm,cc,f,g,h,ss,scale,x,y,z;
    VectorDynamic<T> rv1;
    rv1.resize(n);

    LRESULT ret = S_OK;

    g=scale=anorm=0;  // Householder reduction to bidiagonal form

    // by Oliver Liyin, to clean /W4
    // In case the following loop is empty (n < 0), which is impossible
    // give l a init value
    l = 0;

    for (i = 0; i < n; i++)
    {
        int l = i + 1;
        rv1[i] = scale * g;

        g=ss=scale=0;
        if(i < m)
        {
            for(k = i; k < m; k++)
            {
                scale += static_cast<T>(fabs(a[k][i]));
            }

            if(scale != 0)
            {
                for(k = i; k < m; k++)
                {
                    a[k][i] /= scale;
                    ss += a[k][i] * a[k][i];
                }

                f = a[i][i];
                g = static_cast<T>(-SIGN(sqrt(ss),f));
                h = f * g - ss;
                a[i][i] = f - g;

                for(j = l; j <n; j++)
                {
                    ss = 0;
                    for(k = i; k < m; k++) ss+= a[k][i] * a[k][j];
                    f = ss / h;
                    for(k = i; k < m; k++) a[k][j] += f * a[k][i];
                }
                for(k = i; k < m; k++) a[k][i] *= scale;
            }
        }

        w[i] = scale * g;
        g=ss=scale=0;
        if(i < m && i != n - 1)
        {
            for(k = l; k < n; k++) 
            {
                scale += static_cast<T>(fabs(a[i][k]));
            }

            if(scale != 0)
            {
                for(k = l; k < n; k++)
                {
                    a[i][k] /= scale;
                    ss += a[i][k] * a[i][k];
                }

                f = a[i][l];
                g = static_cast<T>(-SIGN(sqrt(ss),f));
                h = f * g - ss;
                a[i][l] = f - g;

                for(k = l; k < n; k++) rv1[k] = a[i][k] / h;

                for(j = l; j < m; j++)
                {
                    ss = 0;
                    for (k = l; k < n; k++) ss += a[j][k] * a[i][k];
                    for (k = l; k < n; k++) a[j][k] += ss * rv1[k];
                }
                for (k = l; k < n; k++) a[i][k] *= scale;
            }
        }
        anorm = __max(anorm, 
            static_cast<T>(fabs(w[i]) + fabs(rv1[i])));
    }

    // Accumulation of right-hand transformations
    for(i = n - 1; i >= 0; i--)
    {
        if(i < n - 1)
        {
            if(g != 0)
            {
                for( j = l; j < n; j++)
                {   // Double division to avoid possible underflow
                    v[j][i] = (a[i][j] / a[i][l]) / g;
                }
                for (j = l; j < n; j++)
                {
                    ss = 0;
                    for (k = l; k < n; k++) ss += a[i][k] * v[k][j];
                    for (k = l; k < n; k++) v[k][j] += ss * v[k][i];
                }
            }
            for(j = l; j < n; j++) v[i][j] = v[j][i] = 0;
        }
        v[i][i] = 1;
        g = rv1[i];
        l = i;
    }

    // Accumulation of left-hand transformations
    for(i = __min(m,n) - 1; i >= 0; i--)
    {
        l = i + 1;
        g = w[i];
        for (j = l; j < n; j++) a[i][j] = 0;

        if (g != 0)
        {
            g = 1 / g;
            for (j = l; j < n; j++)
            {
                ss = 0;
                for (k = l; k < m; k++) ss += a[k][i] * a[k][j];
                f = (ss / a[i][i]) * g;
                for (k = i;k < m; k++) a[k][j] += f * a[k][i];
            }
            for(j = i; j < m; j++) a[j][i] *= g;
        }
        else 
        {
            for (j = i; j < m; j++) a[j][i]=0;
        }
        ++a[i][i];
    }

    // Diagonalization of the bidiagonal form: 
    //  Loop over singular values, and over allowed iterations
    for (k = n - 1; k >= 0; k--)
    {
        for (int its = 1; its <= 30; its++)
        {
            int flag = 1;
            // by Oliver Liyin, to clean /W4
            // in case the following loop is empty (k <  0), which is impossible
            // give nm a init value
            nm = 0;

            for ( l = k; l >= 0; l--)
            {   // Test for splitting
                // Note that rv1[0] is always zero.
                nm = l - 1;
                if( (T) (fabs(rv1[l]) + anorm) == anorm)
                {
                    flag = 0;
                    break;
                }
                if( (T) (fabs(w[nm]) + anorm) == anorm) break;
            }

            if(flag)
            {   // Cancellation of rv1[l], if l > 1
                cc = 0;
                ss = 1;
                for(i = l; i <= k; i++)
                {
                    f = ss * rv1[i];
                    rv1[i] = cc * rv1[i];
      
                    if ((T)(fabs(f) + anorm) == anorm) break;

                    g = w[i];
                    h = pythag(f, g);
                    w[i] = h;
                    h = 1 / h;
                    cc = g * h;
                    ss = -f * h;

                    for(j = 0; j < m; j++)
                    {
                        y = a[j][nm];
                        z = a[j][i];
                        a[j][nm] = y * cc + z * ss;
                        a[j][i]  = z * cc - y * ss;
                    }
                }
            }
            z = w[k];

            // Convergence
            if (l==k)
            {
                // Singular value is made nonnegative.
                if(z < 0)
                {
                    w[k] = -z;
                    for(j = 0; j < n; j++) v[j][k] = -v[j][k];
                }
                break;
            }

            if (its == 30) 
            {
                std::cerr << "no convergence in 30 svdcmp iterations" << endl;
                ret = E_FAIL;
            }

            x = w[l];
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];

            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
            g = pythag(f, T(1));
            f = ((x - z) * (x + z) + h * ((y / (f + static_cast<T>(SIGN(g,f)))) - h)) / x;

            // Next QR transformation
            cc = ss = 1;
            for (j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = ss * g;
                g = cc * g;
                z = pythag(f, h);
                rv1[j] = z;
                cc = f / z;
                ss = h / z;
                f = x * cc + g * ss;
                g = g * cc - x * ss;
                h = y * ss;
                y *= cc;

                for (int jj = 0; jj < n; jj++)
                {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = x * cc + z * ss;
                    v[jj][i] = z * cc - x * ss;
                }

                z = pythag(f, h);
                w[j] = z;   // Rotation can be arbitrary if z = 0
                if (z != 0)
                {
                    z = 1 / z;
                    cc = f * z;
                    ss = h * z;
                }

                f = cc * g + ss * y;
                x = cc * y - ss * g;
                for (jj = 0; jj < m; jj++)
                {
                    y = a[jj][j];
                    z = a[jj][i];
                    a[jj][j] = y * cc + z * ss;
                    a[jj][i] = z * cc - y * ss;
                }
            }

            rv1[l] = 0;
            rv1[k] = f;
            w[k] = x;
        }
    }

    // bellow are coded by Oliver Liyin

    // sort the singular values and corresponding vectors
    // buble sort
    for(i = 0; i < n - 1; i ++)
    {
        // find the maximum after i
        int max_ind = i;
        for(j = i + 1; j < n; j ++)
        {
            if(w[max_ind] < w[j])
            {
                max_ind = j;
            }
        }
        
        // SWAP with i
        if(max_ind != i)
        {
            std::swap(w[i], w[max_ind]);
            for(k = 0; k < m; k ++) std::swap(a[k][i], a[k][max_ind]);
            for(k = 0; k < n; k ++) std::swap(v[k][i], v[k][max_ind]);
        }
    }

    return ret;
}


// ------------householder reduction -------------------------------------
// Householder reduction of a real, symmetric matrix a.
// On output, a is replaced by the orthogonal matrix Q effecting the transformation
// d returns the diagonal elements of the tridiagonal matrix
// e the off-diagonal elements, with e[0] = 0
// 
template <class T, class MT, class VT>
    LRESULT tred2(int n, Matrix<T, MT>& a, VT d[], VT e[], bool bNeedVector)
{
    int l, k, j, i;
    T scale, h, hh, g, f;

    for(i = n - 1; i >= 1; i--)
    {
        l = i - 1;
        h = scale = 0;
        if (l > 0)
        {
            for(k = 0; k <= l; k++) scale += (T) fabs( a[i][k] );

            if (scale == 0)
            {   // Skip Transformation
                e[i] = a[i][l];
            }
            else 
            {
                for(k = 0; k <= l; k++)
                {   // Use scaled a's for transformation
                    a[i][k] /= scale;
                    h += a[i][k] * a[i][k]; // Form \sigma in h
                }

                f = a[i][l];
                g = static_cast<T> ((f > 0) ? -sqrt(h) : sqrt(h));
                e[i] = scale * g;
                h -= f * g;         // Now h is equation (11.2.4)
                a[i][l] = f - g;    // Store u in the i-th row of a

                f = 0;
                for(j = 0; j <= l;j++)
                {
                    if(bNeedVector)
                        a[j][i]=a[i][j]/h;  // Store u/H in i-th column of a
                    g = 0;              // Form an element of Au in g
                    for(k = 0; k <= j; k++) g += a[j][k] * a[i][k];
                    for(k = j + 1;k <= l; k++)    g += a[k][j] * a[i][k];
                    e[j] = g / h;       // Form element of p in temporarily unused element of e
                    f += e[j] * a[i][j];
                }

                hh = f / (h + h);
                for(j = 0;j <= l; j++)              // Form K, equation(11.2.11)
                {   
                    f = a[i][j];                      // Form q and store in e overwriting p
                    e[j] = g = e[j] - hh * f;         // Note that e[l] = e[i-1] survives
                    for(k = 0; k <= j; k++) a[j][k] -= (f * e[k] + g * a[i][k]);
                }
            }
        } 
        else
        {
            e[i]=a[i][l];
        }

        d[i]=h;
    }

    if(bNeedVector)
        d[0] = 0;
    e[0] = 0;

    if(bNeedVector)
    {
        // Begin accumulation of transformation matrices
        for(i = 0; i < n; i++)
        {
            l = i - 1;
            if(d[i] != 0)   // This block skipped when i = 0
            {   
                for(j = 0; j <= l; j++)
                {
                    g = 0;

                    // Use u and u/H stored in a to form PQ
                    for(k = 0; k <= l; k++) g += a[i][k] * a[k][j];
                    for(k = 0; k <= l; k++) a[k][j] -= g * a[k][i];
                }
            }

            d[i] = a[i][i]; // This statement remains
            a[i][i] = 1;    // Reset row and column of a to identity matrix for next iteration
            for(j = 0; j <= l; j++) a[j][i] = a[i][j] = 0;
        }
    }
    else
    {
        // Begin accumulation of transformation matrices
        for(i = 0; i < n; i++)
        {
            d[i] = a[i][i]; // This statement remains
        }
    }

    return S_OK;
}

//  ---------- Tridiagonal QL Implicit ----------------------------------
// QL algorithm with implicit shifs, to determine the eigenvalues and eigenvectors
// of a real, symmetric, tridiagonal matrix, or of a real, symmetric matrix
// previously reduced by tred2.
// On imput, d contains the diagonal elements of the tridiagonal matrix.
// On output, d returns the eigenvalues.
// The vector e inputs the subdiagonal elements of the tridiagonal matrix,
// with e[0] arbitrary.
// If the eigenvectors of a tridiagonal matrix are desired, 
// the matrix z[n][n] is input as the identity matrix. 
// If the eigenvectors of a matrix that has been reduced by tred2 are required, 
// then z is input as the matrix output by tred2.
// Ineither case, the k-th column of z returns the normalized eigenvector corresponding to d[k]
// 
template<class T, class MT, class VT>
    LRESULT tqli(int n, VT d[], VT e[], Matrix<T, MT>& z, bool bNeedVector)
{
    int m,l,iter,i,k;
    T s, r, p, g, f, dd, c, b;

    // Convenient to renumber the elements of e.
    for(i = 1; i < n; i++) e[i - 1] = e[i];
    e[n - 1] = 0;

    for(l = 0; l < n; l++)
    {
        iter = 0;
        do {
            // Look for a single small subdiagonal element to split the matrix
            for(m = l; m < n - 1; m++)
            {
                dd = static_cast<T>(fabs(d[m]) + fabs(d[m + 1]));
                if ((T)(fabs(e[m]) + dd) == dd) break;
            }

            if (m != l)
            {
                if (iter++ == 30)
                { 
                    fprintf(stderr, "Too many iterations in tqli\n");
                    return E_FAIL;
                }

                g = (d[l+1] - d[l]) / (2 * e[l]);   // Form shift
                r = pythag(g, T(1));
                g = d[m] - d[l] + e[l] / (g + static_cast<T>(SIGN(r,g)));   // This is d_m - K_s
                s = c = 1;
                p = 0;

                // A plane rotation as in the original QL, 
                // followed by Givens rotations to restore tridiagonal form.
                for(i = m - 1; i >= l; i--)
                {
                    f = s * e[i];
                    b = c * e[i];

                    e[i + 1] = r = pythag(f,g);
                    if (r == 0)
                    {   // Recover from underflow
                        d[i + 1] -= p;
                        e[m] = 0;
                        break;
                    }
                    s = f / r;
                    c = g / r;

                    g = d[i + 1] - p;
                    r = (d[i] - g) * s + 2 * c * b;
                    p = s * r;
                    d[i + 1] = g + p;
                    g = c * r - b;

                    if(bNeedVector)
                    {
                        for(k = 0; k < n; k++)
                        {
                            f = z[k][i + 1];
                            z[k][i + 1] = s * z[k][i] + c * f;
                            z[k][i] = c * z[k][i] - s * f;
                        }
                    }
                }
                if (r == 0 && i >= 0) continue;
                d[l] -= p;
                e[l] = g;
                e[m] = 0;
            }
        }while( m != l);
    }

    return S_OK;
}

// sort the eigenvalue values and corresponding vectors
// buble sort
template <class T, class MT, class VT>
    LRESULT SortEigenvalue(int n, VT& d, Matrix<T, MT>& a)
{
    int i, j, k;
    for(i = 0; i < n - 1; i ++)
    {
        // find the maximum after i
        for(j = i + 1; j < n; j ++)
        {
            if(d[i] < d[j])
            {
                std::swap(d[i], d[j]);
                for(k = 0; k < n; k ++) std::swap(a[k][i], a[k][j]);
            }
        }
    }

    return S_OK;
}

template<class T, class MT, class VT>
    LRESULT eigen_sys(int n, const Matrix<T, MT>& a, VT d[])
{
    // copy a to z, because tred2 will modify z
    Matrix<T, MT> z = a;

    std::auto_ptr<VT> e(new VT[n]);

    tred2(n, z, d, e.get(), true);
    tqli (n, d, e.get(), z, true);
    SortEigenvalue(n, d, z);

    return S_OK;
}

template<class T, class MT, class VT>
    LRESULT eigen_sys(int n, const Matrix<T, MT>& a, VT d[], Matrix<T, MT>& z)
{
    // copy a to z, because tred2 will modify z
    z = a;

    std::auto_ptr<VT> e(new VT[n]);

    tred2(n, z, d, e.get(), true);
    tqli (n, d, e.get(), z, true);
    SortEigenvalue(n, d, z);

    return S_OK;
}

template <sizetype n, class T, bool bNeedVector>
    LRESULT EigenSystem<n, T, bNeedVector>::Eigenvalue(const A & X)
{
    a = X;

    tred2(n, a, d, e, bNeedVector);
    tqli (n, d, e, a, bNeedVector);
    SortEigenvalue(n, d, a);

    return S_OK;
}


template <class T, class MT, class VT>
    LRESULT choldc(int n, Matrix<T, MT>& a, VT p[])
//Given a positive-definite symmetric matrix a[n][n],
//this routine constructs its Cholesky decomposition, A=L*L^T
//On input, only the upper triangle of a need be given; it is not
//modi?ed. The Cholesky factor L is returned in the lower triangle of a, 
//except for its diagonal elements which are returned in p[n].
{
    for (int i=0;i<n;i++) 
    {
        for (int j=i;j<n;j++) 
        {
            T sum=a[i][j];
            for (int k=i-1;k>=0;k--) 
            {
                sum -= a[i][k]*a[j][k];
            }                
            if (i == j) 
            {
                if (sum <= 0.0)
                {
                    PrintError ("choldc non-positive\n");
                    return E_FAIL;
                }
                p[i]= (T) sqrt(sum);
            } 
            else a[j][i]=sum/p[i];
        }
    }

    return S_OK;
}


template <class T, class MT, class VT>
    void cholsl(int n, Matrix<T, MT>& a, VT p[], VT b[], VT x[])
//Solves the set of n linear equations A*x = b,
//where a is a positive-definite symmetric matrix.
//a[1..n][1..n] and p[1..n] are input as the output of the routine choldc.
//Only the lower subdiagonal portion of a is accessed.
//b[1..n] is input as the right-hand side vector.
//The solution vector is returned in x[1..n].
//a, n, and p are not modified and can be left in place
//for successive calls with different right-hand sides b.
//b is not modified unless you identify b and x in the calling sequence,
//which is allowed.
{
    int i,k;
    T sum;
    for (i=0;i<n;i++) 
    {
        //  Solve L ¡¤ y = b, storing y in x.
        for (sum=b[i],k=i-1;k>=0;k--) 
        {
            sum -= a[i][k]*x[k];
        }
        x[i]=sum/p[i];
    }
    for (i=n-1;i>=0;i--) 
    { 
        // Solve LT ¡¤ x = y.
        for (sum=x[i],k=i+1;k<n;k++) 
        {
            sum -= a[k][i]*x[k];
        }

        x[i]=sum/p[i];
    }
}

}   //namespace vtl

#endif //__VTL_NUMERIC_HPP__