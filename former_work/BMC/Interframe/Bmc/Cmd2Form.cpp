// Cmd2Form.cpp : implementation file
//

#include "stdafx.h"
#include "Bmc.h"
#include "Cmd2Form.h"
#include "CmdForm.h"
#include "BmcDoc.h"
#include "Config.h"
#include "BmcView.h"
#include "MainFrm.h"
#include <fstream>
#include "BmcTR.h"
#include "Frame.h"
#include "Log.h"
#include "Singleton.h"
#include "Watch.h"

using namespace ZH;
// CCmd2Form
extern CArcBall* G_pArcBall;
extern CBmcTR *G_pScene;

IMPLEMENT_DYNCREATE(CCmd2Form, CFormView)

CCmd2Form::CCmd2Form()
	: CFormView(CCmd2Form::IDD)
    , m_log(NULL)
    , m_cbDirect_light(TRUE)
    , m_cbIndirect_light(TRUE)
    , m_cbMirror_effect(TRUE)
    , m_cbCaustic_effect(TRUE)
    , m_cbDirectional_light(FALSE)
    , m_cbTotally_photonmap(FALSE)
    , m_frameNumber(50)
    , m_ebPath("C:\\")
    , m_ebFilterColorFileName(_T(""))
    , m_ebFilterGeoFileName(_T(""))
    , m_ebFilterFrameNum(5)
	, m_videoFilterOutputDir(_T(""))
	, m_nFilteringThreadNum(4)
    , m_cbMotionBlur(true)
    , m_cbResample(FALSE)
    , m_ebSampleThreshold(150.0f)
    , m_ebRenderStart(0)
    , m_ebRenderEnd(49)
    , m_cbOutputWeightImage(FALSE)
    , m_ebOutputResampleImages(true)
    , m_ebFilterIrraFileName(_T(""))
{

    m_sResultPath[0]  =  'C';
    m_sResultPath[1]  =  ':';
    m_sResultPath[2]  =  '\\';
    m_sResultPath[3]  =  '\0';

    //get Log 
    m_log   =  Singleton<ZHLog>::getSingletonPtr();

}

CCmd2Form::~CCmd2Form()
{
}

void CCmd2Form::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_C2_DIRECT_LIGHTING, m_cbDirect_light);
    DDX_Check(pDX, IDC_CHECK_C2_INDIRECT_LIGHTING, m_cbIndirect_light);
    DDX_Check(pDX, IDC_CHECK_C2_MIRROR_EFFECT, m_cbMirror_effect);
    DDX_Check(pDX, IDC_CHECK_C2_CAUSTIC_EFFECT, m_cbCaustic_effect);
    DDX_Check(pDX, IDC_CHECK_C2_DIRECTIONAL_LIGHTING, m_cbDirectional_light);
    DDX_Check(pDX, IDC_CHECK_C2_TOTALLY_PHOTONMAP, m_cbTotally_photonmap);
    DDX_Control(pDX, IDC_LIST_CAMERALIST, m_lbCameraList);
    DDX_Text(pDX, IDC_EDIT_FRAMENUMBER, m_frameNumber);
    DDV_MinMaxInt(pDX, m_frameNumber, 1, 10000);
    DDX_Text(pDX, IDC_EDIT_RESULTPATH, m_ebPath);
    DDX_Text(pDX, IDC_EDIT_COLORFILENAME, m_ebFilterColorFileName);
    DDX_Text(pDX, IDC_EDIT_GEOFILENAME, m_ebFilterGeoFileName);
    DDX_Text(pDX, IDC_EDIT_FILTER_FRAME_NUM, m_ebFilterFrameNum);
    DDV_MinMaxInt(pDX, m_ebFilterFrameNum, 2, 100);
    DDX_Text(pDX, IDC_EDIT_OUTPUTDIR, m_videoFilterOutputDir);
    DDX_Text(pDX, IDC_EDIT5_THREAD_NUM, m_nFilteringThreadNum);
    DDV_MinMaxInt(pDX, m_nFilteringThreadNum, 1, 10);
    DDX_Check(pDX, IDC_CHECK_MOTION_BLUR, m_cbMotionBlur);
    DDX_Check(pDX, IDC_CHECK_RESAMPLE, m_cbResample);
    DDX_Text(pDX, IDC_EDIT_SAMPLE_THRESHOLD,m_ebSampleThreshold);
    DDX_Text(pDX, IDC_EDIT_RENDER_FROM, m_ebRenderStart);
    DDV_MinMaxInt(pDX, m_ebRenderStart, 0, 10000);
    DDX_Text(pDX, IDC_EDIT_RENDER_END, m_ebRenderEnd);
    DDV_MinMaxInt(pDX, m_ebRenderEnd, 1, 10000);
    DDX_Check(pDX, IDC_CHECK_OUT_WEIGHTS, m_cbOutputWeightImage);
    DDX_Check(pDX, IDC_CHECK6, m_ebOutputResampleImages);
    DDX_Text(pDX, IDC_EDIT_IRRADIANCECACHE_PATH, m_ebFilterIrraFileName);
}

