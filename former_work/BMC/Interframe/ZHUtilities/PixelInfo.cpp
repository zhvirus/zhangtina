#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "PixelInfo.h"
#include "Image.h"
#include "Singleton.h"

namespace ZH
{


PixelInfo::PixelInfo(int width, int height):
	m_width(width),
	m_height(height),
	m_depth(NULL),
	m_totalPixels(0),
	m_materialID(NULL),
	m_normal(NULL),
    m_pos(NULL),
    m_tex(NULL),
    m_triangleID(NULL),
    m_samplerNumber(NULL),
	m_imageUtility(NULL)
{
	m_totalPixels	=	m_width * m_height;

	assert(width>0&&height>0);
	m_depth	=	new float[m_totalPixels];
	assert(m_depth);
	memset(m_depth,0,sizeof(float)*m_totalPixels);

	m_materialID	=	new int[m_totalPixels];
	assert(m_materialID);
	memset(m_materialID,0,sizeof(float)*m_totalPixels);

	m_normal	=	new CVec3[m_totalPixels];
	assert(m_normal);
	memset(m_normal, 0, sizeof(CVector3f)*m_totalPixels);

    m_samplerNumber = new int[m_totalPixels];
    assert(m_samplerNumber);
    memset((void*)m_samplerNumber, 0, sizeof(int) * m_totalPixels);

    m_pos  =  new CVec3[m_totalPixels];
    assert(m_pos);
    memset((void*)m_pos,0,sizeof(CVec3) * m_totalPixels);

    m_tex   =  new CVec3[m_totalPixels];
    assert(m_tex);
    memset((void*)m_tex,0,sizeof(CVec3) * m_totalPixels);

    m_triangleID =  new int[m_totalPixels];
    assert(m_triangleID);
    memset((void*)(m_triangleID),0,sizeof(int) * m_totalPixels);
	
	m_imageUtility	=	Singleton<ImageUtility>::getSingletonPtr();

}


PixelInfo::~PixelInfo()
{
	SAFE_DELETE_ARRAY(m_depth);
	SAFE_DELETE_ARRAY(m_materialID);
	SAFE_DELETE_ARRAY(m_normal);
    SAFE_DELETE_ARRAY(m_samplerNumber);
    SAFE_DELETE_ARRAY(m_pos);
    SAFE_DELETE_ARRAY(m_tex);
    SAFE_DELETE_ARRAY(m_triangleID);
}

bool PixelInfo::savePixelInfoToFile(char* fileName)
{
	if(fileName==NULL)
		return false;
	if(m_width<=0||m_height<=0||m_depth==NULL||m_materialID==NULL||m_normal==NULL)
		return false;

	std::ofstream ooFile(fileName,std::ios::binary);
	if(ooFile.fail())
		return false;

	
	//write width and height
	ooFile.write((char*)(&m_width),sizeof(int));
	ooFile.write((char*)(&m_height),sizeof(int));
	//write normals
	ooFile.write((char*)(m_normal),sizeof(CVec3) * m_width * m_height);
	//write depths
	ooFile.write((char*)(m_depth),sizeof(float) * m_width * m_height);
	//write materials
	ooFile.write((char*)(m_materialID),sizeof(int) * m_width * m_height);
    //write pos
    ooFile.write((char*)(m_pos),sizeof(CVec3) * m_width * m_height);
    //write tex
    ooFile.write((char*)(m_tex),sizeof(CVec3) * m_width * m_height);
    //write triangle ID
    ooFile.write((char*)(m_triangleID),sizeof(int) * m_width * m_height);
    //write sampler number
    ooFile.write((char*)(m_samplerNumber) ,sizeof(int) * m_width * m_height);
    
    //save left
    ooFile.write((char*)(&m_fIntensityScale),sizeof(float) * 10);
    ooFile.write((char*)(&m_bUseIrradianceCache),sizeof(bool) * 5);

    ooFile.write((char*)(&m_SunLight),sizeof(DWORD) * 2 + sizeof(float) * 8);


	ooFile.close();

	
	return true;
}

bool PixelInfo::loadPixelInfoFromFile(char* fileName)
{
	if(fileName==NULL)
		return false;

	std::ifstream iiFile(fileName,std::ios::binary);
	if(iiFile.fail())
		return false;

	//get width and height from file
	int widthHeight[2];
	iiFile.read((char*)(widthHeight), sizeof(int)*2);

	if((widthHeight[0] * widthHeight[1] > m_width * m_height)||m_normal==NULL||m_depth==NULL||m_materialID==NULL)
	{
		SAFE_DELETE_ARRAY(m_normal);
		SAFE_DELETE_ARRAY(m_depth);
		SAFE_DELETE_ARRAY(m_materialID);
        SAFE_DELETE_ARRAY(m_samplerNumber);
		
		m_width	=	widthHeight[0];
		m_height	=	widthHeight[1];
		m_totalPixels	=	m_width * m_height;

		m_normal	=	new CVec3[m_totalPixels];
		if(!m_normal)
			return false;
		
		m_depth	=	new float[m_totalPixels];
		if(!m_depth)
			return false;

		m_materialID	=	new int[m_totalPixels];
		if(!m_materialID)
			return false;

        m_pos  =  new CVec3[m_totalPixels];
        if(!m_pos)
            return false;

        m_tex   =  new CVec3[m_totalPixels];
        if(!m_tex)
            return false;

        m_triangleID =  new int[m_totalPixels];
        if(!m_triangleID)
            return false;

        m_samplerNumber = new int[m_totalPixels];
        if(!m_samplerNumber)
            return false;
	}
    else
    {
        m_totalPixels = m_width * m_height;
        memset(m_normal,0,sizeof(CVector3f)* m_totalPixels);
        memset(m_depth, 0, sizeof(float) * m_totalPixels);
        memset(m_materialID, 0, sizeof(int) * m_totalPixels);
        memset(m_pos,0,sizeof(CVec3) * m_totalPixels);
        memset(m_tex,0,sizeof(CVec3) * m_totalPixels);
        memset(m_triangleID, 0, sizeof(int) * m_totalPixels);

        memset((void*)m_samplerNumber,0 , sizeof(int) * m_totalPixels);
        m_width   =  widthHeight[0];
        m_height  =  widthHeight[1];
        m_totalPixels = m_width * m_height;
    }
	
	//read normals
	iiFile.read((char*)(m_normal),sizeof(CVector3f) * m_totalPixels);

	//read depth
	iiFile.read((char*)(m_depth),sizeof(float)*m_totalPixels);
	
	//read materailIDs
	iiFile.read((char*)(m_materialID),sizeof(int)*m_totalPixels);
    //read pos
    iiFile.read((char*)(m_pos), sizeof(CVec3) * m_totalPixels);
    //read tex
    iiFile.read((char*)(m_tex), sizeof(CVec3) * m_totalPixels);
    //read triangle id
    iiFile.read((char*)(m_triangleID), sizeof(int) * m_totalPixels);

    //read sampler number
    iiFile.read((char*)(m_samplerNumber), sizeof(int) * m_totalPixels);
    //read left
    iiFile.read((char*)(&m_fIntensityScale), sizeof(float) * 10);
    iiFile.read((char*)(&m_bUseIrradianceCache), sizeof(bool) * 5);
    iiFile.read((char*)(&m_SunLight), sizeof(DWORD) * 2 + sizeof(float) * 8);

	return true;
}

bool PixelInfo::setPixelInfo(
                             int offset, 
                             float depth,
                             int materialID,
                             const CVec3& normal,
                             const CVec3& pos,
                             const CVec3& tex,
                             int triangleID,
                            float _densityScale, 
                            int samplerNum )
{
   if(offset<0||offset>m_totalPixels-1)
		return false;
   m_depth[offset]	=	depth;
   m_materialID[offset]	=	materialID;
   m_normal[offset]	=	normal;
   m_pos[offset]         = pos;
   m_tex[offset]         =  tex;
   m_triangleID[offset]   =  triangleID;
   m_samplerNumber[offset]   = samplerNum;
   m_fIntensityScale =  _densityScale;
   //std::cout<<offset<<std::endl;
	return true;
}

bool PixelInfo::saveDepthToImageFile(char* fileName)
{
	return m_imageUtility->saveToFileFromFloat(fileName, m_depth, m_width, m_height);

}

bool PixelInfo::saveNormalToImageFile(char* fileName)
{	
	return m_imageUtility->saveToFileFromVec3f(fileName, m_normal, m_width, m_height);
}

bool PixelInfo::clearAllData()
{
	if(m_depth)
	{
		memset((void*)m_depth,0,sizeof(float)*m_totalPixels);
	}

	if(m_materialID)
	{
		memset((void*)m_materialID,0,sizeof(float)*m_totalPixels);
	}
	
	if(m_normal)	
	{
		memset((void*)m_normal, 0, sizeof(CVector3f)*m_totalPixels);
	}

    if(m_pos)
    {
        memset((void*)m_pos,0,sizeof(CVec3) * m_totalPixels);
    }

    if(m_tex)
    {
        memset((void*)m_tex, 0, sizeof(CVec3) * m_totalPixels);
    }

    if(m_triangleID)
    {
        memset((void*)m_triangleID,0, sizeof(int) * m_totalPixels);
    }
    
    
    if(m_samplerNumber)
    {
        memset((void*)m_samplerNumber, 0, sizeof(int) * m_totalPixels);
    }


	return true;
}

//========================================
//========================================
//class pixel information
PixelInformation::PixelInformation(int width, int height):
m_width(width),
m_height(height),
m_depth(NULL),
m_totalPixels(0),
m_materialID(NULL),
m_normal(NULL),
m_pos(NULL),
m_tex(NULL),
m_triangleID(NULL),
m_samplerNumber(NULL),
m_pPixelValue(NULL),
m_bDoneAntiAliasing(false)
{
    m_totalPixels = width * height;

    assert((width>0)&&(height>0));

    m_depth	=	new float[m_totalPixels];
    assert(m_depth);
    memset(m_depth,0,sizeof(float)*m_totalPixels);

    m_materialID	=	new int[m_totalPixels];
    assert(m_materialID);
    memset(m_materialID,0,sizeof(float)*m_totalPixels);

    m_normal	=	new CVec3[m_totalPixels];
    assert(m_normal);
    memset(m_normal, 0, sizeof(CVector3f)*m_totalPixels);

    m_samplerNumber = new int[m_totalPixels];
    assert(m_samplerNumber);
    memset((void*)m_samplerNumber, 0, sizeof(int) * m_totalPixels);

    m_pos  =  new CVec3[m_totalPixels];
    assert(m_pos);
    memset((void*)m_pos,0,sizeof(CVec3) * m_totalPixels);

    m_tex   =  new CVec3[m_totalPixels];
    assert(m_tex);
    memset((void*)m_tex,0,sizeof(CVec3) * m_totalPixels);

    m_triangleID =  new int[m_totalPixels];
    assert(m_triangleID);
    memset((void*)(m_triangleID),0,sizeof(int) * m_totalPixels);

    m_pPixelValue   =  new PixelValue[m_totalPixels];
    assert(m_pPixelValue);
    
}

PixelInformation::~PixelInformation()
{
    SAFE_DELETE_ARRAY(m_depth);
    SAFE_DELETE_ARRAY(m_materialID);
    SAFE_DELETE_ARRAY(m_normal);
    SAFE_DELETE_ARRAY(m_samplerNumber);
    SAFE_DELETE_ARRAY(m_pos);
    SAFE_DELETE_ARRAY(m_tex);
    SAFE_DELETE_ARRAY(m_triangleID);
    SAFE_DELETE_ARRAY(m_pPixelValue);
}

bool PixelInformation::setPixelInfo(
                                    int offset, 
                                    float depth, 
                                    int materialID, 
                                    const CVector3f &normal, 
                                    const CVector3f &pos, 
                                    const CVector3f &tex, 
                                    int triangleID, 
                                    int samplerNum)
{
    if(offset<0||offset>m_totalPixels-1)
        return false;
    int t_row   =  static_cast<int>(offset/m_width);
    int t_col    =  offset    -   t_row * m_width;
    m_pPixelValue[offset].m_nRow   = t_row;
    m_pPixelValue[offset].m_nCol     = t_col;
    m_pPixelValue[offset].m_nIndexNumber =  static_cast<int>(m_pPixelValue[offset].m_vIndex.size());
    m_pPixelValue[offset].m_vIndex[0].m_nNormalIndex     = offset;
    m_pPixelValue[offset].m_vIndex[0].m_nPositionIndex    = offset;
    m_pPixelValue[offset].m_vIndex[0].m_nTextureIndex     = offset;
    m_pPixelValue[offset].m_vIndex[0].m_nTriangleIDIndex = offset;
    m_pPixelValue[offset].m_vIndex[0].m_nDepthIndex        = offset;
    m_pPixelValue[offset].m_vIndex[0].m_nMaterialIndex      = offset;
    m_pPixelValue[offset].m_vIndex[0].m_nSamplerNumberIndex   =  offset;

    m_normal[offset]       = normal;
    m_pos[offset]            = pos;
    m_tex[offset]             =  tex;
    m_triangleID[offset]  =  triangleID;
    m_depth[offset]         =  depth;
    m_materialID[offset]    =  materialID;
    m_samplerNumber[offset]  = samplerNum;
    

    return true;
}

bool PixelInformation::searchForAntiAliasPoints()
{
    if(m_bDoneAntiAliasing==true)
        return false;


    
    return true;
}



}