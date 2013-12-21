// Yanyun Chen
// Internet Graphics Group, Microsoft Research Asia
// September 2003
///////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "McBspTR.h"
#include "Shaders.h"
#include ".\mcbsptr.h"
#include "Log.h"

CShaders G_theSahder;

///////////////////////////////////////////////////////////////////////////////
//		Constructor / Destructor
///////////////////////////////////////////////////////////////////////////////
CMcBspTR::CMcBspTR(void)
{
	m_nGPhotons = 100000;
	m_pGlobalPhotonMap = NULL;
	m_pDirectPhotonMap = NULL;
	m_pCausticPhotonMap = NULL;
	m_bPhotonMapsInitialized = FALSE;
	m_bDirectionalLight = FALSE;
	m_nDistrRayNum = 100;
	m_nLightRay = 10;
	m_nCauPhoSubd = 100;

	m_tCausDir.next = NULL;
	m_nCausDir = 0;
	
	m_pixelInfo	=	NULL;

	m_nFeatures = BSP_MCRT_DIRECT_LIGHTING | BSP_MCRT_INDIRECT_LIGHTING | 
		BSP_MCRT_MIRROR_EFFECT | BSP_MCRT_CAUSTIC_EFFECT;
}

CMcBspTR::~CMcBspTR(void)
{
	_Delete_photon_maps();
	MY_DELETE(m_pixelInfo);
}

void CMcBspTR::setRenderEffect( 
                     BOOL directLight,
                     BOOL inDirectLight,
                     BOOL MirrorEffect,
                     BOOL CausticEffect,
                     BOOL DirectionalLight,
                     BOOL TotallyPhotonMap
                     )
{
    m_nFeatures =  0;
    if(directLight)
        m_nFeatures |= BSP_MCRT_DIRECT_LIGHTING;
    if(inDirectLight)
        m_nFeatures |= BSP_MCRT_INDIRECT_LIGHTING;
    if(MirrorEffect)
        m_nFeatures |= BSP_MCRT_MIRROR_EFFECT;
    if(CausticEffect)
        m_nFeatures |= BSP_MCRT_CAUSTIC_EFFECT;
    if(DirectionalLight)
        m_nFeatures |= BSP_MCRT_DIRECTIONAL_LIGHT;
    if(TotallyPhotonMap)
        m_nFeatures |= BSP_MCRT_TOTALLY_PHOTON_MAP;
}

///////////////////////////////////////////////////////////////////////////////
//		Enable / Disable
// Modify the ray tracing feature
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::Enable (const int nFeatures)
{
	m_nFeatures |= nFeatures;
}

void CMcBspTR::Disable (const int nFeatures)
{
	m_nFeatures &= (~nFeatures);
}

int CMcBspTR::Get_MC_rendering_feature (void)
{
	return m_nFeatures;
}


///////////////////////////////////////////////////////////////////////////////
//		Delete photon maps
// Called before the creation of new photon maps, or by the distructor
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Delete_photon_maps()
{

		MY_DELETE(m_pGlobalPhotonMap);
		MY_DELETE(m_pDirectPhotonMap);
		MY_DELETE(m_pCausticPhotonMap);

}



///////////////////////////////////////////////////////////////////////////////
//		Load_OBJ
// Load the obj scene using the function of the base class
// Modify all material to make sure ks + kd <=1, so that the energy would be 
// preserved.
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::Load_OBJ( char *fn, float fMapSize)
{
	CBspTR::Load_OBJ (fn, fMapSize);
	_Check_materials();
	_Delete_photon_maps();
}


