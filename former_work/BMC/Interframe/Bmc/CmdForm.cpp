// CmdForm.cpp : implementation file
//

#include "stdafx.h"
#include "Bmc.h"
#include "CmdForm.h"
#include "Cmd2Form.h"
#include "PixelInfo.h"
#include ".\cmdform.h"
#include "BmcTR.h"
#include "BmcDoc.h"
#include "Singleton.h"
#include "Log.h"
#include "Singleton.h"
#include "Watch.h"

extern CBmcTR *G_pScene;
extern CArcBall  *G_pArcBall;

// CCmdForm

IMPLEMENT_DYNCREATE(CCmdForm, CFormView)

CCmdForm::CCmdForm()
	: CFormView(CCmdForm::IDD)
	, m_bSunLight(FALSE)
	, m_bSkyLight(FALSE)
	, m_fLightScale(50)
	, m_fSunlightScale(1000)
	, m_fSkylightScale(30)
	, m_nPreviewStyle(2)
	, m_nPhotonNum(100000)
	, m_nSampleRayNumber(200)
	, m_nSampleLightRayNumber(100)
	, m_nImageWidth(320)
	, m_nImageHeight(200)
	, m_fIntensityScale(0.35f)
	, m_nThread(4)
	, m_fTolerance(0.1f)
	, m_bIrradianceCache(true)
	, m_bGradientIC(false)
	, m_bBmcIntegration(FALSE)

	, m_CheckBox_ApplyFilter(true)
	, m_CheckBox_Pixel(true)
	, m_CheckBox_Perception(true)
	, m_CheckBox_Normal(true)
	, m_CheckBox_Depth(true)
	, m_CheckBox_Material(true)

	, m_Edit_FilterBoxSize(5)
	, m_Edit_Perception(0.1)
	, m_Edit_Normal(0.333333f)
	, m_Edit_Depth(0.33333f)
	, m_Edit_Material(1.0f)
    , m_log(NULL)
    , m_nSingleOrVideo(1)
{
	m_Edit_Pixel	=	static_cast<float>(m_Edit_FilterBoxSize-1)/2.0f * 1.414f / 3.0f;

	m_SunLight = RGB(255, 255, 255);
	m_SkyLight = RGB(120, 170, 220);
	m_pRaytracingThread = NULL;
    m_log   =  Singleton<ZHLog>::getSingletonPtr();
}

CCmdForm::~CCmdForm()
{
}

void CCmdForm::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_SUNLIGHT_CK, m_bSunLight);
    DDX_Check(pDX, IDC_SKYLIGHT_CK, m_bSkyLight);
    DDX_Text(pDX, IDC_LIGHT_SCALE, m_fLightScale);
    DDX_Text(pDX, IDC_SUNLIGHT_SCALE, m_fSunlightScale);
    DDX_Text(pDX, IDC_SKYLIGHT_SCALE, m_fSkylightScale);
    DDX_Radio(pDX, IDC_POINT_RD, m_nPreviewStyle);
    DDX_Radio(pDX,IDC_RADIO_SINGLE, m_nSingleOrVideo);
    DDX_Text(pDX, IDC_PHOTON_NUMBER, m_nPhotonNum);
    DDX_Text(pDX, IDC_RAY_NUMBER, m_nSampleRayNumber);
    DDX_Text(pDX, IDC_LIGHT_TEST_RAY_NUMBER, m_nSampleLightRayNumber);
    DDX_Text(pDX, IDC_IMAGE_WIDTH, m_nImageWidth);
    DDX_Text(pDX, IDC_IMAGE_HEIGHT, m_nImageHeight);
    DDX_Control(pDX, IDC_LIGHT_SOURCE_SCALE_SPIN, m_cLightScaleSpin);
    DDX_Text(pDX, IDC_INTENSITY_SCALE, m_fIntensityScale);
    DDX_Text(pDX, IDC_THREAD_NUMBER, m_nThread);
    DDV_MinMaxInt(pDX, m_nThread, 1, 4);
    DDX_Text(pDX, IDC_IRRADIANCE_CACHE_TOLERANCE, m_fTolerance);
    DDX_Check(pDX, IDC_IRRADIANCE_CACHE_CK, m_bIrradianceCache);
    DDX_Check(pDX, IDC_GRADIENT_IRRADIANCE_CACHE_CK, m_bGradientIC);
    DDX_Check(pDX, IDC_BMC_INTEGRATION_CB, m_bBmcIntegration);

    DDX_Check(pDX,IDC_CHECK_APPLYFILTER,m_CheckBox_ApplyFilter);
    DDX_Check(pDX,IDC_CHECK_PIXEL,m_CheckBox_Pixel);
    DDX_Check(pDX,IDC_CHECK_PERCEPTION,m_CheckBox_Perception);
    DDX_Check(pDX,IDC_CHECK_NORMAL,m_CheckBox_Normal);
    DDX_Check(pDX,IDC_CHECK_DEPTH,m_CheckBox_Depth);
    DDX_Check(pDX,IDC_CHECK_MATERIAL,m_CheckBox_Material);

    DDX_Text(pDX,IDC_EDIT_FILTERBOXSIZE,m_Edit_FilterBoxSize);
    DDX_Text(pDX,IDC_EDIT_PIXEL,m_Edit_Pixel);
    DDX_Text(pDX,IDC_EDIT_PERCEPTION,m_Edit_Perception);
    DDX_Text(pDX,IDC_EDIT_NORMAL,m_Edit_Normal);
    DDX_Text(pDX,IDC_EDIT_DEPTH,m_Edit_Depth);
    DDX_Text(pDX,IDC_EDIT_MATERIAL,m_Edit_Material);

    DDX_Control(pDX, IDC_PROGRESS_VIDEORENDER, m_pcProgress);
}