BEGIN_MESSAGE_MAP(CCmd2Form, CFormView)
    ON_BN_CLICKED(IDC_CHECK_C2_DIRECT_LIGHTING, &CCmd2Form::OnBnClickedCheckC2DirectLighting)
    ON_BN_CLICKED(IDC_CHECK_C2_INDIRECT_LIGHTING, &CCmd2Form::OnBnClickedCheckC2IndirectLighting)
    ON_BN_CLICKED(IDC_CHECK_C2_MIRROR_EFFECT, &CCmd2Form::OnBnClickedCheckC2MirrorEffect)
    ON_BN_CLICKED(IDC_CHECK_C2_CAUSTIC_EFFECT, &CCmd2Form::OnBnClickedCheckC2CausticEffect)
    ON_BN_CLICKED(IDC_CHECK_C2_DIRECTIONAL_LIGHTING, &CCmd2Form::OnBnClickedCheckC2DirectionalLighting)
    ON_BN_CLICKED(IDC_CHECK_C2_TOTALLY_PHOTONMAP, &CCmd2Form::OnBnClickedCheckC2TotallyPhotonmap)
    ON_BN_CLICKED(IDC_BUTTON_ADDCAMERA, &CCmd2Form::OnBnClickedButtonAddcamera)
    ON_BN_CLICKED(IDC_BUTTON_RVMCAMERA, &CCmd2Form::OnBnClickedButtonRvmcamera)
    ON_BN_CLICKED(IDC_BUTTON_CLEARALL, &CCmd2Form::OnBnClickedButtonClearall)
    ON_LBN_SELCHANGE(IDC_LIST_CAMERALIST, &CCmd2Form::OnLbnSelchangeListCameralist)
    ON_BN_CLICKED(IDC_BUTTON_CAMPREVIEW, &CCmd2Form::OnBnClickedButtonCampreview)
    ON_BN_CLICKED(IDC_BUTTON_SELECTPATH, &CCmd2Form::OnBnClickedButtonSelectpath)
    ON_BN_CLICKED(IDC_BUTTON_SAVEPATH, &CCmd2Form::OnBnClickedButtonSavepath)
    ON_BN_CLICKED(IDC_BUTTON_LOADPATH, &CCmd2Form::OnBnClickedButtonLoadpath)
    ON_BN_CLICKED(IDC_BUTTON_VIDEO_FILTER_COL, &CCmd2Form::OnBnClickedButtonVideoFilterCol)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOFILTER_GEO, &CCmd2Form::OnBnClickedButtonVideofilterGeo)
    ON_BN_CLICKED(IDC_BUTTON_STARTFILTER, &CCmd2Form::OnBnClickedButtonStartfilter)
	ON_BN_CLICKED(IDC_BUTTON_VIDEOFILTER_OUTPUT, &CCmd2Form::OnBnClickedButtonVideofilterOutput)
    ON_BN_CLICKED(IDC_BUTTON_STOP_VIDEOFILTER, &CCmd2Form::OnBnClickedButtonStopVideofilter)
    ON_BN_CLICKED(IDC_BUTTON_IRR_PATH, &CCmd2Form::OnBnClickedButtonIrrPath)
END_MESSAGE_MAP()


// CCmd2Form diagnostics

#ifdef _DEBUG
void CCmd2Form::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCmd2Form::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCmd2Form message handlers

