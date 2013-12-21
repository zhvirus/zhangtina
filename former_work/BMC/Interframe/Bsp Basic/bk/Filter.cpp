#include "stdafx.h"
#include "Filter.h"
#include "PixelInfo.h"
#include "qmath.h"

BilateralFilter::BilateralFilter(int size):
m_iFilterSize(size),
m_iImageWidth(0),
m_iImageHeight(0),
m_pSourceImage(NULL),
m_pResultImage(NULL),
m_pPixelInfo(NULL),
m_usePixelDistance(true),
m_usePerceptionDistance(true),
m_useNormalDistance(true),
m_useDepthDistance(true),
m_useMaterialDistance(true),
m_pTempImage(NULL),
m_curTempImageSize(512*512)
{
	
	m_torPixelPositionDistance2	=	static_cast<float>(qmathPow((size-1)/2.0f * 1.414f /3.0f, 2.0f));
	m_torColorDistance2	=	m_torPixelPositionDistance2;
	m_torNormalDistance2	=	(1.0f/3.0f) * (1.0f/3.0f);
	m_torDepthDistance2	=	(1.0f/3.0f) * (1.0f/3.0f);
	m_torMaterialDistance2	=	m_torPixelPositionDistance2;
	
	m_pTempImage	=	new CColor4f[m_curTempImageSize];
}

BilateralFilter::~BilateralFilter()
{
	MY_DELETE_ARRAY(m_pTempImage);

}


bool BilateralFilter::setFilterData(
									CColor4f* _sourceImage,
									CColor4f* _resultImage,
									int _imageWidth,
									int _imageHeight,
									int _filterBoxSize,
									PixelInfo* _pixelInfo,
									float _torPixelPositionDistance,
									float _torColorDistance,
									float _torNormalDistance,
									float _torDepthDistance,
									float _torMaterialDistance,
									bool _usePixelDistance,
									bool _usePerceptionDistance,
									bool _useNormalDistance,
									bool _useDepthDistance,
									bool _usematerialDistance
	)
{
	if(_sourceImage==NULL||
		_resultImage==NULL||
		_imageWidth<=0||
		_imageHeight<=0||
		_filterBoxSize<=0||
		_pixelInfo==NULL
		)
	{
		return false;
	}

	m_usePixelDistance			=	_usePixelDistance;
	m_usePerceptionDistance	=	_usePerceptionDistance;
	m_useNormalDistance		=	_useNormalDistance;
	m_useDepthDistance			=	_useDepthDistance;
	m_useMaterialDistance		=	_usematerialDistance;									

	m_pSourceImage	=	_sourceImage;
	m_pResultImage	=	_resultImage;
	m_iImageWidth		=	_imageWidth;
	m_iImageHeight	=	_imageHeight;
	m_iFilterSize			=	_filterBoxSize;
	m_pPixelInfo			=	_pixelInfo;

	if(m_curTempImageSize<_imageWidth*_imageHeight)
	{
		MY_DELETE_ARRAY(m_pTempImage);
		m_pTempImage	=	new CColor4f[_imageHeight*_imageWidth];
		m_curTempImageSize	=	_imageHeight * _imageWidth;
	}

	memset(m_pTempImage,0,m_curTempImageSize*sizeof(CColor4f));
	memcpy(m_pTempImage,m_pSourceImage,_imageHeight * _imageWidth*sizeof(CColor4f));
	
	memset(m_pResultImage,0,_imageHeight * _imageWidth*sizeof(CColor4f));
	
	if(_torPixelPositionDistance!=0)
		m_torPixelPositionDistance2	=	_torPixelPositionDistance * _torPixelPositionDistance;
	if(_torNormalDistance!=0)
		m_torNormalDistance2	=	_torNormalDistance *_torNormalDistance;
	if(_torColorDistance!=0)
		m_torColorDistance2	=	_torColorDistance * _torColorDistance;
	if(_torDepthDistance!=0)
		m_torDepthDistance2	=	_torDepthDistance * _torDepthDistance;
	if(_torMaterialDistance!=0)
		m_torMaterialDistance2	=	_torMaterialDistance * _torMaterialDistance;

	return true;
}