///////////////////////////////////////////////////////////////////////////////
//		_Check_materials
//
// 1. Build light source. We suppose the ambient item is the emission of the
// material because we load only Wavefront OBJ model currently 
// 2. Sacle all the materals, make sure Kd + Ks <= 1, so that the energy would
// be conserved when the photon map is being created;
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Check_materials(void)
{
	float fTemp, fMax = 0;
	int i;

	// Ks + Kd should be no <= 1
	for (i = 0; i < m_nMaterials; i ++)
	{
		// Check light source
		m_pMaterials[i].m_fEmissive[3] = 0.f;

		if (m_pMaterials[i].m_fAmbient[0] + 
			m_pMaterials[i].m_fAmbient[1] + 
			m_pMaterials[i].m_fAmbient[2] >= 0.1f )
		{
			m_pMaterials[i].m_fEmissive[0] = m_pMaterials[i].m_fAmbient[0] * 100;
			m_pMaterials[i].m_fEmissive[1] = m_pMaterials[i].m_fAmbient[1] * 100;
			m_pMaterials[i].m_fEmissive[2] = m_pMaterials[i].m_fAmbient[2] * 100;
			m_pMaterials[i].m_fEmissive[3] = 1.f;

			m_pMaterials[i].m_fAmbient[0]  = 0.f;
			m_pMaterials[i].m_fAmbient[1]  = 0.f;
			m_pMaterials[i].m_fAmbient[2]  = 0.f;
			m_pMaterials[i].m_fAmbient[3]  = 1.f;
			continue;
		}

		m_pMaterials[i].m_fAmbient[0] = 0.f;
		m_pMaterials[i].m_fAmbient[1] = 0.f;
		m_pMaterials[i].m_fAmbient[2] = 0.f;
		m_pMaterials[i].m_fAmbient[3] = 1.f;

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
//		_Initial_light_patches
// Initial the light patches for the creation of globel photon map
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Initial_light_patches(void)
{
	int i;
	float fTemp, fTotalEnergy;

	fTotalEnergy  = 0.f;
	m_nLigPatches = 0;

	for (i = 0; i < m_nTriangles; i ++)
	{
		if (!_Is_light_patch (i)) continue;

		CBspMaterial *pm = &m_pMaterials[m_pTriangles[i].GetMaterialID()];
		fTotalEnergy += m_pTriangles[i].Calc_area() * 
			(pm->m_fEmissive[0] + pm->m_fEmissive[1] + pm->m_fEmissive[2]);
		m_tpLigPatches[m_nLigPatches].nTID = i;
		m_nLigPatches ++;
	}

	if (fTotalEnergy < 1.0e-5f) // No light source, does not need a photon map
	{
		m_nGPhotons = 0;
		return;
	}

	//-------------------------------------------------------------------------
	// Initialize the light patches
	//-------------------------------------------------------------------------

	for (i = 0; i < m_nLigPatches; i ++)
	{
		CBspTriangle *tri = &m_pTriangles[m_tpLigPatches[i].nTID];
		CBspMaterial *pm = &m_pMaterials[tri->GetMaterialID()];
		float fArea = m_pTriangles[m_tpLigPatches[i].nTID].Calc_area();

		float fIntensity = 
			pm->m_fEmissive[0] + pm->m_fEmissive[1] + pm->m_fEmissive[2];
		m_tpLigPatches[i].cEnergy.r() = fArea * pm->m_fEmissive[0];
		m_tpLigPatches[i].cEnergy.g() = fArea * pm->m_fEmissive[1];
		m_tpLigPatches[i].cEnergy.b() = fArea * pm->m_fEmissive[2];
		m_tpLigPatches[i].cEnergy.a() = 1.f;
		m_tpLigPatches[i].nPhotonNum = 
			int(m_nGPhotons * fIntensity * fArea / fTotalEnergy);
		m_tpLigPatches[i].vNml = tri->Calc_normal();
		m_tpLigPatches[i].vNml.Normalize();

		fTemp = fArea / m_tpLigPatches[i].nPhotonNum;
		m_tpLigPatches[i].cPhotonPow.r() = pm->m_fEmissive[0] * fTemp;
		m_tpLigPatches[i].cPhotonPow.g() = pm->m_fEmissive[1] * fTemp;
		m_tpLigPatches[i].cPhotonPow.b() = pm->m_fEmissive[2] * fTemp;

		printf("%d photons for light patch %d\n", m_tpLigPatches[i].nPhotonNum,
			m_tpLigPatches[i].nTID);
	}
}


///////////////////////////////////////////////////////////////////////////////
//		_Calc_diffuse_and_specular_prob
// Calculate the diffuse and specular reflectance probabilities
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Calc_diffuse_and_specular_prob (int &nPID, CCol4 &cPow,
												CVec3 &vTex, float &fDiffProb,
												float &fSpecProb)
{
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];
	if (pm->m_iTexture.IsInitialized())
	{
		CCol4 cTex = pm->m_iTexture.TextureColor (vTex[0], vTex[1]);

		fDiffProb = max3(cPow[0] * pm->m_fDiffuse[0] * cTex[0],
			cPow[1] * pm->m_fDiffuse[1] * cTex[1], 
			cPow[2] * pm->m_fDiffuse[2] * cTex[2]) /
			max3(cPow[0], cPow[1], cPow[2]);
	}
	else
	{
		fDiffProb = max3 (cPow[0] * pm->m_fDiffuse[0],
			cPow[1] * pm->m_fDiffuse[1], cPow[2] * pm->m_fDiffuse[2] ) /
			max3(cPow[0], cPow[1], cPow[2]);
	}

	fSpecProb = max3(cPow[0] * pm->m_fSpecular[0],
		cPow[1] * pm->m_fSpecular[1], cPow[2] * pm->m_fSpecular[2]) /
		max3(cPow[0], cPow[1], cPow[2]);
}



///////////////////////////////////////////////////////////////////////////////
//		Trace the global photon 
// Bounce the global photon in the scene by the Russian Roulette determination
// and the reflectivity of the surfaces
///////////////////////////////////////////////////////////////////////////////
BOOL CMcBspTR::_Trace_global_photon (TBspRay ray, CCol4 cPow, int nPID, int &nStored)
{
	int nDepth = 1;
	int nFlag = 0;
	BOOL bTraceCaustic = FALSE;

	while(1)
	{
		TBspCross crs;	// The intersection point
		float fRussanRoulette;
		float fDiffProb, fSpecProb;

		// Intersection test
		if (!_RayTreeIntersect(ray, crs, nPID ))
			return bTraceCaustic; // the photon miss the scene

		if (_Is_light_patch(nPID)) return bTraceCaustic;
		//---------------------------------------------------------------------
		// Calculate the diffuse and specular probabilities
		//---------------------------------------------------------------------
		_Calc_diffuse_and_specular_prob (nPID, cPow, crs.vTex,
			fDiffProb, fSpecProb);
		if ((nDepth == 1) && (fSpecProb > 0.4f))
			nFlag = 1;

		ray.vOrg = crs.vPos;
		fRussanRoulette = RANDOM_0_1;
		CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];

		//---------------------------------------------------------------------
		// diffuse reflected
		//---------------------------------------------------------------------
		if (fRussanRoulette <fDiffProb)
		{
			// Store the photon
			if (nDepth > 1)
			{
				m_pGlobalPhotonMap->store(cPow, ray.vOrg, ray.vDir);
				nStored ++;

				if (nFlag == 1 && nDepth == 2)
					bTraceCaustic = TRUE;
			}
			else
			{
				m_pDirectPhotonMap->store(cPow, ray.vOrg, ray.vDir);
			}

			// Random direction
			ray.vDir = RANDOM_VEC;
			ray.vDir.Normalize();

			if (ray.vDir.Dot(crs.vNml) < 0.f)
				ray.vDir *= -1;

			ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;

			cPow.r() *= pm->m_fDiffuse[0] / fDiffProb;
			cPow.g() *= pm->m_fDiffuse[1] / fDiffProb;
			cPow.b() *= pm->m_fDiffuse[2] / fDiffProb;
		}

		//---------------------------------------------------------------------
		// specular reflected
		//---------------------------------------------------------------------
		else if (fRussanRoulette < fDiffProb + fSpecProb)
		{
			// Store the photon
			if (nDepth <= 1)
			{
				m_pDirectPhotonMap->store(cPow, ray.vOrg, ray.vDir);
			}
			ray.vDir = _Reflect_the_ray (ray.vDir, crs.vNml,
				(128.f-pm->m_fShininess[0]) / 256);
			ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;

			cPow.r() *= pm->m_fSpecular[0] / fSpecProb;
			cPow.g() *= pm->m_fSpecular[1] / fSpecProb;
			cPow.b() *= pm->m_fSpecular[2] / fSpecProb;
		}

		//---------------------------------------------------------------------
		// absorbed 
		//---------------------------------------------------------------------
		else
		{
			if (nDepth > 1)
			{
				m_pGlobalPhotonMap->store (cPow, ray.vOrg, ray.vDir);
				nStored ++;
			}
			else
			{
				m_pDirectPhotonMap->store (cPow, ray.vOrg, ray.vDir);
			}
			return bTraceCaustic;
		}

		nDepth ++;
	}

	return bTraceCaustic;
}