BEGIN_MESSAGE_MAP(CCmdForm, CFormView)
	ON_BN_CLICKED(IDC_POINT_RD, OnBnClickedPointRd)
	ON_BN_CLICKED(IDC_WIRE_RD, OnBnClickedWireRd)
	ON_BN_CLICKED(IDC_SOLID_RD, OnBnClickedSolidRd)
	ON_STN_CLICKED(IDC_SUNLIGHT_COLOR, OnStnClickedSunlightColor)
	ON_STN_CLICKED(IDC_SKYLIGHT_COLOR, OnStnClickedSkylightColor)
	ON_BN_CLICKED(IDC_NORMAL_MC_RAY_TRACING, OnBnClickedNormalMcRayTracing)
	ON_BN_CLICKED(IDC_BMC_RAY_TRACING, OnBnClickedBmcRayTracing)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SHOW_RAY_TRACING_RESULT, OnBnClickedShowRayTracingResult)
	ON_BN_CLICKED(IDC_CHECK_APPLYFILTER, OnBnClickedApplyFilter)
	ON_BN_CLICKED(IDC_BUTTON_STARTFILTER, OnStartFiltering)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_FILTER, OnShowFilteredImage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE,OnClickSaveTracingResult)
	ON_BN_CLICKED(IDC_BUTTON_LOAD,OnClickLoadTracingResult)
    ON_BN_CLICKED(IDC_RADIO_COLOR_TRIANGLE, &CCmdForm::OnBnClickedRadioColorTriangle)
END_MESSAGE_MAP()


// CCmdForm diagnostics

