#ifndef __VTL_MATRIX_HPP__
#define __VTL_MATRIX_HPP__

/*************************************************************************\
Microsoft Research Asia
Copyright (c) 2001 Microsoft Corporation

Module Name:
    VTL matrix defination and implementation

Abstract:
    1. Matrix can be accessed by mat[m][n]
    2. A number of geometry helper function and C++ operators are defined.
    3. MatrixStatic is simple copied type, as an extension of C type 2D array.
    4. MatrixDynamic is complex copied type.  In current implementation,
       It's a 2D access shell of std::vector

Notes:
    1. Unlike vtl::Vector, Matrix cannot be implicitly converted into T*.
       ptr() must be used to explicitly convert into T*, or const T*.
    2. row_ptr() can be used to convert one row into T*,
    3. begin() end() iterator are defined to access matrix as 1D iteratable.
    4. row_begin() row_end() row_iterator are defined to access one row.
    3. iterator or row_iterator are avalible to many stl functions, like copy, 
       fill, find

Usage:
    1. Ignore MatrixTraits, MatrixrDescription.
    2. Use MatrixStatic<NROWS, NCOLS, T>  and MatrixDynamic<T>
       MatrixDynamic<T> must follows resize(_ncols, _nrows) for allocation
    3. Use Matrix3x3, Matrix4x4 in vtl_geometry.h for preset matrix type

History:
    Created  on 2003 Feb 14 by liyin@ust.hk
    Modified on 2003 Apr 16 by liyin@ust.hk
          
\*************************************************************************/

#include <math.h>
#include "vtl_vector.h"

namespace vtl {
    
#define CONSTRUCTOR_MATRIX_DEFAULT(ClassName, Type)    \
    ClassName() {}                                      \
    FILL_CONSTRUCTOR(ClassName, Type)    
    
    //////////////////////////////////////////////////////////////////
    // Matrix Traits
    //     magic traits, compatibal with both static and dynamic
    //     matrix defination.  Oliver Liyin by Jun, 9, 2003
    // a matrix traits must define:
    //      1. memory function, resize and clear
    //      2. define a T** member, must be named m_ppData
    //      3. define size(), rows() and cols() return size of matrix
    //      4. typedef a bool function empty()
    //      5. define 1D data access interface: ptr()
    //      6. define 2D data access interface: row_ptr()
    //      7. Retreive Row vector and Col vector
    // It must manage the memory by it's self
    ///////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////
    // MatrixTraitsStatic is the basic of all static matrix
    // It's designed for static matrix that can be in stack
    // Usually it has a small size and faster memory operation
    // since the size of the matrix is decided at compile time
    //
    template <sizetype NROWS, sizetype NCOLS, class T>
        class MatrixTraitsStatic
    {
    public:
        vtl_inline sizetype rows() const { return NROWS;}
        vtl_inline sizetype cols() const { return NCOLS;}
        vtl_inline sizetype size() const { return NROWS * NCOLS;}
        vtl_inline bool empty () const { return false;}

        // memory operation is simply nothing
        vtl_inline void resize (sizetype _rows, sizetype _cols) const 
        { 
            assert(NROWS == _rows && NCOLS == _cols);
        }
        vtl_inline void clear () const {}

        // access as 1D ptr, matrix.ptr()
        vtl_inline       T* ptr()       { return &m_ppData[0][0];}
        vtl_inline const T* ptr() const { return &m_ppData[0][0];}
        
        // access each row as 1D ptr, matrix.row_ptr(2)
        vtl_inline       T* row_ptr(sizetype y)       { return m_ppData[y];}
        vtl_inline const T* row_ptr(sizetype y) const { return m_ppData[y];}