///////////////////////////////////////////////////////////////////////////////
//		Trace the caustic photon 
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Trace_caustic_photon (TBspRay ray, CCol4 cPow, int nPID, int &nStored)
{
	TBspCross crs;
	float fDiffProb, fSpecProb;

	// Intersection test
	if (!_RayTreeIntersect(ray, crs, nPID ))
		return; // the photon miss the scene

	//-------------------------------------------------------------------------
	// Calculate the diffuse and specular probabilities
	//-------------------------------------------------------------------------
	_Calc_diffuse_and_specular_prob(nPID, cPow, crs.vTex, fDiffProb, fSpecProb);

	//-------------------------------------------------------------------------
	// If the first hit is not a high-specular surface, return
	//-------------------------------------------------------------------------
	if (fSpecProb < 0.4f)	return;

	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];
	cPow.r() *= pm->m_fSpecular[0];
	cPow.g() *= pm->m_fSpecular[1];
	cPow.b() *= pm->m_fSpecular[2];

	ray.vOrg = crs.vPos;
	ray.vDir = _Reflect_the_ray (ray.vDir, crs.vNml,
		(128.f-pm->m_fShininess[0]) / 256);
	ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;

	if (_RayTreeIntersect(ray, crs, nPID ))
	{
		m_pCausticPhotonMap->store (cPow, crs.vPos, ray.vDir);
		nStored ++;
	}
}



///////////////////////////////////////////////////////////////////////////////
//		_Create_global_photonmap
// Build the global photon map after loading the scene and checking materials
///////////////////////////////////////////////////////////////////////////////
bool CMcBspTR::_Create_global_photonmap(void)
{
	int i, j;
	int nStored = 0;
	CCol4 cCurrPow;
	TBspRay ray;

	// Check light source
	_Initial_light_patches();

	printf("\nBuilding global photon map ...\n");
	int nShotPhotons = 0;

	for (i = 0; i < m_nLigPatches; i ++)
	{
		CBspTriangle *tri = &m_pTriangles[m_tpLigPatches[i].nTID];

		// Shoot photons
		for (j = 0; j < m_tpLigPatches[i].nPhotonNum; j ++)
		{
			if ( (nShotPhotons ++)%100 == 0 ) printf("|");

			//-----------------------------------------------------------------
			// Create a photon (start point, direction and power)
			//-----------------------------------------------------------------
			ray.vOrg = tri->Random_point_on_the_triangle();

			if (m_bDirectionalLight)
			{
				ray.vDir = m_vLightDir * (-1);
			}
			else
			{
				ray.vDir = RANDOM_VEC + m_tpLigPatches[i].vNml;
				ray.vDir.Normalize();
				/*
				ray.vDir = RANDOM_VEC;
				if (ray.vDir.Dot (m_tpLigPatches[i].vNml) < 0.f)
					ray.vDir *= -1;
				*/
			}
			ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;
			cCurrPow = m_tpLigPatches[i].cPhotonPow;

			//-----------------------------------------------------------------
			// Bouncing the photon
			// If the photon hit and specular surface and then hit a diffuse
			// surface, store the information for caustic photon map creation.
			//-----------------------------------------------------------------
			if (_Trace_global_photon (ray, m_tpLigPatches[i].cPhotonPow,
				m_tpLigPatches[i].nTID, nStored))
			{
				TCausDir *p1, *p2 = &m_tCausDir;

				p1 = new TCausDir;
				p1->nLID = i;
				p1->vPos = ray.vOrg;
				p1->vDir = ray.vDir;
				p1->next = NULL;

				for (int k = 0; k < m_nCausDir; k ++)
					p2 = p2->next;

				p2->next = p1;
				m_nCausDir ++;				
			}
		}
	}

	// Balance photon map kd-tree
	printf("\n Balance KD tree");
	m_pGlobalPhotonMap->balance();
	m_pDirectPhotonMap->balance();
	m_bPhotonMapsInitialized = TRUE;
	printf("\nFinish building global photon map, %d photons stored\n", nStored);

    return true;
}



///////////////////////////////////////////////////////////////////////////////
//		_Create_caustic_photonmap
// Build the caustic photon map after the creation of global photon map
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Create_caustic_photonmap(void)
{
	int     i, j;
	CCol4   cCurrPow;
	int     nStored = 0;
	TBspRay ray;
	float   fLen;

	printf("\nBuilding caustic photon map ...\n");

	TCausDir *p = m_tCausDir.next;

	for (i = 0; i < m_nCausDir; i ++)
	{
		printf (".");
		CCol4 cPow = m_tpLigPatches[p->nLID].cPhotonPow / m_nCauPhoSubd * 5;

		if (m_bDirectionalLight)
		{
			CBspTriangle *tri = &m_pTriangles[m_tpLigPatches[p->nLID].nTID];
			fLen = sqrt(tri->Calc_area() / m_tpLigPatches[p->nLID].nPhotonNum);
			CVec3 vZ = tri->Calc_normal();
			CVec3 vY = RANDOM_VEC;
			CVec3 vX = vY.Cross(vZ);
			vX.Normalize();
			vY = vZ.Cross(vX);

			vX *= fLen;
			vY *= fLen;

			for (j = 0; j < m_nCauPhoSubd; j ++)
			{
				ray.vOrg = p->vPos + vX * RANDOM_N1_P1 + vY * RANDOM_N1_P1;
				ray.vDir = p->vDir;
				ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;
				_Trace_caustic_photon (ray, cPow, m_tpLigPatches[p->nLID].nTID,
					nStored);
			}
		}
		else
		{
			double fCos = cos(PI/sqrt(m_tpLigPatches[p->nLID].nPhotonNum*4.));
			fLen = 2.f * (float) tan( acos(fCos) );

			for (j = 0; j < m_nCauPhoSubd; j ++)
			{
				ray.vOrg = p->vPos;
				ray.vDir = p->vDir + (RANDOM_VEC * fLen);
				ray.vDir.Normalize();
				ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;
				_Trace_caustic_photon (ray, cPow, m_tpLigPatches[p->nLID].nTID,
					nStored);
			}
		}
		
		m_tCausDir.next = p->next;
		delete p;
		p = m_tCausDir.next;
	}
	m_tCausDir.next = NULL;

	// Balance photon map kd-tree
	printf("\n Balance KD tree");
	m_pCausticPhotonMap->balance();
	printf("\nFinish building caustic photon map, %d photons stored\n", nStored);
}