bool BilateralFilter::_filterOnePixel(int _row, int _col)
{
	if(_row<0||_row>m_iImageHeight-1||_col<0||_col>m_iImageWidth -1)
		return false;

	int posRow,posCol;
	float totalWeights = 0.0f;
	CColor4f finalColor;
	finalColor.r()	=	0.0f;
	finalColor.g()	=	0.0f;
	finalColor.b()	=	0.0f;

	float wPixel,wColor,wNormal,wDepth,wMaterial,wFinal;
	float distance2=0.0f;

    //index
    int centerIndex  =  m_iImageWidth*_row+_col;
	int biasIndex;

    //two point color
	//////RGB space
    CColor4f	centerColor	=	m_pSourceImage[centerIndex];
	CColor4f	biasColor;
	//////CIElab space
    CColor4f   CIElab_centerColor;
    CColor4f   CIElab_biasColor;

    CVec3 centerNormal, biasNormal;
	float centerDepth, biasDepth;
	int centerMaterialID, biasMaterialID;
    
    //temp normal dot
    float tempDot;


  
	for(int iBlockRow	=	0; iBlockRow<m_iFilterSize; iBlockRow++)
		for(int iBlockCol	=	0; iBlockCol<m_iFilterSize; iBlockCol++)
		{
			posRow	=	_row	- static_cast<int>(m_iFilterSize/2.0f) + iBlockRow;
			posCol	=	_col	- static_cast<int>(m_iFilterSize/2.0f) + iBlockCol;	

			//if out of range
			if(posRow<0||posRow>m_iImageHeight-1||posCol<0||posCol>m_iImageWidth-1)
			{
				continue;
			}

            biasIndex   =  m_iImageWidth*posRow+posCol;

			biasColor		=	m_pSourceImage[biasIndex];			
			
			//pixel position distance weight calculation
			if(m_usePixelDistance)
			{
				distance2	=	pow(posRow	-	_row,2.0f) + pow(posCol	-	_col, 2.0f);
				wPixel	=	qmathExp(-0.5f/m_torPixelPositionDistance2 * distance2);
			}
			else
			{
				wPixel	=	1.0f;
			}

			
			//color perception distance weight calculation
			if(m_usePerceptionDistance)
			{
                CIElab_centerColor = m_pTempImage[centerIndex];
                CIElab_biasColor    = m_pTempImage[biasIndex];

				distance2	=	pow(CIElab_centerColor.r() - CIElab_biasColor.r(), 2.0f) +
									pow(CIElab_centerColor.g() - CIElab_biasColor.g(), 2.0f)+
									pow(CIElab_centerColor.b() - CIElab_biasColor.b(), 2.0f);
                distance2  /=(CIElab_centerColor.r()*CIElab_centerColor.r()+
                                    CIElab_centerColor.g()*CIElab_centerColor.g()+
                                    CIElab_centerColor.b()*CIElab_centerColor.b());

				wColor	=	qmathExp(-0.5f/m_torColorDistance2 * distance2);
			}
			else
			{
				wColor	=	1.0f;
			}

			//normal  distance weight calculation
			if(m_useNormalDistance)
			{
				centerNormal	=	m_pPixelInfo->m_normal[centerIndex];
				biasNormal	=	m_pPixelInfo->m_normal[biasIndex];
                if(biasNormal==CVec3(0,0,0))
                {
                   wNormal = 0.0f;
                }
                else
                {
                    centerNormal.Normalize();
                    biasNormal.Normalize();
                    tempDot    = centerNormal.Dot(biasNormal);
                    if(tempDot<=0)
                    {
                        wNormal   =  0.0f;
                    }
                    else
                    {
                        distance2	=	tempDot;
                        distance2 *=distance2;
                        wNormal	=	qmathExp(-0.5f/m_torNormalDistance2 * distance2);
                    }

                }


			}
			else
			{
				wNormal	=	1.0f;
			}
	
			
			//depth distance weight calculation
			if(m_useDepthDistance)
			{
				centerDepth	=	m_pPixelInfo->m_depth[centerIndex];
				biasDepth	=	m_pPixelInfo->m_depth[biasIndex];
				distance2	=	(centerDepth	-	biasDepth)	;
                distance2 *=distance2;
				wDepth	=	qmathExp(-0.5f/m_torDepthDistance2 * distance2);
			}
			else
			{
				wDepth	=	1.0f;
			}


			//material distance weight calculation
			if(m_useMaterialDistance)
			{
				centerMaterialID	=	m_pPixelInfo->m_materialID[centerIndex];
				biasMaterialID	=	m_pPixelInfo->m_materialID[biasIndex];
				if(centerMaterialID == biasMaterialID)
					wMaterial	=	1.0f;
				else
					wMaterial	=	0.0f;
			}
			else
			{
					wMaterial	=	1.0f;
			}


			wFinal	=	wPixel * wColor * wNormal * wDepth * wMaterial;
			finalColor.r()	+=	biasColor.r() * wFinal;
			finalColor.g()	+=	biasColor.g() * wFinal;
			finalColor.b() +=	biasColor.b() * wFinal;
			totalWeights	+=wFinal;
			
		}

		finalColor.r()	/= totalWeights;
		finalColor.g() /= totalWeights;
		finalColor.b()	/= totalWeights;

		m_pResultImage[centerIndex]	 = finalColor;

	return true;
}

bool BilateralFilter::startFiltering()
{
	if(	m_pSourceImage==NULL||
		m_pTempImage==NULL||
		m_pResultImage==NULL||
		m_iFilterSize<=0||
		m_iImageWidth<=0||
		m_iImageHeight<=0||
		m_pPixelInfo==NULL
		)
	{
		return false;
	}
	
	for(int row	=	0;	row <m_iImageHeight ; row++)
		for(int col	=0; col<m_iImageWidth ; col++)
		{
			_filterOnePixel(row, col);
		}

	return true;
}