#ifdef _DEBUG
void CCmdForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CCmdForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CCmdForm message handlers
CBmcView *GetView()
{
	return (CBmcView *) ((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
		m_wndSplitter.GetPane(0,0);
}

CBmcDoc  *GetDoc() 
{
	return GetView()->GetDocument();
}

void CCmdForm::OnBnClickedPointRd()
{
	GetDoc()->SetPreviewStyle(RENDER_DRAW_POINTS);
	GetView()->Invalidate();
}

void CCmdForm::OnBnClickedWireRd()
{
	GetDoc()->SetPreviewStyle(RENDER_DRAW_WIREFRAME);
	GetView()->Invalidate();
}

void CCmdForm::OnBnClickedSolidRd()
{
	GetDoc()->SetPreviewStyle(RENDER_DRAW_SOLID);
	GetView()->Invalidate();
}

void CCmdForm::OnBnClickedRadioColorTriangle()
{
    GetDoc()->SetPreviewStyle(RENDER_DRAW_COLOR_TRIANGLE);
    GetView()->Invalidate();   
}

void CCmdForm::OnStnClickedSunlightColor()
{
	// TODO: Add your control notification handler code here
	CColorDialog mycoldlg (m_SunLight);

	if (mycoldlg.DoModal() == IDOK)
	{
		m_SunLight = mycoldlg.GetColor();
		Invalidate();
	}
}

void CCmdForm::OnStnClickedSkylightColor()
{
	// TODO: Add your control notification handler code here
	CColorDialog mycoldlg (m_SkyLight);

	if (mycoldlg.DoModal() == IDOK)
	{
		m_SkyLight = mycoldlg.GetColor();
		Invalidate();
	}
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///////////////////////////////////////////////////////////////////////////////
// The ray tracing process
// This is a global routine of multi-thread
///////////////////////////////////////////////////////////////////////////////
UINT Ray_tracing_proc(LPVOID pParam)
{
	TThreadParam *     tp     =  (TThreadParam *)pParam;
    CBmcDoc*         _doc     =  tp->_doc;
    CCmdForm*  _panel1     =  tp->_theForm;
    CCmd2Form*_panel2     =  tp->_controlPanel2;

    //disable UI
    _panel1->enableControl(false);
	_panel2->enableControl(false);

    (_panel1->m_pcProgress).SetPos(0);

	//start tracing
    int t_singleOrVideo = _panel1->m_nSingleOrVideo;
    
    //render single frame
    if(t_singleOrVideo==0)
    {
        //clear irradiance tree
        
        G_pScene->DeleteIrradianceCacheTree();
        //initialize tracing parameter
        _doc->Init_BMC_raytracing (
            _panel1->m_nImageWidth, 
            _panel1->m_nImageHeight, 
            _panel1->m_fLightScale, 
            tp->cSun, 
            tp->cSky, 
            _panel1->m_nPhotonNum, 
            _panel1->m_nSampleRayNumber, 
            _panel1->m_nSampleLightRayNumber, 
            tp->fTolerance, 
            _panel1->m_bGradientIC, 
            _panel1->m_bBmcIntegration);

        if (tp->_nThread > 4) 
            tp->_nThread = 4;	

        G_pScene->Ray_tracing_BMC (tp->_nThread);

    }
                        
    //render video ==[]================================>video render
    else
    {
        //save file name
        char t_pixFileName[MAX_PATH];
        char t_pixbpFileName[MAX_PATH];
        char t_imageFileName[MAX_PATH];
        char t_depthInfoFileName[MAX_PATH];
        char t_normalInfoFileName[MAX_PATH];
        char t_irradianceCacheTreeName[MAX_PATH];

        //get render number 
        int _frameNum  =  _panel2->getFrameNumber();
        if(_frameNum<=1)
        {
            MessageBox(NULL,"Frame Number should should not be less than 0.", "warning", MB_ICONWARNING);
            //enable UI
            _panel1->enableControl(true);
            _panel2->enableControl(true);
            return 0;
        }
        
        //get camera generator
        CKeyMotions& _cams   =  _doc->getKeyMotions();
        int _keyFrameNum = _cams.Size();
        if(_keyFrameNum<=1)
        {
            MessageBox(NULL,"Camera List should not be less than 2.", "warning", MB_ICONWARNING);
            //enable UI
            _panel1->enableControl(true);
            _panel2->enableControl(true);
           return 0;
        }
        
        //step
        float _pos  =  0.0f;
        float _fStep   =  static_cast<float>(_keyFrameNum-1)/static_cast<float>(_frameNum-1);
        float _buffer[CAMERA_BUFFER_SIZE];
        CameraInfo   _camInfo;
        
        G_pScene->DeleteIrradianceCacheTree();

        // render each frame
        int t_startFrame = _panel2->m_ebRenderStart;
        int t_endFrame  =  _panel2->m_ebRenderEnd;
        if(t_startFrame>t_endFrame)
            return 0;
        
        int _curFrame    = t_startFrame;
        _pos =  _fStep * t_startFrame;

        for(;_curFrame<=t_endFrame;_curFrame++,_pos+=_fStep)
        {
            //get interpolated camera data
            if(_pos<0)
                _pos =  0;
            if(_pos>_keyFrameNum-1)
                _pos =  _keyFrameNum-1;

            _cams.GetNewFrameToBuffer(_pos,_buffer,CAMERA_BUFFER_SIZE);
            _camInfo.setBuffer(_buffer,CAMERA_BUFFER_SIZE);
            _panel1->_applyCamera(_camInfo);

            //initialize tracing parameter
            _doc->Init_BMC_raytracing (
                _panel1->m_nImageWidth, 
                _panel1->m_nImageHeight, 
                _panel1->m_fLightScale, 
                tp->cSun, 
                tp->cSky, 
                _panel1->m_nPhotonNum, 
                _panel1->m_nSampleRayNumber, 
                _panel1->m_nSampleLightRayNumber, 
                tp->fTolerance, 
                _panel1->m_bGradientIC, 
                _panel1->m_bBmcIntegration);

            if (tp->_nThread > 4) 
                tp->_nThread = 4;	

            G_pScene->Ray_tracing_BMC (tp->_nThread);

            //after render a frame
           (_panel1->m_pcProgress).SetPos((int)((float)(_curFrame-t_startFrame)/(float)(t_endFrame - t_startFrame)*100));
           std::cout<<"Render Frame "<<_curFrame<<"  completed."<<std::endl;
          
           //save result to file
           //===============================================          
           //Three file need to be saved
           //1. Pixel info file(.pix), record pixel information such as depth,normal,material
           //      for current frame
           //2. Result pixel color file(.pix.bp), record the tracing result to binary file.
           //3. Result Image .
           //===============================================
	       
           //prepare file name
           sprintf(t_pixFileName,"%spixelInfo_%dx%d_photon%d_d%d_i%d_scale%d_irCache(%d)_%04d.pix",
                                           tp->_savePath,
                                           _panel1->m_nImageWidth,
                                           _panel1->m_nImageHeight,
                                           _panel1->m_nPhotonNum,
                                           _panel1->m_nSampleRayNumber,
                                           _panel1->m_nSampleLightRayNumber,
                                           static_cast<int>((_panel1->m_fIntensityScale)*100),
                                           _panel1->m_bIrradianceCache,
                                           _curFrame);

           sprintf(t_pixbpFileName,"%spixelInfo_%dx%d_photon%d_d%d_i%d_scale%d_irCache(%d)_%04d.pix.bp",
               tp->_savePath,
               _panel1->m_nImageWidth,
               _panel1->m_nImageHeight,
               _panel1->m_nPhotonNum,
               _panel1->m_nSampleRayNumber,
               _panel1->m_nSampleLightRayNumber,
                static_cast<int>((_panel1->m_fIntensityScale)*100),
                _panel1->m_bIrradianceCache,
               _curFrame);

           sprintf(t_imageFileName,"%srenderedImage_%dx%d_photon%d_d%d_i%d_scale%d_irCache(%d)_%04d.bmp",
               tp->_savePath,
               _panel1->m_nImageWidth,
               _panel1->m_nImageHeight,
               _panel1->m_nPhotonNum,
               _panel1->m_nSampleRayNumber,
               _panel1->m_nSampleLightRayNumber,
                static_cast<int>((_panel1->m_fIntensityScale)*100),
               _panel1->m_bIrradianceCache,
               _curFrame);

           sprintf(t_depthInfoFileName,"%sdepth_%dx%d_photon%d_d%d_i%d_scale%d_irCache(%d)_%04d.bmp",
               tp->_savePath,
               _panel1->m_nImageWidth,
               _panel1->m_nImageHeight,
               _panel1->m_nPhotonNum,
               _panel1->m_nSampleRayNumber,
               _panel1->m_nSampleLightRayNumber,
                static_cast<int>((_panel1->m_fIntensityScale)*100),
               _panel1->m_bIrradianceCache,
               _curFrame);

           sprintf(t_normalInfoFileName,"%snormal_%dx%d_photon%d_d%d_i%d_scale%d_irCache(%d)_%04d.bmp",
               tp->_savePath,
               _panel1->m_nImageWidth,
               _panel1->m_nImageHeight,
               _panel1->m_nPhotonNum,
               _panel1->m_nSampleRayNumber,
               _panel1->m_nSampleLightRayNumber,
                static_cast<int>((_panel1->m_fIntensityScale)*100),
               _panel1->m_bIrradianceCache,
               _curFrame);
            
           sprintf(t_irradianceCacheTreeName, "%sIrraTree_%dx%d_photon%d_d%d_i%d_scale%d_irCache(%d)_%04d.irr",
               tp->_savePath,
               _panel1->m_nImageWidth,
               _panel1->m_nImageHeight,
               _panel1->m_nPhotonNum,
               _panel1->m_nSampleRayNumber,
               _panel1->m_nSampleLightRayNumber,
               static_cast<int>((_panel1->m_fIntensityScale)*100),
               _panel1->m_bIrradianceCache,
               _curFrame);


           if(_curFrame   ==   t_startFrame)
           {
                printf("Save irradiance cache tree..\n");
                G_pScene->saveIrradianceTree(t_irradianceCacheTreeName);
           }
           else
           {
                printf("Save increased tree nodes..\n");
                G_pScene->saveIncreasedTreeNode(t_irradianceCacheTreeName);
           }


           //1. Save depth, normal,material images
	        PixelInfo*_pix	=	G_pScene->getPixelInfo();
            if(_pix!=NULL)
            {
     
                int _pixelNumber   = (_panel1->m_nImageWidth) * (_panel1->m_nImageHeight);
                for(int p=  0;p< _pixelNumber; p++ )
                {
                    _pix->m_samplerNumber[p]  =  _panel1->m_nSampleLightRayNumber;
                }
                _pix->m_fIntensityScale            =  _panel1->m_fIntensityScale;
                _pix->m_fCurCameraFloat          =  _pos;
                _pix->m_fCameraFloatStep        =  _fStep;
                _pix->m_nTotalFrames              =  _panel2->m_frameNumber;
                _pix->m_nCurrentFrame            =  _curFrame;
                _pix->m_nStartFrame                =  t_startFrame;
                _pix->m_nEndFrame                 =   t_endFrame;
                _pix->m_nIndirectLightNumber  =  _panel1->m_nSampleRayNumber;
                _pix->m_nPhotonNum               =  _panel1->m_nPhotonNum;
                _pix->m_fTolerance                   =  _panel1->m_fTolerance;
                
                _pix->m_bUseIrradianceCache   =  _panel1->m_bIrradianceCache;
                _pix->m_bSunLight                   =  _panel1->m_bSunLight;
                _pix->m_bSkyLight                   =  _panel1->m_bSkyLight;
                _pix->m_bGradientIC                =  _panel1->m_bGradientIC;
                _pix->m_bBmcIntegration         =  _panel1->m_bBmcIntegration;
                
                _pix->m_fLightScale                  = _panel1->m_fLightScale;
                _pix->m_fSunlightScale             = _panel1->m_fSkylightScale;

                _pix->m_SunLight                     = _panel1->m_SunLight;
                _pix->m_SkyLight                     =  _panel1->m_SkyLight;

               //_pix->saveDepthToImageFile(t_depthInfoFileName);
                //_pix->saveNormalToImageFile(t_normalInfoFileName);
                printf("Save pixel info..\n");
                _pix->savePixelInfoToFile(t_pixFileName);    
            }
            else
            {
                printf("Get pixel pointer == NULL===========>warning!\n");
            }

            //2.Save pixel value file
            CCamera* _camera    =  G_pScene->Get_camera();
            if(_camera==NULL)
                continue;

            printf("Save traced color data..\n");
            _camera->saveResultImageDataToFile(t_pixbpFileName);

            printf("Save result image..\n");
            //3. Save tracing result image
            if(tp->_img!=NULL)
            {
                tp->_img->saveToFileFromColor4f(
                    t_imageFileName,
                    _camera->ResultImagePtr(),
                    _camera->Film_width(),
                    _camera->Film_height(),
                    tp->_theForm->m_fIntensityScale
                    );


            }



        }//end for each frame


    }//end if video render

    	
    //enable UI
    _panel1->enableControl(true);
    _panel2->enableControl(true);
    
	return 0;
}

void CCmdForm::OnBnClickedNormalMcRayTracing()
{
	// TODO: Add your control notification handler code here
	CCol4 cSun = COLOR_BLACK, cSky = COLOR_BLACK;
    //Init progress control
    m_pcProgress.SetRange(1,100);

	UpdateData(true);

	if (m_bSunLight)
	{
		cSun.r() = m_fSunlightScale * (m_SunLight & 0xff) / 255.f;
		cSun.g() = m_fSunlightScale * ((m_SunLight & 0xff00) >> 8) / 255.f;
		cSun.b() = m_fSunlightScale * ((m_SunLight & 0xff0000) >> 16) / 255.f;
		cSun.a() = 1.0f;
	}

	if (m_bSkyLight)
	{
		cSky.r() = m_fSkylightScale * (m_SkyLight & 0xff) / 255.f;
		cSky.g() = m_fSkylightScale * ((m_SkyLight & 0xff00) >> 8) / 255.f;
		cSky.b() = m_fSkylightScale * ((m_SkyLight & 0xff0000) >> 16) / 255.f;
		cSky.a() = 1.0f;
	}

	float fTolerance = (m_bIrradianceCache)? m_fTolerance : -1.f;

    //prepare for thread params
	static TThreadParam tParam;
	tParam._nThread = m_nThread;
	tParam._theForm = this;
    tParam.cSun  =  cSun;
    tParam.cSky  =  cSky;
    tParam.fTolerance  =  fTolerance;
    tParam._controlPanel2    = (CCmd2Form*)(((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
        m_wndSplitter2.GetPane(1,0));
    tParam._controlPanel2->UpdateData(true);
    tParam._doc  =  (CBmcDoc*)(GetDocument());
    strcpy(tParam._savePath,tParam._controlPanel2->getResultPath());
    tParam._img   =  Singleton<ImageUtility>::getSingletonPtr();
	tParam._log   = m_log;

    tParam._controlPanel2->UpdateData(true);
    char PATHNAME[MAX_PATH];
    strcpy(PATHNAME,tParam._controlPanel2->m_sResultPath);
    strcat(PATHNAME,"path.pth");
    tParam._controlPanel2->_savePathToFile(PATHNAME);
    UpdateData(true);

    //laugh the thread
	m_pRaytracingThread = 
		AfxBeginThread (Ray_tracing_proc, &tParam, THREAD_PRIORITY_NORMAL);
}

void CCmdForm::OnBnClickedBmcRayTracing()
{
	// TODO: Add your control notification handler code here
}

void CCmdForm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CBrush brh;
	CDC *pDC;
	RECT rect;

	brh.CreateSolidBrush (m_SunLight);
	pDC = GetDlgItem (IDC_SUNLIGHT_COLOR)->GetDC();
	GetDlgItem (IDC_SUNLIGHT_COLOR)->GetClientRect (&rect);
	pDC->FillRect (&rect,&brh);
	brh.DeleteObject ();

	brh.CreateSolidBrush (m_SkyLight);
	pDC = GetDlgItem (IDC_SKYLIGHT_COLOR)->GetDC();
	GetDlgItem (IDC_SKYLIGHT_COLOR)->GetClientRect (&rect);
	pDC->FillRect (&rect,&brh);
	brh.DeleteObject ();
}

void CCmdForm::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1)
	{
		GetDoc()->Set_result_image(m_fIntensityScale);
		SetTimer(1,1000,0);
	}
	if(nIDEvent==2)
	{	
		int w	=G_pScene->Get_camera()->Film_width();	
		int h =G_pScene->Get_camera()->Film_height();
		GetDoc()->m_theFilteredWin.Set_brightness_scale(m_fIntensityScale);
		GetDoc()->m_theFilteredWin.Set_image(w,h,G_pScene->Get_camera()->FilteredResultImagePtr());
		SetTimer(2,1000,0);
	}
}

void CCmdForm::OnBnClickedShowRayTracingResult()
{
	// TODO: Add your control notification handler code here
	static bool bShow = false;

	bShow = !bShow;
	if (bShow)
	{
		SetTimer(1,1000,0);
		UpdateData();
		GetDoc()->Set_result_image(m_fIntensityScale);
		GetDoc()->m_theResultWin.ShowWindow (SW_SHOW);
	}
	else
	{
		GetDoc()->m_theResultWin.ShowWindow (SW_HIDE);
		KillTimer(1);
	}
}

//if doing filter
void CCmdForm::OnBnClickedApplyFilter()
{
	UpdateData(true);
	if(m_CheckBox_ApplyFilter)
	{
		GetDlgItem(IDC_CHECK_PIXEL)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_PERCEPTION)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_NORMAL)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_DEPTH)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_MATERIAL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_PIXEL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_PERCEPTION)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_DEPTH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_MATERIAL)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_FILTERBOXSIZE)->EnableWindow(true);
		GetDlgItem(IDC_STATIC_FILTERBOXSIZE)->EnableWindow(true);	
	}
	else
	{
		GetDlgItem(IDC_CHECK_PIXEL)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_PERCEPTION)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_NORMAL)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_DEPTH)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_MATERIAL)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PIXEL)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PERCEPTION)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DEPTH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_MATERIAL)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_FILTERBOXSIZE)->EnableWindow(false);
		GetDlgItem(IDC_STATIC_FILTERBOXSIZE)->EnableWindow(false);

	}
	
}

