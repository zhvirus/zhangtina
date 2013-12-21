// Shaders.cpp: implementation of the CShaders class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\inc\\Shaders.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IlluminantC     0.3101f, 0.3162f  /* For NTSC television */
#define IlluminantD65   0.3127f, 0.3291f  /* For EBU and SMPTE */

struct colourSystem
			/* xRed   yRed  xGreen yGreen  xBlue  yBlue  White point */
	NTSCsystem  =  { 0.67f,  0.33f,  0.21f,  0.71f,  0.14f,  0.08f,  IlluminantC   },
	EBUsystem   =  { 0.64f,  0.33f,  0.29f,  0.60f,  0.15f,  0.06f,  IlluminantD65 },
	SMPTEsystem =  { 0.630f, 0.340f, 0.310f, 0.595f, 0.155f, 0.070f, IlluminantD65 };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
float CShaders::s_spectrums[81];

CShaders::CShaders()
{
	m_fFilmThickness = 700e-10f;
	m_fRefractionRatio = 2.0f;
	m_nViewFactor = 1;
	m_nFilmMaterialID = -1;
}

CShaders::~CShaders()
{

}

float CShaders::MyPow(float s_cos, int shineness)
{
	int	i;
	float	tmp, rsl = 1.0;
	
	rsl = s_cos;
	
	if( shineness >= 128 )
	{
		tmp = s_cos;
		for( i = 0; i < 7; i ++ ) tmp *= tmp;
		rsl *= tmp;
		return( rsl );
	}
	
	if( shineness > 64 )
	{
		tmp = s_cos;
		for( i = 0; i < 6; i ++ ) tmp *= tmp;
		shineness -= 64;
		rsl *= tmp;
	}
	
	if( shineness > 32 )
	{
		tmp = s_cos;
		for( i = 0; i < 5; i ++ ) tmp *= tmp;
		shineness -= 32;
		rsl *= tmp;
	}
	
	if( shineness > 16 )
	{
		tmp = s_cos;
		for( i = 0; i < 4; i ++ ) tmp *= tmp;
		shineness -= 16;
		rsl *= tmp;
	}
	
	if( shineness > 8 )
	{
		tmp = s_cos;
		for( i = 0; i < 3; i ++ ) tmp *= tmp;
		shineness -= 8;
		rsl *= tmp;
	}
	
	if( shineness > 4 )
	{
		rsl *= (s_cos * s_cos * s_cos * s_cos);
		shineness -= 4;
	}
	
	for( i = 0; i < shineness; i ++ )
		rsl *= s_cos;
	
	return( rsl );
}

CColor4f CShaders::PhongShading(CColor4f &LightColor, const CVector3f &vLig,
			      const CVector3f &vView,
			      const CVector3f &vPos, const CVector3f &vNml,
			      float fTexU, float fTexV, CBspMaterial *pMtrl)
{
	float     d_cos, s_cos;
	CVector3f dv;
	CColor4f  ds_color;
	CColor4f  md, ms;	/*material diffuse iluminace*/
	float     kd, ks;	/*coefficient of surface diffuse*/
	float     shineness;

	md.r() = pMtrl->m_fDiffuse[0];
	md.g() = pMtrl->m_fDiffuse[1];
	md.b() = pMtrl->m_fDiffuse[2];
	kd     = pMtrl->m_fDiffuse[3];

	ms.r() = pMtrl->m_fSpecular[0];
	ms.g() = pMtrl->m_fSpecular[1];
	ms.b() = pMtrl->m_fSpecular[2];
	ks     = pMtrl->m_fSpecular[3];

	shineness = pMtrl->m_fShininess[0];
	
	// The diffuse component
	d_cos = vLig.Dot(vNml);
	if(d_cos <= 0.f)
	{
		md.r() = pMtrl->m_fAmbient[0]*pMtrl->m_fAmbient[3];
		md.g() = pMtrl->m_fAmbient[1]*pMtrl->m_fAmbient[3];
		md.b() = pMtrl->m_fAmbient[2]*pMtrl->m_fAmbient[3];
		if( strlen(pMtrl->m_sTexture) > 4 )
			md = md * pMtrl->m_iTexture.TextureColor(fTexU, fTexV);
		md.Unify();
		return md;//d_cos = 0.f;
	}

	md = md * (d_cos * kd) * LightColor;
	md.r() += pMtrl->m_fAmbient[0]*pMtrl->m_fAmbient[3];
	md.g() += pMtrl->m_fAmbient[1]*pMtrl->m_fAmbient[3];
	md.b() += pMtrl->m_fAmbient[2]*pMtrl->m_fAmbient[3];
	
	md.Unify();

	if( strlen(pMtrl->m_sTexture) > 4 )
		md *= pMtrl->m_iTexture.TextureColor(fTexU, fTexV);

	dv = vLig + vView;
	dv.Normalize();

	s_cos = vNml.Dot(dv);
	s_cos = 2 * s_cos* s_cos -1;

	s_cos = (s_cos < 0.f) ? 0.f : MyPow (s_cos, (int)shineness);
	ms = ms * LightColor * (ks * s_cos);

	ds_color = md + ms;
	ds_color.Unify();

	return(ds_color);
}