void CCmd2Form::enableControl(bool enable)
{

    GetDlgItem(IDC_CHECK_C2_DIRECT_LIGHTING)->EnableWindow(enable);
    GetDlgItem(IDC_CHECK_C2_INDIRECT_LIGHTING)->EnableWindow(enable);
    GetDlgItem(IDC_CHECK_C2_MIRROR_EFFECT)->EnableWindow(enable);
    GetDlgItem(IDC_CHECK_C2_CAUSTIC_EFFECT)->EnableWindow(enable);
    GetDlgItem(IDC_CHECK_C2_DIRECTIONAL_LIGHTING)->EnableWindow(enable);
    GetDlgItem(IDC_CHECK_C2_TOTALLY_PHOTONMAP)->EnableWindow(enable);
    GetDlgItem(IDC_LIST_CAMERALIST)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_FRAMENUMBER)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_ADDCAMERA)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_RVMCAMERA)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_CLEARALL)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_CAMPREVIEW)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_RESULTPATH)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_SELECTPATH)->EnableWindow(enable);
    
    GetDlgItem(IDC_EDIT_GEOFILENAME)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT5_THREAD_NUM)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_FILTER_FRAME_NUM)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_COLORFILENAME)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_OUTPUTDIR)->EnableWindow(enable);
    GetDlgItem(IDC_LIST_CAMERALIST)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_FRAMENUMBER)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_RESULTPATH)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_SAVEPATH)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_LOADPATH)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_VIDEO_FILTER_COL)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_VIDEOFILTER_GEO)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_STARTFILTER)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_VIDEOFILTER_OUTPUT)->EnableWindow(enable);
    GetDlgItem(IDC_CHECK_MOTION_BLUR)->EnableWindow(enable);

    GetDlgItem(IDC_CHECK_RESAMPLE)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_SAMPLE_THRESHOLD)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_IRR_PATH)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_IRRADIANCECACHE_PATH)->EnableWindow(enable);

}

void CCmd2Form::_updateDataToDoc()
{    
    UpdateData(true);
    CBmcDoc* _doc =     (CBmcDoc*)(GetDocument());
    _doc->setRenderEffect(  m_cbDirect_light,
                                        m_cbIndirect_light,
                                        m_cbMirror_effect,
                                        m_cbCaustic_effect,
                                        m_cbDirectional_light,
                                        m_cbTotally_photonmap
                                        );

}

void CCmd2Form::OnBnClickedCheckC2DirectLighting()
{
    // TODO: Add your control notification handler code here

    _updateDataToDoc();

}

void CCmd2Form::OnBnClickedCheckC2IndirectLighting()
{
    // TODO: Add your control notification handler code here

    _updateDataToDoc();
}

void CCmd2Form::OnBnClickedCheckC2MirrorEffect()
{
    // TODO: Add your control notification handler code here

    _updateDataToDoc();
}

void CCmd2Form::OnBnClickedCheckC2CausticEffect()
{
    // TODO: Add your control notification handler code here

    _updateDataToDoc();
}

void CCmd2Form::OnBnClickedCheckC2DirectionalLighting()
{
    // TODO: Add your control notification handler code here

    _updateDataToDoc();
}

void CCmd2Form::OnBnClickedCheckC2TotallyPhotonmap()
{
    // TODO: Add your control notification handler code here

    _updateDataToDoc();
}

void CCmd2Form::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // TODO: Add your specialized code here and/or call the base class
    _updateDataToDoc();
}

void CCmd2Form::saveConfigDataToDoc()
{
    UpdateData(true);
    //get doc
    CBmcDoc* _doc =     (CBmcDoc*)(GetDocument());
    Config* _config  = _doc->getConfig();
    
    if(_doc==NULL||_config==NULL)
        return;

    _config->setDirectLightEnable(m_cbDirect_light);
    _config->setIndirectLightEnable(m_cbIndirect_light);
    _config->setMirrorEffectEnable(m_cbMirror_effect);
    _config->setCausticEffectEnable(m_cbCaustic_effect);
    _config->setDirectionalLightEnable(m_cbDirectional_light);
    _config->setPhotonMapTotallyEnable(m_cbTotally_photonmap);

   


}

void CCmd2Form::loadConfigDataFromDoc()
{
    //get doc
    CBmcDoc* _doc =     (CBmcDoc*)(GetDocument());
    Config* _config  = _doc->getConfig();

    m_cbDirect_light        = _config->getDirectLightEnable();
    m_cbIndirect_light     = _config->getIndirectLightEnable();      
    m_cbMirror_effect       = _config->getMirrorEffectEnable();
    m_cbCaustic_effect     = _config->getCausticEffectEnable();
    m_cbDirectional_light  = _config->getDirectionalLightEnable();
    m_cbTotally_photonmap = _config->getPhotonMapTotallyEnable();

    UpdateData(false);
}


void CCmd2Form::OnBnClickedButtonAddcamera()
{
    CBmcDoc* _doc = (CBmcDoc*)GetDocument();
    if(!_doc)
        return;

    CameraInfo* _camInfo = new CameraInfo;
    _camInfo->m_fDistance = _doc->Get_distance();
    _camInfo->m_fDistScale    = _doc->Get_distance_scale();
    _camInfo->m_vCent  = _doc->Get_cent_pos();
    _camInfo->m_vEye   =  _doc->Get_eye_pos();
    _camInfo->m_vTrans = _doc->Get_translate();

    _camInfo->m_archBall   = *G_pArcBall;
    memcpy((void*)((_camInfo->m_rotateMatrix)),(void*)(G_pArcBall->GetBallMatrix()),sizeof(float) * 16);
    
    _doc->getCameralist().add(_camInfo);

    CKeyMotions& _motion =  _doc->getKeyMotions();
    _motion.AddOneFrameFromBuffer(_camInfo->getBuffer(),_camInfo->getBufferSizeInFloat());

    updateCameraListBox();
}

