/**************************************************************************************
***************************************************************************************

                        Author: Ligang Liu    
                            Jan, 2003
            Internet Graphics Group, Microsoft Research Asia

Module Name: 
    Implementations of class CKeyMotions

History:
    Created on 01/09/2003 by Ligang Liu ( i-ligliu@microsoft.com )
    Modified on 01/11/2003 by Ligang Liu

***************************************************************************************
**************************************************************************************/



#include "MathHeader.h"
#include "MotionFrames.h"


// initialize the static variables for CMatrix4x4LG
int     CKeyMotions::CMatrix4x4LG::m_nDim = 4;     // the dimension of the 4x4 CMatrix3x3LG


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyMotions::CKeyMotions()
{
    Clear();
}

CKeyMotions::~CKeyMotions()
{
    Clear();
}

// initialization: set the CMatrix3x3LG number and scalar number
bool    CKeyMotions::Init(int nMatrix, int nScalar)
{
    Clear();

    if( nMatrix < 0 || nScalar < 0 )
    {
        PRINT(" Error: negative numbers in Init(): %d %d!\n", nMatrix, nScalar );
        return false;
    }

    m_nRotation = nMatrix;
    m_nScalar = nScalar;
    m_bInitial = true;

    m_nInterpolationMethodForMatrix = QUATERNION_CUBIC;
    m_ninterpolationMethodForScalar = CUBIC;

    return true;
}

// clear all the data
void    CKeyMotions::Clear()
{
    m_nRotation = 0;
    m_nScalar = 0;
    m_bInitial = false;
    m_arKeyFrames.clear();
}


// inquire key frame data from data buffer pData
bool    CKeyMotions::InquireOneFrameFromBuffer( CKeyMotions::CKeyFrame &frame, float *pData, int nSize )
{
    if( !m_bInitial )
    {
        PRINT(" Error: please do initialization first before calling InquireOneFrameFromBuffer()!\n");
        return false;
    }

    if( nSize != ElementsLength() )
    {
        PRINT(" Error: wrong size = %d in InquireOneFrameFromBuffer()!\n", nSize );
        return false;
    }

    //int nCount = 0;
    int nCount = 0;
    // get the CMatrix3x3LG
    int nDim = CMatrix4x4LG::Dim() ;
    for( int iMat=0; iMat<m_nRotation; iMat++ )
    {
        CMatrix4x4LG  newMatrix;
        //for( int nRow=0; nRow<nDim; nRow++ )
        //{
        //    for( int nCol=0; nCol<nDim; nCol++ )
        //    {
        //        newMatrix.SetElement( nRow, nCol, pData[ nCount ] );
        //        nCount++ ;
        //    }
        //}

        memcpy( newMatrix.GetElementsAddress(), pData+nCount, sizeof(float)*16 );
		nCount+=16;
        if( !newMatrix.IsMatrix3x3() )
        {
            PRINT(" Warning: non 3x3 matrix in key motion frame in InquireOneFrameFromBuffer()!\n");
        }
        frame.AddMatrix( newMatrix );
    }

    // get the scalar
    for( int iSc = 0; iSc < m_nScalar; iSc++ )
    {
        frame.AddScalar( pData[nCount++] );
    }

    ASSERT( nCount == nSize );

    return true;
}

// insert a new frame
bool    CKeyMotions::InsertOneFrameFromBuffer( int nPos, float* pData, int nSize, bool bIsMatrix )
{
    if( bIsMatrix ) // get the data by Matrix and Scalar form
    {
        CKeyFrame newFrame;
        if( !InquireOneFrameFromBuffer( newFrame, pData, nSize ) )
            return false;

        // insert a new key frame
        m_arKeyFrames.insert( m_arKeyFrames.begin()+nPos , newFrame);
    }
    else    // get the data by Quaternion and Scalar form
    {
        // not done yet!
        PRINT(" Warning: not support non-matrix type rotation now in InsertOneFrameFromBuffer()!\n" );
        return false;
    }

    return true;
}

bool    CKeyMotions::AddOneFrame( CKeyMotions::CKeyFrame frame )
{
    m_arKeyFrames.push_back( frame );
    return true;
}

