// BmcDoc.cpp : implementation of the CBmcDoc class
//

#include "stdafx.h"
#include "Bmc.h"

#include "MainFrm.h"
#include "BmcDoc.h"
#include "BmcView.h"
#include ".\bmcdoc.h"

#include "BmcTR.h"
#include "ArcBall.h"
#include "Log.h"
#include "Singleton.h"
#include "CmdForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBmcTR *G_pScene;
CArcBall *G_pArcBall;


// CBmcDoc

IMPLEMENT_DYNCREATE(CBmcDoc, CDocument)

BEGIN_MESSAGE_MAP(CBmcDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
END_MESSAGE_MAP()

extern CBmcApp theApp;

// CBmcDoc construction/destruction

CBmcDoc::CBmcDoc():
m_pRandColor(NULL),
m_pPanel1(NULL)
{
	// TODO: add one-time construction code here
	G_pScene       = new CBmcTR;
	G_pArcBall     = new CArcBall;
	m_fDistScale   = 1.f;
	m_fDistance    = 1.f;
	m_nMouseStatus = NO_BUTTON;
	m_nRenderStyle = RENDER_DRAW_SOLID;
	m_bLightOn     = true;
	m_vTrans       = CVec3(0.f, 0.f, 0.f);

	G_pArcBall->InitBall();
	m_theResultWin.Create (IDD_DIALOG_IMAGE, NULL);
	m_theResultWin.ShowWindow (SW_HIDE);

	m_theFilteredWin.Create(IDD_DIALOG_FILTEREDIMAGE,NULL);
	m_theFilteredWin.ShowWindow(SW_HIDE);

    m_config   =new Config;

    m_keyMotions.Init(1,11);
    m_keyMotions.SetInterpolationMethods( QUATERNION_CUBIC, CUBIC);
    
    //Log initialization
    _log  =  Singleton<ZHLog>::getSingletonPtr();
    if(_log)
    {
        if(_log->initialize("c:\\Logs\\"))
        {
            printf("Log system initialize successfully!\n");
        }
        else
        {
            printf("Log system initialize failed!\n");
        }
    }
    else
    {
        printf("Log system initialize failed!\n");
    }



}

CBmcDoc::~CBmcDoc()
{
	MY_DELETE(G_pScene);
	MY_DELETE(G_pArcBall);
    MY_DELETE(m_config);
    MY_DELETE(_log);
    MY_DELETE_ARRAY(m_pRandColor);
	m_theResultWin.DestroyWindow();
	m_theFilteredWin.DestroyWindow();
}

// CBmcDoc serialization

void CBmcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CBmcDoc diagnostics

#ifdef _DEBUG
void CBmcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBmcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBmcDoc commands

BOOL CBmcDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	G_pScene->Load_OBJ((char *)lpszPathName);
	UpdateAllViews (NULL);
    
    //create rand color
    int t_triangleNum  =  G_pScene->TriangleNum();
    if(t_triangleNum<=0)
        return false;
    MY_DELETE_ARRAY(m_pRandColor);
    m_pRandColor   =  new CCol4[t_triangleNum];
    if(m_pRandColor==NULL)
        return false;

    for (int nTID = 0; nTID <t_triangleNum ; nTID ++)
    { 
        m_pRandColor[nTID] =  CCol4(RANDOM_0_1,RANDOM_0_1,RANDOM_0_1);
    }

    m_pPanel1    =  (CCmdForm*)(((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
        m_wndSplitter.GetPane(0,0));
    if(m_pPanel1!=NULL)
        m_pPanel1->UpdateData(true);

	return TRUE;
}

void CBmcDoc::Init_viewport()
{
	CVec3 vMax, vMin;

	G_pScene->Get_boundingbox(vMax, vMin);
	m_vCent = CVec3((vMax[0]+vMin[0]),(vMax[1]+vMin[1]),(vMax[2]+vMin[2]))/2.f;
	m_vEye  = m_vCent;
	m_vEye.z += (vMax - vMin).Length() * m_fDistance;
		
    //get image size;
    int viewWidth    = 0;
    int viewHeight   =  0;
    m_pPanel1->UpdateData(true);
    int _w   =  m_pPanel1->m_nImageWidth;
    int _h    = m_pPanel1->m_nImageHeight;
    if(_h<1)
        _h=1;
    if(m_nWinHeight<1)
        m_nWinHeight=1;
    float scaleImage = static_cast<float>(_w)/static_cast<float>(_h);
    float scaleWin    =  static_cast<float>(m_nWinWidth)/static_cast<float>(m_nWinHeight);
    if(scaleImage>scaleWin)
    {
        viewWidth     =  m_nWinWidth;
        viewHeight    =  static_cast<int>(m_nWinWidth/scaleImage);  
    }
    else
    {
        viewWidth     =  static_cast<int>(m_nWinHeight * scaleImage);  
        viewHeight    =  m_nWinHeight;
    }

	//viewport
    float fSize = (vMax - vMin).Length();
	glViewport ( 0, 0, viewWidth, viewHeight);

    glEnable (GL_LIGHT0);
    glClearColor (0.f, 0.f, 0.0f, 1.f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-viewWidth/2.0f,viewWidth/2.0f,-viewHeight/2.0f,viewHeight/2.0f,1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,1,0,0,0,0,1,0);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f,1.0f,0.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-viewWidth/2.0f+1,-viewHeight/2.0f+1,-1.0f);
        glVertex3f(-viewWidth/2.0f+1, viewHeight/2.0f-1,-1.0f);
        glVertex3f( viewWidth/2.0f-1, viewHeight/2.0f-1,-1.0f);
        glVertex3f( viewWidth/2.0f-1,-viewHeight/2.0f+1,-1.0f);
    glEnd();
    glFlush();


    //projection
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective ( 
		45*m_fDistScale, 
		(float)(_w)/(float)(_h),
		fSize * 0.01f, 
		fSize * 2.0f);

	//model view
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();

	gluLookAt(m_vEye.x, m_vEye.y, m_vEye.z, m_vCent.x, m_vCent.y, m_vCent.z, 0.f, 1.f, 0.f);
	glTranslatef(m_vTrans.x, m_vTrans.y, m_vTrans.z);

	// 1. Set rendering feature
	glShadeModel ( GL_SMOOTH );
	glDepthFunc ( GL_LESS );
	glEnable ( GL_DEPTH_TEST );

	GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv (GL_LIGHT0, GL_SPECULAR, specular);



	//glPushMatrix ();
	glMultMatrixf(G_pArcBall->GetBallMatrix());
	glGetDoublev (GL_MODELVIEW_MATRIX,  m_mMM);
	glGetDoublev (GL_PROJECTION_MATRIX, m_mPM);
}

void CBmcDoc::DrawScene(int nWidth, int nHeight)
{
    if(m_pPanel1==NULL)
    {
        m_pPanel1    =  (CCmdForm*)(((CMainFrame *) ((CBmcApp *)AfxGetApp())->m_pMainWnd)->
            m_wndSplitter.GetPane(0,0));
        if(m_pPanel1!=NULL)
            m_pPanel1->UpdateData(true);
    }

    //window UI size
	m_nWinWidth  = nWidth;
	m_nWinHeight = nHeight;

	// Initial rendering window
	Init_viewport();

	// Draw the scene

	if (m_bLightOn) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);

	CVec3 *vVrt, *vNml;
	int   nTID, nVID;

	glEnable(GL_CULL_FACE);
    switch (m_nRenderStyle)
	{
	case RENDER_DRAW_POINTS:
        {
		    glColor3f (1.f, 1.f, 0.5f);
		    glDisable(GL_LIGHTING);
		    glBegin(GL_POINTS);
		    for (nTID = 0; nTID < G_pScene->TriangleNum(); nTID ++)
		    {
			    for (nVID = 0; nVID < 3; nVID ++)
			    {
				    vNml = G_pScene->Get_triangle_nml(nTID, nVID);
				    vVrt = G_pScene->Get_triangle_ver(nTID, nVID);
				    glNormal3fv ((float *)vNml);
				    glVertex3fv ((float *)vVrt);
			    }
		    }
		    glEnd();
		    glColor3f (1.f, 1.f, 1.f);

		    break;
        }
	case RENDER_DRAW_WIREFRAME:
    {
		    glColor3f (0.5f, 0.5f, 1.f);
		    glDisable(GL_LIGHTING);
		    for (nTID = 0; nTID < G_pScene->TriangleNum(); nTID ++)
		    {
			    glBegin(GL_LINE_LOOP);
			    for (nVID = 0; nVID < 3; nVID ++)
			    {
				    vNml = G_pScene->Get_triangle_nml(nTID, nVID);
				    vVrt = G_pScene->Get_triangle_ver(nTID, nVID);
				    glNormal3fv ((float *)vNml);
				    glVertex3fv ((float *)vVrt);
			    }
			    glEnd();
		    }
		    glColor3f (1.f, 1.f, 1.f);

		    break;
    }
	case RENDER_DRAW_SOLID:
    {
		    CBspMaterial *pMtrl, *pOldMtrl = NULL;
		    CVec3 *vTex;
		    for (nTID = 0; nTID < G_pScene->TriangleNum(); nTID ++)
		    {
			    pMtrl = G_pScene->Get_tri_material(nTID);
			    if (pMtrl != pOldMtrl)
			    {
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   pMtrl->m_fAmbient  );
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   pMtrl->m_fDiffuse  );
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  pMtrl->m_fSpecular );
				    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  pMtrl->m_fEmissive );
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pMtrl->m_fShininess);

				    if (strlen(pMtrl->m_sTexture) > 4)
				    {
					    pMtrl->m_iTexture.SetTexture();
					    glEnable(GL_TEXTURE_2D);
				    }
				    else glDisable(GL_TEXTURE_2D);

				    pOldMtrl = pMtrl;
			    }

			    glBegin(GL_TRIANGLES);
			    for (nVID = 0; nVID < 3; nVID ++)
			    {
				    vNml = G_pScene->Get_triangle_nml(nTID, nVID);
				    vVrt = G_pScene->Get_triangle_ver(nTID, nVID);
				    vTex = G_pScene->Get_triangle_tex(nTID, nVID);

				    glNormal3fv   ((float *)vNml);
				    glTexCoord2fv ((float *)vTex);
				    glVertex3fv   ((float *)vVrt);
			    }
			    glEnd();
		    }

		    break;
    }
    case RENDER_DRAW_COLOR_TRIANGLE:
    {

        glDisable(GL_LIGHTING);
        for (nTID = 0; nTID < G_pScene->TriangleNum(); nTID ++)
        {  
            glColor3f (m_pRandColor[nTID].r(),m_pRandColor[nTID].g(),m_pRandColor[nTID].b());
            glBegin(GL_TRIANGLES);
            for (nVID = 0; nVID < 3; nVID ++)
            {
                vNml = G_pScene->Get_triangle_nml(nTID, nVID);
                vVrt = G_pScene->Get_triangle_ver(nTID, nVID);
                glNormal3fv ((float *)vNml);
                glVertex3fv ((float *)vVrt);
            }
            glEnd();
        }
        glColor3f (1.f, 1.f, 1.f);


        break;
    }
    default:
        break;

	}

	glPopMatrix();
}