///////////////////////////////////////////////////////////////////////////////
//		_PM_radiance_along_the_ray
// Estimate the radiance at the given direction using photon map
///////////////////////////////////////////////////////////////////////////////
//#define _TOTALLY_USING_PHOTON_MAP_
CCol4 CMcBspTR::_PM_radiance_along_the_ray (TBspRay &ray, int nPID, BOOL &bHitLig,int offset)
{
	TBspCross crs;
	CColor4f cRsl = COLOR_BLACK, cTemp;
	
	if (_RayTreeIntersect(ray, crs, nPID ))
	{

		int _materialID	=	m_pTriangles[nPID].GetMaterialID();
		CBspMaterial *pm = &m_pMaterials[_materialID];

		//record pixel information
		if(offset>=0)
			m_pixelInfo->setPixelInfo(offset, crs.fDist, _materialID, crs.vNml,crs.vPos,crs.vTex,nPID);
		
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
		// Use the Photon map to estimate the indirect lighting intensity
		//---------------------------------------------------------------------
		m_pGlobalPhotonMap->irradianceEstimate
			(cRsl, crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);

		//---------------------------------------------------------------------
		// Use direct photon map to estimate the direct lighting irradiance if 
		// totally using photon map, else, estimate direct lighting intensity
		//---------------------------------------------------------------------
		#ifdef _TOTALLY_USING_PHOTON_MAP_
		m_pDirectPhotonMap->irradianceEstimate
			(cTemp, crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);
		cRsl += cTemp;
		#else
		cRsl += _Direct_lighting(crs.vPos, crs.vNml, ray.vDir, crs.vTex, nPID);
		#endif

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
		if(offset>=0)
			m_pixelInfo->setPixelInfo(offset,1.0f, -1, CVector3f(0,0,0),CVector3f(0,0,0),CVector3f(0,0,0),-1);
		cRsl = m_cBgColor;
	}

	bHitLig = FALSE;
	return (cRsl);
}



///////////////////////////////////////////////////////////////////////////////
//		_Direct_lighting
// Calculate the radiance directly come from light sources
///////////////////////////////////////////////////////////////////////////////
CCol4 CMcBspTR::_Direct_lighting (const CVec3 &vPos, const CVec3 &vNml,
								  const CVec3 &vView, const CVec3 &vTex,
								  int PID)
{
	int l;
	TBspRay ray;
	TBspCross crs;
	CBspMaterial *pm;
	CCol4 cRsl = COLOR_BLACK;

	pm = &m_pMaterials[m_pTriangles[PID].GetMaterialID()];

	if (m_bDirectionalLight)
	{
		ray.vOrg = vPos;
		ray.vDir = m_vLightDir;
		ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;

		if (ray.vDir.Dot(vNml) < 0.f)	return cRsl;

		int PID2 = PID;
		if (_RayTreeIntersect (ray, crs, PID2))
		{
			if (!_Is_light_patch(PID2)) return cRsl;
			cRsl =  G_theSahder.PhongShading (m_cLightColor, ray.vDir,
				vView*(-1), vPos, vNml, vTex.at(0), vTex.at(1), pm);
		}
		return cRsl;
	}

	for (l = 0; l < m_nLigPatches; l ++)
	{
		//---------------------------------------------------------------------
		// construct a ray point to a random point on the selected light patch
		//---------------------------------------------------------------------
		CBspTriangle *tri = &m_pTriangles[m_tpLigPatches[l].nTID];
		ray.vEnd = tri->Random_point_on_the_triangle();
		ray.vOrg = vPos;
		ray.vDir = ray.vEnd - vPos;
		ray.vDir.Normalize();
		ray.vEnd += ray.vDir; // extend the ray a little bit

		//---------------------------------------------------------------------
		// If the patch does not facing the light patch or the light patch dose
		// not facing the patch, return black;
		//---------------------------------------------------------------------
		if (ray.vDir.Dot(vNml) < 0.f) continue;
		float fDirectionalScale = m_tpLigPatches[l].vNml.Dot(ray.vDir);
		if (fDirectionalScale > 0.f) continue;
		else fDirectionalScale = -fDirectionalScale;

		//---------------------------------------------------------------------
		// Check if the light is occluded by another patch
		// Calculate the shading by phong model if is lit
		//---------------------------------------------------------------------
		int PID2 = PID;
		if (_RayTreeIntersect (ray, crs, PID2))
		{
			if (PID2 == m_tpLigPatches[l].nTID)
			{
				// Calculate the lighting intensity according to the the
				// distance and the intensity of the light patch.
				CVec3 vTemp = crs.vPos - vPos;
				//float fTemp = vTemp.Dot(vTemp) * 4 * PI;
				float fTemp = vTemp.Dot(vTemp) * PI;
				CCol4 cIntensity =
					m_tpLigPatches[l].cEnergy * fDirectionalScale / fTemp;

				// Shading computation by Phong model.
				cRsl += G_theSahder.PhongShading (cIntensity, ray.vDir,
					vView*(-1), vPos, vNml, vTex[0], vTex[1], pm);
			}
		}
	}

	return cRsl;
}



///////////////////////////////////////////////////////////////////////////////
//		_Indirect_irradiance
// Estimate the in-direction irradiance (diffuse)
///////////////////////////////////////////////////////////////////////////////
CCol4 CMcBspTR::_Indirect_irradiance (const TBspCross &crs, int nPID, int nDepth)
{
	CCol4 cAccu = COLOR_BLACK;
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];

	if (m_nDistrRayNum <= 0) return cAccu;
	//-------------------------------------------------------------------------
	// If it is the first hit, we would need to estimate the indirect
	// illumination accurately. 
	//-------------------------------------------------------------------------
	if (nDepth == 0)
	{
		int nAccu = 0;
		BOOL bHitLig;
		TBspRay ray;

		// The local frame
		CVec3 vZ = crs.vNml.Normalized();
		CVec3 vY = RANDOM_VEC;
		CVec3 vX = (vY.Cross(vZ)).Normalized();
		vY = vZ.Cross(vX);

		// The inclination and azimuth resolution
		int m = (int) sqrt((double)(m_nDistrRayNum>>2));
		int n = m_nDistrRayNum / m;
		
		// Evenly distribute the sampling ray on the hemi-sphere according to
		// the inclination angle
		for (int i = 1; i <= n; i ++)
		{
			for (int j = 1; j <= m; j ++)
			{
				float fTheta = (float)asin(sqrt((j-RANDOM_0_1)/m));
				float fPhi   = 2 * PI * (i-RANDOM_0_1)/n;

				ray.vDir = vY * sin (fPhi) + vX * cos (fPhi);
				ray.vDir = vZ * sin (fTheta) + ray.vDir * cos (fTheta);
				ray.vOrg = crs.vPos;
				ray.vEnd = ray.vOrg + ray.vDir * m_fSceneSize;
				CCol4 cTemp = _PM_radiance_along_the_ray (ray, nPID, bHitLig);

				if (!bHitLig)
				{
					cAccu += cTemp;
					nAccu ++;
				}
			}
		}

		cAccu = cAccu * PI / nAccu;
		// cAccu = cAccu / nAccu;
	}

	//-------------------------------------------------------------------------
	// If not so we just estimate it by photon map.
	//-------------------------------------------------------------------------
	else
	{
		m_pGlobalPhotonMap->irradianceEstimate
			(cAccu, crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);
	}

	return cAccu;
}



