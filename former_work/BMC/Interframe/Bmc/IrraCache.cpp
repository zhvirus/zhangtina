#include "StdAfx.h"
#include ".\irracache.h"

float CIrraTree::m_fTolerance;

///////////////////////////////////////////////////////////////////////////////
BYTE CIrraTree::_Within_node(const CVec3& vPos)
{
	// ======================================================
	//                  Octree ID
	//        +---+---+          +---+---+       y
	//        | 5 | 1 |          | 4 | 0 |       |
	// front  +---+---+    back  +---+---+       |____x
	//        | 7 | 3 |          | 6 | 2 |        \
	//        +---+---+          +---+---+         z
	// ======================================================

	BYTE nID = 0x00;
	if (vPos[0] < m_vCenter[0]) nID |= 0x04;
	if (vPos[1] < m_vCenter[1]) nID |= 0x02;
	if (vPos[2] < m_vCenter[2]) nID |= 0x01;

	return nID;
}


///////////////////////////////////////////////////////////////////////////////
float CIrraTree::_Local_gathering(const CVec3& vPos, const CVec3& vNml,
								   vector<CCol4>& aI, const int &nDepth,
								   const BOOL bGradient)
{
	UINT i;
	float fWeightSum = 0.f;

	for (i = 0; i < m_aIv.size(); i++)
	{
//		if (m_aIv[i].m_bDepth <= nDepth)
//		{
			float fWeight = min (m_aIv[i].Weight(vPos, vNml), 1.0e8f);

			if (fWeight > 1.f / m_fTolerance)
			{
				CCol4 cIrra;

				if (bGradient) 
				{
					CVec3 vdx = vPos - m_aIv[i].m_vPos;
					CVec3 vcx = m_aIv[i].m_vNml.Cross(vNml);

					cIrra.r() = fWeight * (m_aIv[i].m_cEnergy[0] + 
						vdx.Dot(m_aIv[i].m_tGraP._vR) + vcx.Dot(m_aIv[i].m_tGraR._vR));
					cIrra.g() = fWeight * (m_aIv[i].m_cEnergy[1] + 
						vdx.Dot(m_aIv[i].m_tGraP._vG) + vcx.Dot(m_aIv[i].m_tGraR._vG));
					cIrra.b() = fWeight * (m_aIv[i].m_cEnergy[2] + 
						vdx.Dot(m_aIv[i].m_tGraP._vB) + vcx.Dot(m_aIv[i].m_tGraR._vB));
					cIrra.a() = 1.f;
				}
				else cIrra = m_aIv[i].m_cEnergy * fWeight;

				aI.push_back (cIrra);
				fWeightSum += fWeight;
			}
//		}
	}


	for (i = 0; i < 8; i++)
	{
		if ((m_pChild[i] != NULL) &&
			(m_pChild[i]->m_vCenter[0] - m_fSideLen <= vPos[0]  && 
			 m_pChild[i]->m_vCenter[0] + m_fSideLen >= vPos[0]) &&
			(m_pChild[i]->m_vCenter[1] - m_fSideLen <= vPos[1]  && 
			 m_pChild[i]->m_vCenter[1] + m_fSideLen >= vPos[1]) &&
			(m_pChild[i]->m_vCenter[2] - m_fSideLen <= vPos[2]  && 
			 m_pChild[i]->m_vCenter[2] + m_fSideLen >= vPos[2]))
		{
			fWeightSum += m_pChild[i]->_Local_gathering(vPos, vNml, aI, nDepth, bGradient);
		}
	}

	return fWeightSum;
}

////////////////////////////////////////////////////////////////////////////////
bool CIrraTree::saveTreeToFile(char* fileName)
{
    if(fileName==NULL)
    {
        printf("CIrraTree::saveTreeToFile....fileName cannot be NULL...Error!\n");
        return false;
    }
    //save current node
    std::ofstream ooFile(fileName,std::ios::app|std::ios::binary);
    if(ooFile.fail())
    {
        printf("Open File %s  failed!\n",fileName);
        return false;
    }

    ooFile.write((char*)(&(m_fSideLen)), sizeof(float));
    ooFile.write((char*)(&m_vCenter),sizeof(CVector3f));
    int t_vecSize =  static_cast<int>(m_aIv.size());
    ooFile.write((char*)(&t_vecSize),sizeof(int));
    
    int CIrraValue_size = sizeof(CVector3f) * 2 + sizeof(CCol4) +sizeof(float) +sizeof(BYTE) + sizeof(TIrraGrad) * 2;
    for(int i=0;i<t_vecSize;i++)
    {
       ooFile.write((char*)(&(m_aIv[i].m_vPos)),CIrraValue_size);
    }
    ooFile.write((char*)(&m_bNodeDepth),sizeof(BYTE));
    ooFile.write((char*)(&m_fTolerance),sizeof(float));  
    ooFile.write((char*)(&m_nNodeNumber),sizeof(int) * 2);
    ooFile.write((char*)(&m_pChild[0]),sizeof(CIrraTree*) * 8);

    ooFile.close();
    //write child node
    for(int i=0;i<8;i++)
    {
        if(m_pChild[i]!=NULL)
            m_pChild[i]->saveTreeToFile(fileName);

    }


    return true;


}



bool CIrraTree::buildTreeFromFile(char* fileName)
{
    if(fileName ==NULL)
    {
        printf("CIrraTree::buildTreeFromFile....fileName cannot be NULL...Error!\n");
        return false;
    }
    //save current node
    std::ifstream iiFile(fileName,std::ios::binary);
    if(iiFile.fail())
    {
        printf("Open File %s  failed!\n",fileName);
        return false;
    }

    _buildTree(this,iiFile);
    
    iiFile.close();
    return true;
}


