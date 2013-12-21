#pragma once
#include "ZHDecl.h"
#include "Common.h"

namespace ZH
{



class ZH_DLL BilateralFilter
{

	typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);
	typedef unsigned *THREADID;

	struct _threadPrama
	{
		BilateralFilter* _obj;
		int startRow;
		int endRow;
	};

	public:
		BilateralFilter(int size);
		~BilateralFilter();
	public:
		bool setFilterData(
			CColor4f* _sourceImage,
			CColor4f* _resultImage,
			int _imageWidth,
			int _imageHeight,
			int _filterBoxSize,
			PixelInfo* _pixelInfo,
			float _torPixelPositionDistance,
			float _torColorDistance,
			float _torNormalDistance,
			float _torDepthDistance,
			float _torMaterialDistance,
			bool _usePixelDistance	=	true,
			bool _usePerceptionDistance = true,
			bool _useNormalDistance = true,
			bool _useDepthDistance = true,
			bool _usematerialDistance =true
			);

		bool startFiltering();
		inline int getFilterBoxSize()const{return m_iFilterSize;}
		inline CColor4f* getSourceImage()const{return m_pSourceImage;}
		inline CColor4f* getResultImage()const{return m_pResultImage;}
		inline CColor4f* getTempImage()const{return m_pTempImage;}
		inline int getImageWidth()const{return m_iImageWidth;}
		inline int getImageHeight()const{return m_iImageHeight;}
public:
	HANDLE m_mainThreadHandle;
	DWORD m_mainThreadID;		
	int m_nWorkingThread;		//working thread number
	HANDLE m_workingThreadHandles[4];
	DWORD m_workingThreadIDs[4];	
	_threadPrama m_threadParams[4];
private:
		bool _filterOnePixel(int _row, int _col);
		static DWORD WINAPI MainThreadFunc(LPVOID param);
		static DWORD WINAPI WorkingThread(LPVOID param);

	protected:

		int m_iFilterSize;
		int m_iImageWidth;
		int m_iImageHeight;
		CColor4f* m_pSourceImage; //passed in outside
		CColor4f* m_pResultImage;//passed in outside
		CColor4f* m_pTempImage;//for local calculation.
		int m_curTempImageSize;
		PixelInfo* m_pPixelInfo;
		float m_torPixelPositionDistance2;
		float m_torColorDistance2;
		float m_torNormalDistance2;
		float m_torDepthDistance2;
		float m_torMaterialDistance2;

		//filter weight component
		bool m_usePixelDistance;
		bool m_usePerceptionDistance;
		bool m_useNormalDistance;
		bool m_useDepthDistance;
		bool m_useMaterialDistance;

		float* G_ExpTable;//e^(-x)  x-[0,5]
		const int ExpTableSize;
        float* m_pixelDistTable;


};

}