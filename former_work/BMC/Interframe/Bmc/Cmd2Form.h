#pragma once
#include "afxwin.h"

class CameraInfo;
class ZH::ZHLog;
// CCmd2Form form view
class CCmdForm;

class CCmd2Form : public CFormView
{

    typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);
    typedef unsigned *THREADID;

	DECLARE_DYNCREATE(CCmd2Form)

protected:
	CCmd2Form();           // protected constructor used by dynamic creation
	virtual ~CCmd2Form();

public:
	enum { IDD = IDD_VIEWFORM_CONTROL2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void _updateDataToDoc();
    static DWORD WINAPI StartFilteringThread(LPVOID param);
	DECLARE_MESSAGE_MAP()
public:
    BOOL m_cbDirect_light;
    BOOL m_cbIndirect_light;
    BOOL m_cbMirror_effect;
    BOOL m_cbCaustic_effect;
    BOOL m_cbDirectional_light;
    BOOL m_cbTotally_photonmap;
public:
    afx_msg void OnBnClickedCheckC2DirectLighting();
    afx_msg void OnBnClickedCheckC2IndirectLighting();
    afx_msg void OnBnClickedCheckC2MirrorEffect();
    afx_msg void OnBnClickedCheckC2CausticEffect();
    afx_msg void OnBnClickedCheckC2DirectionalLighting();
    afx_msg void OnBnClickedCheckC2TotallyPhotonmap();
    afx_msg void OnBnClickedButtonAddcamera();
    afx_msg void OnBnClickedButtonRvmcamera();
    afx_msg void OnBnClickedButtonClearall();
    afx_msg void OnLbnSelchangeListCameralist();
    afx_msg void OnBnClickedButtonCampreview();
    afx_msg void OnBnClickedButtonSelectpath();
public:
    virtual void OnInitialUpdate();
public:
    void saveConfigDataToDoc();
    void loadConfigDataFromDoc();
    void updateCameraListBox();
    void applyCamera(int index);
    void applyCamera(CameraInfo* _camInfo);
    void updateRenderView();
    void enableControl(bool enable);

private:
    CListBox m_lbCameraList;
    CCmdForm* _panel1;
public:
    int m_frameNumber;
public:
    CString m_ebPath;
    char m_sResultPath[MAX_PATH];
public:
    inline char* getResultPath()const{return (char*)(m_sResultPath);}
    inline int getFrameNumber(){/*UpdateData(true);*/return m_frameNumber;}

public:
    afx_msg void OnBnClickedButtonSavepath();
public:
    afx_msg void OnBnClickedButtonLoadpath();
public:
    afx_msg void OnBnClickedButtonVideoFilterCol();
public:
    afx_msg void OnBnClickedButtonVideofilterGeo();
public:
    afx_msg void OnBnClickedButtonStartfilter();
public:
    CString m_ebFilterColorFileName;
public:
    CString m_ebFilterGeoFileName;
public:
    int m_ebFilterFrameNum;
	CString m_videoFilterOutputDir;
	afx_msg void OnBnClickedButtonVideofilterOutput();
	int m_nFilteringThreadNum;

private:
    HANDLE m_filteringHandle;
    DWORD m_filteringID;
    BOOL m_cbMotionBlur;
    afx_msg void OnBnClickedButtonStopVideofilter();
    BOOL m_cbResample;
    float m_ebSampleThreshold;
    
public:
    int m_ebRenderStart;
    int m_ebRenderEnd;
    ZHLog* m_log;

public:
    void _savePathToFile(char* fn);
    bool _resample(
        const std::vector<std::string>& _irraFileVec,
        const std::vector<std::string>& _geoFileVec);
public:
    BOOL m_cbOutputWeightImage;
public:
    BOOL m_ebOutputResampleImages;
public:
    afx_msg void OnBnClickedButtonIrrPath();
public:
    CString m_ebFilterIrraFileName;
};


