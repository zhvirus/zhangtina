#pragma once
#include "afxcmn.h"

#include "MainFrm.h"
#include "BmcDoc.h"
#include "BmcView.h"
#include "Image.h"


// CCmdForm form view
class ZH::ZHLog;

class CCmdForm : public CFormView
{	

	typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);
	typedef unsigned *THREADID;
	
	DECLARE_DYNCREATE(CCmdForm)

protected:
	CCmdForm();           // protected constructor used by dynamic creation
	virtual ~CCmdForm();
	static DWORD WINAPI FilterThread(LPVOID param);
public:
	enum { IDD = IDD_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nPreviewStyle;
    int m_nSingleOrVideo;
	BOOL m_bSunLight;
	BOOL m_bSkyLight;
	COLORREF m_SunLight;
	COLORREF m_SkyLight;
	float m_fLightScale;
	float m_fSunlightScale;
	float m_fSkylightScale;
    
	int m_nPhotonNum;
	int m_nSampleRayNumber;
	int m_nSampleLightRayNumber;
	int m_nImageWidth;
	int m_nImageHeight;
	CWinThread *m_pRaytracingThread;
public:	
	BOOL m_CheckBox_ApplyFilter;
	BOOL m_CheckBox_Pixel;
	BOOL m_CheckBox_Perception;
	BOOL m_CheckBox_Normal;
	BOOL m_CheckBox_Depth;
	BOOL m_CheckBox_Material;

	int m_Edit_FilterBoxSize;
	float m_Edit_Pixel;
	float m_Edit_Perception;
	float m_Edit_Normal;
	float m_Edit_Depth;
	float m_Edit_Material;

public:
	afx_msg void OnBnClickedPointRd();
	afx_msg void OnBnClickedWireRd();
	afx_msg void OnBnClickedSolidRd();
	afx_msg void OnStnClickedSunlightColor();
	afx_msg void OnStnClickedSkylightColor();
	afx_msg void OnBnClickedNormalMcRayTracing();
	afx_msg void OnBnClickedBmcRayTracing();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	CSpinButtonCtrl m_cLightScaleSpin;
	afx_msg void OnBnClickedShowRayTracingResult();
	afx_msg void OnBnClickedApplyFilter();
	afx_msg void OnStartFiltering();
	afx_msg void OnShowFilteredImage();
	afx_msg void OnClickSaveTracingResult();
	afx_msg void OnClickLoadTracingResult();

    void saveConfigDataToDoc();
    void loadConfigDataFromDoc();
    void enableControl(bool enable);

	friend UINT Ray_tracing_proc(LPVOID pParam);
	float m_fIntensityScale;
	int m_nThread;
	float m_fTolerance;
	BOOL m_bIrradianceCache;
	BOOL m_bGradientIC;
    BOOL m_bBmcIntegration;
    ZHLog* m_log;

private:	
    void _applyCamera(const CameraInfo& _cameraInfo);

    CProgressCtrl m_pcProgress;
    afx_msg void OnBnClickedRadioColorTriangle();
};

class CCmd2Form;
struct TThreadParam
{
	int _nThread;
	CCmdForm *_theForm;
    CCol4    cSun;
    CCol4    cSky;
    float fTolerance;
    CCmd2Form* _controlPanel2;
    CBmcDoc* _doc;
    char _savePath[MAX_PATH];
    ImageUtility* _img;
    ZH::ZHLog* _log;
};