        const VectorStatic<NCOLS, T> Row(sizetype y) const
        {
        	VectorStatic<NCOLS, T> vecRow;
            for (sizetype x = 0; x < cols(); x ++) vecRow[x] = row_ptr(y)[x];
            return vecRow;
        }
        const VectorStatic<NROWS, T> Col(sizetype x) const
        {
        	VectorStatic<NROWS, T> vecCol;
            for (sizetype y = 0; y < rows(); y ++) vecCol[y] = row_ptr(y)[x];
            return vecCol;
        }
    protected:
        T m_ppData[NROWS][NCOLS];
    };

    ///////////////////////////////////////////////////////////////////
    // MatrixTraitsDynamic is the basic of all dynamic matrix
    // it's designed for dynamically resizable matrix
    // It's size is run-time changable
    // It will act similar as std::matrix, while can be 
    // used in math and geometry functions
    //
    template <class T> 
        class MatrixTraitsDynamic
    {
    // hide these data from children class
    // because they are not appear in static matrix
    private:
        VectorDynamic<T*> m_rgRows;
        VectorDynamic<T>  m_rgData;

    public:
        MatrixTraitsDynamic() {}

        MatrixTraitsDynamic(const MatrixTraitsDynamic& m)
        {
            if(this == &m) return;
            resize(m.rows(), m.cols());
            memcpy(ptr(), m.ptr(), size() * sizeof(T));
        }

        MatrixTraitsDynamic& operator= (const MatrixTraitsDynamic& m)
        {
            if(this == &m) return *this;
            resize(m.rows(), m.cols());
            memcpy(ptr(), m.ptr(), size() * sizeof(T));
            return *this;
        }

        vtl_inline sizetype rows() const { return m_rgRows.size ();}
        vtl_inline sizetype cols() const { return empty() ? 0 : (size() / rows());}
        vtl_inline sizetype size() const { return m_rgData.size ();}

        // only use ncols as size indicator
        vtl_inline bool empty () const { return m_rgData.size() == 0;}

        // return void to be compatible with std::vector
        // check size() for avalibility
        // TODO: handle memory fail exception
        void resize (sizetype _rows, sizetype _cols)
        {
            // if same size, return
            if(_rows == rows() && _cols == cols()) return;

            // clear original memory
            clear ();

            // if empty, return cleared version
            if(_rows == 0 || _cols == 0) return;

            // allocate the data
            m_rgRows.resize (_rows);
            m_rgData.resize (_rows * _cols);

            // assign row pointers
            for(sizetype k = 0; k < rows(); k ++)
            {
                m_rgRows[k] = m_rgData.ptr() + k * cols();
            }
        }

        // clear memory in heap
        void clear ()
        {
            if(empty ()) return;
            m_rgRows.clear ();
            m_rgData.clear ();
        }

        // access as 1D ptr, matrix.ptr()
        vtl_inline T* ptr() { return m_rgData.ptr ();}
        vtl_inline const T* ptr() const { return m_rgData.ptr ();}
        
        // access each row as 1D ptr, matrix.row_ptr(2)
        vtl_inline T* row_ptr(sizetype y) { return m_rgRows[y];}
        vtl_inline const T* row_ptr(sizetype y) const { return m_rgRows[y];}

        const VectorDynamic<T> Row(sizetype y) const
        {
        	VectorDynamic<T> vecRow;
            vecRow.resize(cols());
            for (sizetype x = 0; x < cols(); x ++) vecRow[x] = row_ptr(y)[x];
            return vecRow;
        }
        const VectorDynamic<T> Col(sizetype x) const
        {
        	VectorDynamic<T> vecCol;
            vecCol.resize(rows());
            for (sizetype y = 0; y < rows(); y ++) vecCol[y] = row_ptr(y)[x];
            return vecCol;
        }
    };

    //////////////////////////////////////////////////////////////////
    // class vtl::Matrix
    // provides a basic defination of matrix
    // abstract from geometry and math
    // MatrixTraits is either MatrixTraitsStatic or MatrixTraitsDynamic
    //////////////////////////////////////////////////////////////////
    template <class T, class MatrixTraits>
        class Matrix : public MatrixTraits
    {
    public:        
        CONSTRUCTOR_MATRIX_DEFAULT(Matrix, T)
        CONSTRUCTOR_FROM_PARENT(Matrix, MatrixTraits)

