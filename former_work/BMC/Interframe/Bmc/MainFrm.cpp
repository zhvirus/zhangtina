// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Bmc.h"

#include "MainFrm.h"

#include "BmcView.h"
#include "CmdForm.h"
#include "Cmd2Form.h"
#include ".\mainfrm.h"
#include "BmcTR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int G_FRAME_WIDTH     = 1024;
const int G_FRAME_HEIGHT    = 900; 
// CMainFrame

extern CBmcTR *G_pScene;
extern CArcBall *G_pArcBall;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_COMMAND(ID_CONFIG_SAVE, &CMainFrame::OnConfigSave)
    ON_COMMAND(ID_CONFIG_LOAD, &CMainFrame::OnConfigLoad)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	this->MoveWindow(10,10,G_FRAME_WIDTH,G_FRAME_HEIGHT,true);
    this->m_strTitle = CString("Tracing");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic( this, 1, 2, WS_CHILD | WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// First splitter pane
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CCmdForm), CSize(230,G_FRAME_HEIGHT), pContext))
	{
		TRACE("Failed to create command view pane\n");
		return FALSE;
	}

    // Second splitter pane
    if (!m_wndSplitter2.CreateStatic( &m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE,m_wndSplitter.IdFromRowCol(0,1)))
    {
        TRACE("Failed to CreateStaticSplitter\n");
        return FALSE;
    }

	//// Second splitter pane
	if (!m_wndSplitter2.CreateView(0, 0,
		RUNTIME_CLASS(CBmcView), CSize(G_FRAME_WIDTH -  230,G_FRAME_HEIGHT - 350), pContext))
	{
		TRACE("Failed to create preview pane\n");
		return FALSE;
	}

 //   // Third splitter pane
    if (!m_wndSplitter2.CreateView(1, 0,
        RUNTIME_CLASS(CCmd2Form), CSize(G_FRAME_WIDTH -  230,350), pContext))
    {
        TRACE("Failed to create command view pane\n");
        return FALSE;
    }	

	return TRUE;
}

void CMainFrame::OnConfigSave()
{
    // Image file name
    char	fn[256];
    CFileDialog myfiledlg(FALSE, "bmc", NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "BMC config Files (*.bmc)|*.bmc|All File(*.*)|*.*||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else
    {
        return;
    }

    //get doc
    CBmcDoc* _doc = (CBmcDoc*)(((CCmdForm*)(m_wndSplitter.GetPane(0,0)))->GetDocument());

    //save UI data to config
    ((CCmdForm*)(m_wndSplitter.GetPane(0,0)))->saveConfigDataToDoc();
    ((CCmd2Form*)(m_wndSplitter2.GetPane(1,0)))->saveConfigDataToDoc();

    //save camera info here
    CCamera* _cam =   G_pScene->Get_camera();
    Config* _config  = _doc->getConfig();
    
    _config->m_fViewAngle_trace     = _cam->ViewAngle();
    _config->m_vEye_trace              = _cam->Eye();
    _config->m_vCent_trace             =_cam->Cent();
    _config->m_vUpright_trace        =  _cam->Upright();
    _config->m_fNear_trace             =  _cam->Near();
    _config->m_fFar_trace               = _cam->Far();

    memcpy((void*)(&(_config->m_arcBall)),(void*)G_pArcBall,sizeof(CArcBall));
    
    _config->m_fDistScale   =  _doc->Get_distance_scale();
    _config->m_fDistance    =  _doc->Get_distance();
    _config->m_vTrans        = _doc->Get_translate();
    _config->m_vCent         = _doc->Get_cent_pos();
    _config->m_vEye           = _doc->Get_eye_pos();
    
    //save config data to file
    _config->writeToFile(fn);
    
    
}

void CMainFrame::OnConfigLoad()
{
    // Image file name
    char	fn[256];
    CFileDialog myfiledlg(true, "bmc", NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
        "BMC config Files (*.bmc)|*.bmc|All File(*.*)|*.*||", NULL);
    if ( myfiledlg.DoModal() == IDOK)
    {
        sprintf( fn, "%s", myfiledlg.GetPathName() );
        if( strlen( fn ) == 0 )
        {
            MessageBox("Not an available file name", "Error", MB_OK|MB_ICONWARNING);
            return;
        }
    }
    else
    {
        return;
    }

    //Load config data from file
    CBmcDoc* _doc = (CBmcDoc*)(((CCmdForm*)(m_wndSplitter.GetPane(0,0)))->GetDocument());
    _doc->getConfig()->loadFromFile(fn);

    //Update UI data from config
    ((CCmdForm*)(m_wndSplitter.GetPane(0,0)))->loadConfigDataFromDoc();
    ((CCmd2Form*)(m_wndSplitter2.GetPane(1,0)))->loadConfigDataFromDoc();

    //update camera info here
    CCamera* _cam =   G_pScene->Get_camera();
    Config* _config  = _doc->getConfig();

    _cam->m_fViewAngle    =      _config->m_fViewAngle_trace; 
    _cam->m_vEye             =      _config->m_vEye_trace;          
    _cam->m_vCent            =     _config->m_vCent_trace;            
    _cam->m_vUpright        =    _config->m_vUpright_trace;       
    _cam->m_fNear            =      _config->m_fNear_trace;           
    _cam->m_fFar              =      _config->m_fFar_trace;              

    memcpy((void*)G_pArcBall,(void*)(&(_config->m_arcBall)),sizeof(CArcBall));

    _doc->Set_distance_scale(_config->m_fDistScale);
    _doc->Set_distance(_config->m_fDistance);
    _doc->Set_translate(_config->m_vTrans);
    _doc->Set_cent(_config->m_vCent);
    _doc->Set_eye_pos(_config->m_vEye);


}