void CBmcDoc::Init_BMC_raytracing (
                                   int nWidth,        // Result image width
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
								   )
{
	//-------------------------------------------------------------------------
	// 1. initialize camera parameters
	//-------------------------------------------------------------------------
	float *IM;
	CVec3 vv, vp, vc, vu, vMin, vMax;
	
	IM = G_pArcBall->GetInvertedBallMatrix();
	
	vv = m_vEye - m_vTrans;
	vp = CVec3 ( IM[0]*vv.x + IM[4]*vv.y + IM[8]*vv.z,
		IM[1]*vv.x + IM[5]*vv.y + IM[9]*vv.z,
		IM[2]*vv.x + IM[6]*vv.y + IM[10]*vv.z );
	
	vv = m_vCent - m_vTrans;
	vc = CVec3 ( IM[0]*vv.x + IM[4]*vv.y + IM[8]*vv.z,
		IM[1]*vv.x + IM[5]*vv.y + IM[9]*vv.z,
		IM[2]*vv.x + IM[6]*vv.y + IM[10]*vv.z );
	
	vu = CVec3 (IM[4], IM[5], IM[6]);
	vu.Normalize();
	
	vp.x += vp.x * 0.001f * RANDOM_0_1;
	vp.y += vp.y * 0.001f * RANDOM_0_1;
	vp.z += vp.z * 0.001f * RANDOM_0_1;
	
	G_pScene->Get_boundingbox(vMax, vMin);
	float fSize = (vMax - vMin).Length();

	G_pScene->Get_camera()->InitCamera (
        45*m_fDistScale,   //View Angle
        vp,                        //Eye
        vc,                        //Cent
        vu,                        //Upright
		fSize * 0.01f,         //near
        fSize * 2,               //far
        nWidth,                 //width
        nHeight                 //height
        );  

    //-------------------------------------------------------------------------
    // 2. render effect
    //-------------------------------------------------------------------------
    G_pScene->setRenderEffect(
        m_renderEffect.m_bDirect_light,
        m_renderEffect.m_bIndirect_light,
        m_renderEffect.m_bMirror_effect,
        m_renderEffect.m_bCaustic_effect,
        m_renderEffect.m_bDirectional_light,
        m_renderEffect.m_bTotally_photonmap);

	//-------------------------------------------------------------------------
	// 3. other parameters
	//-------------------------------------------------------------------------
	G_pScene->Set_BMC_parameters (fLightScale, cSun, cSky, nPhoton, nRay,
		nLightRay, fTolerance, bGradient, bBmcIntegration);
}

void CBmcDoc::Set_result_image(float fIntensityScale)
{
	m_theResultWin.Set_brightness_scale (fIntensityScale);
	m_theResultWin.Set_image (
		G_pScene->Get_camera()->Film_width(),
		G_pScene->Get_camera()->Film_height(),
		G_pScene->ResultImagePtr());
}


void CBmcDoc::setRenderEffect( 
                     BOOL directLight,
                     BOOL inDirectLight,
                     BOOL MirrorEffect,
                     BOOL CausticEffect,
                     BOOL DirectionalLight,
                     BOOL TotallyPhotonMap
                     )
{
    m_renderEffect.m_bDirect_light      = directLight;
    m_renderEffect.m_bIndirect_light   = inDirectLight;
    m_renderEffect.m_bMirror_effect     = MirrorEffect;
    m_renderEffect.m_bCaustic_effect   = CausticEffect;
    m_renderEffect.m_bDirectional_light   = DirectionalLight;
    m_renderEffect.m_bTotally_photonmap   = TotallyPhotonMap;

}

void CBmcDoc::getRenderEffect(RenderEffect& _effect)
{

    memcpy(&_effect,&m_renderEffect,sizeof(RenderEffect));

}