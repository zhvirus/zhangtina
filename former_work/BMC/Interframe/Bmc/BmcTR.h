#pragma once
#include "mcbsptr.h"
#include "IrraCache.h"
#include "Frame.h"
#include "ZHLock.h"

// The pixel ray-triangle intection information
struct TPxlInfo
{
	CVec3 _vPos;
	CVec3 _vNml;
	CVec3 _vTex;
	int   _nTID;
};


class reSamplePixel;

class CBmcTR : public CMcBspTR
{
    typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);
    typedef unsigned *THREADID;

protected:
	float      m_fLightScale;
	CCol4      m_cSun, m_cSky;
	BOOL       m_bRunning;
	CIrraTree *m_pIrraTree;
	BOOL       m_bBmcOn;
	float      m_fTolerance;
	TPxlInfo   *m_pPxlInfo;
	BOOL       m_bGradient;
	BOOL       m_bBmc;
	int        m_nPolar;
	int        m_nAzimuth;
	CRITICAL_SECTION m_csInsertion; // The critical section for tree subdivision for multi-thread
	CRITICAL_SECTION m_csGetGpIndex;// The critical section to obtain one GP index
	HANDLE hThread[10];
    bool m_bLightInitialzed;
    std::vector<CIrraValue> m_vIrraValue;

    //Locks
    ZHLock m_lkIrradianceCacheTree;
    ZHLock m_lkGaussMap;
    ZHLock m_lkGlobalMap;
    ZHLock m_lkDirectMap;

    //irradiance cache load related
    std::vector<std::string> m_irraCacheFileNameVec;
    std::vector<std::string> m_geoFileNameVec;

	
	///////////////////////////////////////////////////////////////////////////
	//	Temporary variables for gradient computation
	float  *m_fIcGCeof1, // sin (theta_j_) cos (theta_j)^2
		   *m_fIcGCeof2; // sin(theta_j) - sin (theta_j_)
	///////////////////////////////////////////////////////////////////////////
	// Make sure Kd + Ks <= 1 for energy conservative.
public:
    void saveIrradianceTree(char* fileName);
    void saveIncreasedTreeNode(char* fileName);
    void _setIrradianceCacheFileNameVec(const std::vector<std::string>& _vec);
    void _setGeoFileNameVec(const std::vector<std::string>& _vec);
protected:
	virtual void _Check_materials(void);
	void  _Initialize_light_source (float fScale);
	CCol4 _Specular_contribution(TBspRay &ray, CCol4 cCtr, int nDepth);

	CCol4 _Indirect_irradiance (const TBspCross &crs, int nPID, float &fAvgRadius);
	TPxlInfo _Irradiance_checking (CVec3 vPos, CCol4 *pImg,int _offset);

	void  _Indirect_irradiance_gradients (const TBspCross &crs, int nPID, 
										float **fpThe, float **fpPhi, float **fpD,
										CCol4 **cpL, BOOL **bpHit, CVec3 &vX);
	TPxlInfo _Irradiance_checking_gradient (CVec3 vPos, CCol4 *pImg,
										float **fpThe, float **fpPhi, float **fpD,
										CCol4 **cpL, BOOL **bpHit, CVec3 &vX, int _offset);
	void _Gradient_irradiance (TBspCross crs, float **fpThe, float **fpPhi, float **fpD, 
										CCol4 **cpL, BOOL **bpHit, CVec3 &vX, 
										CIrraValue &iv);

	CCol4 _PM_radiance_along_the_ray (TBspRay &ray, int nPID, float &fDist, BOOL &bHitLig);

	void  _MCRT_seg (const int nStart, const int nEnd);
	void  _Build_irradiance_cache_seg (const int nStart, const int nEnd);
	void  _MCRT_using_irradiance_cache_seg (const int nStart, const int nEnd);

	CCol4 _Shade_using_IC (TPxlInfo *pPif, CVec3 vPos);
	void  _Save_irradaince_cache_image (char *fn);

public:
	bool Set_BMC_parameters (float fLightScale, CCol4 cSun, CCol4 cSky,
							 int nPhoton, int nRay, int nLightRay, 
							 float fTolerance, BOOL bGradient, BOOL bBmc);
	void Ray_tracing_BMC (const int nThread);
	virtual void Load_OBJ (char *fn, float fMapSize = 0.f);

    bool DeleteIrradianceCacheTree(){MY_DELETE(m_pIrraTree);return true;}
    bool buildIrradianceCacheTreeFromFile();

	bool applyFilterForSingleFrame(
		int	_filterBoxSize,
		float _torPixelPositionDistance,
		float _torPerceptionDistance,
		float _torNormalDistance,
		float _torDepthDistance,
		float _torMaterialDistance,
		bool _usePixelDistance	=	true,
		bool _usePerceptionDistance = true,
		bool _useNormalDistance = true,
		bool _useDepthDistance = true,
		bool _usematerialDistance =true);
    
    //re sample related
    class _tResampleParam
    {
        public:
          int _row;
          int _col;
          CCol4* _color;
          CBmcTR* _object;
    };

   typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);

    class _tResampleArrayParam
    {
        public:
            int m_nStartPixel;
            int m_nEndPixel;
            std::vector<ZH::reSamplePixel*>* m_vPixels;
            CBmcTR* _object;
    };

    HANDLE m_hResampleHandles[10];
    DWORD m_dResampleIDs[10];
    _tResampleParam m_vResampleParams[10];
    _tResampleArrayParam m_vResampleRenderParam[10];

    bool _reSampleOneFrame(std::vector<ZH::reSamplePixel*>& _vPixels, int _threadNum,int _frameIndex);
    static DWORD WINAPI _sampleOnePixel_func (LPVOID lpParam);
    static DWORD WINAPI _sampleOnePixel_irradianceCache_func (LPVOID lpParam);

	// The thread functions
	friend DWORD WINAPI Nml_thread_func (LPVOID lpParam);
	friend DWORD WINAPI Build_IC_thread_func (LPVOID lpParam);
	friend DWORD WINAPI Bmc_IC_thread_func (LPVOID lpParam);

	CBmcTR(void);
	virtual ~CBmcTR(void);
};

struct TRtThredParam
{
	short _nStart, _nEnd;
	CBmcTR *_theBmcTRIns;
};
