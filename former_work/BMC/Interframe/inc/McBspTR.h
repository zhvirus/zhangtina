// Yanyun Chen
// Internet Graphics Group, Microsoft Research Asia
// September 2003
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhotonMap.h"
#include "bsptr.h"
#include "time.h"


class CStopWatch
{
private:
	LONGLONG m_lStart;
	LONGLONG m_lCurrent;
	LONGLONG m_lFrequency;

public:
	void Start(char *msg)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lStart);
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_lFrequency);
		m_lCurrent = m_lStart;
		printf ("\n%s\n", msg);
	}

	void Show_total_time (char *msg)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lCurrent);
		printf ("\n%s %f seconds\n", msg, (m_lCurrent - m_lStart)/(float)m_lFrequency);
		printf ("%c", 7);
	}

	void Show_last_interval (char *msg)
	{
		LONGLONG t;
		QueryPerformanceCounter((LARGE_INTEGER*)&t);
		printf ("\n%s %f seconds\n", msg, (t - m_lCurrent)/(float)m_lFrequency);
		m_lCurrent = t;
	}

	CStopWatch()
	{
		m_lStart = 0L;
		m_lCurrent = 0L;
		m_lFrequency = 4096L;
	}
	~CStopWatch(){}
};



// The Monte Carlo ray tracing components
#define BSP_MCRT_DIRECT_LIGHTING    0x0001
#define BSP_MCRT_INDIRECT_LIGHTING  0x0002
#define BSP_MCRT_MIRROR_EFFECT      0x0004

#define BSP_MCRT_CAUSTIC_EFFECT     0x0008
#define BSP_MCRT_DIRECTIONAL_LIGHT  0x0010
#define BSP_MCRT_TOTALLY_PHOTON_MAP 0x0020

#define LIGHT_STYLE_SURFACE     0x00
#define LIGHT_STYLE_DIRECTIONAL 0x01

#define MAX_LIGHT_PATCH 100
#define IS_SPECULAR_SURFACE(a) (((a)->m_fSpecular[0] > 0.1f) || \
	((a)->m_fSpecular[1] > 0.1f) || ((a)->m_fSpecular[2] > 0.1f) )

