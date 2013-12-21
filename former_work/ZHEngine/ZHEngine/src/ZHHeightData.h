#pragma once

#include "ZHRenderDecl.h"

namespace ZH
{
	class ZH_DLL HeightData
	{
	public:
		struct DATABLOCK
		{
			float* _data;
			int _dataCount;
			int _blockSize;
		};

		HeightData();
		~HeightData();
		bool initialize(int startSize,int levels,float camX,float camZ,int stackTexSize);
		bool updateStackData(float camX,float camZ);
	private:
		bool buildingPyramidData(int startSize,int levels);

		bool readDataFromFile(int size,DATABLOCK** dataRev);
		bool fetchDataFromPyramid(DATABLOCK* _revData, int revSize,DATABLOCK* _fromData,int frmSize,float biasU, float biasV );
	public:	
		int m_minSize;
		int m_maxSize;
		int m_totalLevels;
		int m_stackTexSize;
		int m_stackTexNum;
		std::vector<DATABLOCK*> m_dataSource;
		std::vector<DATABLOCK*> m_dataCache;
		DATABLOCK* normalSource;
		//DATABLOCK* 

	};


}