void CCmd2Form::OnBnClickedButtonRvmcamera()
{
    int index   =  m_lbCameraList.GetCurSel();
    if(index<0)
        return;

    CBmcDoc* _doc = (CBmcDoc*)GetDocument();
    if(!_doc)
        return;

    _doc->getCameralist().delAt(index);

    CKeyMotions& _motion =  _doc->getKeyMotions();
    _motion.DeleteOneFrame(index);


    updateCameraListBox();
}

void CCmd2Form::OnBnClickedButtonClearall()
{
    CBmcDoc* _doc = (CBmcDoc*)GetDocument();
    if(!_doc)
        return;

    _doc->getCameralist().clearAll();

    CKeyMotions& _motion =  _doc->getKeyMotions();
    _motion.deleteAllkeyFrames();

    updateCameraListBox();
}


void CCmd2Form::updateCameraListBox()
{
    m_lbCameraList.SetRedraw(false);

    CBmcDoc* _doc = (CBmcDoc*)GetDocument();
    if(!_doc)
        return;
    
    int number1  =  m_lbCameraList.GetCount();
    for(int i=0;i<number1;i++)
    {
        m_lbCameraList.DeleteString(0);
    }

    int number2  =  _doc->getCameralist().getListSize();
    char ff[15];
    for(int i=0;i<number2;i++)
    {
        sprintf(ff,"%x",(void*)(_doc->getCameralist().getAt(i)));
        m_lbCameraList.AddString(ff);
    }

    m_lbCameraList.SetRedraw(true);


}

void CCmd2Form::OnLbnSelchangeListCameralist()
{
    int index= m_lbCameraList.GetCurSel();
    applyCamera(index);
    
}


void CCmd2Form::applyCamera(int index)
{
    CBmcDoc* _doc = (CBmcDoc*)GetDocument();
    if(!_doc)
        return;

    CameraInfo* _camInfo   = _doc->getCameralist().getAt(index);

    //update camera info here
    
    memcpy((void*)G_pArcBall,(void*)(&(_camInfo->m_archBall)),sizeof(CArcBall));

    _doc->Set_distance_scale(_camInfo->m_fDistScale);
    _doc->Set_distance(_camInfo->m_fDistance);
    _doc->Set_translate(_camInfo->m_vTrans);
    _doc->Set_cent(_camInfo->m_vCent);
    _doc->Set_eye_pos(_camInfo->m_vEye);

    updateRenderView();

}

//replace rotation matrix in ArcBall, and some value in Doc
void CCmd2Form::applyCamera(CameraInfo* _camInfo)
{
    CBmcDoc* _doc = (CBmcDoc*)GetDocument();
    if(!_doc)
        return;

    //update camera info here

    memcpy((void*)(G_pArcBall->GetBallMatrix()),(void*)((_camInfo->m_rotateMatrix)),sizeof(float) * 16);

    _doc->Set_distance_scale(_camInfo->m_fDistScale);
    _doc->Set_distance(_camInfo->m_fDistance);
    _doc->Set_translate(_camInfo->m_vTrans);
    _doc->Set_cent(_camInfo->m_vCent);
    _doc->Set_eye_pos(_camInfo->m_vEye);

    updateRenderView();
}

void CCmd2Form::updateRenderView()
{
    ((CBmcView *) (((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
        m_wndSplitter2.GetPane(0,0)))->DrawScene(); 
}


//get new interpolated data and apply them 
void CCmd2Form::OnBnClickedButtonCampreview()
{
    UpdateData(true);

    if(m_ebRenderEnd>=m_frameNumber||m_ebRenderStart<0)
    {
        MessageBox("Error inPut","warning",MB_OK);
        return;
    }
    GetDlgItem(IDC_BUTTON_ADDCAMERA)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_RVMCAMERA)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_CLEARALL)->EnableWindow(false);
    
    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    BmcList<CameraInfo>& _camList   =   _doc->getCameralist();
    CKeyMotions& _motion =  _doc->getKeyMotions();


    float _pos  =  0.0f;
    //key frame number
    int _keyFrameNum    = _motion.Size();
    //step
    float _fStep   =  static_cast<float>(_keyFrameNum-1)/static_cast<float>(m_frameNumber);
    float _buffer[CAMERA_BUFFER_SIZE];
    CameraInfo   _camInfo;

    _pos =  _fStep * m_ebRenderStart;
    for(int i=m_ebRenderStart;i<=m_ebRenderEnd;i++)
    {
        _pos += _fStep; 
        if(_pos>_keyFrameNum-1)
            _pos =  _keyFrameNum -   1;

        _motion.GetNewFrameToBuffer(_pos,_buffer,CAMERA_BUFFER_SIZE);
        _camInfo.setBuffer(_buffer,CAMERA_BUFFER_SIZE);
        applyCamera(&_camInfo);
        //Sleep(40);

    }


    GetDlgItem(IDC_BUTTON_ADDCAMERA)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_RVMCAMERA)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_CLEARALL)->EnableWindow(true);
    // TODO: Add your control notification handler code here
}