// add a new frame
bool    CKeyMotions::AddOneFrameFromBuffer( float* pData, int nSize, bool bIsMatrix )
{
    if( bIsMatrix ) // get the data by Matrix and Scalar form
    {
        CKeyFrame newFrame;
        if( !InquireOneFrameFromBuffer( newFrame, pData, nSize ) )
            return false;

        // add a new key frame
        AddOneFrame(newFrame);
        return true;
    }
    else    // get the data by Quaternion and Scalar form
    {
        // not done yet!
        PRINT(" Warning: not support non-matrix type rotation now in AddOneFrameFromBuffer()!\n" );
        return false;
    }
}

// remove a key frame
bool    CKeyMotions::DeleteOneFrame(int nPos)
{
    if( Size() == 0 )
        return false;

    // clamp the position index
    if( nPos<0 )
        nPos = 0;
    if( nPos>Size()-1 )
        nPos = Size()-1 ;

    m_arKeyFrames.erase( m_arKeyFrames.begin() + nPos );

    return true;
}


// save all the key frames into a file
bool    CKeyMotions::SaveList(FILE* pFile)
{
    // output the number of CMatrix3x3LG and scalars for each frame
    fprintf( pFile, "CMatrix3x3LG=%d  scalar=%d\n\n", m_nRotation, m_nScalar );

    // output the number of frames
    fprintf( pFile, "key_frame_number: %d\n\n", Size() );

    // output each frame
    for( int iFrame = 0; iFrame < Size(); iFrame++ )
    {
        // output the frame index
        fprintf( pFile, "#Keyframe %d\n", iFrame );

        // get the frame for outputing
        CKeyFrame &frame = m_arKeyFrames[ iFrame ];

        // save the CMatrix3x3LG part
        ASSERT( m_nRotation == frame.m_arMatrixElems.size() );
        for( int iMat=0; iMat<m_nRotation; iMat++ )
        {
            // get the CMatrix3x3LG
            CMatrix4x4LG &mat = frame.GetMatrix( iMat );
            int nDim = CMatrix4x4LG::Dim();

            for( int nRow=0; nRow<nDim; nRow++ )
            {
                for( int nCol=0; nCol<nDim; nCol++ )
                {
                    fprintf( pFile, "%9.6f  ", mat.GetElement( nRow, nCol ) );
                }
                fprintf( pFile, "\n" );
            }
            fprintf( pFile, "\n");
        }

        // save the scalar part
        ASSERT( m_nScalar == frame.m_arScalarElems.size() );
        if( m_nScalar > 0 )
        {
            for( int iVal=0; iVal<m_nScalar; iVal++ )
            {
                fprintf(pFile, "%9.6f  ", frame.GetScalar( iVal ) );
            }
            fprintf(pFile, "\n\n" );
        }
    }

    return true;
}


// save all the key frames into a file
bool    CKeyMotions::SaveList(CString strFileName)
{
    FILE *fp;

    if( (fp=fopen(strFileName, "wt") ) == NULL )
    {
        PRINT(" Error : can not write to the file %s in SaveList()!\n", strFileName );
        return false;
    }

    SaveList(fp);

    fclose(fp);
    return true;
}


// load the key frames from a file
bool    CKeyMotions::LoadList(FILE* fp)
{
    // input the number of CMatrix3x3LG and scalars
    int nMatrix, nScalar ;
    fscanf( fp, "CMatrix3x3LG=%d  scalar=%d\n\n", &nMatrix, &nScalar );
    
    // Initialization
    Init( nMatrix, nScalar );

    // number of frames
    int nFrames;
    fscanf( fp, "key_frame_number: %d\n\n", &nFrames );

    // read each frame
    for( int iFrame = 0; iFrame < nFrames; iFrame++ )
    {
        int nIndex;
        fscanf(fp, "#Keyframe %d\n", &nIndex );
        ASSERT( nIndex == iFrame );

        CKeyFrame frame ;

        // load the CMatrix3x3LG part
        for( int iMat=0; iMat<m_nRotation; iMat++ )
        {
            CMatrix4x4LG mat;
            int nDim = CMatrix4x4LG::Dim();
            for( int nRow=0; nRow<nDim; nRow++ )
            {
                for( int nCol=0; nCol<nDim; nCol++ )
                {
                    float elem;
                    fscanf( fp, "%f ", &elem );
                    mat.SetElement( nRow, nCol, elem );
                }
                fscanf( fp, "\n" );
            }
            fscanf( fp, "\n");

            // add a new CMatrix3x3LG to the frame
            frame.AddMatrix( mat );
        }

        // load the scalar part
        if( nScalar > 0 )
        {
            for( int iVal=0; iVal<m_nScalar; iVal++ )
            {
                float value;
                fscanf(fp, "%f ", &value );
                frame.AddScalar( value );
            }
            fscanf(fp, "\n" );
        }

        // add a new key frame
        AddOneFrame( frame );
    }

    return true;
}