//start filtering work
void CCmdForm::OnStartFiltering()
{
	UpdateData(true);
	//launch thread 
	DWORD t_threadID;
	HANDLE m_filterThreadHandle =	(HANDLE)_beginthreadex(
		NULL,
		0,
		(THREADFUNC)(CCmdForm::FilterThread),
		(LPVOID)this,
		0,
		(THREADID)(&t_threadID)
		);

	if(m_filterThreadHandle)
	{
		SetThreadPriority(m_filterThreadHandle,THREAD_PRIORITY_NORMAL);
		CloseHandle(m_filterThreadHandle);
		printf("CCmdForm::FilterThread thread launched!\n");
	}
	

}

//show filtered image
void CCmdForm::OnShowFilteredImage()
{	
	// TODO: Add your control notification handler code here
	static bool bShow2 = false;
	bShow2 = !bShow2;    
	
	if (bShow2)
	{	
		UpdateData(true);
		int w	=G_pScene->Get_camera()->Film_width();	
		int h =G_pScene->Get_camera()->Film_height();
		SetTimer(2,1000,0);
		GetDoc()->m_theFilteredWin.Set_brightness_scale(m_fIntensityScale);
		GetDoc()->m_theFilteredWin.Set_image(w,h,G_pScene->Get_camera()->FilteredResultImagePtr());
		GetDoc()->m_theFilteredWin.ShowWindow(SW_SHOW);
	}
	else
	{
		GetDoc()->m_theFilteredWin.ShowWindow(SW_HIDE);
		KillTimer(2);
	}


}