//select result path
void CCmd2Form::OnBnClickedButtonSelectpath()
{
    UpdateData(true);

    BROWSEINFO   bBinfo;   
    memset(&bBinfo,   0,   sizeof(BROWSEINFO));       
    bBinfo.hwndOwner   =   m_hWnd;       
    bBinfo.lpszTitle   =   _T("Please select a path:   ");     
    bBinfo.ulFlags   =   BIF_RETURNONLYFSDIRS;     

    char   strPath[MAX_PATH];
    memset(strPath,0,MAX_PATH);

    LPITEMIDLIST   lpDlist;     
    lpDlist   =   SHBrowseForFolder(&bBinfo)   ;     
    if   (   lpDlist   !=   NULL   )     
    {     
        SHGetPathFromIDList(lpDlist,   strPath);     
        TRACE(strPath);     
        
        //deal with '/' problem
        int i  =  0;
        for(;strPath[i+1]!='\0';i++);
        if(strPath[i]!='\\')
        {
            strPath[i+1]  =  '\\';
            strPath[i+2]  =  '\0';
        }

        m_ebPath  = strPath; 
        strcpy(m_sResultPath,strPath);
        UpdateData(false);    
    }


}

//save camera path to file
void CCmd2Form::OnBnClickedButtonSavepath()
{
    //file name
    char	fn[MAX_PATH];

    CFileDialog myfiledlg(FALSE, "pth", NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "Path Files (*.pth)|*.pth||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else	return;

    //save data to file
    UpdateData(true);
    _savePathToFile(fn);
   

}

//load camera path from file
void CCmd2Form::OnBnClickedButtonLoadpath()
{
    //file name
    char	fn[MAX_PATH];
    char  fn2[MAX_PATH];
    CFileDialog myfiledlg(true, NULL, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "Path Files (*.pth)|*.pth||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else	return;

    sprintf(fn2,"%s.nb",fn);

    //save data to file
    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    if(!_doc)
    {
        return;
    }

    BmcList<CameraInfo>& _camList   =   _doc->getCameralist();
    if(_camList.loadFromFile(fn)==false)
        return;

    CKeyMotions& _keyMotions    = _doc->getKeyMotions();
   
    int keyNumber   = _camList.getListSize();
    if(keyNumber<=0)
        return;

    _keyMotions.deleteAllkeyFrames();
    for(int i=0;i<keyNumber;i++)
    {
        _keyMotions.AddOneFrameFromBuffer(_camList[i]->getBuffer(),_camList[i]->getBufferSizeInFloat());
    }

    std::ifstream iiFile(fn2,std::ios::binary);
    if(iiFile.fail())
        return;

    //read frameNumber
    UpdateData(true);
    iiFile.read((char*)(&m_frameNumber),sizeof(int));
    iiFile.read((char*)(&m_ebRenderStart),sizeof(int));
    iiFile.read((char*)(&m_ebRenderEnd),sizeof(int));

    UpdateData(false);

    updateCameraListBox();
}

//select color data files
void CCmd2Form::OnBnClickedButtonVideoFilterCol()
{
    // color file name
    char	fn[256];
    CFileDialog myfiledlg(true, NULL, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "Color Files (*.bp)|*.bp|All File(*.*)|*.*||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else	return;

    UpdateData(true);
    m_ebFilterColorFileName    = fn;
    UpdateData(false);

    return;
}

//select geometry data files
void CCmd2Form::OnBnClickedButtonVideofilterGeo()
{
    // color file name
    char	fn[256];
    CFileDialog myfiledlg(true, NULL, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "Color Files (*.pix)|*.pix|All File(*.*)|*.*||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else	return;

    UpdateData(true);
    m_ebFilterGeoFileName    = fn;
    UpdateData(false);

    return;
}

//select video filter result directory
void CCmd2Form::OnBnClickedButtonVideofilterOutput()
{
	BROWSEINFO   bBinfo;   
	memset(&bBinfo,   0,   sizeof(BROWSEINFO));       
	bBinfo.hwndOwner   =   m_hWnd;       
	bBinfo.lpszTitle   =   _T("Please select a path:   ");     
	bBinfo.ulFlags   =   BIF_RETURNONLYFSDIRS;     

	char   strPath[MAX_PATH];
	memset(strPath,0,MAX_PATH);

	LPITEMIDLIST   lpDlist;     
	lpDlist   =   SHBrowseForFolder(&bBinfo)   ;     
	if   (   lpDlist   !=   NULL   )     
	{     
		SHGetPathFromIDList(lpDlist,   strPath);     
		TRACE(strPath);     

		//deal with '/' problem
		int i  =  0;
		for(;strPath[i+1]!='\0';i++);
		if(strPath[i]!='\\')
		{
			strPath[i+1]  =  '\\';
			strPath[i+2]  =  '\0';
		}

		UpdateData(true);
		m_videoFilterOutputDir  = strPath; 
		UpdateData(false);    
	}
}


//start filtering
void CCmd2Form::OnBnClickedButtonStartfilter()
{

    //get cmd panel1
    _panel1  =  ((CCmdForm*)(((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
        m_wndSplitter.GetPane(0,0)));

    Watch::startWatch();

	//get document
    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    if(!_doc)
        return;
	
	//save current UI data to config data in doc.
    saveConfigDataToDoc();
    CCmdForm* _panel1  =  ((CCmdForm*)(((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
        m_wndSplitter.GetPane(0,0)));

   _panel1->saveConfigDataToDoc();

	//get Video filter
    FilterVideo& _filter =  _doc->getFilterVideo();
	//get config data
    Config* _config =  _doc->getConfig();
    
	//initialize video filter
    if(_filter.initialize(m_ebFilterColorFileName.GetString(),
                            m_ebFilterGeoFileName.GetString(),
                            m_ebFilterIrraFileName.GetString(),
							m_videoFilterOutputDir.GetString(),
							m_nFilteringThreadNum,
                            _config->getFilterBoxSize(),
                            m_ebFilterFrameNum,
                            _config->getPixelFilterC(),
                            _config->getPerceptionFilterC(),
                            _config->getNormalFilterC(),
                            _config->getDepthFilterC(),
                            _config->getMaterialFilterC(),
                            _config->getPixelFilterEnable(),
                            _config->getPerceptionFilterEnable(),
                            _config->getNormalFilterEnable(),
                            _config->getDepthFilterEnable(),
                            _config->getMaterialEnable(),
                            m_cbMotionBlur,
                            m_cbResample,
                            m_ebSampleThreshold
                            )==false)
    {
        m_log->assertError("Video Filter initialize failed!");
        return;
    }


    ////laugh the filtering thread
   
    m_filteringHandle		=	(HANDLE)_beginthreadex(
        NULL,
        0,
        (THREADFUNC)(CCmd2Form::StartFilteringThread),
        (LPVOID)this,
        0,
        (THREADID)(&m_filteringID)
        );

    Sleep(50);
    if(m_filteringHandle)
    {
        CloseHandle(m_filteringHandle);
        printf("Filtering thread launched!\n");
    }
    else
    {
        printf("Filtering thread launched failed!\n");
        return;
    }


}

DWORD WINAPI CCmd2Form::StartFilteringThread(LPVOID param)
{
    CCmd2Form* _object = (CCmd2Form*)param;
    if(_object==NULL)
        return 0;

    //get document
    CBmcDoc* _doc = (CBmcDoc*)(_object->GetDocument());
    if(!_doc)
        return 0;

    _object->enableControl(false);
        
        
        if(_object->m_cbResample==true)
        {
            if(G_pScene->Is_model_loaded()==false)
            {
                _object->MessageBox("The scene is not loaded yet!","Warning",MB_OK);
                _object->m_log->assertError("The scene is not loaded yet!");
                _object->enableControl(true);              
                return 0;
            }
            
            _object->m_log->assertMsg("Model is loaded...");

			(_doc->getFilterVideo()).m_doingScan	=	true;
            if((_doc->getFilterVideo()).searchForResamplePixels()==false)
            {       
                printf("Search resample pixels failed!\n");
                _object->m_log->assertError("Search resample pixels failed!");
                _object->enableControl(true);
                return 0;
            }

             char t_msg[200];
             sprintf(t_msg,"Search for resample pixels finished...........\n%d/%d = %f  need resampling...........\n",(_doc->getFilterVideo()).m_nTotalResamplePixels,(_doc->getFilterVideo()).m_nTotalPixels,(_doc->getFilterVideo()).m_fResampleRate);
             printf(t_msg);
             _object->m_log->assertMsg(t_msg);
             
             printf("start resampling...\n");
       
             const std::vector<std::string>& t_irraNameVec   = (_doc->getFilterVideo()).getIrraFileVec();
             const std::vector<std::string>& t_geoNameVec   = (_doc->getFilterVideo()).getGeoFileVec();
             if(_object->_resample(t_irraNameVec,t_geoNameVec)==false)
             {
                printf("Resample pixels failed!\n");
                 _object->m_log->assertError("Search resample pixels failed!");
                 _object->enableControl(true);
                 return 0;
             }
             _object->m_log->assertMsg("Re-sample finished...");



        //out test
        //_doc->getFilterVideo().saveResamplingDataToFile("d:\\other\\out.txt");
        if(_object->m_ebOutputResampleImages)
        {
            _doc->getFilterVideo().saveResamplingDataToImages(const_cast<char*>(_object->m_videoFilterOutputDir.GetString()));
        }

    }
		//start filtering
        _object->m_log->assertMsg("start filtering...");
		(_doc->getFilterVideo()).m_doingScan	=	false;
        (_doc->getFilterVideo()).startFiltering();

    _object->enableControl(true);
return 1;
}

bool CCmd2Form::_resample(
                          const std::vector<std::string>& _irraFileVec,
                          const std::vector<std::string>& _geoFileVec)
{
    if(_irraFileVec.size()<=0||_geoFileVec.size()<=0)
    {
        printf("CCmd2Form::_resample.. .size()<=0\n");
        return false;
    }

    //set irradiance cache files
    G_pScene->_setIrradianceCacheFileNameVec(_irraFileVec);
    G_pScene->_setGeoFileNameVec(_geoFileVec);
    //get document
    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    if(!_doc)
        return false;

    if(_panel1==NULL)
    {
        printf("CCmd2Form::_resample,  get _panel1 failed!\n");
        return false;
    }
    //get camera generator
    CKeyMotions& _cams   =  _doc->getKeyMotions();
    float _buffer[CAMERA_BUFFER_SIZE];
    CameraInfo   _camInfo;

    //get re-sample pixels
    std::map<int,FrameForResample*>& t_resamplePixels  =  (_doc->getFilterVideo()).getResamplePixels();

    //start reSampling
    std::map<int,FrameForResample*>::iterator t_ite;
    std::vector<ZH::reSamplePixel*>::iterator t_ite2;
    
    //get and apply InDirect light number
    FilterVideo& _filterVideo =  _doc->getFilterVideo();
    
    //get and apply some render parameter
    _panel1->m_bSunLight  = _filterVideo.m_bSunLight;
    _panel1->m_bSkyLight  =  _filterVideo.m_bSkyLight;
    _panel1->m_SunLight   =  _filterVideo.m_SunLight;
    _panel1->m_SkyLight    = _filterVideo.m_SkyLight;
    _panel1->m_fLightScale = _filterVideo.m_fLightScale;
    _panel1->m_fSunlightScale   =  _filterVideo.m_fSunlightScale;
    _panel1->m_fSkylightScale    = _filterVideo.m_fSkylightScale;
    _panel1->m_nPhotonNum      = _filterVideo.m_nPhotonNum;
    _panel1->m_nSampleRayNumber   = _filterVideo.m_nIndirectLightNumber;
    _panel1->m_nSampleLightRayNumber    = static_cast<int>(_filterVideo.getSamplingThreshold());
    _panel1->m_nImageWidth = _filterVideo.m_iImageWidth;
    _panel1->m_nImageHeight    = _filterVideo.m_iImageHeight;
    _panel1->m_fIntensityScale  = _filterVideo.m_fIntensityScale;
    _panel1->m_bIrradianceCache   =  _filterVideo.m_bUseIrradianceCache;
    _panel1->m_bGradientIC       =  _filterVideo.m_bGradientIC;
    _panel1->m_bBmcIntegration    =  _filterVideo.m_bBmcIntegration;
    _panel1->m_fTolerance          = (_filterVideo.m_bUseIrradianceCache)? (_filterVideo.m_fTolerance) : -1.f;;  


    int t_samples = static_cast<int>(_filterVideo.getSamplingThreshold());
    float t_fTolerance   = (_filterVideo.m_bUseIrradianceCache)? (_filterVideo.m_fTolerance) : -1.f;;  

    CCol4 cSun = COLOR_BLACK, cSky = COLOR_BLACK;

    if (_panel1->m_bSunLight)
    {
        cSun.r() = _panel1->m_fSunlightScale * (_panel1->m_SunLight & 0xff) / 255.f;
        cSun.g() = _panel1->m_fSunlightScale * (((_panel1->m_SunLight) & 0xff00) >> 8) / 255.f;
        cSun.b() = _panel1->m_fSunlightScale * (((_panel1->m_SunLight) & 0xff0000) >> 16) / 255.f;
        cSun.a() = 1.0f;
    }

    if (_panel1->m_bSkyLight)
    {
        cSky.r() = _panel1->m_fSkylightScale * (_panel1->m_SkyLight & 0xff) / 255.f;
        cSky.g() = _panel1->m_fSkylightScale * (((_panel1->m_SkyLight) & 0xff00) >> 8) / 255.f;
        cSky.b() = _panel1->m_fSkylightScale * (((_panel1->m_SkyLight) & 0xff0000) >> 16) / 255.f;
        cSky.a() = 1.0f;
    }

    G_pScene->DeleteIrradianceCacheTree();
    if(!G_pScene->buildIrradianceCacheTreeFromFile())
    {
        printf("build irradiance cache tree from file failed!\n");
        return false;
    }
    printf("Build irradiance cache tree from file successful!\n");
    //for each frame;
    int t_frameCount    = 0;
    for(t_ite=t_resamplePixels.begin();t_ite!=t_resamplePixels.end();t_ite++,t_frameCount++)
    {
        printf("For frame  %d...\n",t_ite->first);
        //generate camera 
        float _camFloat  =  (t_ite->second)->m_fCameraFloat;
        _cams.GetNewFrameToBuffer(_camFloat,_buffer,CAMERA_BUFFER_SIZE);
        _camInfo.setBuffer(_buffer,CAMERA_BUFFER_SIZE);
        //apply camera
        printf("Apply camera  %d...\n",t_ite->first);

        applyCamera(&_camInfo);

        //initialize tracing parameter
        printf("Init_BMC_raytracing...  %d...\n",t_ite->first);

        _doc->Init_BMC_raytracing
            (
            _filterVideo.m_iImageWidth,
            _filterVideo.m_iImageHeight,
            _filterVideo.m_fLightScale,
            cSun,
            cSky,
            _filterVideo.m_nPhotonNum,
            _filterVideo.m_nIndirectLightNumber,
            t_samples,
            t_fTolerance,
            _filterVideo.m_bGradientIC,
            _filterVideo.m_bBmcIntegration);

        //for each pixel in current frame
        std::vector<ZH::reSamplePixel*>& t_pixelVector   =  t_ite->second->m_vPixels;
        if(static_cast<int>(t_pixelVector.size())>0)
        {   
            printf("start resampling one frame...  %d...\n",t_ite->first);
            char t_msg[200];
            if(G_pScene->_reSampleOneFrame(t_pixelVector,m_nFilteringThreadNum,t_frameCount)==false)
            {

                sprintf(t_msg,"Resampling Frame %d failed!\n",t_ite->first);
                printf(t_msg);
                m_log->assertError(t_msg);
            }
            else
            {
                sprintf(t_msg,"Sampling Frame %d successfully!======================>%d...ok\n",t_ite->first, t_ite->first);
                printf(t_msg);
                m_log->assertMsg(t_msg);           
            }
        }

    }

    return true;
}


//stop video filter
void CCmd2Form::OnBnClickedButtonStopVideofilter()
{
    //get document
    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    if(!_doc)
        return ;

    _doc->getFilterVideo().stopFiltering();

    // TODO: Add your control notification handler code here
}

void CCmd2Form::_savePathToFile(char* fn)
{
    char  fn2[MAX_PATH];
    sprintf(fn2,"%s.nb",fn);

    int    nCams =  m_frameNumber; 
    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    if(!_doc)
    {
        return;
    }
    BmcList<CameraInfo>& _camList   =   _doc->getCameralist();
    if(_camList.saveDataToFile(fn)==false)
        return;

    std::ofstream ooFile(fn2,std::ios::binary);
    if(ooFile.fail())
        return;

    //write frameNumber
    ooFile.write((char*)(&nCams),sizeof(int));
    ooFile.write((char*)(&m_ebRenderStart),sizeof(int));
    ooFile.write((char*)(&m_ebRenderEnd),sizeof(int));
    // for()
}


void CCmd2Form::OnBnClickedButtonIrrPath()
{
    // color file name
    char	fn[256];
    CFileDialog myfiledlg(true, NULL, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "Irradiance Cache Files (*.irr)|*.irr|All File(*.*)|*.*||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else	return;

    UpdateData(true);
    m_ebFilterIrraFileName    = fn;
    UpdateData(false);

    return;
}