class ZH::PixelInfo;
///////////////////////////////////////////////////////////////////////////////
//		CMcBspTR
// The Monte Carlo ray tracing class using Photon Map technique
///////////////////////////////////////////////////////////////////////////////
class CMcBspTR :
	public CBspTR
{
	///////////////////////////////////////////////////////////////////////////
	//		The light patch structure
	// Used to store the light patch information
	///////////////////////////////////////////////////////////////////////////
	struct TLightPch
	{
		int    nStyle;                  // The light patch style
		int    nTID;                    // The index of the light patch in the triangle list
		CVec3  vNml;                    // Normal of the light patch
		CCol4  cEnergy;                 // Energy of the patch = light color * area of the patch
		CCol4  cPhotonPow;              // Power of the photons emitted by this patch
		int    nPhotonNum;              // Number of photons emitted by this patch 
	};

	///////////////////////////////////////////////////////////////////////////
	//		The caustic direction structure
	// Used to store the information for the creation of caustic photon map.
	///////////////////////////////////////////////////////////////////////////
	struct TCausDir
	{
		int   nLID;                     // Index of the light patch
		CVec3 vPos;                     // Start point of the caustic photons
		CVec3 vDir;                     // The possible direction of caustic photons

		struct TCausDir *next;          // link to next record
	};

protected:

	//m_pixelInfo stores normal, depth and materialIDs for each pixel
    ZH::PixelInfo* m_pixelInfo;

	int       m_nFeatures;
	///////////////////////////////////////////////////////////////////////////
	// Photon map related
	///////////////////////////////////////////////////////////////////////////
	int                m_nGPhotons;             // The number of photons used to create the photon map
	CPhotonMap *m_pGlobalPhotonMap;	    // The global photon map
	CPhotonMap *m_pDirectPhotonMap;	    // The direct lighting photon map
	CPhotonMap *m_pCausticPhotonMap;    // The direct lighting photon map
	BOOL            m_bPhotonMapsInitialized;// Flag
	BOOL            m_bCaustic;              // Flag
	float              m_fLocalRadius;          // Radius of the estimation circle
	int                m_nLocalPhoton;          // The maximum number of photon in the estimation circle

	TCausDir   m_tCausDir;              // Temporary link of possible caustic photon directions
	int        m_nCausDir;              // Number of possible caustic photon directions
	int        m_nCauPhoSubd;           // Caustic photon subdivision number

	CVec3     m_vTopMax;                //
	CVec3     m_vTopMin;			    //

	///////////////////////////////////////////////////////////////////////////
	// Rendering related
	///////////////////////////////////////////////////////////////////////////
	int            m_nDistrRayNum;          // Number of sample rays for one MC calculation
	BOOL       m_bDirectionalLight;     // Flag. If it is true, the directional lighting would be enabled
	int            m_nLightRay;             // Number of sample to estimate the direct OF EACH light patch
	BOOL       m_bEstimateDiffuseIrra;  // Diffuse irradiance of current pixel
	CVec3      m_vLeftBottom;           // The left bottom position of the sampling plane
	CVec3      m_vDeltaX;               // The horizontal interval between two sample pixels
	CVec3      m_vDeltaY;               // The vertical interval between two sample pixels

	CStopWatch m_theStopWatch;			// The stopwatch. Used to show the rendering performance

	///////////////////////////////////////////////////////////////////////////
	// Light patch related
	///////////////////////////////////////////////////////////////////////////
	int        m_nLigPatches;           // Light patches number
	TLightPch  m_tpLigPatches[MAX_LIGHT_PATCH]; // Light patch table

protected:
	///////////////////////////////////////////////////////////////////////////
	// Photon map related
	///////////////////////////////////////////////////////////////////////////

	// Build light source; Make sure Kd + Ks <= 1 for energy conservative.
	virtual void _Check_materials(void);
	// Initial the light patches for the creation of global photon map and ray tracing.
	virtual void _Initial_light_patches(void);
	// Calculate the diffuse and specular reflectance probabilities
	void _Calc_diffuse_and_specular_prob (int &nPID, CCol4 &cPow, CVec3 &vTex, float &fDiffProb, float &fSpecProb);
	// Build the global photon map after loading the scene and checking materials.
	virtual bool _Create_global_photonmap (void);
	// Build the caustic photon map after the creation of global photon map.
	void  _Create_caustic_photonmap(void);
	// Build global and caustic photon maps
	bool  _Create_photon_maps(void);
	// Bounce the global photon in the scene by the Russian-Roulette determination and the reflectivity of the surfaces.
	BOOL  _Trace_global_photon (TBspRay ray, CCol4 cPow, int nPID, int &nStored);
	// Bounce the caustic photon by the specular surface and store it on the first-hit diffuse surface.
	void  _Trace_caustic_photon(TBspRay ray, CCol4 cPow, int nPID, int &nStored);
	// Clean up the created photon maps.
	void  _Delete_photon_maps();

	///////////////////////////////////////////////////////////////////////////
	// Rendering related
	///////////////////////////////////////////////////////////////////////////

	// Calculate the radiance directly come from light sources.
	CCol4 _Direct_lighting (const CVec3 &vPos, const CVec3 &vNml, const CVec3 &vView, const CVec3 &vTex, int PID);
	// Estimate the in-direction irradiance (diffuse).
	CCol4 _Indirect_irradiance (const TBspCross &crs, int nPID, int nDepth);
	// Estimate the radiance at the given direction; Using Monte Carlo integration to calculate the irradiance.
	CCol4 _MC_radiance_along_the_ray (TBspRay &ray, int nDepth, CCol4 &cDiffIrra, int offset=-1);
	// Estimate the radiance at the given direction using photon map
	CCol4 _PM_radiance_along_the_ray (TBspRay &ray, int nPID, BOOL &bHitLight, int offset=-1);
	// Estimate the radiance of a pixel along the given direction using Mento Carlo integration and photon map.
	CCol4 _Pixel_radiance_PM (CVec3 &vPPos, CCol4 &cDiffIrra, int offset = -1);
	// Double the resolution of the ray-tracing result. Sample only those pixels whose neighbor pixels are significantly differernt. 
	void  _Double_image_resolution (CCol4 **cpLow, CCol4 **cpLowIrra,
									CCol4 **cpHig, CCol4 **cpHigIrra,
									int &nWid, int &nHei,
									CVec3 vDeltaX, CVec3 vDeltaY, float fEsp);

	BOOL  _Similar_color (CCol4 &c1, CCol4 &c2, CCol4 cEsp)
	{
		if (cEsp[0] < 1.0e-4f) cEsp.r() = 1.0e-4f;
		if (cEsp[1] < 1.0e-4f) cEsp.g() = 1.0e-4f;
		if (cEsp[2] < 1.0e-4f) cEsp.b() = 1.0e-4f;

		return ((MY_ABS(c1[0] - c2[0]) <= cEsp[0]) &&
			(MY_ABS(c1[1] - c2[1]) <= cEsp[1]) &&
			(MY_ABS(c1[2] - c2[2]) <= cEsp[2]));
	}

	// Check the indexed material. Return true if it is a light patch.
	BOOL  _Is_light_patch(const int & nPID);

public:
    inline ZH::PixelInfo*& getPixelInfo(){return m_pixelInfo;}
	bool  Allocate_Mem_for_photnMap();
	int   Get_photon_number (void) {return m_nGPhotons;}
	void  Set_caustic_photon_subd_ratio (int &nRatio) {m_nCauPhoSubd = nRatio;}
	int   Get_caustic_photon_subd_ratio (void) {return m_nCauPhoSubd;}
	void  Set_direct_lighting_testing_ray_number (const int &nNum) {m_nLightRay = nNum;}
	int   Get_direct_lighting_testing_ray_number (void) {return m_nLightRay;}
	void  Set_indirect_lighting_ray_number (const int &nRay);
	int   Get_indirect_lighting_ray_number (void) {return m_nDistrRayNum;}
	CVec3 Get_lighting_dir() {return m_vLightDir;}

	void  SetLight(const BOOL bDirectional, const CVec3 vLigDir, const CCol4 color);
	void  Set_sample_ray_number_for_each_light_patch(const int nNum) {m_nLightRay = nNum;}
	void  Set_caustic_photon_subdivision_number (const int nNum) {m_nCauPhoSubd = nNum;}

    void setRenderEffect( 
        BOOL directLight,
        BOOL inDirectLight,
        BOOL MirrorEffect,
        BOOL CausticEffect,
        BOOL DirectionalLight,
        BOOL TotallyPhotonMap
        );

	// Set the Monte Carlo ray tracing components
	int Get_MC_rendering_feature (void);
	virtual void Enable (const int nFeatures);
	virtual void Disable (const int nFeatures);
	// Load the obj scene using the function of the base class and then check materials
	virtual void Load_OBJ( char *fn, float fMapSize = 0.f);
	// Normal Monte Carlo ray tracing using global and caustic photon maps
	virtual void MCPM_ray_tracing_normal ();
	// Multi-pass Monte Carlo ray tracing using global and caustic photon maps
	virtual void MCPM_ray_tracing_multi_pass ();
	// Normal Monte Carlo ray tracing with
	virtual void MC_ray_tracing_normal ();

	CMcBspTR(void);
	virtual ~CMcBspTR(void);
protected:
};