        // access as 2 dimension array, matrix[3][2]
        T* operator [] (sizetype y) { return row_ptr(y);}
        const T* operator [] (sizetype y) const { return row_ptr(y);}
        
        // copy from  a different type matrix, one by one
        template <class _T, class _Traits>
            Matrix& CopyFrom(const Matrix<_T, _Traits>& src)
        {
            if((void*)this != (void*)&src)
            {
                resize (src.rows(), src.cols());
                std::copy(src.begin(), src.end(), begin());
            }
            return *this;
        }
    
    public:
        //////////////////////////////////////////////////////////////////////
        // Iterator for STL compatibility, ref <algorithm>
        typedef T* iterator;
        typedef const T* const_iterator;
        
        iterator begin()    { return ptr();}
        iterator end()      { return ptr() + size();}
        
        const_iterator begin()  const { return ptr();}
        const_iterator end()    const { return ptr() + size();}    

        //////////////////////////////////////////////////////////////////////
        // Iterator for each row
        typedef T* row_iterator;
        typedef const T* const_row_iterator;
        
        row_iterator row_begin(sizetype y)    { return row_ptr(y);}
        row_iterator row_end(sizetype y)      { return row_ptr(y) + cols();}
        
        const_row_iterator row_begin(sizetype y)  const { return row_ptr(y);}
        const_row_iterator row_end(sizetype y)    const { return row_ptr(y) + cols();}    

    public:
        //////////////////////////////////////////////////////////////////////
        // Helper Member Functions
        //////////////////////////////////////////////////////////////////////
        
        // fill in zeros, quick
        Matrix& SetZero()
        {
            if(empty ()) return *this;
            memset(ptr(), 0, sizeof(T) * size());
            return *this;
        }
        
        // load a identity matrix
        Matrix& SetIdentity()
        {
            SetZero();
            for(sizetype i = 0; i < rows() && i < cols(); i ++)
            {
                (*this)[i][i] = 1;
            }
            return *this;
        }

        // fill in a value to matrix
        Matrix& Fill(const T& value)
        {
            if(empty ()) return *this;
            std::fill(begin(), end(), value);
            return *this;
        }
        
        //check if *this is a zero vector
        bool IsZero(T threshold = std::numeric_limits<T>::epsilon()) const
        {
            for (sizetype i = 0; i < size(); i++)
            {
                // using bounding box
                if(!vtl::IsZero(ptr()[i], threshold)) return false;
            }
            return true;
        }

        bool EqualsTo(const Matrix& m, 
            T threshold = std::numeric_limits<T>::epsilon()) const
        {
            if(size () != m.size ()) return false;

            for (sizetype i = 0; i < size(); i++)
            {
                // using bounding box
                if(!vtl::IsZero(ptr()[i] - m.ptr()[i], threshold)) return false;
            }
            return true;
        }    

        //////////////////////////////////////////////////////////////////////
        //  Geometry functions
        //////////////////////////////////////////////////////////////////////

        // Determinant of matrix, using LU decomposition
        // Must be a square matrix
        T Determinant() const
        {
            assert(rows() == cols());
            const sizetype n = rows();

            Matrix a = *this;
            
            VectorDynamic<int> indx;
            indx.resize(n);

            T d;
            ludcmp (int(n), a, indx, &d);
            
            for(sizetype j = 0; j < n; j ++) 
            {
                d *= a[j][j];
            }
            
            return d;
        }
    
        // return a copy of inverse, require square matrix
        const Matrix Inverted() const
        {
            Matrix mt = *this;
            mt.Invert ();
            return mt;
        }