///////////////////////////////////////////////////////////
// Fresnel approximation
// F = eta + (1-eta) * (1-cos_theta)^5
float CShaders::FresnelTerm(float fCos_Theta, float fEta)
{
	float f1 = 1 - fCos_Theta;
	float f2 = f1 * f1;
	return (fEta + (1-fEta) * f2 * f2 * f1);
}

float CShaders::GetSpectrum(int wavelength)
{
	assert(wavelength>=380&&wavelength<=780);
	return s_spectrums[(wavelength-380)/5];
}

/*                          SPECTRUM_TO_XYZ

    Calculate the CIE X, Y, and Z coordinates corresponding to a light
    source  with  spectral  distribution   given   by   the   function
    SPEC_INTENS,  which is called with a series of wavelengths between
    380 and 780 nm  (the  argument  is  expressed  in  meters),  which
    returns  emittance  at  that  wavelength  in arbitrary units.  The
    chromaticity coordinates of the spectrum are returned in the x, y,
    and z arguments which respect the identity:

            x + y + z = 1.
*/

void CShaders::spectrum_to_xyz(float (*spec_intens)(int wavelength), CColor4f &color)
{
	int i;
	float lambda, X = 0, Y = 0, Z = 0, XYZ;
	
    /* CIE colour matching functions xBar, yBar, and zBar for
       wavelengths from 380 through 780 nanometers, every 5
       nanometers.  For a wavelength lambda in this range:

            cie_colour_match[(lambda - 380) / 5][0] = xBar
            cie_colour_match[(lambda - 380) / 5][1] = yBar
            cie_colour_match[(lambda - 380) / 5][2] = zBar

       To  save  memory,  this  table can be declared as floats rather
       than doubles; (IEEE)  float  has  enough  significant  bits  to
       represent  the values.  It's declared as a FLOAT here to avoid
       warnings about "conversion between floating-point  types"  from
       certain persnickety compilers. */

	static float cie_colour_match[81][3] = {
		{0.0014f,0.0000f,0.0065f}, {0.0022f,0.0001f,0.0105f}, {0.0042f,0.0001f,0.0201f},
		{0.0076f,0.0002f,0.0362f}, {0.0143f,0.0004f,0.0679f}, {0.0232f,0.0006f,0.1102f},
		{0.0435f,0.0012f,0.2074f}, {0.0776f,0.0022f,0.3713f}, {0.1344f,0.0040f,0.6456f},
		{0.2148f,0.0073f,1.0391f}, {0.2839f,0.0116f,1.3856f}, {0.3285f,0.0168f,1.6230f},
		{0.3483f,0.0230f,1.7471f}, {0.3481f,0.0298f,1.7826f}, {0.3362f,0.0380f,1.7721f},
		{0.3187f,0.0480f,1.7441f}, {0.2908f,0.0600f,1.6692f}, {0.2511f,0.0739f,1.5281f},
		{0.1954f,0.0910f,1.2876f}, {0.1421f,0.1126f,1.0419f}, {0.0956f,0.1390f,0.8130f},
		{0.0580f,0.1693f,0.6162f}, {0.0320f,0.2080f,0.4652f}, {0.0147f,0.2586f,0.3533f},
		{0.0049f,0.3230f,0.2720f}, {0.0024f,0.4073f,0.2123f}, {0.0093f,0.5030f,0.1582f},
		{0.0291f,0.6082f,0.1117f}, {0.0633f,0.7100f,0.0782f}, {0.1096f,0.7932f,0.0573f},
		{0.1655f,0.8620f,0.0422f}, {0.2257f,0.9149f,0.0298f}, {0.2904f,0.9540f,0.0203f},
		{0.3597f,0.9803f,0.0134f}, {0.4334f,0.9950f,0.0087f}, {0.5121f,1.0000f,0.0057f},
		{0.5945f,0.9950f,0.0039f}, {0.6784f,0.9786f,0.0027f}, {0.7621f,0.9520f,0.0021f},
		{0.8425f,0.9154f,0.0018f}, {0.9163f,0.8700f,0.0017f}, {0.9786f,0.8163f,0.0014f},
		{1.0263f,0.7570f,0.0011f}, {1.0567f,0.6949f,0.0010f}, {1.0622f,0.6310f,0.0008f},
		{1.0456f,0.5668f,0.0006f}, {1.0026f,0.5030f,0.0003f}, {0.9384f,0.4412f,0.0002f},
		{0.8544f,0.3810f,0.0002f}, {0.7514f,0.3210f,0.0001f}, {0.6424f,0.2650f,0.0000f},
		{0.5419f,0.2170f,0.0000f}, {0.4479f,0.1750f,0.0000f}, {0.3608f,0.1382f,0.0000f},
		{0.2835f,0.1070f,0.0000f}, {0.2187f,0.0816f,0.0000f}, {0.1649f,0.0610f,0.0000f},
		{0.1212f,0.0446f,0.0000f}, {0.0874f,0.0320f,0.0000f}, {0.0636f,0.0232f,0.0000f},
		{0.0468f,0.0170f,0.0000f}, {0.0329f,0.0119f,0.0000f}, {0.0227f,0.0082f,0.0000f},
		{0.0158f,0.0057f,0.0000f}, {0.0114f,0.0041f,0.0000f}, {0.0081f,0.0029f,0.0000f},
		{0.0058f,0.0021f,0.0000f}, {0.0041f,0.0015f,0.0000f}, {0.0029f,0.0010f,0.0000f},
		{0.0020f,0.0007f,0.0000f}, {0.0014f,0.0005f,0.0000f}, {0.0010f,0.0004f,0.0000f},
		{0.0007f,0.0002f,0.0000f}, {0.0005f,0.0002f,0.0000f}, {0.0003f,0.0001f,0.0000f},
		{0.0002f,0.0001f,0.0000f}, {0.0002f,0.0001f,0.0000f}, {0.0001f,0.0000f,0.0000f},
		{0.0001f,0.0000f,0.0000f}, {0.0001f,0.0000f,0.0000f}, {0.0000f,0.0000f,0.0000f}
	};
	
	for (i = 0, lambda = 380; lambda < 780.1; i++, lambda += 5)
	{
		float Me;
		
		Me = (*spec_intens)((int)lambda);
		X += Me * cie_colour_match[i][0];
		Y += Me * cie_colour_match[i][1];
		Z += Me * cie_colour_match[i][2];
	}
	XYZ = (X + Y + Z);
	color.r() = X / XYZ;
	color.g() = Y / XYZ;
	color.b() = Z / XYZ;
}