///////////////////////////////////////////////////////////////////////////////
//		_MC_radiance_along_the_ray
// Estimate the radiance at the given viewing direction; Using Monte Carlo
// integration to calculate the irradiance.
///////////////////////////////////////////////////////////////////////////////
CCol4 CMcBspTR::_MC_radiance_along_the_ray(TBspRay &ray, int nDepth,
										   CCol4 &cDiffIrra, int offset)
{
	TBspRay	ray2;
	TBspCross crs, crs2;
	
	int nPID = -1;
	CCol4 cRsl = COLOR_BLACK, cAccu, cTemp;

	if (!_RayTreeIntersect(ray, crs, nPID )) 
	{
		ASSERT(m_pixelInfo);
		if(offset>=0)
			m_pixelInfo->setPixelInfo(offset, 1.0f, -1, CVector3f(0,0,0),CVector3f(0,0,0),CVector3f(0,0,0),-1);
		return m_cBgColor;
	}

	int _materialID	=	m_pTriangles[nPID].GetMaterialID();
	CBspMaterial *pm = &m_pMaterials[_materialID];

	if(offset>=0)
	{
		ASSERT(m_pixelInfo);
		m_pixelInfo->setPixelInfo(offset, crs.fDist, _materialID, crs.vNml, crs.vPos, crs.vTex, nPID);
	}

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
	if (m_nFeatures & BSP_MCRT_DIRECT_LIGHTING)
	{
		for (int i = 0; i < m_nLightRay; i ++)
		{
			cRsl += _Direct_lighting
				(crs.vPos, crs.vNml, ray.vDir, crs.vTex, nPID);
		}
		cRsl /= m_nLightRay;
	}

	//-------------------------------------------------------------------------
	// Calculate the diffuse irradiance
	//-------------------------------------------------------------------------
	if (m_bEstimateDiffuseIrra)
	{
		if (m_nFeatures & BSP_MCRT_INDIRECT_LIGHTING)
			cDiffIrra = _Indirect_irradiance (crs, nPID, nDepth);
		else
		{
			cDiffIrra = COLOR_BLACK;
		//	m_pGlobalPhotonMap->irradianceEstimate (cDiffIrra,
		//		crs.vPos, crs.vNml, m_fLocalRadius, m_nLocalPhoton);
		}
	}

	// Consider the caustic contribution
	if ((m_bCaustic) && (m_nFeatures & BSP_MCRT_CAUSTIC_EFFECT))
	{
		m_pCausticPhotonMap->GaussianIrradianceEstimate(cTemp, 
			crs.vPos, crs.vNml,	m_fLocalRadius*0.1f, m_nLocalPhoton);
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
		cAccu.r() = pm->m_fSpecular[0];
		cAccu.g() = pm->m_fSpecular[1];
		cAccu.b() = pm->m_fSpecular[2];
		if ((nDepth<4) && (cAccu[0]>.01f || cAccu[1]>.01f || cAccu[2]>.01f))
		{
			ray2.vDir = _Reflect_the_ray (ray.vDir, crs.vNml,
				(128 - pm->m_fShininess[0]) / 128);
			ray2.vOrg = crs.vPos;
			ray2.vEnd = crs.vPos + ray2.vDir * m_fSceneSize;

			CCol4 cTemp;
			m_bEstimateDiffuseIrra = TRUE;
			cAccu = _MC_radiance_along_the_ray (ray2, nDepth+1, cTemp);
			cAccu.r() *= pm->m_fSpecular[0];
			cAccu.g() *= pm->m_fSpecular[1];
			cAccu.b() *= pm->m_fSpecular[2];

			cRsl += cAccu;
		}
	}

	return (cRsl);
}



///////////////////////////////////////////////////////////////////////////////
//		_Pixel_radiance_PM
// Estimate the radiance of a pixel along the given direction
// int offset: image pixel position offset.
///////////////////////////////////////////////////////////////////////////////
CCol4 CMcBspTR::_Pixel_radiance_PM (CVec3 & vPPos, CCol4 &cDiffIrra, int offset)
{
	TBspRay	ray;
	
	// The ray
	ray.vOrg = m_theCamera.Eye();
	ray.vEnd = vPPos;
	ray.vDir = vPPos - m_theCamera.Eye();
	ray.vDir.Normalize();

	//m_cRemainder = COLOR_WHITE;

	if (m_nFeatures & BSP_MCRT_TOTALLY_PHOTON_MAP)
	{
		BOOL bHit;
		return _PM_radiance_along_the_ray (ray, -1, bHit, offset);
	}
	else
	{
		return _MC_radiance_along_the_ray(ray, 0, cDiffIrra, offset);
	}
}



///////////////////////////////////////////////////////////////////////////////
//		_Create_photon_maps
// Create global and caustic photon maps
///////////////////////////////////////////////////////////////////////////////
bool CMcBspTR::_Create_photon_maps()
{
	if (m_bDirectionalLight) m_nLightRay = 1;

	if (!m_bPhotonMapsInitialized)
	{
		if(_Create_global_photonmap()==false)
        {
            return false;
        }
		if (m_nCausDir > 0)
		{
			printf ("\nCaustic dir = %d\n", m_nCausDir);
            
            MY_DELETE(m_pCausticPhotonMap);
			m_pCausticPhotonMap = new CPhotonMap (m_nCausDir * m_nCauPhoSubd);
			if(m_pCausticPhotonMap==NULL)
            {
                Singleton<ZHLog>::getSingletonPtr()->assertError("CMcBspTR::_Create_photon_maps()--new ");
                return false;
            }
            
            
            _Create_caustic_photonmap();
			m_bCaustic = TRUE;
		}
		else m_bCaustic = FALSE;
	}

	m_fLocalRadius = m_fSceneSize * 0.025f;
	m_nLocalPhoton = 1000;
    return true;
}



