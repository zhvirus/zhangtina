
#include <stdio.h>

#include "MotionFrames.h"

void    main()
{
    //-----------------------------------------------------------------------
    // 1. create a CKeyMotions object
    CKeyMotions KeyMotionFrames;


    //-----------------------------------------------------------------------
    // 2. initialize the CKeyMotions object
    //    set the number of rotations and number of scalars for each key frame
    int nRotation = 1;  // number of rotations, for example: the matrix obtained from CArcBall
    int nScalar = 7;    // number of scalars, for example: the translations, the scales, 
                        // and the lights positions
    m_KeyFrames.Init( nRotation, nScalar );

    // 2.1 you can set the interpolation methods here (optionally)
    //     default: both cubic method for rotation and scalar
    m_KeyFrames.SetInterpolationMethods( QUATERNION_CUBIC, CUBIC );


    //-----------------------------------------------------------------------
    // 3. set all key frames
    //the length of the buffer for each key frame
    int nLength = nRotation*16 + nScalar;

    // the data buffer of each key frame
    float *pData = new float [ nLength ];
    for( int iFrame = 0; iFrame<nNumKeyFrames; iFrame++ )
    {
        // get the data buffer by some routine
        pData = GetKeyFrameBuffer( );

        // add one key frame
        m_KeyFrames.AddOneFrameFromBuffer( pData, nLength );
    }
    delete [] pData;


    //-----------------------------------------------------------------------
    // 4. get the interpolated new frame by some time parameter fPos in the
    //    range of [0, nNumKeyFrames-1], the frame data are set to the buffer
    //    pData
    m_KeyFrames.GetNewFrameToBuffer( fPos, pData, nLength, true );
}