/*                             XYZ_TO_RGB

    Given an additive tricolour system CS, defined by the  CIE  x  and  y
    chromaticities  of  its  three  primaries (z is derived trivially as
    1-(x+y)), and a desired chromaticity (XC,  YC,  ZC)  in  CIE  space,
    determine  the  contribution of each primary in a linear combination
    which  sums  to  the  desired  chromaticity.    If   the   requested
    chromaticity falls outside the Maxwell triangle (colour gamut) formed
    by the three primaries, one of the  r,  g,  or  b  weights  will  be
    negative.   Use  inside_gamut()  to  test  for  a  valid  colour  and
    constrain_rgb() to desaturate an outside-gamut colour to the  closest
    representation within the available gamut. */

void CShaders::xyz_to_rgb(colourSystem *cs, CColor4f color_xyz, CColor4f &color)
{
	float xr, yr, zr, xg, yg, zg, xb, yb, zb, d;
	
	xr = cs->xRed;    yr = cs->yRed;    zr = 1 - (xr + yr);
	xg = cs->xGreen;  yg = cs->yGreen;  zg = 1 - (xg + yg);
	xb = cs->xBlue;   yb = cs->yBlue;   zb = 1 - (xb + yb);
	
	d = xr*yg*zb - xg*yr*zb - xr*yb*zg + xb*yr*zg + xg*yb*zr - xb*yg*zr;
	
	color.r() = (-xg*color_xyz.g()*zb + color_xyz.r()*yg*zb + xg*yb*color_xyz.b() -
		xb*yg*color_xyz.b() - color_xyz.r()*yb*zg + xb*color_xyz.g()*zg) / d;
	color.g() = (xr*color_xyz.g()*zb - color_xyz.r()*yr*zb - xr*yb*color_xyz.b() +
		xb*yr*color_xyz.b() + color_xyz.r()*yb*zr - xb*color_xyz.g()*zr) / d;
	color.b() = (xr*yg*color_xyz.b() - xg*yr*color_xyz.b() - xr*color_xyz.g()*zg +
		color_xyz.r()*yr*zg + xg*color_xyz.g()*zr - color_xyz.r()*yg*zr) / d;
}