        // Invert this matrix, require square matrix
        void Invert ()
        {
            assert(rows () == cols ());        
            const sizetype n = rows();

            VectorDynamic<int> indx;
            indx.resize (n);

            T d;        
            ludcmp (int(n), *this, indx, &d);
            
            Matrix result;
            result.resize (n, n);
            
            VectorDynamic<T> col;
            col.resize (n);

            for(sizetype j = 0; j < n; j ++)
            {
                col.SetZero ();
                col[j] = 1;
                lubksb (int(n), *this, indx, col);
                for(sizetype i = 0; i < n; i ++) 
                {
                    result[i][j] = col[i];
                }
            }
            
            *this = result;
        }
        
        // transpose self, must be a square matrix
        void Transpose ()
        {
            MatTranspose(*this, *this);
        }
    
        // return a transposed matrix, const, must be square matrix
        const Matrix Transposed () const
        {
            Matrix dst;
            MatTranspose (dst, *this);
            return dst;
        }
        
        // r = Mv,  transform a vector, same dimension
        template<class T, class VCore> 
            const Vector<T, VCore> 
            Transform (const Vector<T, VCore>& v) const
        {
            assert(v.size() == cols());
            Vector<T, VCore> r;
            r.resize(v.size ());
            vtl::MatMulVec(r, *this, v);
            return r;
        }

        // r = M[v, 1],  transform a direction v, no perspective
        template<class T, class VCore> 
            const Vector<T, VCore> 
            TransformPoint (const Vector<T, VCore>& v) const
        {
            assert(rows() == cols());
            assert(cols() == v.size() + 1);
            Vector<T, VCore> ret;
            ret.resize(v.size ());
            ret.SetZero();

            for(size_t r = 0; r < rows() - 1; r ++)
            {
                ret[r] = (*this)[r][cols()-1];
                for(size_t c = 0; c < cols() - 1; c ++)
                {
                	ret[r] += (*this)[r][c] * v[c];
                }
            }
            return ret;
        }

        // r = M[v, 0],  transform a direction v, ignore translation
        template<class T, class VCore> 
            const Vector<T, VCore> 
            TransformDirection (const Vector<T, VCore>& v) const
        {
            assert(rows() == cols());
            assert(cols() == v.size() + 1);
            Vector<T, VCore> ret;
            ret.resize(v.size ());
            ret.SetZero();

            for(size_t r = 0; r < rows() - 1; r ++)
            {
                for(size_t c = 0; c < cols() - 1; c ++)
                {
                	ret[r] += (*this)[r][c] * v[c];
                }
            }
            return ret;
        }

        //////////////////////////////////////////////////////////////////////
        // IO stream functions

        friend std::istream& operator >> (std::istream& is, Matrix& mat)
        {
            for (sizetype m = 0; m < mat.rows(); m ++) 
            {
                for (sizetype n = 0; n < mat.cols(); n ++)
                {
                    is >> mat[m][n];
                }
            }
            return is;
        }

        friend std::ostream& operator << (std::ostream& os, const Matrix& mat)
        {
            for (sizetype m = 0; m < mat.rows(); m ++) 
            {
                os << endl;
                for (sizetype n = 0; n < mat.cols(); n ++)
                {
                    os << mat[m][n] << " ";
                }
            }
            return os;
        }
    };

    // ---------------------------------------------------------------------
    // ---------------------  Matrix .vs. Matrix ---------------------------
    // ---------------------------------------------------------------------

    // ?= operator
    #define Matrix_OPU_Matrix(OP)                               \
    template<class TD, class TS, class CoreD, class CoreS>      \
        Matrix<TD, CoreD>& operator OP (Matrix<TD, CoreD> & dst,\
            const Matrix<TS, CoreS> & src)                      \
    {                                                           \
        assert(dst.rows() == src.rows());                       \
        assert(dst.size() == src.size());                       \
        Matrix<TD, CoreD>::iterator id = dst.begin ();          \
        Matrix<TS, CoreS>::const_iterator is = src.begin ();    \
        for(; id != dst.end(); ++id, ++is)                      \
        {                                                       \
            (*id) OP (*is);                                     \
        }                                                       \
        return dst;                                             \
    }

