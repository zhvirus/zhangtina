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

#include "vector3f.h"
#include "Color.h"
#include "BspTexture.h"

#ifndef __BSP_MATERIAL_
#define __BSP_MATERIAL_

class CBspMaterial
{
public:
	char m_sName[256];	// name of material
	char m_sTexture[256];	// name of texture image
	CBspTexture m_iTexture;

	float m_fOpacity;
	float m_fOilFilm;
	float m_fEmissive[4];	// emissive component
	float m_fDiffuse[4];	// diffuse component
	float m_fAmbient[4];	// ambient component
	float m_fSpecular[4];	// specular component
	float m_fShininess[1];	// specular exponent
	
	char * MaterialName() {return (m_sName);}
	float * DiffuseComponent() {return (m_fDiffuse);}
	float * AmbientComponent() {return (m_fAmbient);}
	float * SpecularComponent() {return (m_fSpecular);}
	float * EmissiveComponent() {return (m_fEmissive);}
	float * ShininessComponent() {return (m_fShininess);}
	
	CBspMaterial()
	{
		sprintf (m_sName, "default");;
		m_fOpacity = 1.f;
		m_fOilFilm = 0.f;
		m_fEmissive[0] = m_fEmissive[1] = m_fEmissive[2] = m_fEmissive[3] = 0.0f;
		m_fDiffuse[0] = m_fDiffuse[1] = m_fDiffuse[2] = m_fDiffuse[3] = 1.0f;
		m_fAmbient[0] = m_fAmbient[1] = m_fAmbient[2] = m_fAmbient[3] = 0.1f;
		m_fSpecular[0] = m_fSpecular[1] = m_fSpecular[2] = m_fSpecular[3] = 0.0f;
		m_fShininess[0] = 10;
	}
	virtual ~CBspMaterial() {}
};// ------------------------------------------------------------------

#endif