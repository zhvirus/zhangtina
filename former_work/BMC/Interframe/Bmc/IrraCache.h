#pragma once

#include "common.h"

/*!
	Stores the position, normal, and energy at a point to be stored in the
	irradiance cache. Description of this algorithm can be found in
	"A Ray Tracing Solution for Diffuse Interreflection" by Greg Ward
	http://radsite.lbl.gov/radiance/papers/sg88/paper.html.
 */

// The irradiance gradient structure
struct TIrraGrad
{
	CVec3 _vR;	// Red channel gradient
	CVec3 _vG;	// Green channel gradient
	CVec3 _vB;	// Blue channel gradient
};

class CIrraValue
{
public:
	// Attributes
	CVec3		m_vPos;		// Position
	CVec3		m_vNml;		// Normal
	CCol4		m_cEnergy;	// The irradiance intensity
	float		m_fRadius;	// The average distance of the samples
	BYTE		m_bDepth;
	TIrraGrad	m_tGraP;	// The positional gradient
	TIrraGrad	m_tGraR;	// The orientational gradient

	// Operations
	float Weight(const CVec3& vPos, const CVec3& vNml)
	{
		float fTemp = 1.0f - m_vNml.Dot(vNml);
		fTemp = (fTemp < 0.f) ? 0.f : sqrt (fTemp);
		return 1.f / ((vPos-m_vPos).Length()/m_fRadius + fTemp);
	}

	float exclude(const CVec3& vPos, const CVec3& vNml)
	{
		return 0.5f * (vPos - m_vPos).Dot (vNml + m_vNml);
	}

	// Constructor / Destructor
	CIrraValue (const CVec3 &vPos = CVec3(0.f, 0.f, 0.f),
				const CVec3 &vNml = CVec3(0.f, 0.f, 0.f),
				const CCol4 &cEng = COLOR_BLACK,
				const float fRad  = 0.f,
				const BYTE  bDep  = 0)
	{
		m_vPos    = vPos;
		m_vNml    = vNml;
		m_cEnergy = cEng;

		m_fRadius = fRad;
		m_bDepth  = bDep;
	}

	CIrraValue (const CIrraValue &iv)
	{
		m_vPos    = iv.m_vPos;
		m_vNml    = iv.m_vNml;
		m_cEnergy = iv.m_cEnergy;

		m_fRadius = iv.m_fRadius;
		m_bDepth  = iv.m_bDepth;

		m_tGraP._vR = iv.m_tGraP._vR;
		m_tGraP._vG = iv.m_tGraP._vG;
		m_tGraP._vB = iv.m_tGraP._vB;
		m_tGraR._vR = iv.m_tGraR._vR;
		m_tGraR._vG = iv.m_tGraR._vG;
		m_tGraR._vB = iv.m_tGraR._vB;
	}

	virtual ~CIrraValue()
	{
	}
};


//Octree class for irradiance cacheing
//	Store the computed irradiance value in an octree
//	Get irradiance by weighted interpolation. If no precomputed irradiance can
//	be used, the irradiance need to be computed by Monte Carlo or other method
//	And stored into the octree for further use.
#define _OT_MAX_DEPTH_	7

using namespace std;

class CIrraTree
{
public:
	// Attributes
	float m_fSideLen;				// actually half the side length. (ie. from center to edge)
	CVec3 m_vCenter;				// Center position of the node
	CIrraTree* m_pChild[8];			// pointers to the children of the node
	vector<CIrraValue> m_aIv;	// The pre-computed irradiance list
	BYTE m_bNodeDepth;
	static float m_fTolerance;		// Scene tolerance
    int m_nNodeNumber;
    int m_nIrraValueNumber;

private:    
    // Operations
	BYTE  _Within_node(const CVec3& vPos);
	float _Local_gathering (const CVec3& vPos, const CVec3& vNml, vector<CCol4>& aI, const int &nSearchDepth, const BOOL bGradient);
    void _buildTree(CIrraTree* _root,std::ifstream& iiFile);


public:
    inline int getNodeNumber()const {return m_nNodeNumber;}
    inline int getIrraValueNumber()const{return m_nIrraValueNumber;}
	void  Set_tolerance (float fTolerance)        {m_fTolerance = fTolerance;}
	BOOL  Get_irradiance (CCol4 &cIrra, const CVec3& vPos, const CVec3& vNml, const int &nSearchDepth = 0, const BOOL bGradient = FALSE);
	BOOL  Need_computation (const CVec3& vPos, const CVec3& vNml);
	void  Insert (CIrraValue iv,CIrraTree* _root);
    bool saveTreeToFile(char* fileName);
    bool buildTreeFromFile(char* fileName);

    CIrraTree(float fSideLen = 500000.f, CVec3 vCenter = CVec3(0.f, 0.f, 0.f), BYTE bDepth = 0):
    m_nNodeNumber(1),
    m_nIrraValueNumber(1)
	{
		m_fSideLen   = fSideLen;
		m_vCenter    = vCenter;
		m_bNodeDepth = bDepth;
		for (int i = 0; i < 8; i ++) 
            m_pChild[i] = NULL;
	}

	virtual ~CIrraTree(void)
	{
		for (int i = 0; i < 8; i ++) 
			MY_DELETE (m_pChild[i]);
	}
};
