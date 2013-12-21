/*
 * Internet Graphics Group, MSR Asia
 * Microsoft Confidential.  All Rights Reserved.             

 * --------------------------------------------------------------------
 * Permission to use, copy, or modify this software and its documentation
 * for educational and research purposes only and without fee is hereby
 * granted, provided that this copyright notice appear on all copies and
 * supporting documentation.  For any other uses of this software, in
 * original or modified form, including but not limited to distribution
 * in whole or in part, specific prior permission must be obtained from
 * Microsoft research and the authors.  These programs shall not be used,
 * rewritten, or adapted as the basis of a commercial software or hardware
 * product without first obtaining appropriate licenses from MSRCN.  MSRCN
 * makes no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 * --------------------------------------------------------------------
 * 
 * Sept. 2001
 */


// BspTR.h: interface for the CBspTR class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BSP_TEXTURE_
#define __BSP_TEXTURE_

#include "vector3f.h"
#include "color.h"
#include <gl\gl.h>

// Texture
class CBspTexture
{
private:
	int m_nWidth;
	int m_nHeight;
	BYTE *m_pTexData;
	BOOL m_bInitialized;
public:
	// Load the texture image from a BMP file
	void LoadTex(char *fn)
	{
		int rslp, index, index2, x, y;
		BYTE *SImageData;
		
		if (m_nWidth != 0)
		{
			delete [] m_pTexData;
			m_nWidth = m_nHeight = 0;
			m_pTexData = NULL;
		}
		
		BITMAPFILEHEADER head;
		BITMAPINFOHEADER info;
		
		FILE *fp = fopen(fn,"rb");
		if (fp ==NULL) return;
		
		fread(&head,sizeof(BITMAPFILEHEADER),1,fp);
		fread(&info,sizeof(BITMAPINFOHEADER),1,fp);
		m_nWidth  = info.biWidth;
		m_nHeight = info.biHeight;	
		
#ifdef _DEBUG
		printf("\nLoading texture %s (%d,%d).\n", fn, m_nWidth, m_nHeight);
#endif
		
		rslp = m_nWidth * 3;
		while (rslp%4)	rslp ++;
		
		SImageData = new BYTE[rslp*m_nHeight];
		m_pTexData = new BYTE[m_nWidth*m_nHeight*3];
		fread(SImageData,1,rslp*m_nHeight,fp);
		
		for(y = 0; y < m_nHeight; y++)
		{
			index  = y * rslp;
			index2 = y * m_nWidth * 3;
			
			for( x = 0; x < m_nWidth; x++)
			{
				m_pTexData[index2+2] = SImageData[index  ];
				m_pTexData[index2+1] = SImageData[index+1];
				m_pTexData[index2  ] = SImageData[index+2];
				
				index  += 3;
				index2 += 3;
			}
		}
		fclose(fp);
		delete [] SImageData;
		m_bInitialized = TRUE;
	}

	void LoadTex (int nResX, int nResY, BYTE *pBuff)
	{
		if (m_nWidth != 0)
		{
			delete [] m_pTexData;
			m_nWidth = m_nHeight = 0;
			m_pTexData = NULL;
		}

		m_nWidth  = nResX;
		m_nHeight = nResY;

		m_pTexData = new BYTE [m_nWidth * m_nHeight * 3];
		memcpy (m_pTexData, pBuff, m_nWidth * m_nHeight * 3 * sizeof(BYTE));
	}
	
	// Texture image lookup
	CColor4f	TextureColor (const float &u, const float &v)
	{
		int	 x, y, index;
		CColor4f color;
		
		if ( m_nWidth == 0 )
		{
			color.r() = color.g() = color.b() = 1.0f;
		}
		else
		{
			float fIndexU = u - (int)u;
			float fIndexV = v - (int)v;	
			
			if( fIndexU < 0.0f ) fIndexU = 1 + fIndexU;
			if( fIndexV < 0.0f ) fIndexV = 1 + fIndexV;	
			
			x = (int)(fIndexU * m_nWidth);
			y = (int)(fIndexV * m_nHeight);
			
			index = ( y * m_nWidth + x ) * 3;
			color.r() = m_pTexData[index]   * 0.00392f;
			color.g() = m_pTexData[index+1] * 0.00392f;
			color.b() = m_pTexData[index+2] * 0.00392f;
		}		
		return ( color );
	}
	
	void SetTexture()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
//		gluBuild2DMipmaps(GL_TEXTURE_2D, 1,	m_nWidth, m_nHeight, GL_RGB, GL_UNSIGNED_BYTE, m_pTexData);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,	m_nWidth, m_nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pTexData);
	}

	BOOL &IsInitialized() {return m_bInitialized;};
	
	CBspTexture()
	{
		m_nWidth = m_nHeight = 0;
		m_pTexData = NULL;
		m_bInitialized = FALSE;
	}
	
	virtual ~CBspTexture()
	{
		if (m_nWidth != 0) delete [] m_pTexData;
	}
};

#endif