    Matrix_OPU_Matrix( += )
    Matrix_OPU_Matrix( -= )
    Matrix_OPU_Matrix( *= )
    Matrix_OPU_Matrix( /= )
    Matrix_OPU_Matrix( &= )
    Matrix_OPU_Matrix( |= )
    Matrix_OPU_Matrix( ^= )
    Matrix_OPU_Matrix( >>= )
    Matrix_OPU_Matrix( <<= )

    // Binocular operation
    #define Matrix_OPB_Matrix(OPB, OPU)                     \
    template<class TL, class TR, class CoreL, class CoreR>  \
        const Matrix<TL, CoreL> operator OPB (              \
            const Matrix<TL, CoreL> & left,                 \
            const Matrix<TR, CoreR> & right)                \
    {                                                       \
        Matrix<TL, CoreL> dst(left);                        \
        dst OPU right;                                      \
        return dst;                                         \
    }

    Matrix_OPB_Matrix( +, += )
    Matrix_OPB_Matrix( -, -= )
    Matrix_OPB_Matrix( *, *= )
    Matrix_OPB_Matrix( /, /= )
    Matrix_OPB_Matrix( &, &= )
    Matrix_OPB_Matrix( |, |= )
    Matrix_OPB_Matrix( ^, ^= )
    Matrix_OPB_Matrix(>>,>>= )
    Matrix_OPB_Matrix(<<,<<= )

    // Bool operator, lexicographical_compare
    #define Matrix_LESS_Matrix(OP, OPS, EQUAL)                  \
    template<class TL, class TR, class CoreL, class CoreR>      \
        bool operator OP (                                      \
            const Matrix<TL, CoreL> & left,                     \
            const Matrix<TR, CoreR> & right)                    \
    {                                                           \
        assert(left.rows() == right.rows());                    \
        assert(left.size() == right.size());                    \
        Matrix<TL, CoreL>::const_iterator il = left.begin ();   \
        Matrix<TR, CoreR>::const_iterator ir = right.begin ();  \
        for(; il != left.end(); ++il, ++ir)                     \
        {                                                       \
            if((*il) OPS (*ir)) return true;                    \
            if((*ir) OPS (*il)) return false;                   \
        }                                                       \
        return EQUAL;                                           \
    }

    Matrix_LESS_Matrix( >  , > , false)
    Matrix_LESS_Matrix( <  , < , false)
    Matrix_LESS_Matrix( >= , > , true)
    Matrix_LESS_Matrix( <= , < , true)

    template<class TL, class TR, class CoreL, class CoreR>
        bool operator == (
            const Matrix<TL, CoreL> & left,
            const Matrix<TR, CoreR> & right)
    {
        assert(left.rows() == right.rows());
        assert(left.size() == right.size());
        Matrix<TL, CoreL>::const_iterator il = left.begin();
        Matrix<TR, CoreR>::const_iterator ir = right.begin();
        for(; il != left.end(); ++il, ++ir)
        {
            if((*il) != (*ir)) return false;
        }
        return true;
    }

    template<class TL, class TR, class CoreL, class CoreR>
        bool operator != (
            const Matrix<TL, CoreL> & left,
            const Matrix<TR, CoreR> & right)
    {
        return !(left == right);
    }

    // ---------------------------------------------------------------------
    // -------------------  Matrix .vs. Scalar -----------------------------
    // ---------------------------------------------------------------------

    #define Matrix_OPE_Scalar(OP)                       \
    template<class T, class Core>                       \
        Matrix<T, Core>& operator OP                    \
            (Matrix<T, Core> & dst, const T scalar)     \
    {                                                   \
        Matrix<T, Core>::iterator it = dst.begin();     \
        for(; it != dst.end(); ++it)                    \
        {                                               \
            (*it) OP scalar;                            \
        }                                               \
        return dst;                                     \
    }