///////////////////////////////////////////////////////////////////////////////
//		MCPM_ray_tracing_normal
// Normal Monte Carlo ray tracing
//
// This code has not been finished, yet.
//
//
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::MC_ray_tracing_normal ()
{
	m_theStopWatch.Start("Starting normal MCPM ray tracing");

	//-------------------------------------------------------------------------
	// Create the photon map to capture in-direct lighitng
	//-------------------------------------------------------------------------
	_Create_photon_maps();
	m_theStopWatch.Show_last_interval("Photon map creation");

	int   x, y;
	CVec3 m_vLeftBottom, CurrPixel, m_vDeltaX, m_vDeltaY, CurrY;
	m_theCamera.ComputerRenderingParameters(m_vDeltaX, m_vDeltaY, m_vLeftBottom);

	//-------------------------------------------------------------------------
	// So far the code has not considered the adaptive sampling, yet.
	//-------------------------------------------------------------------------
	m_bEstimateDiffuseIrra = TRUE;
	CCol4 cIrra;
	CCol4 *pCurr, *pImage = ResultImagePtr();
	int nOffset;
	int nWidth  = m_theCamera.Film_width();
	int nHeight = m_theCamera.Film_height();
	printf ("\nStart MC ray tracing (%dx%d)\n", nWidth, nHeight);
	for (y = 0; y < nHeight; y ++)
	{
		CurrY   = m_vLeftBottom + m_vDeltaY * y;
		nOffset = nHeight - y - 1;
		nOffset = nOffset * nWidth;
		pCurr   = &pImage[nOffset];
		if ((y % 20) == 0)
			printf ("\nLine: %4d  %2d%%  ", y, y * 100 / nHeight);
		else printf (".");
		for (x = 0; x < nWidth; x ++, pCurr ++)
		{
			CurrPixel = CurrY + m_vDeltaX * x;
			*pCurr = _Pixel_radiance_PM (CurrPixel, cIrra);
		}
	}

	m_theStopWatch.Show_total_time("Total rendering time: ");
}



///////////////////////////////////////////////////////////////////////////////
//		MCPM_ray_tracing_normal
// Normal Monte Carlo ray tracing using global and caustic photon maps
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::MCPM_ray_tracing_normal ()
{
	m_theStopWatch.Start("Starting normal MCPM ray tracing");

	//-------------------------------------------------------------------------
	// 1. Create the photon map to capture in-direct lighitng
	//-------------------------------------------------------------------------
	_Create_photon_maps();
	m_theStopWatch.Show_last_interval("Photon map creation");

	int   x, y;
	CVec3 m_vLeftBottom, CurrPixel, m_vDeltaX, m_vDeltaY, CurrY;
	m_theCamera.ComputerRenderingParameters(m_vDeltaX, m_vDeltaY, m_vLeftBottom);

	//-------------------------------------------------------------------------
	// 2. First pass
	// So far the code has not considered the adaptive sampling, yet.
	//-------------------------------------------------------------------------
	m_bEstimateDiffuseIrra = TRUE;
	CCol4 cIrra, *pCurr;
	int    nOffset;
	int    nWidth  = m_theCamera.Film_width();
	int    nHeight = m_theCamera.Film_height();
	CCol4 *pImage  = m_theCamera.ResultImagePtr();
	printf ("\nStart MC ray tracing (%dx%d)\n", nWidth, nHeight);
	for (y = 0; y < nHeight; y ++)
	{
		CurrY   = m_vLeftBottom + m_vDeltaY * y;
		nOffset = nHeight - y - 1;
		nOffset = nOffset * nWidth;
		pCurr   = &pImage[nOffset];
		if ((y % 20) == 0)
			printf ("\nLine: %4d  %2d%%  ", y, y * 100 / nHeight);
		else printf (".");
		for (x = 0; x < nWidth; x ++, pCurr ++)
		{
			CurrPixel = CurrY + m_vDeltaX * x;
			*pCurr = _Pixel_radiance_PM (CurrPixel, cIrra);
		}
	}

	m_theStopWatch.Show_total_time("Total rendering time: ");
}



