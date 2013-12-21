// Shaders.h: interface for the CShaders class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADERS_H__503C669F_FCD6_470D_9AA9_5EFCA231536A__INCLUDED_)
#define AFX_SHADERS_H__503C669F_FCD6_470D_9AA9_5EFCA231536A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vector3f.h"
#include "color.h"
#include "BspMaterial.h"

/* A colour system is defined by the CIE x and y coordinates of its
   three primary illuminants and the x and y coordinates of the white
   point. */
struct colourSystem
{
	float	xRed,	yRed,
		xGreen,	yGreen,
		xBlue,	yBlue,
		xWhite,	yWhite;
};

#define CLAMP(v,l,h)    ((v)<(l) ? (l) : (v) > (h) ? (h) : (v))

/* White point chromaticities. */

class CShaders  
{
protected:
	float m_fFilmThickness;
	float m_fRefractionRatio;
	int m_nViewFactor;
	int m_nFilmMaterialID;

	static int inside_gamut(CColor4f color);
	static int constrain_rgb(struct colourSystem *cs, CColor4f &xyz, CColor4f &rgb);
	static void xyz_to_rgb(struct colourSystem *cs, CColor4f color_xyz, CColor4f &color);
	static void spectrum_to_xyz (float (*spec_intens)(int wavelength), CColor4f &color);
	static float s_spectrums[81];

	static float FresnelTerm(float fEta, float fCos_Theta);
	static int GetRGB(float w, float eta, float costheta, CColor4f &color);
	static float GetSpectrum(int wavelength);
	static float GetR(float lamda, float w, float eta, float theta_t);

	float MyPow(float s_cos, int shineness);

public:
	float GetFilmThickness() {return m_fFilmThickness;}
	float GetRefractionRatio() {return m_fRefractionRatio;}
	int GetViewFactor() {return m_nViewFactor;}
	int GetInterferenceMaterialID() {return m_nFilmMaterialID;}

	void SetFilmThickness(float fThickness) {m_fFilmThickness = fThickness;}
	void SetRefractionRatio(float fRatio) {m_fRefractionRatio = fRatio;}
	void SetViewFactor(int nViewFactor) {m_nViewFactor = nViewFactor;}
	void SetInterferenceMaterialID(int nID) {m_nFilmMaterialID = nID;}

	CColor4f PhongShading(CColor4f &LightColor, const CVector3f &vLig,
			      const CVector3f &vView,
			      const CVector3f &vPos, const CVector3f &vNml,
			      float fTexU, float fTexV, CBspMaterial *pMtrl);

	CColor4f PhongShadingInterference (CColor4f LightColor, CVector3f vLig,
			      CVector3f vView,
			      CVector3f vPos, CVector3f vNml,
			      CVector3f vTex, CBspMaterial *pMtrl);

	CShaders();
	virtual ~CShaders();
};

#endif // !defined(AFX_SHADERS_H__503C669F_FCD6_470D_9AA9_5EFCA231536A__INCLUDED_)
