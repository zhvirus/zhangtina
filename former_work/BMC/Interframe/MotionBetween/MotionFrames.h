
/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Interfaces of class CKeyMotions

Abstract:
    This program gives the best way for smoothly in-betweening (i.e. interpolating)
    sequences of arbitrary motions, including rotation and translations as well as
    the light positions.

    For rotations interpolation, it is well known that the common solution, using 
    three Euler's angles intepolated independently, is not ideal. Quaternion coordinates 
    are best for interpolating orientations. For example, the set of all possible 
    directions in space of forms a sphere, the so-called Gaussian sphere, on which
    one might want to control the positions of infinitely distant light sources.

    Please read the course "Visualing Quaternions" in Siggraph'2001 for more about
    quaternions.

    Please see the file "demo.cpp" for the usage.

Functionility: 
    This program can interpolate both rotation and scalars. 
    Linear as well as Catmull-Rom interpolations for scalars are implemented. 
    Euler angle interpolation and quanternion interpolation for rotations are implemented.

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/10/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/

#ifndef __LG_MOTION_KEYFRAMES__
#define __LG_MOTION_KEYFRAMES__

#include "std.h"
#include "MathHeader.h"

#include <vector>


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

// interpolation method for rotations
enum
{
    EULERANGLE,             // interpolate Euler angle linearly
    QUATERNION_LINEAR,      // interpolate by Slerp quaternion
    QUATERNION_CUBIC,       // interpolate by cubic quaternion
    ROTATE_METHOD_NUM
};

// interpolation method for scalars
enum
{
    LINEAR,         // linear interpolation
    CUBIC,          // cubic Bezier interpolation ( Catmull-Rom spline )
    SCALAR_METHOD_NUM
};


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

// the scalar element
typedef float       CScalar;


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// class of the key frame of motions
class CKeyMotions
{
protected:

    // class of 4x4 matrix
    class CMatrix4x4LG
    {
    private:
        static  int     m_nDim;     // row or col dimension of the CMatrix3x3LG == 4
        float   m_fElements[16];    // elements of the CMatrix3x3LG

    public:
        CMatrix4x4LG()
        {
            Clear();
        }

        ~CMatrix4x4LG() {}

        float   *GetElementsAddress()
        {
            return m_fElements ;
        }

        void Clear()
        {
            for( int i=0; i<m_nDim*m_nDim; i++ )
            {
                m_fElements[i] = 0.0f;
            }
        }

        CMatrix3x3LG  GetMatrix3x3()
        {
            CMatrix3x3LG  res;
            for( int nRow=0; nRow<m_nDim-1; nRow++ )
            {
                for( int nCol=0; nCol<m_nDim-1; nCol++ )
                {
                    res.setValue( nRow, nCol, GetElement(nRow, nCol) );
                }
            }

            return res;
        }

        void    SetFromMatrix3x3( CMatrix3x3LG mat )
        {
            Clear();
            for( int nRow=0; nRow<m_nDim-1; nRow++ )
            {
                for( int nCol=0; nCol<m_nDim-1; nCol++ )
                {
                    SetElement( nRow, nCol, mat.getValue( nRow, nCol) );
                }
            }

            SetElement( m_nDim-1, m_nDim-1, 1.0f );
        }

        bool    IsMatrix3x3()
        {
            for( int nRow=0; nRow<m_nDim-1; nRow++ )
            {
//                if( fabs( GetElement( nRow, m_nDim-1 ) ) > EPSILON )
                if( !FloatsEqual( GetElement( nRow, m_nDim-1 ), 0.0f ) )
                    return false;
            }

            for( int nCol=0; nCol<m_nDim-1; nCol++ )
            {
//                if( fabs( GetElement( m_nDim-1, nCol ) ) > EPSILON )
                if( !FloatsEqual( GetElement( m_nDim-1, nCol ), 0.0f ) )
                    return false;
            }

//            if( fabs( GetElement( m_nDim-1, m_nDim-1 ) - 1.0f ) > EPSILON )
            if( !FloatsEqual( GetElement( m_nDim-1, m_nDim-1 ), 1.0f ) )
                return false;

            return true;
        }

        float   GetElement( int nRow, int nCol )
        {
            ASSERT( nRow>=0 && nRow<m_nDim && nCol>=0 && nCol<m_nDim );
            return  m_fElements[ Index(nRow, nCol) ];
        }

        void    SetElement( int nRow, int nCol, float val )
        {
            ASSERT( nRow>=0 && nRow<m_nDim && nCol>=0 && nCol<m_nDim );
            m_fElements[ Index(nRow, nCol) ] = val;
        }

