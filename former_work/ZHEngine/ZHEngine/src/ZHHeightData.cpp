#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHHeightData.h"

namespace ZH
{
	HeightData::HeightData():
		m_minSize(0),
		m_maxSize(0),
		m_totalLevels(0),
		m_stackTexNum(0),
		m_stackTexSize(0)
	{

	}

	HeightData::~HeightData()
	{
		std::vector<DATABLOCK*>::iterator ite;
		for(ite=m_dataSource.begin();ite!=m_dataSource.end();ite++)
		{
			SAFE_DELETE_ARRAY((*ite)->_data);
			SAFE_DELETE((*ite));
		}

		m_dataSource.clear();

		for(ite=m_dataCache.begin();ite!=m_dataCache.end();ite++)
		{
			SAFE_DELETE_ARRAY((*ite)->_data);
			SAFE_DELETE((*ite));
		}

		m_dataCache.clear();

	}

	bool HeightData::initialize(int startSize, int levels,float camX,float camZ,int stackTexSize)
	{
		m_stackTexNum	=	levels -1;
		m_stackTexSize	=	stackTexSize;
		m_totalLevels	=	levels;
		m_minSize	=	startSize;
		m_maxSize	=	startSize*static_cast<int>(ZH::Math::Pow(2.0f,static_cast<float>(levels-1)));

		if (m_stackTexSize<0)
			return false;	

		for(int i=0;i<m_stackTexNum;i++)
		{
			DATABLOCK* _tempBlock	=	new DATABLOCK;

			if(_tempBlock==NULL) 
				return false;

			_tempBlock->_data	=	new float[m_stackTexSize*m_stackTexSize];

			if(_tempBlock->_data==NULL)
				return false;
			_tempBlock->_dataCount	=	m_stackTexSize*m_stackTexSize;
			_tempBlock->_blockSize	=	m_stackTexSize;

			m_dataCache.push_back(_tempBlock);
		}

		if(!buildingPyramidData(startSize,levels)||!updateStackData(camX,camZ))
		{
			return false;
		}

		//========================
		//To check if the data is correct.
		//========================
		//for(int uu=0;uu<4;uu++)
		//{
		//	float *pp	=	m_dataCache[uu]->_data;
		//	int countcc	=	m_dataCache[uu]->_dataCount;

		//	for(int kk=0;kk<countcc;kk++)
		//	{
		//		if(*(pp+kk)>230||*(pp+kk)<0)
		//		{
		//			std::cout<<"["<<uu<<"]: "<<"("<<kk<<")"<<*(pp+kk)<<std::endl;
		//		}
		//	}

		//}
		//========================
		//========================

		return true;
	}

	bool HeightData::buildingPyramidData(int startSize,int levels)
	{
		for(int S=startSize,i=0; i<levels;i++,S*=2)
		{
			DATABLOCK* _data	=	NULL;
			if(!readDataFromFile(S,&_data))
				return false;
			if(_data==NULL)
				return false;
			m_dataSource.push_back(_data);
		}

		return true;
	}

	bool HeightData::readDataFromFile(int size,DATABLOCK** dataRev)
	{
		char fileName[MAX_PATH];
		switch(size)
		{		
		case 32:strcpy(fileName,"res\\dataSource32.zhx");break;
		case 64:strcpy(fileName,"res\\dataSource64.zhx");break;
		case 128:strcpy(fileName,"res\\dataSource128.zhx");break;
		case 256:strcpy(fileName,"res\\dataSource256.zhx");break;
		case 512:strcpy(fileName,"res\\dataSource512.zhx");break;
		case 1024:strcpy(fileName,"res\\dataSource1024.zhx");break;
		case 2048:strcpy(fileName,"res\\dataSource2048.zhx");break;
		case 4096:strcpy(fileName,"res\\dataSource4096.zhx");break;
		case 8192:strcpy(fileName,"res\\dataSource8192.zhx");break;
		case 16384:strcpy(fileName,"res\\dataSource16384.zhx");break;
		default:
			{
				return false;
			}
		};

		std::ifstream inFile(fileName,std::ios::binary);
		if(inFile.fail())
			return false;

		float *_temp    =  new float[size*size];
		if(_temp==NULL)
			return false;

		inFile.read((char*)(_temp),size*size*sizeof(float));

		*dataRev	=	new DATABLOCK;
		if(dataRev==NULL)
		{
			SAFE_DELETE_ARRAY(_temp);
			return false;
		}

		(*dataRev)->_data	=	_temp;
		(*dataRev)->_dataCount	=	size*size;
		(*dataRev)->_blockSize	=	size;

		return true;

	}

	bool HeightData::updateStackData(float camX,float camZ)
	{
		float _localBiasX = camX;
		float _localBiasZ = camZ;

		for(int i=0;i<m_stackTexNum;i++)
		{
			if(!fetchDataFromPyramid(m_dataCache[i],m_stackTexSize,m_dataSource[m_totalLevels-1-i],m_dataSource[m_totalLevels-1-i]->_blockSize,_localBiasX,_localBiasZ))
			{
				return false;
			}

			_localBiasX/=2;
			_localBiasZ/=2;
		}

		float* fine = NULL;
		float* cors = NULL;
		int t_dataNumber	=	m_stackTexSize*m_stackTexSize;
		
		for(int i=0;i<m_stackTexNum-1;i++)
		{
			fine = m_dataCache[i]->_data;
			cors = m_dataCache[i+1]->_data;

			for(int s = 0;s<t_dataNumber;s++)
			{
				*(fine+s) =  static_cast<int>(*(fine+s))+static_cast<float>(static_cast<int>(*(cors+s))/10000.0f);

			}			

		}



		return true;
	}

	bool HeightData::fetchDataFromPyramid(DATABLOCK* _revData, int revSize, DATABLOCK* _fromData, int frmSize, float biasU, float biasV )
	{
		//接收数据块大小
		int texSize	=	revSize;
		int fromTexSize	=	frmSize;
		float curU = 0;
		float curV = 0;
		float halfRev	=	(texSize-1)/2.0f;
		float halfFrm	=	(fromTexSize-1)/2.0f;
		int index =0;

		//linear- interplation
		int C0,R0;
		float d1,d2,d3,d4,d5,d6;

		for(int i=0;i<texSize;i++)
			for(int j=0;j<texSize;j++)
			{

				index  = i*texSize+j;
				
				curU = j - halfRev  + biasU + halfFrm;
				curV = i - halfRev  + biasV + halfFrm;
				//在高度数据区域
				if( curU>=0 && curU<(fromTexSize-1)&&
					curV>=0 && curV<(fromTexSize-1)
					)
				{
					
					C0		=	static_cast<int>(curU);
					R0		=	static_cast<int>(curV);
					d1		=	_fromData->_data[R0*fromTexSize+ C0];
					d2		=	_fromData->_data[R0*fromTexSize+ C0+1];
					d3		=	_fromData->_data[(R0+1)*fromTexSize+ C0];
					d4		=	_fromData->_data[(R0+1)*fromTexSize+ C0+1];
					d5		=	d1+(d3-d1)*(curV-R0);
					d6		=	d2+(d4-d2)*(curV-R0);


					*(_revData->_data+index)  =  floor(d5+(d6-d5)*(curU-C0));
					//==========
					//if(*(_revData->_data+index)<0)
					//{
					//	std::cout<<*(_revData->_data+index)<<std::endl;
					//}
					//==========

				}
				else
					//在区域外
				{
					*(_revData->_data+index)  =  0.0f;

				}

			}

		return true;
	}



}