/*                            INSIDE_GAMUT

    Test  whether  a requested colour is within the gamut achievable with
    the primaries of the current colour system.  This amounts  simply  to
    testing whether all the primary weights are non-negative. */

int CShaders::inside_gamut(CColor4f color)
{
    return ( (color.r() >= 0) && (color.g() >= 0) && (color.b() >= 0) );
}

/*                           CONSTRAIN_RGB

    If  the  requested RGB shade contains a negative weight for one of
    the primaries, it lies outside the colour gamut accessible from the
    given triple of primaries.  Desaturate it by mixing with the white
    point of the colour system so as to reduce  the  primary  with  the
    negative weight to  zero.   This  is  equivalent  to  finding  the
    intersection  on the CIE diagram of a line drawn between the white
    point and the  requested  colour  with  the  edge  of  the  Maxwell
    triangle formed by the three primaries. */

int CShaders::constrain_rgb(struct colourSystem *cs, CColor4f &xyz, CColor4f &rgb)
{
	/* Is the contribution of one of the primaries negative ? */
	
	if (!inside_gamut(rgb))
	{
		float par;
		CColor4f w;
		
		/* Yes.  Find the RGB mixing weights of the white point (we
		   assume the white point is in the gamut!). */
		
		xyz_to_rgb(cs, CColor4f(cs->xWhite, cs->yWhite, 1 - (cs->xWhite + cs->yWhite)), w);
		
		/* Find the primary with negative weight and calculate the
		parameter of the point on the vector from the white point
		to the original requested colour in RGB space. */
		
		if (rgb.r() < rgb.g() && rgb.r() < rgb.b()) {
			par = w.r() / (w.r() - rgb.r());
		} else if (rgb.g() < rgb.r() && rgb.g() < rgb.b()) {
			par = w.g() / (w.g() - rgb.g());
		} else {
			par = w.b() / (w.b() - rgb.b());
		}
		
		/* Since XYZ space is a linear transformation of RGB space, we
		   can find the XYZ space coordinates of the point where the
		   edge of the gamut intersects the vector from the white point
		   to the original colour by multiplying the parameter in RGB
		   space by the difference vector in XYZ space. */
		
		xyz.r() = CLAMP(cs->xWhite + par * (xyz.r() - cs->xWhite), 0, 1);
		xyz.g() = CLAMP(cs->yWhite + par * (xyz.g() - cs->yWhite), 0, 1);
		xyz.b() = CLAMP(1 - (xyz.r() + xyz.g()), 0, 1);
		
		/* Now finally calculate the gamut-constrained RGB weights. */
		
		rgb.r() = CLAMP(w.r() + par * (rgb.r() - w.r()), 0, 1);
		rgb.g() = CLAMP(w.g() + par * (rgb.g() - w.g()), 0, 1);
		rgb.b() = CLAMP(w.b() + par * (rgb.b() - w.b()), 0, 1);
		return 1;	/* Colour modified to fit RGB gamut */
	}
	return 0;		/* Colour within RGB gamut */
}