    Matrix_OPE_Scalar( += )
    Matrix_OPE_Scalar( -= )
    Matrix_OPE_Scalar( *= )
    Matrix_OPE_Scalar( /= )
    Matrix_OPE_Scalar( &= )
    Matrix_OPE_Scalar( |= )
    Matrix_OPE_Scalar( ^= )

    #define Scalar_OP_Matrix(OP)                            \
    template<class T, class Core>                           \
        const Matrix<T, Core> operator OP (                 \
            const T scalar, const Matrix<T, Core> & src)    \
    {                                                       \
        Matrix<T, Core> dst;                                \
        dst.resize (src.rows(), src.cols());                \
        Matrix<T, Core>::iterator id = dst.begin ();        \
        Matrix<T, Core>::const_iterator is = src.begin();   \
        for(; id != dst.end(); ++id, ++is)                  \
        {                                                   \
            (*id) = scalar OP (*is);                        \
        }                                                   \
        return dst;                                         \
    }

    Scalar_OP_Matrix( + )
    Scalar_OP_Matrix( - )
    Scalar_OP_Matrix( * )
    Scalar_OP_Matrix( / )
    Scalar_OP_Matrix( & )
    Scalar_OP_Matrix( | )
    Scalar_OP_Matrix( ^ )

    #define Matrix_OP_Scalar(OP)                            \
    template<class T, class Core>                           \
        const Matrix<T, Core> operator OP (                 \
            const Matrix<T, Core> & src, const T scalar)    \
    {                                                       \
        Matrix<T, Core> dst;                                \
        dst.resize (src.rows(), src.cols());                \
        Matrix<T, Core>::iterator id = dst.begin ();        \
        Matrix<T, Core>::const_iterator is = src.begin();   \
        for(; id != dst.end(); ++id, ++is)                  \
        {                                                   \
            (*id) = (*is) OP scalar ;                       \
        }                                                   \
        return dst;                                         \
    }
    Matrix_OP_Scalar( + )
    Matrix_OP_Scalar( - )
    Matrix_OP_Scalar( * )
    Matrix_OP_Scalar( / )
    Matrix_OP_Scalar( & )
    Matrix_OP_Scalar( | )
    Matrix_OP_Scalar( ^ )

    // ---------------------------------------------------------------------
    // -----------------------------  Matrix Self --------------------------
    // ---------------------------------------------------------------------

    // Single prefix Operand operator
    #define Matrix_OPS_Matrix(OP)                   \
    template<class T, class Core>                   \
        const Matrix<T, Core>  operator OP (        \
            const Matrix<T, Core> & src)            \
    {                                               \
        Matrix<T, Core> dst(src);                   \
        Matrix<T, Core>::iterator id = dst.begin(); \
        for(; id != dst.end(); ++id)                \
        {                                           \
            (*id) = OP (*id);                       \
        }                                           \
        return dst;                                 \
    }

    Matrix_OPS_Matrix( - )
    Matrix_OPS_Matrix( ! )
    Matrix_OPS_Matrix( ~ )

    // no return value
    #define Matrix_OPSS_Matrix(OP)                  \
    template<class T, class Core>                   \
        void operator OP (Matrix<T, Core> & dst)    \
    {                                               \
        Matrix<T, Core>::iterator id = dst.begin(); \
        for(; id != end(); ++id)                    \
        {                                           \
            OP (*id);                               \
        }                                           \
    }

    Matrix_OPSS_Matrix( ++ )
    Matrix_OPSS_Matrix( -- )

    //////////////////////////////////////////////////////////////////
    // class vtl::MatrixDynamic and vtl::MatrixStatic
    // short alias for matrix defination
    //////////////////////////////////////////////////////////////////

