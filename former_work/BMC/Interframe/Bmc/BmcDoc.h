// BmcDoc.h : interface of the CBmcDoc class
//


#pragma once

#include "DlgResult.h"
#include "Config.h"
#include "BmcListTemplate.h"
#include "CameraInfo.h"
#include "..\\MotionBetween\\MotionFrames.h"


#define NO_BUTTON               0x00
#define MOUSE_LEFT_BUTTON       0x01
#define MOUSE_RIGHT_BUTTON      0x02
#define MOUSE_MID_BUTTON        0x04

#define RENDER_DRAW_POINTS      0x01
#define RENDER_DRAW_WIREFRAME   0x02
#define RENDER_DRAW_SOLID       0x04
#define RENDER_DRAW_COLOR_TRIANGLE  0x08

struct RenderEffect
{
    BOOL m_bDirect_light;
    BOOL m_bIndirect_light;
    BOOL m_bMirror_effect;
    BOOL m_bCaustic_effect;
    BOOL m_bDirectional_light;
    BOOL m_bTotally_photonmap;
};


using namespace ZH;
class ZH::ZHLog;
class CCmdForm;
class CBmcDoc : public CDocument
{

protected: // create from serialization only
	CBmcDoc();
	DECLARE_DYNCREATE(CBmcDoc)

// Attributes
protected:
	int    m_nMouseStatus;
	double m_mMM[16], m_mPM[16];
	int    m_nRenderStyle;
	int    m_nKey;
	BOOL   m_bLightOn;

	float  m_fDistScale;
	float  m_fDistance;
	CVec3  m_vTrans, m_vCent, m_vEye;
	int    m_nWinWidth, m_nWinHeight;

    RenderEffect m_renderEffect;
    Config* m_config;
    BmcList<CameraInfo> m_cameraList;
    CKeyMotions  m_keyMotions;
    ZHLog* _log;
    FilterVideo m_FilterVideo;
    
    //rand color for each triangle
    CCol4* m_pRandColor;
    CCmdForm* m_pPanel1;

// Operations
public:
	CDlgResult m_theResultWin;
	CDlgResult m_theFilteredWin;

    inline Config* getConfig(){return m_config;}
    inline BmcList<CameraInfo>& getCameralist(){return m_cameraList;}
    inline CKeyMotions&   getKeyMotions(){return m_keyMotions;}
    inline FilterVideo& getFilterVideo(){return m_FilterVideo;}

    void setRenderEffect( BOOL directLight,
                                    BOOL inDirectLight,
                                    BOOL MirrorEffect,
                                    BOOL CausticEffect,
                                    BOOL DirectionalLight,
                                    BOOL TotallyPhotonMap
                                    );

    void getRenderEffect(RenderEffect& _effect);
	void  SetPreviewStyle (int nStyle)   {m_nRenderStyle = nStyle;}
//	CBmcTR *Get_scene()                  {return ((m_pScene->Is_model_loaded())? m_pScene : NULL);};
//	CArcBall *Get_Arcball()              {return m_pArcBall;}
	void  Set_mouse_status (int nStatus) {m_nMouseStatus = nStatus;}
	int   Get_mouse_status ()            {return m_nMouseStatus;}
	void  Set_key_status (UINT nKey)     {m_nKey = nKey;}
	UINT  Get_key_status ()              {return m_nKey;}

	void  Set_distance(float fDist)      {m_fDistance = fDist;}
	float Get_distance()                 {return m_fDistance;}
	void  Set_distance_scale(float fDistScale) {m_fDistScale = fDistScale;}
	float Get_distance_scale()           {return m_fDistScale;}
	CVec3 Get_eye_pos()                  {return m_vEye;}
	CVec3 Get_cent_pos()                 {return m_vCent;}
	CVec3 Get_translate()                {return m_vTrans;}
	void  Set_translate(CVec3 vTrans)    {m_vTrans = vTrans;}
    void Set_cent(CVec3 _vCent){m_vCent   = _vCent;}
	void Set_eye_pos(CVec3 _vEye) { m_vEye  =  _vEye;}
	void  Init_viewport();

	void  Set_result_image(float fIntensityScale);

	void  DrawScene (int nWidth, int nHeight);
//	CBmcTR *Get_scene_ptr()              {return m_pScene;}

	void  Init_BMC_raytracing(int nWidth,        // Result image width
							  int nHeight,       // Result image height
							  float fLightScale, // Scale the intensity of the light source
							  CCol4 cSun,        // Sunlight intensity
							  CCol4 cSky,        // Skylight intensity
						      int nPhoton,       // Global photon number
							  int nRay,          // Number of ray for MC integration
							  int nLightRay,     // Number of shadow ray
							  float fTolerance,  // The tolerance of irradiance cache
							  BOOL bGradient,    // Consider gradient when using irradaince cache or not
							  BOOL bBmcIntegration// Using BMC or normal MC for integration computation
							  );

	friend UINT Ray_tracing_proc(LPVOID pParam);
// Overrides
public:
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBmcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
//	afx_msg void OnFileOpen();
};