void CIrraTree::_buildTree(CIrraTree* _root,std::ifstream& iiFile)
{
    if(_root==NULL)
    {
        printf("_root==NULL\n");
        return;
    }
    //build current node
    iiFile.read((char*)(&(_root->m_fSideLen)),sizeof(float)+sizeof(CVector3f));
    int t_nVIrra;
    iiFile.read((char*)(&t_nVIrra),sizeof(int));
    if(t_nVIrra<0)
    {
        printf("CIrraTree::_buildTree...t_nVIrra <0\n");
        return;
    }

    CIrraValue t_irraValue;
    int CIrraValue_size = sizeof(CVector3f) * 2 + sizeof(CCol4) +sizeof(float) +sizeof(BYTE) + sizeof(TIrraGrad) * 2;
    for(int i=0;i<t_nVIrra;i++)
    {
        iiFile.read((char*)(&(t_irraValue.m_vPos)),CIrraValue_size);
        _root->m_aIv.push_back(t_irraValue);
    }

    iiFile.read((char*)(&(_root->m_bNodeDepth)),sizeof(BYTE));
    iiFile.read((char*)(&(_root->m_fTolerance)),sizeof(float));
    iiFile.read((char*)(&(_root->m_nNodeNumber)),sizeof(int) * 2);
    iiFile.read((char*)(&(_root->m_pChild[0])),sizeof(CIrraTree*) * 8);

    for(int i=0;i<8;i++)
    {
        if(_root->m_pChild[i]!=0)
        {
            CIrraTree* _child   =  new CIrraTree;
            _root->m_pChild[i]    = _child;
            _buildTree(_child,iiFile);
        }

    }


}


///////////////////////////////////////////////////////////////////////////////
BOOL CIrraTree::Get_irradiance(CCol4 &cIrra, const CVec3& vPos, const CVec3& vNml,
								const int &nSearchDepth, const BOOL bGradient)
{
	cIrra = COLOR_BLACK;
	vector<CCol4> aI;
	float fWeightSum = _Local_gathering(vPos ,vNml, aI, nSearchDepth, bGradient);

	if (static_cast<int>(aI.size()) > 0)
	{
		for (UINT i = 0; i < aI.size(); i ++)
			cIrra += aI[i];
		cIrra /= fWeightSum;
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void  CIrraTree::Insert (CIrraValue iv,CIrraTree* _root)
{

	if ((m_fSideLen < 2 * iv.m_fRadius * m_fTolerance) ||
		(m_bNodeDepth == _OT_MAX_DEPTH_))
	{
		m_aIv.push_back(iv);
        (_root->m_nIrraValueNumber)++;
	}
	else	// subdivide, and test which subcube it is in
	{
		BYTE ID = _Within_node (iv.m_vPos);

		if (m_pChild[ID] == NULL)
		{
			float sl    = 0.5f * m_fSideLen;
			CVec3 vCent = m_vCenter;
			switch(ID)
			{
				case 0x00: vCent += CVec3 ( sl,  sl,  sl); break;
				case 0x01: vCent += CVec3 ( sl,  sl, -sl); break;
				case 0x02: vCent += CVec3 ( sl, -sl,  sl); break; 
				case 0x03: vCent += CVec3 ( sl, -sl, -sl); break;
				case 0x04: vCent += CVec3 (-sl,  sl,  sl); break;
				case 0x05: vCent += CVec3 (-sl,  sl, -sl); break;
				case 0x06: vCent += CVec3 (-sl, -sl,  sl); break;
				case 0x07: vCent += CVec3 (-sl, -sl, -sl);
			}
			m_pChild[ID] = new CIrraTree (sl, vCent);
            (_root->m_nNodeNumber)++;
			m_pChild[ID]->m_bNodeDepth = m_bNodeDepth + 1;
		}

		m_pChild[ID]->Insert(iv,_root);
	}
}

BOOL  CIrraTree::Need_computation (const CVec3& vPos, const CVec3& vNml)
{
	for (UINT i = 0; i < m_aIv.size(); i++)
	{
        //use two normals and two positions  to calculate a weight. More  similar, more big weight 
		float fTemp = m_aIv[i].Weight(vPos, vNml);
		float fWeight = min (fTemp, 1.0e8f);
		//if (fWeight > 1.f / m_fTolerance)	return FALSE;
		if (fWeight > 1.5f / m_fTolerance)	return FALSE;
	}

	BOOL bNeed = TRUE;
    float t_r   =  m_fSideLen;
	for (UINT i = 0; i < 8; i++)
	{
		if ((m_pChild[i] != NULL) &&
			(m_pChild[i]->m_vCenter[0] - t_r <= vPos[0]  && 
			 m_pChild[i]->m_vCenter[0] + t_r >= vPos[0]) &&
			(m_pChild[i]->m_vCenter[1] - t_r <= vPos[1]  && 
			 m_pChild[i]->m_vCenter[1] + t_r >= vPos[1]) &&
			(m_pChild[i]->m_vCenter[2] - t_r <= vPos[2]  && 
			 m_pChild[i]->m_vCenter[2] + t_r >= vPos[2]))
		{
			if (!m_pChild[i]->Need_computation (vPos, vNml))
				bNeed = FALSE;
		}
	}

	return bNeed;
}