    template<sizetype NROWS, sizetype NCOLS, class T>
        class MatrixStatic : 
            public Matrix<T, MatrixTraitsStatic<NROWS, NCOLS, T> > 
        {
            typedef Matrix<T, MatrixTraitsStatic<NROWS, NCOLS, T> > ParentType;
        public:
            CONSTRUCTOR_MATRIX_DEFAULT(MatrixStatic, T)
            CONSTRUCTOR_FROM_PARENT(MatrixStatic, ParentType)
        };

    template<class T>
        class MatrixDynamic : public Matrix<T, MatrixTraitsDynamic<T> > 
        {
            typedef Matrix<T, MatrixTraitsDynamic<T> > ParentType;
        public:
            CONSTRUCTOR_MATRIX_DEFAULT(MatrixDynamic, T)
            CONSTRUCTOR_FROM_PARENT(MatrixDynamic, ParentType)
        };

    //////////////////////////////////////////////////////////////////
    //  Matrix2x2<>, Matrix2x2f, Matrix2x2d
    //////////////////////////////////////////////////////////////////
    template<class T> 
        class Matrix2x2 : public MatrixStatic<2, 2, T>
    {
        typedef MatrixStatic<2, 2, T> ParentType;
    public:
        CONSTRUCTOR_MATRIX_DEFAULT(Matrix2x2, T)
        CONSTRUCTOR_FROM_PARENT(Matrix2x2, ParentType)
    };

    typedef Matrix2x2<float>    Matrix2x2F;
    typedef Matrix2x2<double>   Matrix2x2D;

    //////////////////////////////////////////////////////////////////
    //  Matrix3x3<>, Matrix3x3f, Matrix3x3d
    //////////////////////////////////////////////////////////////////
    template<class T> 
        class Matrix3x3 : public MatrixStatic<3, 3, T>
    {
        typedef MatrixStatic<3, 3, T> ParentType;
    public:
        CONSTRUCTOR_MATRIX_DEFAULT(Matrix3x3, T)
        CONSTRUCTOR_FROM_PARENT(Matrix3x3, ParentType)
    };

    typedef Matrix3x3<float>     Matrix3x3F;
    typedef Matrix3x3<double>    Matrix3x3D;

    //////////////////////////////////////////////////////////////////
    //  Matrix4x4<>, Matrix4x4f, Matrix4x4d
    //////////////////////////////////////////////////////////////////
    template<class T> 
        class Matrix4x4 : public MatrixStatic<4, 4, T>
    {
        typedef MatrixStatic<4, 4, T> ParentType;
    public:
        CONSTRUCTOR_MATRIX_DEFAULT(Matrix4x4, T)
        CONSTRUCTOR_FROM_PARENT(Matrix4x4, ParentType)

        // apply the translate to the matrix by vector v
        template<class Traits>
            void Translate3D (const Vector3<T, Traits>& v)
        {
            Matrix4x4& mm = (*this);
            mm[0][3] = mm[0][0] * v[0] + mm[0][1] * v[1] + mm[0][2] * v[2] + mm[0][3];
            mm[1][3] = mm[1][0] * v[0] + mm[1][1] * v[1] + mm[1][2] * v[2] + mm[1][3];
            mm[2][3] = mm[2][0] * v[0] + mm[2][1] * v[1] + mm[2][2] * v[2] + mm[2][3];
        }

        // scale the matrix by vector (s, 1)
        template<class Traits>
            void Scale3D (const Vector3<T, Traits>& s)
        {
            Matrix4x4& mm = (*this);
            for(int i = 0; i < 4; i ++) 
            {
                for(int j = 0; j < 3; j ++)
                {
                    mm[i][j] *= s[j];
                }
            }
        }
    };

    typedef Matrix4x4<float>     Matrix4x4F;
    typedef Matrix4x4<double>    Matrix4x4D;

}// namespace vtl