// load the key frames from a file
bool    CKeyMotions::LoadList(CString strFileName)
{
    FILE *fp;

    if( (fp=fopen(strFileName, "rt") ) == NULL )
    {
        PRINT(" Error : can not open the file %s in SaveList()!\n", strFileName );
        return false;
    }

    LoadList(fp);

    fclose(fp);
    return true;
}


// get a key frame at some index
bool    CKeyMotions::GetFrameToBuffer(int nPos, float* pData, int nSize, bool bIsMatrix)
{
    pData = NULL;
    if( nSize != ElementsLength() )
    {
        PRINT(" Error: wrong size = %d in GetFrameToBuffer()!\n", nSize );
        return false;
    }

    if( bIsMatrix ) // get the data by Matrix and Scalar form
    {
        // get the frame
        CKeyFrame &frame = m_arKeyFrames[ nPos ];

        // element index of pData
        int nCount = 0;

        // get the CMatrix3x3LG data
        int nDim = CMatrix4x4LG::Dim() ;
        ASSERT( m_nRotation == frame.m_arMatrixElems.size() );
        for( int iMat=0; iMat<m_nRotation; iMat++ )
        {
            // get the CMatrix3x3LG
            CMatrix4x4LG &mat = frame.GetMatrix( iMat );
            for( int nRow=0; nRow<nDim; nRow++ )
            {
                for( int nCol=0; nCol<nDim; nCol++ )
                {
                    pData[nCount++] = mat.GetElement( nRow, nCol );
                }
            }
        }

        // get the scalar data
        ASSERT( m_nScalar == frame.m_arScalarElems.size() );
        if( m_nScalar > 0 )
        {
            for( int iVal=0; iVal<m_nScalar; iVal++ )
            {
                pData[nCount++] = frame.GetScalar( iVal ) ;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}


// get a frame by interpolation at some parameter fPos
bool    CKeyMotions::GetNewFrameToBuffer(float fPos, float* pData, int nSize, bool bIsMatrix)
{
    memset(pData,0,sizeof(float) * nSize);

    if( Size() == 0 )
    {
        PRINT(" Error in GetNewFrameToBuffer(): no key frames!\n" ); 
        return false;
    }

    if( nSize != ElementsLength() )
    {
        PRINT(" Error in GetNewFrameToBuffer(): wrong size = %d\n", nSize );
        return false;
    }

    if( fPos < 0.0f || fPos > (float)Size()-1 )
    {
        PRINT(" (CKeyMotions::GetNewFrameToBuffer,fPos=%f)Waring: time parameter %f exceeding the range in GetNewFrameToBuffer()!\n", fPos,fPos );
        if( fPos < 0.0f )
            fPos = 0.0f ;
        else
            fPos = (float)Size()-1;

		printf("fPos clamped to %f\n",fPos);
    }


    if( bIsMatrix ) // get the data by Matrix and Scalar form
    {
        // element index of pData
        int nCount = 0;

        // for each CMatrix3x3LG
        for( int iMat=0; iMat<m_nRotation; iMat++ )
        {
            // get the CMatrix3x3LG array
            std::vector<CMatrix4x4LG>      arMatrixElems;
            arMatrixElems.clear();
            for( int iFrame = 0; iFrame < Size(); iFrame++ )
            {
                // get the frame
                CKeyFrame &frame = m_arKeyFrames[ iFrame ];

                // get the CMatrix3x3LG
                CMatrix4x4LG &mat = frame.GetMatrix( iMat );
                arMatrixElems.push_back( mat );
            }

            CMatrix4x4LG mat;
            // interpolating rotation CMatrix3x3LG here...
            InterpolateMatrix( mat, arMatrixElems, fPos, m_nInterpolationMethodForMatrix, bIsMatrix );


            // set the CMatrix3x3LG value to the buffer data
            //int nDim = CMatrix4x4LG::Dim() ;
            //for( int nRow=0; nRow<nDim; nRow++ )
            //{
            //    for( int nCol=0; nCol<nDim; nCol++ )
            //    {
            //        pData[nCount++] = mat.GetElement( nRow, nCol );
            //    }
            //}
            memcpy( pData+nCount, mat.GetElementsAddress(), sizeof(float)*16 );
            nCount += 16;
        }

        // for each scalar
        if( m_nScalar > 0 )
        {
            std::vector<CScalar>         arScalarElems;
            for( int iVal=0; iVal<m_nScalar; iVal++ )
            {
                arScalarElems.clear();
                for( int iFrame = 0; iFrame < Size(); iFrame++ )
                {
                    CKeyFrame &frame = m_arKeyFrames[ iFrame ];
                    arScalarElems.push_back( frame.GetScalar(iVal) );
                }

                float val;
                // interpolating scalar here...
                InterpolateScalar( val, arScalarElems, fPos, m_ninterpolationMethodForScalar );
                pData[nCount++] = val ;
            }
        }

        ASSERT( nCount == ElementsLength() );
        return true;
    }
    else    // get the data by Quaternion and Scalar form
    {
        PRINT(" Warning: not support non-matrix rotation form now in GetNewFrameToBuffer()!\n");
        return false;
    }
}





//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//     the following routines can be regarded as global routines
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void    CKeyMotions::InterpolateMatrixCubic( CMatrix4x4LG &mat, CMatrix3x3LG m1, CMatrix3x3LG m2, CMatrix3x3LG m3, CMatrix3x3LG m4, float t )
{
    CQuaternion q1, q2, q3, q4;
    CQuaternion a, b, tmp;

    q1 = Matrix2Quater( m1 );
    q2 = Matrix2Quater( m2 );
    q3 = Matrix2Quater( m3 );
    q4 = Matrix2Quater( m4 );

    q2 = Closetofirst(q1,q2);
    q3 = Closetofirst(q2,q3);
    q4 = Closetofirst(q3,q4);

    /*
    ASSERT( FloatsEqual( q1.Length(), 1.0f ) );
    ASSERT( FloatsEqual( q2.Length(), 1.0f ) );
    ASSERT( FloatsEqual( q3.Length(), 1.0f ) );
    ASSERT( FloatsEqual( q4.Length(), 1.0f ) );
    */
    if( !FloatsEqual( q1.Length(), 1.0f ) || !FloatsEqual( q2.Length(), 1.0f )
        || !FloatsEqual( q3.Length(), 1.0f ) || !FloatsEqual( q4.Length(), 1.0f ) )
    {
        //PRINT(" Non unit quaternions in InterpolateMatrixCubic().\n" );
    }

    a = Slerp(q2,Bisect(Doublequater(q1,q2),q3),1.0f/3.0f);
	tmp = Slerp(q3,Bisect(Doublequater(q2,q3),q4),1.0f/3.0f);
	b = Doublequater(tmp,q3);

	a = Closetofirst(q2,a);
	b = Closetofirst(a, b);
	q3 = Closetofirst(b,q3);

	tmp = DeCasteljau(q2,a,b,q3,t);
    CMatrix3x3LG m = Quater2Matrix( tmp );
    mat.SetFromMatrix3x3( m );
}

// interpolate two rotation CMatrix3x3LG by Slerp
void    CKeyMotions::InterpolateMatrixLinear( CMatrix4x4LG &mat, CMatrix4x4LG mat0, CMatrix4x4LG mat1, float t )
{
    CMatrix3x3LG  m0 = mat0.GetMatrix3x3();
    CMatrix3x3LG  m1 = mat1.GetMatrix3x3();

    CQuaternion q0 = Matrix2Quater( m0 );
    CQuaternion q1 = Matrix2Quater( m1 );

    CQuaternion q = Slerp( q0, q1, t );
    CMatrix3x3LG m = Quater2Matrix( q );
    mat.SetFromMatrix3x3( m );
}


void    CKeyMotions::InterpolateMatrixEuler( CMatrix4x4LG &mat, CMatrix4x4LG mat0, CMatrix4x4LG mat1, float t )
{
    CMatrix3x3LG  m0 = mat0.GetMatrix3x3();
    CMatrix3x3LG  m1 = mat1.GetMatrix3x3();

    CVector3LG  angle0 = Matrix2EulerAngle( m0 );
    CVector3LG  angle1 = Matrix2EulerAngle( m1 );

    CMatrix3x3LG m = EulerAngle2Matrix( LinearInterpolate(t, angle0, angle1 ) );
    mat.SetFromMatrix3x3( m );
}


void    CKeyMotions::InterpolateMatrix( CMatrix4x4LG &mat, std::vector<CMatrix4x4LG> arMatrix, float fPos, int nInterpolationMethod, bool bIsMatrix )
{
    int nSize = arMatrix.size();
    if( nSize == 0 )
    {
        PRINT(" No matrix in key frame array in InterpolateMatrix().\n");
    }

    if( nSize == 1 )
    {
        mat = arMatrix[0];
        return ;
    }

    //-----------------------------------------------------------------
    // 1. find the index, fPos in the interval [ nIndex, nIndex+1 ]
    int nIndex = FindIndexByParameter( fPos, nSize );
    ASSERT( fPos>=((float)nIndex) && fPos<( (float)(nIndex+1) ) );

    // if the time parameter is just the key frame, need not to interpolate
    if( FloatsEqual( fPos, nIndex ) )
    {
        mat = arMatrix[ nIndex ];
        return;
    }

    ASSERT( nIndex>=0 && nIndex<nSize-1 );


    //-----------------------------------------------------------------
    // 2. get the parameter in interpolation
    float t = fPos - (float)nIndex ;


    //-----------------------------------------------------------------
    // 3. if use Euler angle interpolation method
    if(nInterpolationMethod == EULERANGLE)
    {
        InterpolateMatrixEuler( mat, arMatrix[nIndex], arMatrix[nIndex+1], t );
        return ;
    }


    //-----------------------------------------------------------------
    // 4. if use linear quaternion interpolation method
    if( nInterpolationMethod == QUATERNION_LINEAR || nSize == 2 )
    {
        InterpolateMatrixLinear( mat, arMatrix[nIndex], arMatrix[nIndex+1], t );
        return ;
    }
    ASSERT( nInterpolationMethod == QUATERNION_CUBIC );
    ASSERT( nSize > 2 );


    //-----------------------------------------------------------------
    // 5. if use cubic quaternion interpolation method: Catmull-Rom spline
    if( nInterpolationMethod == QUATERNION_CUBIC )
    {
        CMatrix3x3LG m1, m2, m3, m4;
        m2 = arMatrix[nIndex].GetMatrix3x3();
        m3 = arMatrix[nIndex+1].GetMatrix3x3();

        // judge whether two matrices m2 and m3 are equal
        bool bEqual = true;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if( !FloatsEqual( m2.getValue(i,j), m3.getValue(i,j) ) )
                {
                    bEqual = false;
                    break; 
                }
                if( !bEqual )
                    break;
            }
        }

        if( bEqual )
        {
            m1 = m4 = m2 ;
        }
        else
        {
            if( nIndex == 0 )
            {
                m4 = arMatrix[nIndex+2].GetMatrix3x3();
                m1 = m2;
            }
            else if( nIndex == nSize-2 )
            {
                m1 = arMatrix[nIndex-1].GetMatrix3x3();
                m4 = m2;
            }
            else
            {
                ASSERT( nIndex>0 && nIndex<nSize-2 );
                m1 = arMatrix[nIndex-1].GetMatrix3x3();
                m4 = arMatrix[nIndex+2].GetMatrix3x3();
            }
        }

        InterpolateMatrixCubic( mat, m1, m2, m3, m4, t );

        return ;
    }

    ASSERT(0);
}


// interpolate two scalar number by linear method
void    CKeyMotions::InterpolateScalarLinear( float &val, float val0, float val1, float t )
{
    val = LinearInterpolate( t, val0, val1 );
}

void    CKeyMotions::InterpolateScalarCubic( float &val, float val0, float val1, float val2, float val3, float t )
{
    val = DeCasteljau(val0, val1, val2, val3, t);
}

void    CKeyMotions::InterpolateScalar( float &val, std::vector<float> arScalar, float fPos, int nInterpolationMethod )
{
    int nSize = arScalar.size();
    ASSERT( nSize > 0 );

    if( nSize == 1 )
    {
        val = arScalar[0];
        return ;
    }

    //-----------------------------------------------------------------
    // 1. find the index, fPos in the interval [ nIndex, nIndex+1 ]
    int nIndex = FindIndexByParameter( fPos, nSize );
    ASSERT( fPos>=((float)nIndex) && fPos<( (float)(nIndex+1) ) );

    // if the time parameter is just the key frame, need not to interpolate
    if( FloatsEqual( fPos, nIndex ) )
    {
        val = arScalar[ nIndex ];
        return;
    }

    ASSERT( nIndex>=0 && nIndex<nSize-1 );


    //-----------------------------------------------------------------
    // 2. get the parameter in interpolation
    float t = fPos - (float)nIndex ;


    //-----------------------------------------------------------------
    // 3. if use linear interpolation method
    if( nInterpolationMethod == LINEAR || nSize == 2 )
    {
        CScalar val0, val1;
        val0 = arScalar[ nIndex ];
        val1 = arScalar[ nIndex+1 ];
        InterpolateScalarLinear( val, val0, val1, t );
        return ;
    }
    ASSERT( nInterpolationMethod == CUBIC );
    ASSERT( nSize > 2 );


    //-----------------------------------------------------------------
    // 4. if use cubic interpolation method
    if( nInterpolationMethod == CUBIC )
    {
        CScalar val0, val1, val2, val3;
        CScalar p1, p2, p3, p4;
        p2 = arScalar[ nIndex ];
        p3 = arScalar[ nIndex+1 ];
        if( FloatsEqual( p2, p3 ) )
        {
            p1 = p4 = p2;
        }
        else
        {
            if( nIndex == 0 )
            {
                p4 = arScalar[ nIndex+2 ];
                p1 = p2;
            }
            else if( nIndex == nSize-2 )
            {
                p1 = arScalar[ nIndex-1 ];
                p4 = p3;
            }
            else
            {
                ASSERT( nIndex>0 && nIndex<nSize-2 );
                p1 = arScalar[ nIndex-1 ];
                p4 = arScalar[ nIndex+2 ];
            }
        }

        val0 = p2;
        val3 = p3;
        val1 = p2 + (p3-p1)/6.0f;
        val2 = p3 - (p4-p2)/6.0f;
        InterpolateScalarCubic( val, val0, val1, val2, val3, t );
        return ;
    }

    ASSERT( 0 );
}



// find the index in an array
int     CKeyMotions::FindIndexByParameter( float fPos, int nSize )
{
    //ASSERT( fPos >=0.0f && fPos<=(float)(nSize-1+EPSILON) );
    if( fPos < 0.0f || fPos>(float)(nSize-1) )
    {
        PRINT(" Warning: parameter exceed the range in FindIndexByParameter()!\n" );
        if( fPos < 0.0f )
            fPos = 0.0f;
        else
            fPos = (float)(nSize-1) ;
    }

    if( FloatsEqual( fPos, (nSize-1) ) )
        return nSize-1;

    // find the index interval [nIndex, nIndex+1];
    
    int nIdx=1;
    for( ; nIdx<nSize; nIdx++ )
    {
        if( fPos<((float)nIdx) )
            break;
    }

    return (nIdx-1);
}


void CKeyMotions::deleteAllkeyFrames()
{
   int num  = Size(); 
   for(int i =0;i<num;i++)
   {
       DeleteOneFrame(0);
   }


}
