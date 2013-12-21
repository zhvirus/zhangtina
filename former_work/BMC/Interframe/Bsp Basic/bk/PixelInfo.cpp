#include "stdafx.h"
#include "PixelInfo.h"
#include "Image.h"
#include "Singleton.h"
#include <fstream>

PixelInfo::PixelInfo(int width, int height):
	m_width(width),
	m_height(height),
	m_depth(NULL),
	m_totalPixels(0),
	m_materialID(NULL),
	m_normal(NULL),
	m_imageUtility(NULL)
{
	m_totalPixels	=	m_width * m_height;

	ASSERT(width>0&&height>0);
	m_depth	=	new float[m_totalPixels];
	ASSERT(m_depth);
	memset(m_depth,0,sizeof(float)*m_totalPixels);

	m_materialID	=	new int[m_totalPixels];
	ASSERT(m_materialID);
	memset(m_materialID,0,sizeof(float)*m_totalPixels);

	m_normal	=	new CVec3[m_totalPixels];
	ASSERT(m_normal);
	memset(m_normal, 0, sizeof(CVector3f)*m_totalPixels);
	
	m_imageUtility	=	Singleton<ImageUtility>::getSingletonPtr();

}


PixelInfo::~PixelInfo()
{
	MY_DELETE_ARRAY(m_depth);
	MY_DELETE_ARRAY(m_materialID);
	MY_DELETE_ARRAY(m_normal);
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

	ooFile.close();

	//MY_DELETE_ARRAY(normalBuf);
	
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
		MY_DELETE_ARRAY(m_normal);
		MY_DELETE_ARRAY(m_depth);
		MY_DELETE_ARRAY(m_materialID);
		
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
	}
    else
    {
        m_totalPixels = m_width * m_height;
        memset(m_normal,0,sizeof(CVector3f)* m_totalPixels);
        memset(m_depth, 0, sizeof(float) * m_totalPixels);
        memset(m_materialID, 0, sizeof(int) * m_totalPixels);
        m_width   =  widthHeight[0];
        m_height  =  widthHeight[1];
        m_totalPixels = m_width * m_height;
    }
	
	//read normals
	//float* normalBuf	=	new float[3 * m_totalPixels];
	//if(normalBuf==NULL)
		//return false;
	iiFile.read((char*)(m_normal),sizeof(CVector3f) * m_totalPixels);

	//for(int i	=	0;i<m_totalPixels;i++)
	//{
	//	m_normal[i].x	=	normalBuf[i*3];
	//	m_normal[i].y	=	normalBuf[i*3+1];
	//	m_normal[i].z	=	normalBuf[i*3+2];
	//}
	
	//MY_DELETE_ARRAY(normalBuf);

	//read depth
	iiFile.read((char*)(m_depth),sizeof(float)*m_totalPixels);
	
	//read materailIDs
	iiFile.read((char*)(m_materialID),sizeof(int)*m_totalPixels);

	return true;
}

bool PixelInfo::setPixelInfo(int offset, float depth, int materialID, const CVec3& normal)
{
   if(offset<0||offset>m_totalPixels-1)
		return false;
   m_depth[offset]	=	depth;
   m_materialID[offset]	=	materialID;
   m_normal[offset]	=	normal;

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
		memset(m_depth,0,sizeof(float)*m_totalPixels);
	}

	if(m_materialID)
	{
		memset(m_materialID,0,sizeof(float)*m_totalPixels);
	}
	
	if(m_normal)	
	{
		memset(m_normal, 0, sizeof(CVector3f)*m_totalPixels);
	}

	return true;
}


