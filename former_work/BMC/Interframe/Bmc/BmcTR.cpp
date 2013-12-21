#include "StdAfx.h"
#include ".\bmctr.h"
#include "atlimage.h"

#include "PixelInfo.h"
#include "Singleton.h"
#include "Image.h"
#include "Log.h"


using namespace ZH;


CBmcTR::CBmcTR(void):
m_bLightInitialzed(false)
{
	m_fLightScale = 1.f;
	m_cSun = m_cSky = COLOR_BLACK;
	m_bRunning = false;
	m_bBmcOn = FALSE;
	m_pIrraTree = NULL;

	m_fIcGCeof1 = NULL;
	m_fIcGCeof2 = NULL;
    m_pPxlInfo    = NULL;


}

CBmcTR::~CBmcTR(void)
{
	//for(int i=0;i<4;i++)
	//{
	//	if(hThread[i]!=0)
	//	{
	//		DWORD _exitCode;
	//		GetExitCodeThread(hThread[i], &_exitCode);
	//		ExitThread(_exitCode);
	//	}
	//}

	MY_DELETE_ARRAY (m_fIcGCeof1);
	MY_DELETE_ARRAY (m_fIcGCeof2);

}

///////////////////////////////////////////////////////////////////////////////
//		Load_OBJ
// Load the obj scene using the function of the base class
// Modify all material to make sure ks + kd <=1, so that the energy would be 
// preserved.
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::Load_OBJ (char *fn, float fMapSize)
{
	CBspTR::Load_OBJ (fn, fMapSize);
	_Check_materials();
	_Delete_photon_maps();
}

///////////////////////////////////////////////////////////////////////////////
//		Set_BMC_parameters
// Set parameters for MC ray tracing
///////////////////////////////////////////////////////////////////////////////
bool CBmcTR::Set_BMC_parameters (float fLightScale, CCol4 cSun, CCol4 cSky,
								  int nPhoton, int nRay, int nLightRay, 
								  float fTolerance, BOOL bGradient, BOOL bBmc)
{
	if (m_bRunning) return false;

	m_fLightScale = fLightScale;
	m_cSun = cSun;
	m_cSky = cSky;
	m_nDistrRayNum = nRay;
	m_nLightRay = nLightRay;
	m_bRunning = false;
	m_fTolerance = fTolerance;
	m_bGradient = bGradient;
	m_bBmc = bBmc;

    if(m_nGPhotons!=nPhoton)
    {
        m_bPhotonMapsInitialized  =  false;
        m_nGPhotons    =  nPhoton;
    }

	//allocate imageInfo memory 
	int _w	=	m_theCamera.Film_width();
	int _h	=	m_theCamera.Film_height();
	if(m_pixelInfo==NULL)
	{
		m_pixelInfo	=	new PixelInfo(_w, _h);
        if(m_pixelInfo==NULL)
        {
            (Singleton<ZHLog>::getSingletonPtr())->assertError("bool CBmcTR::Set_BMC_parameters--> new pixelInfo failed!");
            return false;
        }
	}
    else if(m_pixelInfo->m_height!=_h||m_pixelInfo->m_width!=_w)
    {
        MY_DELETE(m_pixelInfo);
        m_pixelInfo   = new PixelInfo(_w,_h);
        if(m_pixelInfo==NULL)
        {
            (Singleton<ZHLog>::getSingletonPtr())->assertError("bool CBmcTR::Set_BMC_parameters--> new pixelInfo failed!");
            return false;
        }
    }
    else
    {
		
    }

    m_pixelInfo->clearAllData();
	

	// The inclination and azimuth resolution
	if (nRay > 10)
	{
		m_nPolar   = (int) sqrt((double)(nRay>>2));
		m_nAzimuth = nRay / m_nPolar;
	}
	else
	{
		m_nPolar   = 6;  // A very small number of sample
		m_nAzimuth = 12;
	}

	MY_DELETE_ARRAY (m_fIcGCeof1);
	MY_DELETE_ARRAY (m_fIcGCeof2);

	m_fIcGCeof1 = new float [m_nPolar + 1];
	m_fIcGCeof2 = new float [m_nPolar + 1];
    if(m_fIcGCeof1==NULL||m_fIcGCeof2==NULL)
    {
        (Singleton<ZHLog>::getSingletonPtr())->assertError("bool CBmcTR::Set_BMC_parameters--> new m_fIcGCeof1/m_fIcGCeof2 failed!");
        return false;
    }

	for (int i = 1; i <= m_nPolar; i ++)
	{
		m_fIcGCeof1[i] = sin(asin(sqrt((float)i/(float)m_nPolar))) * 
			cos(asin(sqrt((float)i/(float)m_nPolar))) * cos(asin(sqrt((float)i/(float)m_nPolar)));
		m_fIcGCeof2[i] = sin(asin(sqrt((i + 1.f)/m_nPolar))) - 
			sin(asin(sqrt((float)i/(float)m_nPolar)));
	}

    return true;
}

void CBmcTR::saveIrradianceTree(char* fileName)
{
    if(m_pIrraTree!=NULL)
        m_pIrraTree->saveTreeToFile(fileName);

}

void CBmcTR::saveIncreasedTreeNode(char* fileName)
{  
    if(fileName!=NULL)
    {
        std::ofstream ooFile(fileName,std::ios::out|std::ios::binary);
        if(ooFile.fail())
        {
            printf("CBmcTR::saveIncreasedTreeNode failed!\n");
            return;
        }

        int CIrraValue_size = sizeof(CVector3f) * 2 + sizeof(CCol4) +sizeof(float) +sizeof(BYTE) + sizeof(TIrraGrad) * 2;
        
        int t_irrSize  = static_cast<int>(m_vIrraValue.size());
        ooFile.write((char*)(&t_irrSize),sizeof(int));

        std::vector<CIrraValue>::iterator t_ite;
        for(t_ite=m_vIrraValue.begin();t_ite!=m_vIrraValue.end();t_ite++)
        {   
            ooFile.write((char*)(&((*t_ite).m_vPos)), CIrraValue_size);   
        }

    }

}