///////////////////////////////////////////////////////////////////////////////
//		_Double_image_resolution
// Double the resolution of the ray-tracing result.
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::_Double_image_resolution (CCol4 **cpLow, CCol4 **cpLowIrra,
										 CCol4 **cpHig, CCol4 **cpHigIrra,
										 int &nWid, int &nHei,
										 CVec3 vDeltaX, CVec3 vDeltaY,
										 float fEsp)
{
	int x, y;
	CVec3 CurrY, CurrPixel;
	CCol4 cMean, cIMean;

	int nHigWid = nWid + nWid - 1;
	int nHigHei = nHei + nHei - 1;

	// ------------------------------------------------------------------------
	//        @ - o - @ - o - @ - o - @ - o - @ - o - @ - o - @
	//        |   |   |   |   |   |   |   |   |   |   |   |   |
	//        o - * - o - * - o - * - o - * - o - * = o - * - o
	//        |   |   |   |   |   |   |   |   |   |   |   |   |
	//        @ - o - @ - o - @ - o - @ - o - @ - o - @ - o - @
	//        |   |   |   |   |   |   |   |   |   |   |   |   |
	//        o - * - o - * - o - * - o - * - o - * = o - * - o
	//        |   |   |   |   |   |   |   |   |   |   |   |   |
	//        @ - o - @ - o - @ - o - @ - o - @ - o - @ - o - @
	//
	//  @  CORNER pixels
	//  o  EDGE pixles
	//  *  CENTER pixels
	// ------------------------------------------------------------------------

	// Copy the sampled CORNER pixels from the lower resolution result
	// Then delete the lower buffer
	for (y = 0; y < nHigHei; y += 2)
	{
		for (x = 0; x < nHigWid; x += 2)
		{
			cpHig[y][x] = cpLow[y>>1][x>>1];
			cpHigIrra[y][x] = cpLowIrra[y>>1][x>>1];
		}
		MY_DELETE_ARRAY (cpLow[y>>1]);
		MY_DELETE_ARRAY (cpLowIrra[y>>1]);
	}
	MY_DELETE_ARRAY (cpLow);
	MY_DELETE_ARRAY (cpLowIrra);

	// Check and sample the CENTER pixels
	printf ("\n1. Center pixels");
	float fEsp2 = fEsp * 2;
	CCol4 cEsp;
	for (y = 1; y < nHigHei-1; y +=2)
	{
		CurrY = m_vLeftBottom + vDeltaY * y;
		for (x = 1; x < nHigWid-1; x +=2)
		{
			cMean = (cpHig[y-1][x-1] + cpHig[y-1][x+1] + 
				cpHig[y+1][x-1] + cpHig[y+1][x+1]) * 0.25f;
			cIMean = (cpHigIrra[y-1][x-1] + cpHigIrra[y-1][x+1] + 
				cpHigIrra[y+1][x-1] + cpHigIrra[y+1][x+1]) * 0.25f;

			// If the radiances of the pixel's neighbor are similar, set the
			// radiance as the mean of its neighbor,
			cEsp = cMean*fEsp;
			if (_Similar_color (cpHig[y-1][x-1], cMean, cEsp) &&
				_Similar_color (cpHig[y-1][x+1], cMean, cEsp) &&
				_Similar_color (cpHig[y+1][x-1], cMean, cEsp) &&
				_Similar_color (cpHig[y+1][x+1], cMean, cEsp))
			{
				cpHig[y][x] = cMean;
				cpHigIrra[y][x] = cIMean;
			}
			// if not so, estimate the radiance by ray-tracing
			else
			{
				CurrPixel = CurrY + vDeltaX * x;

				// If the diffuse irradiances of the pixel's neighbor are
				// similar, set the irradiance as the mean of its neighbor
				cEsp = cMean*fEsp2;
				if (_Similar_color (cpHigIrra[y-1][x-1], cIMean, cEsp) &&
					_Similar_color (cpHigIrra[y-1][x+1], cIMean, cEsp) &&
					_Similar_color (cpHigIrra[y+1][x-1], cIMean, cEsp) &&
					_Similar_color (cpHigIrra[y+1][x+1], cIMean, cEsp))
				{
					cpHigIrra[y][x] = cIMean;
					m_bEstimateDiffuseIrra = FALSE;
				}
				// if not so, estimate the diffuse irradiance by MC
				else m_bEstimateDiffuseIrra = TRUE;

				cpHig[y][x] = _Pixel_radiance_PM (CurrPixel, cpHigIrra[y][x]);
			}
		}
		if ((y % 20) == 1)
			printf ("\nLine: %4d  %2d%%  ", y, y * 100 / (nHigHei - 1));
		else printf (".");
	}

	// Check and sample the EDGE pixels
	printf ("\n2. Edge pixels");
	for (y = 0; y < nHigHei; y ++)
	{
		CurrY = m_vLeftBottom + vDeltaY * y;
		for (x = !(y & 0x01); x < nHigWid; x +=2)
		{
			if ((x == 0) || (x == nHigWid - 1) ||
				(y == 0) || (y == nHigHei - 1))
			{
				m_bEstimateDiffuseIrra = TRUE;
				CurrPixel = CurrY + vDeltaX * x;
				cpHig[y][x] = _Pixel_radiance_PM (CurrPixel, cpHigIrra[y][x]);
				continue;
			}

			cMean = (cpHig[y][x-1] + cpHig[y][x+1] + 
				cpHig[y+1][x] + cpHig[y-1][x]) * 0.25f;
			cIMean = (cpHigIrra[y][x-1] + cpHigIrra[y][x+1] + 
				cpHigIrra[y+1][x] + cpHigIrra[y-1][x]) * 0.25f;

			// If the radiances of the pixel's neighbor are similar, set the
			// radiance as the mean of its neighbor,
			cEsp = cMean*fEsp;
			if (_Similar_color (cpHig[y  ][x-1], cMean, cEsp) &&
				_Similar_color (cpHig[y  ][x+1], cMean, cEsp) &&
				_Similar_color (cpHig[y-1][x  ], cMean, cEsp) &&
				_Similar_color (cpHig[y+1][x  ], cMean, cEsp))
			{
				cpHig[y][x] = cMean;
				cpHigIrra[y][x] = cIMean;
			}
			// if not so, estimate the radiance by ray-tracing
			else
			{
				CurrPixel = CurrY + vDeltaX * x;

				// If the diffuse irradiances of the pixel's neighbor are
				// similar, set the irradiance as the mean of its neighbor
				cEsp = cMean*fEsp2;
				if (_Similar_color (cpHigIrra[y  ][x-1], cIMean, cEsp) &&
					_Similar_color (cpHigIrra[y  ][x+1], cIMean, cEsp) &&
					_Similar_color (cpHigIrra[y-1][x  ], cIMean, cEsp) &&
					_Similar_color (cpHigIrra[y+1][x  ], cIMean, cEsp))
				{
					cpHigIrra[y][x] = cIMean;
					m_bEstimateDiffuseIrra = FALSE;
				}
				// if not so, estimate the diffuse irradiance by MC
				else m_bEstimateDiffuseIrra = TRUE;

				cpHig[y][x] = _Pixel_radiance_PM (CurrPixel, cpHigIrra[y][x]);
			}
		}
		if ((y % 20) == 0)
			printf ("\nLine: %4d  %2d%%  ", y, y * 100 / (nHigHei - 1));
		else printf (".");
	}

	nWid = nHigWid;
	nHei = nHigHei;
}