void CCmdForm::OnClickSaveTracingResult()
{
	// Image file name
	char	fn[256];
	CFileDialog myfiledlg(FALSE, "pix", NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
		"Pix Files (*.pix)|*.pix|All File(*.*)|*.*||", NULL);
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

	PixelInfo* t_pixelInfo	=	G_pScene->getPixelInfo();
	CCamera* t_camera	=	G_pScene->Get_camera();

	if(t_pixelInfo && t_camera)
	{
		t_pixelInfo->savePixelInfoToFile(fn);
		//int i;
		//for(i=0;fn[i]!='\0';i++);
		//fn[i]	=	
		strcat(fn,".bp");
		t_camera->saveResultImageDataToFile(fn);
	}

	return;
}

void CCmdForm::OnClickLoadTracingResult()
{
	// Image file name
	char	fn[256];
	CFileDialog myfiledlg(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
		"Pix Files (*.pix)|*.pix|All File(*.*)|*.*||", NULL);
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

	PixelInfo*& t_pixelInfo	=	G_pScene->getPixelInfo();
	if(t_pixelInfo==NULL)
		t_pixelInfo	=	new PixelInfo(1,1);

	CCamera* t_camera	=	G_pScene->Get_camera();

	if(t_pixelInfo && t_camera)
	{
		if(t_pixelInfo->loadPixelInfoFromFile(fn)==false)
		{
			MessageBox("Load pixel file failed!", "error",MB_OK|MB_ICONWARNING);
			return;
		}
	
		strcat(fn,".bp");
		if(t_camera->loadResultImageDataFromFile(fn)==false)
		{
			MessageBox("Load image file failed!", "error",MB_OK|MB_ICONWARNING);
			return;
		}

		m_nImageWidth	=	t_pixelInfo->m_width;
		m_nImageHeight	=	t_pixelInfo->m_height;
		UpdateData(false);
        
        //save loaded depth and normal data to image file to check if loaded correctly.
        t_pixelInfo->saveNormalToImageFile("c:\\loadNormal.bmp");
        t_pixelInfo->saveDepthToImageFile("c:\\loadDepth.bmp");
	}

	return;
}