///////////////////////////////////////////////////////////////////////////////
//		_Check_materials
//
// Sacle all the materals, make sure Kd + Ks <= 1, so that the energy would be
// conserved when the photon map is being created;
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_Check_materials(void)
{
	float fTemp, fMax = 0;
	int i;

	// Ks + Kd should be no <= 1
	for (i = 0; i < m_nMaterials; i ++)
	{
		// Find the largest Kd + Ks;
		fTemp = m_pMaterials[i].m_fDiffuse[3] + m_pMaterials[i].m_fSpecular[3];
		if (fTemp > fMax) fMax = fTemp;

		#ifdef _DEBUG
			// Show original materials
			printf("Original Material:\n %f %f %f %f\n %f %f %f %f\n", 
				m_pMaterials[i].m_fDiffuse[0],
				m_pMaterials[i].m_fDiffuse[1],
				m_pMaterials[i].m_fDiffuse[2],
				m_pMaterials[i].m_fDiffuse[3],
				m_pMaterials[i].m_fSpecular[0],
				m_pMaterials[i].m_fSpecular[1],
				m_pMaterials[i].m_fSpecular[2],
				m_pMaterials[i].m_fSpecular[3]);
		#endif //_DEBUG
	}

	// Make sure all materials are energy conservative
	if (fMax > 1.f)
	{
		for (i = 0; i < m_nMaterials; i ++)
		{
			m_pMaterials[i].m_fDiffuse [0] /= fMax;
			m_pMaterials[i].m_fDiffuse [1] /= fMax;
			m_pMaterials[i].m_fDiffuse [2] /= fMax;

			m_pMaterials[i].m_fSpecular[0] /= fMax;
			m_pMaterials[i].m_fSpecular[1] /= fMax;
			m_pMaterials[i].m_fSpecular[2] /= fMax;

			m_pMaterials[i].m_fDiffuse [3] = 1;
			m_pMaterials[i].m_fSpecular[3] = 1;
			#ifdef _DEBUG
				// Show original materials
				printf("Modified Material:\n %f %f %f %f\n %f %f %f %f\n", 
					m_pMaterials[i].m_fDiffuse[0],
					m_pMaterials[i].m_fDiffuse[1],
					m_pMaterials[i].m_fDiffuse[2],
					m_pMaterials[i].m_fDiffuse[3],
					m_pMaterials[i].m_fSpecular[0],
					m_pMaterials[i].m_fSpecular[1],
					m_pMaterials[i].m_fSpecular[2],
					m_pMaterials[i].m_fSpecular[3]);
			#endif //_DEBUG
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//		MCPM_ray_tracing_normal
// Normal Monte Carlo ray tracing using global and caustic photon maps
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_Initialize_light_source (float fScale)
{
	for (int i = 0; i < m_nMaterials; i ++)
	{
		// Check light source
		m_pMaterials[i].m_fEmissive[3] = 0.f;

		if (m_pMaterials[i].m_fAmbient[0] + 
			m_pMaterials[i].m_fAmbient[1] + 
			m_pMaterials[i].m_fAmbient[2] >= 0.1f )
		{
			m_pMaterials[i].m_fEmissive[0] = m_pMaterials[i].m_fAmbient[0] * fScale;
			m_pMaterials[i].m_fEmissive[1] = m_pMaterials[i].m_fAmbient[1] * fScale;
			m_pMaterials[i].m_fEmissive[2] = m_pMaterials[i].m_fAmbient[2] * fScale;
			m_pMaterials[i].m_fEmissive[3] = 1.f;
		}
	}
}  


///////////////////////////////////////////////////////////////////////////////
//Filter the result Image		
// 
///////////////////////////////////////////////////////////////////////////////
bool CBmcTR::applyFilterForSingleFrame(
									   int	_filterBoxSize,
									   float _torPixelPositionDistance,
									   float _torPerceptionDistance,
									   float _torNormalDistance,
									   float _torDepthDistance,
									   float _torMaterialDistance,
									   bool _usePixelDistance	,
									   bool _usePerceptionDistance,
									   bool _useNormalDistance,
									   bool _useDepthDistance,
									   bool _usematerialDistance)
{
	CCol4* _pImage	=	m_theCamera.ResultImagePtr();
	CCol4* _pFilteredImage	=	m_theCamera.FilteredResultImagePtr();
	int _iWidth	=	m_theCamera.Film_width();
	int _iHeight	=	m_theCamera.Film_height();
	ImageUtility* t_image	=	Singleton<ImageUtility>::getSingletonPtr();

	if(t_image==NULL)
		return false;

	bool rt =true;
	rt = t_image->setBilateralFilterData(
														_pImage,
														_pFilteredImage,
														_iWidth,
														_iHeight,
														_filterBoxSize,
														m_pixelInfo,
														_torPixelPositionDistance,
														_torPerceptionDistance,
														_torNormalDistance,
														_torDepthDistance,
														_torMaterialDistance,
														_usePixelDistance	,
														_usePerceptionDistance,
														_useNormalDistance,
														_useDepthDistance,
														_usematerialDistance											
														);
	if(rt)
	{
		rt = t_image->startBilateralFilter();
	}

	t_image->saveToFileFromColor4f("c:\\noFilter.bmp",_pImage,_iWidth,_iHeight);
	t_image->saveToFileFromColor4f("c:\\Filter.bmp",_pFilteredImage,_iWidth,_iHeight);

	return rt;
}

///////////////////////////////////////////////////////////////////////////////
//		Ray_tracing_normal_BMC
// Normal Monte Carlo ray tracing using global and caustic photon maps
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::Ray_tracing_BMC (const int nThread)
{
	if (!Is_model_loaded()) 
        return;
	if (m_bRunning) 
        return;
	m_bRunning = true;
	m_cBgColor = COLOR_BLACK;

	//-------------------------------------------------------------------------
	// 1. Light source initialization.
	//-------------------------------------------------------------------------
	_Initialize_light_source (m_fLightScale);

	//-------------------------------------------------------------------------
	// 2. Create the photon map to capture in-direct lighting
	//-------------------------------------------------------------------------
	Allocate_Mem_for_photnMap ();
	_Create_photon_maps();

	//-------------------------------------------------------------------------
	// 3. Rendering
	//-------------------------------------------------------------------------
	m_theCamera.ComputerRenderingParameters (m_vDeltaX, m_vDeltaY, m_vLeftBottom);

	int t;
    static DWORD dwTId[4];
	static TRtThredParam tTParam[4];

 
	bool bAllThreadRun;

	int nYStep = m_theCamera.Film_height();
	nYStep /= nThread;

	m_theStopWatch.Start("Start ray tracing");

	// If do not use irradiance cache, call the original MCRT threads using 
	// multi-thread technique
    // No irradiance cache
	if (m_fTolerance <= 0.f)
	{
		bAllThreadRun = true;
		for (t = 0; t < nThread; t ++)
		{
			tTParam[t]._nStart = nYStep * t;
			if (t == nThread - 1)
				tTParam[t]._nEnd = m_theCamera.Film_height()-1;
			else
				tTParam[t]._nEnd = tTParam[t]._nStart + nYStep;
			tTParam[t]._theBmcTRIns = this;
			hThread[t] = CreateThread (NULL, 0, Nml_thread_func, &(tTParam[t]), 0, &(dwTId[t]));
			SetThreadPriority (hThread[t], THREAD_PRIORITY_BELOW_NORMAL);
			if (hThread[t] == NULL)
			{
				printf ("Create thread %d failed!", t); 
				bAllThreadRun = false;
			}
		}

		if (bAllThreadRun)
		{
			WaitForMultipleObjects (nThread, hThread, TRUE, INFINITE);
			for (t = 0; t < nThread; t ++)
				CloseHandle (hThread[t]);
			m_bRunning = false;
		}
	}
	// Else, build the irradiance cache first and then use the irradiance cache 
	// to estimate the indirect lighting contribution
    // Use irradiance cache
	else
	{
		//---------------------------------------------------------------------
		// Initialize irradiance caching tree and the critical section
		//---------------------------------------------------------------------
		       
           CVec3 vMax, vMin;
		    float fSize;
		    Get_boundingbox (vMax, vMin);
		    fSize = max3(fabs(vMax.x-vMin.x), fabs(vMax.y-vMin.y),fabs(vMax.z-vMin.z));
            
            if(m_pIrraTree==NULL)
            {            
                m_pIrraTree = new CIrraTree (fSize, (vMax + vMin)*.5f, 0);
            }

		    m_pIrraTree->Set_tolerance (m_fTolerance);

		    //---------------------------------------------------------------------
		    // First pass: Build the irradiance cache
		    //---------------------------------------------------------------------
		    InitializeCriticalSection (&m_csInsertion);
		    InitializeCriticalSection (&m_csGetGpIndex);
            
            m_vIrraValue.clear();
		    m_pPxlInfo = new TPxlInfo [m_theCamera.Film_height()*m_theCamera.Film_width()];
		    bAllThreadRun = true;
		    for (t = 0; t < nThread; t ++)
		    {
			    tTParam[t]._nStart = nYStep * t;
			    if (t == nThread - 1)
				    tTParam[t]._nEnd = m_theCamera.Film_height();
			    else
				    tTParam[t]._nEnd = tTParam[t]._nStart + nYStep +1;
			    tTParam[t]._theBmcTRIns = this;
			    hThread[t] = CreateThread (NULL, 0, Build_IC_thread_func, &(tTParam[t]), 0, &(dwTId[t]));
			    SetThreadPriority (hThread[t], THREAD_PRIORITY_BELOW_NORMAL);
			    if (hThread[t] == NULL)
			    {
				    printf ("Create thread %d failed when trying to build irradiance cache!", t); 
				    bAllThreadRun = false;
			    }
		    }

		    if (bAllThreadRun)
		    {
			    WaitForMultipleObjects (nThread, hThread, TRUE, INFINITE);
			    for (t = 0; t < nThread; t ++)
				    CloseHandle (hThread[t]);
		    }
		    DeleteCriticalSection (&m_csInsertion);
		    DeleteCriticalSection (&m_csGetGpIndex);

       //===============================================
            //start test code
           //Build irradiance cache tree from file 

            //m_pIrraTree->buildTreeFromFile("e:\\IrraTree_256x256_photon100000_d20_i20_scale34_irCache(1)_0000.irr");
            
            //m_pIrraTree->saveTreeToFile("d:\\aaaaa");
            //m_pIrraTree->buildTreeFromFile("d:\\aaaaa");

            /*CIrraTree* _tree  =  new CIrraTree;
            _tree->buildTreeFromFile("d:\\aaaaa");
            _tree->saveTreeToFile("d:\\bbbbb");  */
            ////_tree->buildTreeFromFile("d:\\bbbbb");
            ////_tree->saveTreeToFile("d:\\ccccc");
                //PixelInfo t_info(256,256);
                //t_info.loadPixelInfoFromFile("e:\\pixelInfo_256x256_photon100000_d20_i20_scale34_irCache(1)_0000.pix");
                //MY_DELETE_ARRAY(m_pPxlInfo);
                //int _total   =  m_theCamera.Film_height()*m_theCamera.Film_width();
                //m_pPxlInfo = new TPxlInfo[_total];
                //for(int i=0;i<_total;i++)
                //{
                //    m_pPxlInfo[i]._nTID  =  t_info.m_triangleID[i];
                //    m_pPxlInfo[i]._vNml  = t_info.m_normal[i];
                //    m_pPxlInfo[i]._vPos   = t_info.m_pos[i];
                //    m_pPxlInfo[i]._vTex   = t_info.m_tex[i];
                //}

            //end test code
        //===============================================
		    _Save_irradaince_cache_image ("IrradainceCachePoint.bmp");

        
        printf("IrradianceCache tree, Node:%10d, IrraValue: %15d\n",m_pIrraTree->m_nNodeNumber,m_pIrraTree->m_nIrraValueNumber);
        

		//---------------------------------------------------------------------
		// Second pass: Shading
		//---------------------------------------------------------------------
		m_theStopWatch.Show_last_interval("Finish building irradiance cache");
		bAllThreadRun = true;
		

		// Calculate the step size and segment number
		int nSegment = m_theCamera.Film_height() / 10;
		nYStep = m_theCamera.Film_height() / nSegment;

		if (nThread * nYStep >= m_theCamera.Film_height())
		{
			nSegment = nThread;
			nYStep = m_theCamera.Film_height() / nThread;
		}

		// Create all threads
		for (t = 0; t < nThread; t ++)
		{
			if (t == 0)
				tTParam[t]._nEnd = m_theCamera.Film_height();
			else tTParam[t]._nEnd = nSegment * nYStep;
			nSegment --;
			tTParam[t]._nStart = nYStep * nSegment;
            tTParam[t]._theBmcTRIns  =  this;

			hThread[t] = (HANDLE)_beginthreadex(NULL, 0, (THREADFUNC)Bmc_IC_thread_func, &(tTParam[t]), 0, (unsigned int*)(&(dwTId[t])));
			SetThreadPriority (hThread[t], THREAD_PRIORITY_BELOW_NORMAL);
			if (hThread[t] == NULL)
			{
				printf ("Create thread %d failed when trying to run using irradiance cache!", t); 
				bAllThreadRun = false;
			}
		}

		if (bAllThreadRun)
		{
			// Assigne a new segment to a thread as soon as it finished its work
			// until all segment are assigned
			while (nSegment > 0)
			{
				t = WaitForMultipleObjects (nThread, hThread, FALSE, INFINITE) -
					WAIT_OBJECT_0;
				CloseHandle (hThread[t]);

				tTParam[t]._nEnd = nSegment * nYStep;
				nSegment --;
				tTParam[t]._nStart = nYStep * nSegment;
                tTParam[t]._theBmcTRIns  =  this;
				hThread[t] = (HANDLE)_beginthreadex(NULL, 0, (THREADFUNC)Bmc_IC_thread_func, &(tTParam[t]), 0, (unsigned int*)(&(dwTId[t])));
			}

			// Wait for all threads
			WaitForMultipleObjects (nThread, hThread, TRUE, INFINITE);
			for (t = 0; t < nThread; t ++)
				CloseHandle (hThread[t]);
			m_bRunning = false;
		}

		//---------------------------------------------------------------------
		// delete irradiance caching tree
		//---------------------------------------------------------------------
		MY_DELETE_ARRAY (m_pPxlInfo);
		//MY_DELETE (m_pIrraTree);
	}

	m_theStopWatch.Show_total_time("Finish ray tracing");
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////	Functioned for normal MCRT using multi-thread technique
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DWORD WINAPI Nml_thread_func (LPVOID lpParam)
{
	TRtThredParam *rtp = (TRtThredParam *)lpParam;
	rtp->_theBmcTRIns->_MCRT_seg (rtp->_nStart, rtp->_nEnd);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//		_MCRT_seg
// Normal Monte Carlo ray tracing using global and caustic photon maps
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_MCRT_seg (const int nStart, const int nEnd)
{
	const int nWidth = m_theCamera.Film_width();
	const int nHeight = m_theCamera.Film_height();
	CCol4 *pImage = m_theCamera.ResultImagePtr();

	int x, y, nOffset;
	CVec3 CurrY, CurrPixel;
	CCol4 *pCurr, cIrra;
	CVec3 vLB = m_vLeftBottom;
	CVec3 vDy = m_vDeltaY;
	CVec3 vDx = m_vDeltaX;

	for (y = nStart; y <= nEnd; y ++)
	{
		CurrY   = vLB + vDy * y;
		nOffset = (nHeight - y - 1) * nWidth;

		pCurr   = &pImage[nOffset];
		for (x = 0; x < nWidth; x ++, pCurr ++)
		{
			CurrPixel = CurrY + vDx * x;
			*pCurr = _Pixel_radiance_PM (CurrPixel, cIrra, nOffset+x);
		}
	}
}









///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////	Functiond for MCRT using irradiance cache technique and multithree
////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DWORD WINAPI Build_IC_thread_func (LPVOID lpParam)
{
	TRtThredParam *rtp = (TRtThredParam *)lpParam;
	rtp->_theBmcTRIns->_Build_irradiance_cache_seg (rtp->_nStart, rtp->_nEnd);
	return 0;
}

DWORD WINAPI Bmc_IC_thread_func (LPVOID lpParam)
{
	TRtThredParam *rtp = (TRtThredParam *)lpParam;
	rtp->_theBmcTRIns->_MCRT_using_irradiance_cache_seg (rtp->_nStart, rtp->_nEnd);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//	_Build_irradiance_cache_seg 
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_Build_irradiance_cache_seg (const int nStart, const int nEnd)
{
	const int nWidth = m_theCamera.Film_width();
	const int nHeight = m_theCamera.Film_height();
	CCol4 *pImage = m_theCamera.ResultImagePtr();

	int x, y, nOffset;
	CVec3 CurrY, CurrPixel;
	TPxlInfo *pCurr;

	if (m_bGradient)
	{
		// Preparing the buffers (so that we would not need to assign temporay all the time
		float **fpThe = new float *[m_nAzimuth + 1];
		float **fpPhi = new float *[m_nAzimuth + 1];
		float **fpD   = new float *[m_nAzimuth + 1];
		CCol4 **cpL   = new CCol4 *[m_nAzimuth + 1];
		BOOL  **bpHit = new BOOL  *[m_nAzimuth + 1];
		CVec3 vX;

		for (int i = 0; i <= m_nAzimuth; i ++)
		{
			fpThe[i] = new float [m_nPolar + 1];
			fpPhi[i] = new float [m_nPolar + 1];
			fpD[i]   = new float [m_nPolar + 1];
			cpL[i]   = new CCol4 [m_nPolar + 1];
			bpHit[i] = new BOOL  [m_nPolar + 1];
		}

		// Irradiance computation and caching
		for (y = nStart; y < nEnd; y ++)
		{
			CurrY   = m_vLeftBottom + m_vDeltaY * y;
			nOffset = nHeight - y - 1;
			nOffset = nOffset * nWidth;
			pCurr   = &m_pPxlInfo[nOffset];
			CCol4 *pImg = &pImage[nOffset];
			for (x = 0; x < nWidth; x ++, pCurr ++, pImg ++)
			{
				CurrPixel = CurrY + m_vDeltaX * x;
				*pCurr = _Irradiance_checking_gradient (CurrPixel, pImg,
					fpThe, fpPhi, fpD, cpL, bpHit, vX, nOffset+x);
			}
		}

		// Delete buffers
		for (int i = 0; i <= m_nAzimuth; i ++)
		{
			MY_DELETE_ARRAY (fpThe[i]);
			MY_DELETE_ARRAY (fpPhi[i]);
			MY_DELETE_ARRAY (fpD[i]);
			MY_DELETE_ARRAY (cpL[i]);
			MY_DELETE_ARRAY (bpHit[i]);
		}

		MY_DELETE_ARRAY (fpThe);
		MY_DELETE_ARRAY (fpPhi);
		MY_DELETE_ARRAY (fpD);
		MY_DELETE_ARRAY (cpL);
		MY_DELETE_ARRAY (bpHit);
	}//end if gradient

	else
	{
		for (y = nStart; y < nEnd; y ++)
		{
            if(y>=nHeight)
                y=nHeight-1;
			CurrY   = m_vLeftBottom + m_vDeltaY * y;
			nOffset = nHeight - y - 1;
			nOffset = nOffset * nWidth;
			pCurr   = &m_pPxlInfo[nOffset];
			CCol4 *pImg = &pImage[nOffset];
			for (x = 0; x < nWidth; x ++, pCurr ++, pImg ++)
			{
				CurrPixel = CurrY + m_vDeltaX * x;
				*pCurr = _Irradiance_checking (CurrPixel, pImg,nOffset+x);
			}
		}
}	
}


///////////////////////////////////////////////////////////////////////////////
//	_MCRT_using_irradiance_cache_seg 
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_MCRT_using_irradiance_cache_seg (const int nStart, const int nEnd)
{
	const int nWidth = m_theCamera.Film_width();
	const int nHeight = m_theCamera.Film_height();
	CCol4 *pImage = m_theCamera.ResultImagePtr();

	int x, y, nOffset;
	CVec3 CurrY, CurrPixel;
	TPxlInfo *pPif;
	CCol4 *pCurr;

	for (y = nStart; y < nEnd; y ++)
	{
		CurrY   = m_vLeftBottom + m_vDeltaY * y;
		nOffset = nHeight - y - 1;
		nOffset = nOffset * nWidth;
		pCurr   = &pImage [nOffset];
		pPif    = &m_pPxlInfo [nOffset];
		for (x = 0; x < nWidth; x ++, pCurr ++, pPif++)
		{
			CurrPixel = CurrY + m_vDeltaX * x;
			*pCurr = _Shade_using_IC (pPif, CurrPixel);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//	_Irradiance_checking
///////////////////////////////////////////////////////////////////////////////
TPxlInfo CBmcTR::_Irradiance_checking (CVec3 vPos, CCol4 *pImg,int _offset)
{
	TBspCross crs;
	TPxlInfo  tPif;
	TBspRay	  ray;
	
	int nPID   = -1;
	tPif._nTID = nPID;
	*pImg = COLOR_BLACK;

	ray.vOrg = m_theCamera.Eye();
	ray.vEnd = vPos;
	ray.vDir = vPos - m_theCamera.Eye();
	ray.vDir.Normalize();

	if (!_RayTreeIntersect(ray, crs, nPID))	
	{
		//record pixel info
		m_pixelInfo->setPixelInfo(_offset,1.0f,-1,CVector3f(0,0,0),CVector3f(0,0,0),CVector3f(0,0,0),-1);
		return tPif;
	}

	//record pixel info
	int _materialID	=	m_pTriangles[nPID].GetMaterialID();
	m_pixelInfo->setPixelInfo(_offset,crs.fDist,_materialID,crs.vNml,crs.vPos,crs.vTex, nPID);

	tPif._vPos  = crs.vPos;
	tPif._vNml  = crs.vNml;
	tPif._vTex  = crs.vTex;
	tPif._nTID  = nPID;

	if (_Is_light_patch (nPID))		return tPif;

	// Calculate the diffuse irradiance
    m_lkIrradianceCacheTree.Lock();
        bool _need = m_pIrraTree->Need_computation (crs.vPos, crs.vNml);
    m_lkIrradianceCacheTree.UnLock();
	
    if (_need)
	{

		float fRadius;
		CIrraValue iv;
		CCol4 cDiffIrra;
		
		cDiffIrra = _Indirect_irradiance (crs, nPID, fRadius);

		iv.m_bDepth  = 0;
		iv.m_cEnergy = cDiffIrra;
		iv.m_fRadius = fRadius;
		iv.m_vPos    = crs.vPos;
		iv.m_vNml    = crs.vNml;

		// Use a critical section to make sure the subdivision would not cause
		// conflict for multi-thread 
        m_lkIrradianceCacheTree.Lock();
        m_vIrraValue.push_back(iv);
		m_pIrraTree->Insert (iv,m_pIrraTree);
        m_lkIrradianceCacheTree.UnLock();

		*pImg = cDiffIrra;
	}

	return tPif;
}


///////////////////////////////////////////////////////////////////////////////
//	_Shade_using_IC
///////////////////////////////////////////////////////////////////////////////
CCol4 CBmcTR::_Shade_using_IC (TPxlInfo *pPif, CVec3 vPos)
{
	TBspRay	  ray;
	TBspCross crs, crs2;
	CVec3     vView;
	
	CCol4 cRsl = COLOR_BLACK, cAccu, cTemp;

	if (pPif->_nTID < 0) return cRsl;
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[pPif->_nTID].GetMaterialID()];

	//-------------------------------------------------------------------------
	// If it is a light source, just return the source intensity
	//-------------------------------------------------------------------------
	if (_Is_light_patch (pPif->_nTID))
	{
		if (m_bDirectionalLight)	return m_cBgColor;

		cRsl.r() = pm->m_fEmissive[0];
		cRsl.g() = pm->m_fEmissive[1];
		cRsl.b() = pm->m_fEmissive[2];

		return (cRsl);
	}

	//-------------------------------------------------------------------------
	// Calculate the direct lighting using given sample number
	//-------------------------------------------------------------------------
	vView = (vPos - m_theCamera.Eye()).Normalized();
	for (int i = 0; i < m_nLightRay; i ++)
		cRsl += _Direct_lighting (pPif->_vPos, pPif->_vNml, vView, pPif->_vTex, pPif->_nTID);
	cRsl /= m_nLightRay;

	//-------------------------------------------------------------------------
	// Calculate the diffuse irradiance
	//-------------------------------------------------------------------------
	CCol4 cDiffIrra;
    m_lkIrradianceCacheTree.Lock();
	m_pIrraTree->Get_irradiance (cDiffIrra, pPif->_vPos, pPif->_vNml, 0, m_bGradient);
    m_lkIrradianceCacheTree.UnLock();

	// Consider the caustic contribution
	if ((m_bCaustic) && (m_nFeatures & BSP_MCRT_CAUSTIC_EFFECT))
	{
        //m_lkGaussMap.Lock();
		m_pCausticPhotonMap->GaussianIrradianceEstimate(cTemp, 
			pPif->_vPos, pPif->_vNml, m_fLocalRadius * 0.1f, m_nLocalPhoton);
        //m_lkGaussMap.UnLock();

		cDiffIrra += cTemp;
	}

	CCol4 cDiffuse;
    cDiffuse.r() = cDiffIrra.r() * pm->m_fDiffuse[0];
    cDiffuse.g() = cDiffIrra.g() * pm->m_fDiffuse[1];
    cDiffuse.b() = cDiffIrra.b() * pm->m_fDiffuse[2];

	if (pm->m_iTexture.IsInitialized())
		cDiffuse *= pm->m_iTexture.TextureColor(pPif->_vTex.x, pPif->_vTex.y);
	cRsl += cDiffuse;

	//-------------------------------------------------------------------------
	// Calculate the specular irradiance
	//-------------------------------------------------------------------------
	if ((IS_SPECULAR_SURFACE(pm)) && (m_nFeatures & BSP_MCRT_MIRROR_EFFECT))
	{
		cAccu.r() = pm->m_fSpecular[0];
		cAccu.g() = pm->m_fSpecular[1];
		cAccu.b() = pm->m_fSpecular[2];
		if (cAccu[0]>.01f || cAccu[1]>.01f || cAccu[2]>.01f)
		{
			float fGloosy = pm->m_fShininess[0] / 128.f;
			if (fGloosy > 0.8f) fGloosy = 0.8f;

			ray.vDir = _Reflect_the_ray (vView, pPif->_vNml, fGloosy);
			ray.vOrg = pPif->_vPos;
			ray.vEnd = pPif->_vPos + ray.vDir * m_fSceneSize;

			CCol4 cTemp;
			m_bEstimateDiffuseIrra = TRUE;
			cAccu = _Specular_contribution (ray, cAccu, 1);
			cAccu.r() *= pm->m_fSpecular[0];
			cAccu.g() *= pm->m_fSpecular[1];
			cAccu.b() *= pm->m_fSpecular[2];

			cRsl += cAccu;
		}
	}

	return (cRsl);
}


///////////////////////////////////////////////////////////////////////////////
//		_Specular_contribution
// Estimate the specular contribution along the given direction
///////////////////////////////////////////////////////////////////////////////
CCol4 CBmcTR::_Specular_contribution (TBspRay &ray, CCol4 cCtr, int nDepth)
{
	TBspRay	  ray2;
	TBspCross crs, crs2;
	
	int nPID = -1;
	CCol4 cRsl = COLOR_BLACK, cAccu, cTemp;

	if (!_RayTreeIntersect(ray, crs, nPID )) return m_cBgColor;
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];

	//-------------------------------------------------------------------------
	// If it is a light source, just return the source intensity
	//-------------------------------------------------------------------------
	if (_Is_light_patch (nPID))
	{
		if (m_bDirectionalLight)	return m_cBgColor;

		cRsl.r() = pm->m_fEmissive[0];
		cRsl.g() = pm->m_fEmissive[1];
		cRsl.b() = pm->m_fEmissive[2];

		return (cRsl);
	}

	//-------------------------------------------------------------------------
	// Calculate the direct lighting using given sample number
	//-------------------------------------------------------------------------
	if (nDepth == 1)
	{
		for (int i = 0; i < m_nLightRay; i ++)
			cRsl += _Direct_lighting (crs.vPos, crs.vNml, ray.vDir, crs.vTex, nPID);
		cRsl /= m_nLightRay;
	}
	else
	{
        //m_lkDirectMap.Lock();
		m_pDirectPhotonMap->irradianceEstimate (cRsl, crs.vPos, crs.vNml, 
			m_fLocalRadius, m_nLocalPhoton);
        //m_lkDirectMap.UnLock();
	}


	//-------------------------------------------------------------------------
	// Calculate the diffuse irradiance
	//-------------------------------------------------------------------------
	CCol4 cDiffIrra;
    //m_lkGaussMap.Lock();
	m_pGlobalPhotonMap->irradianceEstimate
		(cDiffIrra, crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);
    //m_lkGaussMap.UnLock();

	// Consider the caustic contribution
	if ((m_bCaustic) && (m_nFeatures & BSP_MCRT_CAUSTIC_EFFECT))
	{
        //m_lkGaussMap.Lock();
		m_pCausticPhotonMap->GaussianIrradianceEstimate(cTemp, 
			crs.vPos, crs.vNml,	m_fLocalRadius*0.1f, m_nLocalPhoton);
		//m_lkGaussMap.UnLock();
        cDiffIrra += cTemp;
	}

	CCol4 cDiffuse;
    cDiffuse.r() = cDiffIrra.r() * pm->m_fDiffuse[0];
    cDiffuse.g() = cDiffIrra.g() * pm->m_fDiffuse[1];
    cDiffuse.b() = cDiffIrra.b() * pm->m_fDiffuse[2];

	if (pm->m_iTexture.IsInitialized())
		cDiffuse *= pm->m_iTexture.TextureColor(crs.vTex[0], crs.vTex[1]);
	cRsl += cDiffuse;

	//-------------------------------------------------------------------------
	// Calculate the specular irradiance
	//-------------------------------------------------------------------------
	if ((IS_SPECULAR_SURFACE(pm)) && (m_nFeatures & BSP_MCRT_MIRROR_EFFECT))
	{
		cAccu.r() = pm->m_fSpecular[0] * cCtr[0];
		cAccu.g() = pm->m_fSpecular[1] * cCtr[1];
		cAccu.b() = pm->m_fSpecular[2] * cCtr[2];
		if ((nDepth < 4) && (cAccu[0]>.01f || cAccu[1]>.01f || cAccu[2]>.01f))
		{
			float fGloosy = pm->m_fShininess[0] / 128.f;
			if (fGloosy > 0.8f) fGloosy = 0.8f;

			ray2.vDir = _Reflect_the_ray (ray.vDir, crs.vNml, fGloosy);
			ray2.vOrg = crs.vPos;
			ray2.vEnd = crs.vPos + ray2.vDir * m_fSceneSize;

			CCol4 cTemp;
			m_bEstimateDiffuseIrra = TRUE;
			cAccu = _Specular_contribution (ray2, cAccu, nDepth+1);
			cAccu.r() *= pm->m_fSpecular[0];
			cAccu.g() *= pm->m_fSpecular[1];
			cAccu.b() *= pm->m_fSpecular[2];

			cRsl += cAccu;
		}
	}

	return (cRsl);
}


///////////////////////////////////////////////////////////////////////////////
//		_Indirect_irradiance
// Estimate the in-direction irradiance (diffuse)
///////////////////////////////////////////////////////////////////////////////
CCol4 CBmcTR::_Indirect_irradiance (const TBspCross &crs, int nPID, float &fAvgRadius)
{
	CCol4 cAccu = COLOR_BLACK;
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];

	if (m_nDistrRayNum <= 0) return cAccu;

	int nAccu = 0;
	BOOL bHitLig;
	TBspRay ray;

	// The local frame
	CVec3 vZ = crs.vNml.Normalized();
	CVec3 vY = RANDOM_VEC;
	CVec3 vX = (vY.Cross(vZ)).Normalized();
	vY = vZ.Cross(vX);

	// Evenly distribute the sampling ray on the hemi-sphere according to
	// the inclination angle
	fAvgRadius = 0;
	for (int i = 1; i <= m_nAzimuth; i ++)
	{
		for (int j = 1; j <= m_nPolar; j ++)
		{
			float fTheta = (float)asin(sqrt((j-RANDOM_0_1)/m_nPolar));
			float fPhi   = 2 * PI * (i-RANDOM_0_1)/m_nAzimuth;
			float fDist;

			ray.vDir = vY * sin (fPhi) + vX * cos (fPhi);
			ray.vDir = vZ * sin (fTheta) + ray.vDir * cos (fTheta);
			ray.vOrg = crs.vPos;
			ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;
			CCol4 cTemp = _PM_radiance_along_the_ray (ray, nPID, fDist, bHitLig);

			if (!bHitLig)
			{
				fAvgRadius += fDist;
				cAccu += cTemp;
				nAccu ++;
			}
		}
	}

	if (nAccu > 0)
	{
		fAvgRadius /= nAccu;
		cAccu = cAccu * PI / nAccu;
	}

	return cAccu;
}


///////////////////////////////////////////////////////////////////////////////
//	_Irradiance_checking_gradient
///////////////////////////////////////////////////////////////////////////////
TPxlInfo CBmcTR::_Irradiance_checking_gradient (CVec3 vPos, CCol4 *pImg,
												float **fpThe, float **fpPhi, float **fpD,
												CCol4 **cpL, BOOL **bpHit, CVec3 &vX, int _offset)

{
	TBspCross crs;
	TPxlInfo  tPif;
	TBspRay	  ray;
	
	int nPID   = -1;
	tPif._nTID = nPID;
	*pImg = COLOR_BLACK;

	ray.vOrg = m_theCamera.Eye();
	ray.vEnd = vPos;
	ray.vDir = vPos - m_theCamera.Eye();
	ray.vDir.Normalize();

	if (!_RayTreeIntersect(ray, crs, nPID))	
	{
		m_pixelInfo->setPixelInfo(_offset,1.0f,-1,CVector3f(0,0,0),CVector3f(0,0,0),CVector3f(0,0,0),-1);
		return tPif;
	}

	//record pixel info
	int _materialID	=	m_pTriangles[nPID].GetMaterialID();
	m_pixelInfo->setPixelInfo(_offset,crs.fDist,_materialID,crs.vNml,crs.vPos,crs.vTex,nPID);

	tPif._vPos  = crs.vPos;
	tPif._vNml  = crs.vNml;
	tPif._vTex  = crs.vTex;
	tPif._nTID  = nPID;

	if (_Is_light_patch (nPID))		return tPif;

	// Calculate the diffuse irradiance
    m_lkIrradianceCacheTree.Lock();    
    bool _need = m_pIrraTree->Need_computation (crs.vPos, crs.vNml);
    m_lkIrradianceCacheTree.UnLock();

	if (_need)
	{

		CIrraValue iv;

		_Indirect_irradiance_gradients (crs, nPID, fpThe, fpPhi, fpD, cpL, bpHit, vX);
		_Gradient_irradiance (crs, fpThe, fpPhi, fpD, cpL, bpHit, vX, iv);

		// Use a critical section to make sure the subdivision would not cause
		// conflict for multi-thread 
		m_lkIrradianceCacheTree.Lock();
		m_pIrraTree->Insert (iv,m_pIrraTree);
        m_lkIrradianceCacheTree.UnLock();

		*pImg = iv.m_cEnergy;
	}

	return tPif;
}


///////////////////////////////////////////////////////////////////////////////
//	_Gradient_irradiance
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_Gradient_irradiance(TBspCross crs, float **fpThe, float **fpPhi, float **fpD, 
								  CCol4 **cpL, BOOL **bpHit, CVec3 &vX, CIrraValue &iv)
{
	int i, j, nAccu = 0;

	// Check all sample, get the average intensity
	iv.m_cEnergy = COLOR_BLACK;
	iv.m_fRadius = 0.f;
	for (i = 1; i <= m_nAzimuth; i ++)
	{
		for (j = 1; j <= m_nPolar; j ++)
		{
			iv.m_fRadius += fpD[i][j];
			if (bpHit[i][j]) continue;

			iv.m_cEnergy += cpL[i][j];
			nAccu ++;
		}
	}

	iv.m_cEnergy /= nAccu;
	iv.m_fRadius /= m_nAzimuth * m_nPolar;

	iv.m_vNml = crs.vNml;
	iv.m_vPos = crs.vPos;
	iv.m_bDepth = 0x00;

	iv.m_tGraP._vR = CVec3 (0.f, 0.f, 0.f);
	iv.m_tGraP._vG = CVec3 (0.f, 0.f, 0.f);
	iv.m_tGraP._vB = CVec3 (0.f, 0.f, 0.f);

	iv.m_tGraR._vR = CVec3 (0.f, 0.f, 0.f);
	iv.m_tGraR._vG = CVec3 (0.f, 0.f, 0.f);
	iv.m_tGraR._vB = CVec3 (0.f, 0.f, 0.f);

	// Set all the samples that hit the light source with the average radiance
	for (i = 1; i <= m_nAzimuth; i ++)
	{
		for (j = 1; j <= m_nPolar; j ++)
			if (bpHit[i][j]) cpL[i][j] = iv.m_cEnergy;
	}

	iv.m_cEnergy *= PI;

	for (j = 1; j <= m_nPolar; j ++)
	{
		cpL[0][j] = cpL[m_nAzimuth][j];
		fpD[0][j] = fpD[m_nAzimuth][j];
	}

	CVec3 vTt, vpTt[200], vTr, vY;
	vY = crs.vNml.Cross(vX);
	
	for (i = 1; i <= m_nAzimuth; i ++)
	{
		float fAngle = (i - 1.f) / m_nAzimuth * 2 * PI;
		vpTt[i] = vX * cos (fAngle) + vY * sin (fAngle);
		vpTt[i].Normalized();
	}

	for (i = 1; i <= m_nAzimuth; i ++)
	{
		for (j = 1; j <= m_nPolar; j ++)
		{
			float c1, c2;
			vTt  = vX * cos(fpPhi[i][j]) + vY * sin(fpPhi[i][j]);

			c1 = (j >= 2) ? 2*PI/m_nAzimuth*m_fIcGCeof1[j]/min(fpD[i][j],fpD[i][j-1]) : 0.f;
			c2 = m_fIcGCeof2[j] / min (fpD[i][j], fpD[i-1][j]);

			iv.m_tGraP._vR += vTt * c1 * (cpL[i][j][0] - cpL[i][j-1][0]) +
				vpTt[i] * c2 * (cpL[i][j][0] - cpL[i-1][j][0]);
			iv.m_tGraP._vG += vTt * c1 * (cpL[i][j][1] - cpL[i][j-1][1]) +
				vpTt[i] * c2 * (cpL[i][j][1] - cpL[i-1][j][1]);
			iv.m_tGraP._vB += vTt * c1 * (cpL[i][j][2] - cpL[i][j-1][2]) +
				vpTt[i] * c2 * (cpL[i][j][2] - cpL[i-1][j][2]);

			vTr  = crs.vNml.Cross(vTt);
			iv.m_tGraR._vR += vTr * (-cpL[i][j][0] * tan(fpThe[i][j]));
			iv.m_tGraR._vG += vTr * (-cpL[i][j][1] * tan(fpThe[i][j]));
			iv.m_tGraR._vB += vTr * (-cpL[i][j][2] * tan(fpThe[i][j]));
		}
	}

	iv.m_tGraR._vR *= PI / (m_nAzimuth * m_nPolar);
	iv.m_tGraR._vG *= PI / (m_nAzimuth * m_nPolar);
	iv.m_tGraR._vB *= PI / (m_nAzimuth * m_nPolar);

	iv.m_tGraP._vR = CVec3 (0.f, 0.f, 0.f);
	iv.m_tGraP._vG = CVec3 (0.f, 0.f, 0.f);
	iv.m_tGraP._vB = CVec3 (0.f, 0.f, 0.f);

	//iv.m_tGraR._vR = CVec3 (0.f, 0.f, 0.f);
	//iv.m_tGraR._vG = CVec3 (0.f, 0.f, 0.f);
	//iv.m_tGraR._vB = CVec3 (0.f, 0.f, 0.f);
}


///////////////////////////////////////////////////////////////////////////////
//		_Indirect_irradiance_gradients
// Estimate the in-direction irradiance (diffuse) and the irradiance gradients
// according to the position and orientation respectively.
// Input: 
//     crs: The hit point
//     nPID: The index of the hit triangle
// Output:
//		vpDir: The sample direction
//		fpD:   The distance between the point and the sample hit point
//		cpL:   The irradiance
//		bpHit: The BOOL table of samples.
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_Indirect_irradiance_gradients (const TBspCross &crs, int nPID, 
											 float **fpThe, float **fpPhi, float **fpD, 
											 CCol4 **cpL, BOOL **bpHit, CVec3 &vX)
{
	CCol4 cAccu = COLOR_BLACK;
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];

	int nAccu = 0;
	TBspRay ray;

	// The local frame
	CVec3 vZ = crs.vNml.Normalized();
	CVec3 vY = RANDOM_VEC;
	vX = (vY.Cross(vZ)).Normalized();
	vY = vZ.Cross(vX);

	// Evenly distribute the sampling ray on the hemi-sphere according to
	// the inclination angle
	for (int i = 1; i <= m_nAzimuth; i ++)
	{
		for (int j = 1; j <= m_nPolar; j ++)
		{
			// The direction
			float fTheta = (float)asin(sqrt((j-RANDOM_0_1)/m_nPolar));	// Polar
			float fPhi   = 2 * PI * (i-RANDOM_0_1)/m_nAzimuth;           // Azimuth

			// The testing ray
			ray.vDir = vY * sin (fPhi) + vX * cos (fPhi);
			ray.vDir = vZ * sin (fTheta) + ray.vDir * cos (fTheta);
			ray.vOrg = crs.vPos;
			ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;

			fpPhi[i][j] = fPhi;
			fpThe[i][j] = fTheta;

			// Radaince from the given direction
			cpL[i][j] = _PM_radiance_along_the_ray (ray, nPID, fpD[i][j], bpHit[i][j]);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//		_PM_radiance_along_the_ray
// Estimate the radiance at the given direction using photon map
///////////////////////////////////////////////////////////////////////////////
CCol4 CBmcTR::_PM_radiance_along_the_ray (TBspRay &ray, int nPID, 
										  float &fDist, BOOL &bHitLig)
{
	TBspCross crs;
	CColor4f cRsl = COLOR_BLACK, cTemp;
	
	if (_RayTreeIntersect(ray, crs, nPID ))
	{
		CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];
		fDist = (crs.vPos - ray.vOrg).Length();

		//---------------------------------------------------------------------
		// If it is a light source, just return the source intensity
		//---------------------------------------------------------------------
		if (_Is_light_patch (nPID))
		{
			cRsl.r() = pm->m_fEmissive[0];
			cRsl.g() = pm->m_fEmissive[1];
			cRsl.b() = pm->m_fEmissive[2];

			bHitLig = TRUE;
			return (cRsl);
		}

		//---------------------------------------------------------------------
		// Use the Photom map to estimate the indirect lighting intensity
		//---------------------------------------------------------------------
        m_lkGlobalMap.Lock();
		m_pGlobalPhotonMap->irradianceEstimate
			(cRsl, crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);
        m_lkGlobalMap.UnLock();
		//---------------------------------------------------------------------
		// Use direct photon map to estimate the direct lighting irradiance if 
		// totally using photon map, else, estimate direct lighting intensity
		//---------------------------------------------------------------------
		cRsl += _Direct_lighting (crs.vPos, crs.vNml, ray.vDir, crs.vTex, nPID);
		/*m_pDirectPhotonMap->irradianceEstimate
			(cTemp, crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);
		cRsl += cTemp;
		*/

		//---------------------------------------------------------------------
		// Irradiance to intensity
		//---------------------------------------------------------------------
		cRsl.r() *= pm->m_fDiffuse[0];
		cRsl.g() *= pm->m_fDiffuse[1];
		cRsl.b() *= pm->m_fDiffuse[2];

		if (pm->m_iTexture.IsInitialized())
			cRsl *= pm->m_iTexture.TextureColor(crs.vTex[0], crs.vTex[1]);
	}
	else
	{
		cRsl = m_cBgColor;
		fDist = m_fSceneSize;
	}

	bHitLig = FALSE;
	return (cRsl);
}

///////////////////////////////////////////////////////////////////////////////
//		_Save_irradaince_cache_image
///////////////////////////////////////////////////////////////////////////////
void CBmcTR::_Save_irradaince_cache_image (char *fn)
{
	CImage img;
	int nWidth    = m_theCamera.Film_width ();
	int nHeight   = m_theCamera.Film_height();
	CCol4 *pImage = m_theCamera.ResultImagePtr();

	img.Create(nWidth, nHeight, 24);
	for (int y = 0; y < nHeight; y ++)
	{
		for (int x = 0; x < nWidth; x ++, pImage ++)
		{
			if (pImage->r() + pImage->g() + pImage->b())
				img.SetPixel(x, y, RGB(255, 255, 255));
			else img.SetPixel(x, y, RGB(0, 0, 0));
		}
	}

	img.Save(fn);
	img.Destroy();
}

//reTRacing some points in one frames
bool CBmcTR::_reSampleOneFrame(std::vector<ZH::reSamplePixel*>& _vPixels, int _threadNum,int _frameIndex)
{
    m_cBgColor = COLOR_BLACK;

    //-------------------------------------------------------------------------
    // 1. Light source initialization.
    //-------------------------------------------------------------------------
  
    if(m_bLightInitialzed==false)
    {
        printf("Initilize light source...........\n");
        _Initialize_light_source (m_fLightScale);
    }
    m_bLightInitialzed =  true;
    //-------------------------------------------------------------------------
    // 2. Create the photon map to capture in-direct lighting
    //-------------------------------------------------------------------------
    Allocate_Mem_for_photnMap ();
    printf("Creating photon maps...........\n");
    _Create_photon_maps();
    //-------------------------------------------------------------------------
    // 3. Rendering
    //-------------------------------------------------------------------------
    m_theCamera.ComputerRenderingParameters (m_vDeltaX, m_vDeltaY, m_vLeftBottom);

    int t_nPoints  = static_cast<int>(_vPixels.size());
    int t_n4 = static_cast<int>(t_nPoints/_threadNum);
    int t_nL =  t_nPoints  -   t_n4 *_threadNum;

    //No Irradiance
    if (m_fTolerance <= 0.f)
    {
            printf("Start resampling with no irradiance cache.\n");

            int index;
            for(int k=0;k<t_n4;k++)
            {

                for(int i=0;i<_threadNum;i++)
                {
                    index = i+_threadNum*k;
                    m_vResampleParams[i]._col     =  _vPixels[index]->m_nCol;
                    m_vResampleParams[i]._row    =  _vPixels[index]->m_nRow;  
                    m_vResampleParams[i]._color   = &(_vPixels[index]->m_color); 
                    m_vResampleParams[i]._object  =  this;

                    m_hResampleHandles[i] =	
                        (HANDLE)_beginthreadex(
                        NULL,
                        0,
                        (THREADFUNC)(CBmcTR::_sampleOnePixel_func),
                        (LPVOID)(&m_vResampleParams[i]),
                        0,
                        (THREADID)(&(m_dResampleIDs[i]))
                        );	

                    if(m_hResampleHandles[i]!=NULL)
                    {
                        SetThreadPriority(m_hResampleHandles[i],THREAD_PRIORITY_NORMAL);
                    }
                }
                
 
                WaitForMultipleObjects (_threadNum, m_hResampleHandles, TRUE, INFINITE);

                //DWORD _tt;
                for(int j	=	0;j<_threadNum;j++)
                {
                    if(m_hResampleHandles[j]!=NULL)
                    {
                        CloseHandle(m_hResampleHandles[j]);
                    }
                }

            }

            for(int i=t_n4*_threadNum;i<t_nPoints;i++)
            {
                index = i - t_n4*_threadNum;
                m_vResampleParams[index]._col     =  _vPixels[i]->m_nCol;
                m_vResampleParams[index]._row    =  _vPixels[i]->m_nRow;  
                m_vResampleParams[index]._color   = &(_vPixels[i]->m_color); 
                m_vResampleParams[index]._object  =  this;

                m_hResampleHandles[index] =	
                    (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    (THREADFUNC)(CBmcTR::_sampleOnePixel_func),
                    (LPVOID)(&m_vResampleParams[index]),
                    0,
                    (THREADID)(&(m_dResampleIDs[index]))
                    );	

                if(m_hResampleHandles[index]!=NULL)
                {
                    SetThreadPriority(m_hResampleHandles[i],THREAD_PRIORITY_NORMAL);
                }
            }
            

            WaitForMultipleObjects (t_nL, m_hResampleHandles, TRUE, INFINITE);
    }
    else//irradiance cache
    {
        printf("Start filtering with irradiance cache\n");
        //---------------------------------------------------------------------
        // Initialize irradiance caching tree and the critical section
        //---------------------------------------------------------------------
        bool bAllThreadRun;
        int nThread    = _threadNum;
        if(nThread>10)
            nThread = 10;
        TRtThredParam tTParam[10];
        unsigned int dwTId[10];
        int t;
        int nYStep = static_cast<int>(m_theCamera.Film_height()/nThread);  
        CVec3 vMax, vMin;
        float fSize;

        Get_boundingbox (vMax, vMin);
        fSize = max3(fabs(vMax.x-vMin.x), fabs(vMax.y-vMin.y),fabs(vMax.z-vMin.z));

        if(m_pIrraTree==NULL)
        {            
            m_pIrraTree = new CIrraTree (fSize, (vMax + vMin)*.5f, 0);
            m_pIrraTree->Set_tolerance (m_fTolerance);
        }



        //---------------------------------------------------------------------
        // First pass: Build the irradiance cache
        //---------------------------------------------------------------------
        printf("Building irradiance cache.............\n");

       /* MY_DELETE_ARRAY(m_pPxlInfo);
        m_pPxlInfo = new TPxlInfo [m_theCamera.Film_height()*m_theCamera.Film_width()];
        bAllThreadRun = true;

        for (t = 0; t < nThread; t ++)
        {
            tTParam[t]._nStart = nYStep * t;
            if (t == nThread - 1)
                tTParam[t]._nEnd = m_theCamera.Film_height();
            else
                tTParam[t]._nEnd = nYStep * (t+1)+1;

            tTParam[t]._theBmcTRIns = this;
            hThread[t] = (HANDLE)_beginthreadex (
                NULL, 
                0, 
                (THREADFUNC)Build_IC_thread_func, 
                &(tTParam[t]), 
                0, 
                &(dwTId[t]));

            SetThreadPriority (hThread[t], THREAD_PRIORITY_BELOW_NORMAL);
            if (hThread[t] == NULL)
            {
                printf ("Create thread %d failed when trying to build irradiance cache!", t); 
                bAllThreadRun = false;
            }
        }

        if (bAllThreadRun)
        {
            WaitForMultipleObjects (nThread, hThread, TRUE, INFINITE);
            for (t = 0; t < nThread; t ++)
                CloseHandle (hThread[t]);
        }*/

        //_Save_irradaince_cache_image ("IrradainceCachePoint.bmp");


        if(_frameIndex>=static_cast<int>(m_geoFileNameVec.size()))
            _frameIndex  = static_cast<int>(m_geoFileNameVec.size()) -1;
        //add increased node;
        if(_frameIndex>0)
        {
            std::ifstream iiFile(
                const_cast<char*>(m_irraCacheFileNameVec[_frameIndex].c_str()),
                std::ios::binary);
            if(iiFile.fail())
            {
                printf("Open irr file failed!\n");
                return false;
            }

            int num = 0;
            iiFile.read((char*)(&num),sizeof(int));
            if(num<0)
                return false;
            printf("Increased %d nodes\n", num);
            
            int CIrraValue_size = sizeof(CVector3f) * 2 + sizeof(CCol4) +sizeof(float) +sizeof(BYTE) + sizeof(TIrraGrad) * 2;
            CIrraValue t_tempValue;
            for(int i=0;i<num;i++)
            {
                iiFile.read((char*)(&(t_tempValue.m_vPos)),CIrraValue_size);
                m_pIrraTree->Insert(t_tempValue,m_pIrraTree);

            }


        }

        //apply geo data
        PixelInfo t_info(256,256);
        if(t_info.loadPixelInfoFromFile(const_cast<char*>(m_geoFileNameVec[_frameIndex].c_str()))==false)
        {
            printf("t_info.loadPixelInfoFromFile:\n%s failed!\n",const_cast<char*>(m_geoFileNameVec[_frameIndex].c_str()));
            return false;
        }

        MY_DELETE_ARRAY(m_pPxlInfo);
        int _total   =  m_theCamera.Film_height()*m_theCamera.Film_width();
        m_pPxlInfo = new TPxlInfo[_total];
        for(int i=0;i<_total;i++)
        {
            m_pPxlInfo[i]._nTID  =  t_info.m_triangleID[i];
            m_pPxlInfo[i]._vNml  = t_info.m_normal[i];
            m_pPxlInfo[i]._vPos   = t_info.m_pos[i];
            m_pPxlInfo[i]._vTex   = t_info.m_tex[i];
        }


        printf("IrradianceCache tree, Node:%10d, IrraValue: %15d\n",m_pIrraTree->m_nNodeNumber,m_pIrraTree->m_nIrraValueNumber);

        //---------------------------------------------------------------------
        // Second pass: Shading
        //---------------------------------------------------------------------
        printf("Shading.............\n");
       // Bmc_IC_thread_func
        printf("Resampling  %d pixels\n",t_nPoints);
        
        //using one thread
        if(t_nPoints<=11)
        {
                m_vResampleRenderParam[0].m_nStartPixel   = 0;
                m_vResampleRenderParam[0].m_nEndPixel    =  t_nPoints-1;
                m_vResampleRenderParam[0].m_vPixels = &(_vPixels);
                m_vResampleRenderParam[0]._object =  this;
                
                m_hResampleHandles[0] =	
                    (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    (THREADFUNC)(CBmcTR::_sampleOnePixel_irradianceCache_func),
                    (LPVOID)(&m_vResampleRenderParam[0]),
                    0,
                    (THREADID)(&(m_dResampleIDs[0]))
                    );	

                if(m_hResampleHandles[0]!=NULL)
                {
                    //printf("Shading thread %d laughed!\n",i);
                    SetThreadPriority(m_hResampleHandles[0],THREAD_PRIORITY_NORMAL);
                }

                WaitForSingleObject(m_hResampleHandles[0],INFINITE);
                CloseHandle(m_hResampleHandles[0]);

        }
        else
        {
            int t_step  =  static_cast<int>(t_nPoints/nThread);
            bool t_allrun  =  true;
    
            for(int j=0;j<nThread;j++)
            {
                m_vResampleRenderParam[j].m_nStartPixel      = t_step * j;
                if(j==nThread-1)
                {
                    m_vResampleRenderParam[j].m_nEndPixel    =  t_nPoints-1;
                }
                else
                {
                    m_vResampleRenderParam[j].m_nEndPixel    =  t_step * (j+1) -1;
                }

                m_vResampleRenderParam[j].m_vPixels = &(_vPixels);
                m_vResampleRenderParam[j]._object =  this;

                m_hResampleHandles[j] =	
                    (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    (THREADFUNC)(CBmcTR::_sampleOnePixel_irradianceCache_func),
                    (LPVOID)(&m_vResampleRenderParam[j]),
                    0,
                    (THREADID)(&(m_dResampleIDs[j]))
                    );	

                if(m_hResampleHandles[j]!=NULL)
                {
                    //printf("Shading thread %d laughed!\n",i);
                    SetThreadPriority(m_hResampleHandles[j],THREAD_PRIORITY_NORMAL);
                }
                else
                {
                    t_allrun =  false;
                }

            }

            if(t_allrun==true)
            {

                WaitForMultipleObjects (nThread, m_hResampleHandles, TRUE, INFINITE);
                for(int k=0;k<nThread;k++)
                {
                    CloseHandle(m_hResampleHandles[k]);
                }

            }


        }

}

    return true;
}


DWORD WINAPI CBmcTR::_sampleOnePixel_func (LPVOID lpParam)
{

    _tResampleParam* _p    = (_tResampleParam*)lpParam;
    CBmcTR* _obj   =  _p->_object;
    const int nHeight = _obj->m_theCamera.Film_height();

    int x  =   _p->_col;
    int y  =   nHeight   -   _p->_row -1;

    CVec3 CurrPixel;
    CCol4  cIrra;

    CVec3 vLB = _obj->m_vLeftBottom;
    CVec3 vDy = _obj->m_vDeltaY;
    CVec3 vDx = _obj->m_vDeltaX;

    CurrPixel = vLB + vDy * y + vDx * x;
    *(_p->_color) = _obj->_Pixel_radiance_PM (CurrPixel, cIrra, 0);

    return 0;
}

DWORD WINAPI CBmcTR::_sampleOnePixel_irradianceCache_func (LPVOID lpParam)
{
    //printf("CBmcTR::_sampleOnePixel_irradianceCache_func (LPVOID lpParam)\n");
    _tResampleArrayParam* _p    = (_tResampleArrayParam*)lpParam;
    if(_p==NULL)
    {
        printf("p==0================================>error!\n");
        return 0;
    }
    CBmcTR* _obj   =  _p->_object;
    if(_obj==NULL)
    {
        printf("_obj==0==============================>error!\n");
        return 0;
    }

    const int nWidth = _obj->m_theCamera.Film_width();
    const int nHeight = _obj->m_theCamera.Film_height();
    if(nWidth<=0||nHeight<=0)
    {
        printf("nWidth=%d,nHeight=%d==0===================>error!\n",nWidth,nHeight);
        return 0;
    }
    
    std::vector<ZH::reSamplePixel*>& t_vPixel =  *(_p->m_vPixels);
    int t_size   = static_cast<int>(t_vPixel.size());

    for(int k=_p->m_nStartPixel;k<=_p->m_nEndPixel;k++)
    {
            if(k<0)
                k=0;
            if(k>=t_size)
                k=t_size-1;

            int x  = t_vPixel[k]->m_nCol;
            int y  = nHeight -   (t_vPixel[k]->m_nRow) -1;   
            if(x<0||x>=nWidth||y<0||y>=nHeight)
            {
                printf("x=%d,y=%d==0===================>error!\n",x,y);
                return 0;      
            }

            CVec3 CurrPixel= _obj->m_vLeftBottom + _obj->m_vDeltaY * y + _obj->m_vDeltaX * x;
            int nOffset = (nHeight - y - 1) * nWidth + x;

            
            TPxlInfo *pPif;
            pPif    = &((_obj->m_pPxlInfo)[nOffset]);
            if(pPif==NULL)
            {
                printf("pPif==NULL===================>error!\n");
                return 0;    
            }

            (t_vPixel[k]->m_color) = _obj->_Shade_using_IC (pPif, CurrPixel);
    }


    return 0;
}


void CBmcTR::_setIrradianceCacheFileNameVec(const std::vector<std::string>& _vec)
{
    m_irraCacheFileNameVec.clear();
    std::vector<std::string>::const_iterator t_ite;
    for(t_ite=_vec.begin();t_ite!=_vec.end();t_ite++)
    {
        m_irraCacheFileNameVec.push_back(*t_ite);
    }

}

void CBmcTR::_setGeoFileNameVec(const std::vector<std::string>& _vec)
{
    m_geoFileNameVec.clear();
    std::vector<std::string>::const_iterator t_ite;
    for(t_ite=_vec.begin();t_ite!=_vec.end();t_ite++)
    {
        m_geoFileNameVec.push_back(*t_ite);
    }

}

bool CBmcTR::buildIrradianceCacheTreeFromFile()
{
    DeleteIrradianceCacheTree();
    m_pIrraTree  =  new CIrraTree;
    if(m_pIrraTree==NULL||m_irraCacheFileNameVec.size()<=0)
    {
        printf("CBmcTR::buildIrradianceCacheTreeFromFile()-failed!\n");
        return true;
    }

     return m_pIrraTree->buildTreeFromFile(const_cast<char*>(m_irraCacheFileNameVec[0].c_str()));

}