///////////////////////////////////////////////////////////////////////////////
//		MCPM_ray_tracing_multi_pass
// Multi-pass Monte Carlo ray tracing using global and caustic photon maps
///////////////////////////////////////////////////////////////////////////////
void CMcBspTR::MCPM_ray_tracing_multi_pass ()
{
	m_theStopWatch.Start("Starting multi pass MCPM ray tracing");

	//-------------------------------------------------------------------------
	// 1. Create the photon map to capture in-direct lighitng
	//-------------------------------------------------------------------------
	_Create_photon_maps();
	m_theStopWatch.Show_last_interval("Photon map creation");

	int   x, y;
	CVec3 CurrPixel, CurrY;
	m_theCamera.ComputerRenderingParameters(m_vDeltaX, m_vDeltaY, m_vLeftBottom);

	//-------------------------------------------------------------------------
	// First pass: 1/2 resolution.
	//-------------------------------------------------------------------------

	CCol4 cMean, cIMean;
	int nFinalWid = m_theCamera.Film_width();
	int nFinalHei = m_theCamera.Film_height();

	CCol4 *pCurr, *pImage;
	CCol4 **cpLow, **cpMid, **cpSup;
	CCol4 **cpLowIrra, **cpMidIrra, **cpSupIrra;

	int nWid = ((nFinalWid>>1) & 0x01)? (nFinalWid>>1)+2 : (nFinalWid>>1)+1;
	int nHei = ((nFinalHei>>1) & 0x01)? (nFinalHei>>1)+2 : (nFinalHei>>1)+1;
	
	cpLow = new CCol4 *[nHei];
	cpLowIrra = new CCol4 *[nHei];

	printf ("\nStart MC ray tracing ...\n \nPass 1: %dx%d\n", nWid, nHei);
	m_bEstimateDiffuseIrra = TRUE;
	for (y = 0; y < nHei; y ++)
	{
		cpLow[y] = new CCol4 [nWid];
		cpLowIrra[y] = new CCol4 [nWid];

		CurrY = m_vLeftBottom + m_vDeltaY * y * 2;
		for (x = 0; x < nWid; x ++)
		{
			CurrPixel = CurrY + m_vDeltaX * x * 2;
			cpLow[y][x] = _Pixel_radiance_PM (CurrPixel, cpLowIrra[y][x]);
		}

		if ((y % 20) == 0)
			printf ("\nLine: %4d  %2d%%  ", y, y * 100 / (nHei-1));
		else printf (".");
	}

	m_theStopWatch.Show_last_interval("First pass: ");

	//-------------------------------------------------------------------------
	// Second pass: normal resolution.
	//-------------------------------------------------------------------------
	cpMid = new CCol4 * [nHei+nHei-1];
	cpMidIrra = new CCol4 * [nHei+nHei-1];
	for (y = 0; y < nHei+nHei-1; y ++)
	{
		cpMid[y] = new CCol4 [nWid+nWid-1];
		cpMidIrra[y] = new CCol4 [nWid+nWid-1];
	}
	_Double_image_resolution (cpLow, cpLowIrra, cpMid, cpMidIrra, nWid, nHei,
		m_vDeltaX, m_vDeltaY, 0.05f);

	m_theStopWatch.Show_last_interval("Second pass: ");

	//-------------------------------------------------------------------------
	// Third pass: super resolution pass.
	//-------------------------------------------------------------------------
	cpSup = new CCol4 * [nHei+nHei-1];
	cpSupIrra = new CCol4 * [nHei+nHei-1];
	for (y = 0; y < nHei+nHei-1; y ++)
	{
		cpSup[y] = new CCol4 [nWid+nWid-1];
		cpSupIrra[y] = new CCol4 [nWid+nWid-1];
	}
	_Double_image_resolution (cpMid, cpMidIrra, cpSup, cpSupIrra, nWid, nHei,
		m_vDeltaX*0.5f, m_vDeltaY*0.5f, 0.2f);

	m_theStopWatch.Show_last_interval("Final pass: ");

	//-------------------------------------------------------------------------
	//		Store results
	// Store the super-resolution result into normal resolution buffer
	//-------------------------------------------------------------------------
	int nOffset;
	pImage = ResultImagePtr();
	for (y = 0; y < nFinalHei; y ++)
	{
		nOffset = nFinalHei - y - 1;
		nOffset = nOffset * nFinalWid;
		pCurr   = &pImage[nOffset];

		for (x = 0; x < nFinalWid; x ++, pCurr ++)
		{
			x <<= 1;
			y <<= 1;
            *pCurr = (
				 cpSup[y  ][x  ] + cpSup[y  ][x+2] +
				 cpSup[y+2][x  ] + cpSup[y+2][x+2] +
				(cpSup[y  ][x+1] + cpSup[y+1][x  ] +
				 cpSup[y+1][x+2] + cpSup[y+2][x+1]) * 2 +
				 cpSup[y+1][x+1] * 4) / 16;
			x >>= 1;
			y >>= 1;
		}
	}

	for (y = 0; y < nHei; y ++)
	{
		MY_DELETE_ARRAY(cpSup[y]);
		MY_DELETE_ARRAY(cpSupIrra[y]);
	}
	MY_DELETE_ARRAY(cpSup);
	MY_DELETE_ARRAY(cpSupIrra);

	m_theStopWatch.Show_total_time("Total rendering time: ");
}



///////////////////////////////////////////////////////////////////////////////
//		_Is_light_patch
// Check if the indexed patch is a light source
///////////////////////////////////////////////////////////////////////////////
BOOL CMcBspTR::_Is_light_patch(const int & nPID)
{
	CBspMaterial *pm = &m_pMaterials[m_pTriangles[nPID].GetMaterialID()];
	return (pm->m_fEmissive[3] > 0.1f);
}


bool CMcBspTR::Allocate_Mem_for_photnMap ()
{
	if (!m_bPhotonMapsInitialized)
	{
		_Delete_photon_maps();
		
	    if(m_nGPhotons<=0)
            return false;
		m_pGlobalPhotonMap = new CPhotonMap (m_nGPhotons * 4);
		m_pDirectPhotonMap = new CPhotonMap (m_nGPhotons);

		if ((m_pGlobalPhotonMap == NULL) || (m_pDirectPhotonMap == NULL))
        {
			printf ("\nInsufficient memory to store photons!\n");
            Singleton<ZHLog>::getSingletonPtr()->assertError("bool CMcBspTR::Allocate_Mem_for_photnMap () --failed");
            return false;
        }
	}
    
    return true;
}

void CMcBspTR::Set_indirect_lighting_ray_number (const int &nRay)
{
	m_nDistrRayNum = (nRay >= 16)? nRay : 0;
}
void CMcBspTR::SetLight(const BOOL bDirectional, const CVec3 vLigDir, const CCol4 color)
{
	m_bDirectionalLight = bDirectional;
	m_vLightDir = vLigDir.Normalized();
	m_cLightColor = color;

	CVec3 m_vTopMax = m_vMax;
	CVec3 m_vTopMin = m_vMin;

	float t = (m_vMax.z - m_vMin.z) / m_vLightDir.z;
	float fOffsetX = m_vLightDir.x * t;
	float fOffsetY = m_vLightDir.y * t;

	(fOffsetX > 0) ? m_vTopMax.x += fOffsetX : m_vTopMin.x += fOffsetX;
	(fOffsetY > 0) ? m_vTopMax.y += fOffsetY : m_vTopMin.y += fOffsetY;
}