void CCmdForm::saveConfigDataToDoc()
{
    UpdateData(true);
    //get doc
    CBmcDoc* _doc = GetDoc();
    Config* _config  = _doc->getConfig();

    //UI data   ====>  m_config in Doc
    _config->setPreviewType(m_nPreviewStyle);
    _config->setSunLight(m_bSunLight);
    _config->setSkyLight(m_bSkyLight);
    _config->setSunLightColor(m_SunLight);
    _config->setSkyLightColor(m_SkyLight);
    _config->setLightScale(m_fLightScale);
    _config->setSunLightScale(m_fSunlightScale);
    _config->setSkyLightScale(m_fSkylightScale);

    _config->setPhotonNumber(m_nPhotonNum);
    _config->setSamplerRayNumber(m_nSampleRayNumber);
    _config->setSamplerLightRayNumber(m_nSampleLightRayNumber);
    _config->setImageWidth(m_nImageWidth);
    _config->setImageHeight(m_nImageHeight);
    
    _config->setApplyFilter(m_CheckBox_ApplyFilter);
    _config->setPixelFilterEnable(m_CheckBox_Pixel);
    _config->setPerceptionFilterEnable(m_CheckBox_Perception);
    _config->setNormalFilterEnable(m_CheckBox_Normal);
    _config->setDepthFilterEnable(m_CheckBox_Depth);
    _config->setMaterialEnable(m_CheckBox_Material);

    _config->setFilterBoxSize(m_Edit_FilterBoxSize);
    _config->setPixelFilterC(m_Edit_Pixel);
    _config->setPerceptionFilterC(m_Edit_Perception);
    _config->setNormalFilterC(m_Edit_Normal);
    _config->setDepthFilterC(m_Edit_Depth);
    _config->setMaterialFiterC(m_Edit_Material);

    _config->setIntensityScale(m_fIntensityScale);
    _config->setThreadNumber(m_nThread);
    _config->setTolerance(m_fTolerance);
    _config->setIrradianceCache(m_bIrradianceCache);
    _config->setGradient(m_bGradientIC);
    _config->setBMC(m_bBmcIntegration);

}