#include "vtl_numeric.h"

namespace vtl {
    
    // ---------------------------------------------------------------------
    // ---------- Individual operation on VectorEx or Matrix -----------------
    // ---------------------------------------------------------------------
    
    // Matrix transpose, for non-square matrix
    template<class MD, class MS>
        MD& MatTranspose (OUT MD & dst, IN const MS & src)
    {
        if(&dst == &src)
        {
            if(dst.empty()) return dst;
            assert(dst.rows() == dst.cols());
            for(sizetype i = 0; i < dst.rows (); i ++)
                for(sizetype j = i+1; j < dst.cols (); j ++)
                {
                    std::swap(dst[i][j], dst[j][i]);
                }
        }
        else
        {
            dst.resize (src.cols(), src.rows());            
            for(sizetype i = 0; i < dst.rows (); i ++)
                for(sizetype j = 0; j < dst.cols (); j ++)
                {
                    dst[i][j] = src[j][i];
                }
        }

        return dst;
    }
    
    // Matrix Multiply with Matrix
    // dst = s1s2
    template<class MD, class MS1, class MS2>
        MD& MatMul(OUT MD & dst, IN const MS1& _s1, IN const MS2& _s2)
    {
        const MS1 s1 = ((void*)(&dst) == (void*)(&_s1)) ? MS1(_s1) : _s1;
        const MS2 s2 = ((void*)(&dst) == (void*)(&_s2)) ? MS2(_s2) : _s2;

        const sizetype M = s1.rows ();
        const sizetype N = s1.cols ();
        const sizetype L = s2.cols ();
        
        assert(N == s2 .rows ());
        dst.resize (M, L);
        dst.SetZero();

        for(sizetype m = 0; m < M; m++)
        {
            for(sizetype l = 0; l < L; l++)
            {
                for(sizetype n = 0; n < N; n ++) 
                {
                    dst[m][l] += s1[m][n] * s2[n][l];
                }
            }
        }

        return dst;
    }
    
    // Chain Multiply of Satrix
    // dst = s1*s2*s3
    template<class MD, class MS1, class MS2, class MS3>
        MD&  MatMul3 (OUT MD& dst, 
            IN const MS1& s1, IN const MS2& s2, IN const MS3& s3)
    {
        // MD must be a satrix
        assert(dst.rows () == dst.cols());
        
        MD tmp;
        MatMul (tmp, s1, s2);
        MatMul (dst, tmp, s3);
        return dst;
    }
    
    // Matrix Multiply with VectorEx
    // dst = ms*vs
    template<class VD, class MS, class VS>
        VD&  MatMulVec (OUT VD & dst, IN const MS & ms, IN const VS& _src)
    {
        const VS src = (&dst == &_src) ? VS(_src) : _src;

        const sizetype M = ms.rows ();
        const sizetype N = ms.cols ();
        
        assert(M == dst.size ());
        assert(N == src.size ());

        dst.SetZero();

        for(sizetype m = 0; m < M; m++)
        {        
            for(sizetype n = 0; n < N; n ++) 
            {
                dst[m] += ms[m][n] * src[n];
            }
        }

        return dst;
    }
    
    // Matrix Multiply with VectorEx
    // dst = ms*vs
    template<class VD, class MS, class VS>
        VD& VecMulMat (OUT VD & dst, IN const VS& _src, IN const MS & ms)
    {
        const VS src = (&dst == &_src) ? VS(_src) : _src;

        const sizetype M = ms.rows ();
        const sizetype N = ms.cols ();
        
        assert(M == dst.size ());
        assert(N == src.size ());
        
        dst.SetZero();
        for(sizetype m = 0; m < M; m++)
        {        
            for(sizetype n = 0; n < N; n ++) 
            {
                dst[m] += src[n] * ms[n][m];
            }
        }

        return dst;
    }

} // namespace vtl

#endif//__VTL_MATRIX_HPP__