int CShaders::GetRGB(float w, float eta, float costheta, CColor4f &color)
{
	int i, lamda;
	float sina;
//	float t;
	CColor4f color_xyz;
	struct colourSystem *cs = &NTSCsystem;

	float sinetheta  = (float)sqrt(1-costheta*costheta);
	float nsinetheta = sinetheta/eta;
	float ntheta_t   = (float)asin(nsinetheta);
	float a = 2 * 3.1416f * w * eta * costheta;
	for(i=0; i<=80; i++)
	{
		lamda = (int) (380 + 400 * (i / 80.0f) );
		sina = (float)(sin(a/lamda/1e-9));
		s_spectrums[i] = 4 * FresnelTerm (costheta, eta) * sina * sina;
	}
	
	spectrum_to_xyz(GetSpectrum, color_xyz);
	xyz_to_rgb(cs, color_xyz, color);
	return constrain_rgb(cs, color_xyz, color);
}

#define VIEW_FACTOR 10
CColor4f CShaders::PhongShadingInterference (CColor4f LightColor, CVector3f vLig,
			      CVector3f vView,
			      CVector3f vPos, CVector3f vNml,
			      CVector3f vTex, CBspMaterial *pMtrl)
{
	CColor4f c1, c2;
	c1 = PhongShading (LightColor, vLig, vView, vPos, vNml, vTex.x, vTex.y, pMtrl);

	vLig.Normalize();
	vView.Normalize();
	vNml.Normalize();

	if ( vLig.Dot(vNml)  > 0.f) 
	{
		//GetRGB(115000e-10, 3.2f,(vLig & vNml) + (1 + (vView&vNml))/VIEW_FACTOR, c2);
		GetRGB(m_fFilmThickness, m_fRefractionRatio,
			vLig.Dot(vNml) + (1 + (vView.Dot(vNml)))/m_nViewFactor, c2);

		c1 = c1 + c2;
		c1.Unify();
	}
	return (c1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
#define AMBIENT 0.2f
#define SCALE_OFFSET(x) (AMBIENT+(x*(1-AMBIENT)))

void CookTShader(CVector3f vN, CVector3f vL, CVector3f vV, CColor4f color, float *dc,float *sc)
{
	D3DXCOLOR *c=(D3DXCOLOR*)color;
	D3DXVECTOR3 *nrm=(D3DXVECTOR3 *)N;
	D3DXVECTOR3 *light=(D3DXVECTOR3 *)L;
	D3DXVECTOR3 *view=(D3DXVECTOR3 *)V;

	float Cln=vN & vL;
	xBRDF_cook brdf;

	brdf.SetDiffuseColor(dc);
	brdf.SetNormalIncidenceColor(sc);
	brdf.Value(color,*light, *view, *nrm);
	if(Cln>0){
		D3DXColorScale(c,c,Cln);
		c->a=1;
	}else{
		D3DXColorScale(c,c,Cln*Cln/2);
		c->r=c->g=c->b=0.5f;
		c->a=0;
	}


HRESULT CShadingTool::Shader(float *N, float *L, float *V,float *color,float *dc,float *sc,float power)
{
	D3DXCOLOR *c=(D3DXCOLOR*)color;
	D3DXVECTOR3 *nrm=(D3DXVECTOR3 *)N;
	D3DXVECTOR3 *light=(D3DXVECTOR3 *)L;
	D3DXVECTOR3 *view=(D3DXVECTOR3 *)V;
	float Cln=D3DXVec3Dot(nrm,light);
	FLOAT RefractionIndex=2.2f;
	D3DXCOLOR *Dc=(D3DXCOLOR*)dc;
	D3DXCOLOR *Sc=(D3DXCOLOR*)sc;
	c->a=c->r=c->g=c->b=0;
	if(D3DXVec3Dot(nrm,view)<0){
		c->r=c->g=c->b=0.1f;
		c->a=1;

		return ADE_GENERAL;
	}
	float F;
	//float T=F;
	float d,s;
	d=Cln;
	if(Cln>=0){
		F=D3DXFresnelTerm(Cln,RefractionIndex);
		D3DXVECTOR3 halfv=*light+*view;
		D3DXVec3Normalize(&halfv,&halfv);
		s=D3DXVec3Dot(&halfv,nrm);
		if(s<0)s=0;
		s=pow(s,power);
		s*=F;
		d*=(1-F)*(1-F);
	}else{
		F=D3DXFresnelTerm(-Cln,RefractionIndex);
		s=-D3DXVec3Dot(view,light);
		if(s<0)s=0;
		s=pow(s,power);
		s*=(1-F)*F;
		d*=-(1-F)*(1-F)/2;
	}
	*c=d**Dc*Dc->a+s**Sc*Sc->a;
	//*c=d**Dc*Dc->a;//*c=s**Sc*Sc->a;
	*c*=4;

	c->r=1;
	c->g=1;
	c->b=1;

	assert(c->r>=0);
	assert(c->g>=0);
	assert(c->b>=0);
	c->a=1;

	//char temp[1256];	sprintf(temp,"F:%3.2f C:%3.2f nrm:%3.2f %3.2f %3.2f light:%3.2f %3.2f %3.2f \n",F,Cln,nrm->x,nrm->y,nrm->z,light->x,light->y,light->z);	OutputDebugString(temp);
	
	return NULL;
}

#define VIEW_FACTOR 10
HRESULT CShadingTool::CookTSeperated(float*N,float *L,float* V,float *color,float *dc,float *sc){
	D3DXCOLOR *c=(D3DXCOLOR*)color;
	D3DXVECTOR3 *nrm=(D3DXVECTOR3 *)N;
	D3DXVECTOR3 *light=(D3DXVECTOR3 *)L;
	D3DXVECTOR3 *view=(D3DXVECTOR3 *)V;
	float Cln=D3DXVec3Dot(nrm,light);
	xBRDF_cook brdf;
	brdf.SetDiffuseColor(dc);
	brdf.SetNormalIncidenceColor(sc);

	brdf.ValueSeperate(*c,*light,*view,*nrm);

	if(Cln>0){
		*c*=Cln;
	}else{
		*c*=Cln*Cln/2;
	}
	assert(c->r>=0);
	assert(c->g>=0);
	assert(c->b>=0);
	assert(c->a>=0);

	//SCALE_OFFSET(c->a);

	return NULL;
}

#define SPECULAR_FACTOR 0.9f
#define SCALE_ONLY(x) (x*SPECULAR_FACTOR)

BOOL CookTSeperatedAndInterference(CVector3f vN, CVector3f vL, CVector3f vV, CColor4f color, float *dc, float *sc)
{
	CookTSeperated(vN, vL, vV, color, dc, sc);

	D3DXCOLOR *c=(D3DXCOLOR*)color;
	D3DXVECTOR3 *nrm=(D3DXVECTOR3 *)N;
	D3DXVECTOR3 *light=(D3DXVECTOR3 *)L;
	D3DXVECTOR3 *view=(D3DXVECTOR3 *)V;

	SCALE_ONLY(c->r);
	SCALE_ONLY(c->g);
	SCALE_ONLY(c->b);

	D3DXCOLOR interference;
	float Cln=D3DXVec3Dot(nrm,light);
	float Cvn=D3DXVec3Dot(nrm,view);
	CInterferenceTool::GetRGB(115000e-10, 4.2f, Cln+(1+Cvn)/VIEW_FACTOR, &interference.r(),&interference.g(),&interference.b());

	c->r+=interference.r();
	c->g+=interference.g();
	c->b+=interference.b();

	return TRUE;
}
*/