void CCmdForm::_applyCamera(const CameraInfo& _cameraInfo)
{

    CBmcDoc* _doc = (CBmcDoc*)(GetDocument());
    if(!_doc)
        return;
    memcpy((void*)(G_pArcBall->GetBallMatrix()),(void*)((_cameraInfo.m_rotateMatrix)),sizeof(float) * 16);

    _doc->Set_distance_scale(_cameraInfo.m_fDistScale);
    _doc->Set_distance(_cameraInfo.m_fDistance);
    _doc->Set_translate(_cameraInfo.m_vTrans);
    _doc->Set_cent(_cameraInfo.m_vCent);
    _doc->Set_eye_pos(_cameraInfo.m_vEye);

    //CBmcApp* _app    =  (CBmcApp*)(AfxGetApp());
    //if(_app==NULL)
    //    return;
    //CMainFrame* _frame =  (CMainFrame *)(_app->m_pMainWnd);

    //if(_frame==NULL)
    //    return;

    //CBmcView* _view  =  (CBmcView *) ((_frame->m_wndSplitter2).GetPane(0,0));     
    //if(_view)
    //    _view->DrawScene(); 

}


void CCmdForm::loadConfigDataFromDoc()
{
    //get doc
    CBmcDoc* _doc = GetDoc();
    Config* _config  = _doc->getConfig();

    //config data ====> UI 
    m_nPreviewStyle   =  _config->getPreviewType();
    m_bSunLight         = _config->getSunLight();
    m_bSkyLight         =  _config->getSunLight();
    m_SunLight           = _config->getSunLightColor();
    m_SkyLight           =  _config->getSkyLightColor();
    m_fLightScale        = _config->getLightScale();
    m_fSunlightScale   = _config->getSunLightScale();
    m_fSkylightScale   =  _config->getSkyLightScale();
    
    m_nPhotonNum     =  _config->getPhotonNumber();
    m_nSampleRayNumber  =  _config->getSamplerRayNumber();
    m_nSampleLightRayNumber   = _config->getSamplerLightRayNumber();
    m_nImageWidth    = _config->getImageWidth();
    m_nImageHeight   = _config->getImageHeight();

    m_CheckBox_ApplyFilter    =  _config->getApplyFilter();
    m_CheckBox_Pixel             =  _config->getPixelFilterEnable();
    m_CheckBox_Perception     = _config->getPerceptionFilterEnable();
    m_CheckBox_Normal         =  _config->getNormalFilterEnable();
    m_CheckBox_Depth           =  _config->getDepthFilterEnable();
    m_CheckBox_Material        =  _config->getMaterialEnable();
    
    m_Edit_FilterBoxSize =  _config->getFilterBoxSize();
    m_Edit_Pixel             =  _config->getPixelFilterC();
    m_Edit_Perception     =  _config->getPerceptionFilterC();
    m_Edit_Normal          = _config->getNormalFilterC();
    m_Edit_Depth            = _config->getDepthFilterC();
    m_Edit_Material         = _config->getMaterialFilterC();

    m_fIntensityScale      = _config->getIntensityScale();
    m_nThread                = _config->getThreadNumber();
    m_fTolerance             = _config->getTolerance();
    m_bIrradianceCache   = _config->getIrradianceCache();
    m_bGradientIC          =  _config->getGradient();
    m_bBmcIntegration   =  _config->getBMC();

    
    UpdateData(false);


}