        int Index( int nRow, int nCol )
        {
            return nCol*m_nDim + nRow ;
        }

        static  int Size()  { return m_nDim*m_nDim; }
        static  int Dim()  { return m_nDim; }
    };


    class CKeyFrame
    {
    public:
        std::vector<CMatrix4x4LG>      m_arMatrixElems;
        std::vector<CScalar>         m_arScalarElems;

        CKeyFrame() { Clear(); }
        ~CKeyFrame() { Clear(); }
        void Clear() 
        {
            m_arMatrixElems.clear();
            m_arScalarElems.clear();
        }

        void    AddMatrix( CMatrix4x4LG mat ) { m_arMatrixElems.push_back( mat ); }
        void    AddScalar( CScalar value ) { m_arScalarElems.push_back( value ); }

        CMatrix4x4LG  GetMatrix( int nIndex ) 
        {
            ASSERT( (nIndex>=0) && (nIndex<static_cast<int>(m_arMatrixElems.size())) );
            return m_arMatrixElems[ nIndex ]; 
        }
        CScalar     GetScalar( int nIndex ) 
        { 
            ASSERT( (nIndex>=0) && (nIndex<static_cast<int>(m_arScalarElems.size())) );
            return m_arScalarElems[ nIndex ]; 
        }
    };


protected:
    bool    m_bInitial;                 // whether it is be initialized

    ///////////////////////////////////////////////////
    int     m_nRotation;                // number of rotation CMatrix3x3LG for each frame
    int     m_nScalar;                  // number of scalars for each frame

    ///////////////////////////////////////////////////
    std::vector<CKeyFrame>  m_arKeyFrames;    // all the key frames

    // interpolation method
    int     m_nInterpolationMethodForMatrix;
    int     m_ninterpolationMethodForScalar;

public:
    void deleteAllkeyFrames(); 
protected:
    void    Clear();
    int     ElementsLength() { return (CMatrix4x4LG::Size())*m_nRotation + m_nScalar; }
    bool    InquireOneFrameFromBuffer( CKeyMotions::CKeyFrame &frame, float *pData, int nSize );

private:

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------
    // the following routines can also be regarded as global ones
    void    InterpolateMatrix( CMatrix4x4LG &mat, std::vector<CMatrix4x4LG> arMatrix, float fPos, int nInterpolationMethod, bool bIsMatrix=true );
    void    InterpolateMatrixLinear( CMatrix4x4LG &mat, CMatrix4x4LG mat0, CMatrix4x4LG mat1, float t );
    void    InterpolateMatrixCubic( CMatrix4x4LG &mat, CMatrix3x3LG m1, CMatrix3x3LG m2, CMatrix3x3LG m3, CMatrix3x3LG m4, float t );
    void    InterpolateMatrixEuler( CMatrix4x4LG &mat, CMatrix4x4LG mat0, CMatrix4x4LG mat1, float t );

    void    InterpolateScalar( float &val, std::vector<float> arScalar, float fPos, int nInterpolationMethod );
    void    InterpolateScalarLinear( float &val, float val0, float val1, float t );
    void    InterpolateScalarCubic( float &val, float val0, float val1, float val2, float val3, float t );

    int     FindIndexByParameter( float fPos, int nSize );
    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------


public:
	CKeyMotions();
	virtual ~CKeyMotions();

    bool    Init(int nMatrix, int nScalar);
    int     Size()   { return static_cast<int>(m_arKeyFrames.size()); }
    void    SetInterpolationMethods( int nRotation, int nScalar )
    {
        m_nInterpolationMethodForMatrix = nRotation % ROTATE_METHOD_NUM;
        m_ninterpolationMethodForScalar = nScalar % SCALAR_METHOD_NUM;
    }

    //  File I/Os
    bool    SaveList(CString strFileName);
    bool    SaveList(FILE* pFile);
    bool    LoadList(CString strFileName);
    bool    LoadList(FILE* pFile);


    //  Key Frame Operations
    bool    InsertOneFrameFromBuffer( int nPos, float* pData, int nSize, bool bIsMatrix = true );
    bool    AddOneFrameFromBuffer( float* pData, int nSize, bool bIsMatrix = true );

    bool    AddOneFrame( CKeyMotions::CKeyFrame frame );
    bool    DeleteOneFrame(int nPos = -1);


    //  Access Functions : the primary interfaces
    bool    GetFrameToBuffer(int nPos, float* pData, int nSize, bool bIsMatrix = true);
    bool    GetNewFrameToBuffer(float fPos, float* pData, int nSize, bool bIsMatrix = true);
};


#endif  // __LG_MOTION_KEYFRAMES__