void CCmdForm::enableControl(bool enable)
{
    GetDlgItem (IDC_LIGHT_SCALE)->EnableWindow (enable);
    GetDlgItem (IDC_LIGHT_SOURCE_SCALE_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_SUNLIGHT_CK)->EnableWindow (enable);
    GetDlgItem (IDC_SUNLIGHT_COLOR)->EnableWindow (enable);
    GetDlgItem (IDC_SUNLIGHT_SCALE)->EnableWindow (enable);
    GetDlgItem (IDC_SUNLIGHT_SCALE_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_SKYLIGHT_CK)->EnableWindow (enable);
    GetDlgItem (IDC_SKYLIGHT_COLOR)->EnableWindow (enable);
    GetDlgItem (IDC_SKYLIGHT_SCALE)->EnableWindow (enable);
    GetDlgItem (IDC_SKYLIGHT_SCALE_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_PHOTON_NUMBER)->EnableWindow (enable);
    GetDlgItem (IDC_PHOTON_NUMBER_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_RAY_NUMBER)->EnableWindow (enable);
    GetDlgItem (IDC_RAY_NUMBER_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_LIGHT_TEST_RAY_NUMBER)->EnableWindow (enable);
    GetDlgItem (IDC_SHADOW_RAY_NUMBER_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_IRRADIANCE_CACHE_CK)->EnableWindow (enable);
    GetDlgItem (IDC_IRRADIANCE_CACHE_TOLERANCE)->EnableWindow (enable);
    GetDlgItem (IDC_GRADIENT_IRRADIANCE_CACHE_CK)->EnableWindow (enable);
    GetDlgItem (IDC_BMC_INTEGRATION_CB)->EnableWindow (enable);

    GetDlgItem (IDC_IMAGE_WIDTH)->EnableWindow (enable);
    GetDlgItem (IDC_IMAGE_WIDTH_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_IMAGE_HEIGHT)->EnableWindow (enable);
    GetDlgItem (IDC_IMAGE_HEIGHT_SPIN)->EnableWindow (enable);

    GetDlgItem (IDC_THREAD_NUMBER)->EnableWindow (enable);

    GetDlgItem (IDC_NORMAL_MC_RAY_TRACING)->EnableWindow (enable);
    GetDlgItem (IDC_BMC_RAY_TRACING)->EnableWindow (enable);

    GetDlgItem(IDC_BUTTON_STARTFILTER)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_SHOW_FILTER)->EnableWindow(enable);

    GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(enable);
    GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(enable);


}

DWORD WINAPI CCmdForm::FilterThread(LPVOID param)
{
	CCmdForm* _object	=	(CCmdForm*)param;


	//Do filtering work if needed.
	_object->GetDlgItem(IDC_BUTTON_STARTFILTER)->EnableWindow(false);


	if(_object->m_CheckBox_ApplyFilter==false)
	{
		_object->MessageBox("Enable Filtering first.","Warning",MB_OK|MB_ICONWARNING);
		_object->GetDlgItem(IDC_BUTTON_STARTFILTER)->EnableWindow(true);
		return 0;
	}

	if((_object->m_CheckBox_Pixel||_object->m_CheckBox_Perception||_object->m_CheckBox_Normal||
		_object->m_CheckBox_Depth||_object->m_CheckBox_Material)==false)
	{
		_object->MessageBox("Select at least one filter component.","Warning",MB_OK|MB_ICONWARNING);
		_object->GetDlgItem(IDC_BUTTON_STARTFILTER)->EnableWindow(true);
		return 0;
	}

	G_pScene->applyFilterForSingleFrame(
		_object->m_Edit_FilterBoxSize,		
		_object->m_Edit_Pixel,
		_object->m_Edit_Perception,
		_object->m_Edit_Normal,
		_object->m_Edit_Depth,
		_object->m_Edit_Material,
		_object->m_CheckBox_Pixel,
		_object->m_CheckBox_Perception,
		_object->m_CheckBox_Normal,
		_object->m_CheckBox_Depth,
		_object->m_CheckBox_Material);


	_object->GetDlgItem(IDC_BUTTON_STARTFILTER)->EnableWindow(true);

	return 0;

}

