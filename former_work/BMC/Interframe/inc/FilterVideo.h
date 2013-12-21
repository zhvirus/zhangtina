#pragma once
#include "ZHDecl.h"
#include "Common.h"
#include "ZHLock.h"

namespace ZH
{


    class ZH_DLL FilterVideo
    {	
        //Thread function definition
		typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);
		typedef unsigned *THREADID;
    
        struct _threadPrama
        {
            FilterVideo* _obj;
            int startRow;
            int endRow;
            int front;
            int back;
        };
    
    public:
        FilterVideo();
        ~FilterVideo();
    public:
        bool initialize(
            const char* pixelColorDataBaseFileName,
            const char* pixelGeoDataBaseFileName,
            const char* irradianceCacheFileName,
			const char* outputPath,
			int _workingThread,
            int _filterBoxSize,      //filter rectangle size in one frame
            int _filterFrameNumber, //number of frames for filter at one time
            float _torPixelPositionDistance,
            float _torColorDistance,
            float _torNormalDistance,
            float _torDepthDistance,
            float _torMaterialDistance,
            bool _usePixelDistance	=	true,
            bool _usePerceptionDistance = true,
            bool _useNormalDistance = true,
            bool _useDepthDistance = true,
            bool _usematerialDistance =true,
            bool _motionBlur=true,
            bool _needResampling=false,
            float _samplerThreshold=50.0f
            );


        bool startFiltering();
        void stopFiltering();
        inline int getFilterBoxSize()const{return m_iFilterSize;}
        inline int getFilterFrameNum()const{return m_iFilterFrame;}
        inline int getImageWidth()const{return m_iImageWidth;}
        inline int getImageHeight()const{return m_iImageHeight;}
        
        inline int getIndirectLightNumber()const{return m_nIndirectLightNumber;}
        inline int getPhotonNumber()const{return m_nPhotonNum;}
        inline bool getIfUseIrradianceCache()const{return m_bUseIrradianceCache;}
        inline float getSamplingThreshold()const{return m_fSamplerThreshold;}
        inline float getTolerance()const{return m_fTolerance;}
        const std::vector<std::string>& getIrraFileVec()const{return m_vIrraFileName;}
        const std::vector<std::string>& getGeoFileVec()const{return m_vGeoFileName;}        
        std::map<int,FrameForResample*>& getResamplePixels() {return m_vReSamplerSets;}
        bool searchForResamplePixels();
    private:
        bool _initializeSourceFile(        
            const char* pixelColorDataBaseFileName,
            const char* pixelGeoDataBaseFileName,
            const char* irradianceCacheFileName);
        int _initializeFileVec(const char* pixelDataBaseFileName,std::vector<std::string>& vec);


    private:
		static DWORD WINAPI MainThreadFunc(LPVOID param);   //Filtering main thread
		static DWORD WINAPI WorkingThread(LPVOID param);     //Filtering working thread   
        static DWORD WINAPI secondWorkingThread(LPVOID param);//Motion Blur working thread
        static DWORD WINAPI MonitorThread(LPVOID param);           //Frame Loading thread

        static DWORD WINAPI MainThreadFunc2(LPVOID param);
        static DWORD WINAPI scanThreadFunc(LPVOID param);
        //file vector
        std::vector<std::string> m_vColorFileName;
        std::vector<std::string> m_vGeoFileName;
        std::vector<std::string> m_vIrraFileName;
        //output path
        char m_outputPath[MAX_PATH];
        //frame buffer
        std::map<int,Frame*> m_FrameMap;
        std::map<int,Frame*>::iterator m_currentFrameIte;

        //result map
        bool m_bMotionBlur;
        float _tempTor2;
        std::map<int,CCol4*> m_middleResultMap;
        std::map<int,CCol4*>::iterator  m_middleMapIte;
        int m_nCurrentFrameFor2Filtering;
        CCol4* m_resultFrame;

        //temp Msg
        char m_tempMsg[300];

        float* m_fWeightAddup;//weightAddup data array
        float* m_fWeightAddupForDisplay;
        bool _saveWeightAddupToImage(char* _fileName,float* _data,int _width,int _height);

        //data output
        float * m_fCubic;
        void _saveWeightCubicToFile(char* fileName);
        void _clean();
        void _secondFilterFunc();       
        void _filterOnePixel(int _row, int _col);
        void _filterOnePixelBetweenTwoFrame(int _row,int _col,CCol4& WeightPlusColor,float& Weights,float& WeightsMulSample,int _nBiasFrame);
        
        float _getWeightsAddupForOnePixel(int _row,int _col);   
        float _getWeightsAddupFromTwoFrame(int _row,int _col,int _nBiasFrame);
    public:    
        ImageUtility* m_img;

		//thread

	private:
		//main thread
        HANDLE m_mainThreadHandle;
		DWORD m_mainThreadID;
        HANDLE m_mainExitEventHandle;
        //main2 thread
        HANDLE m_main2ThreadHandle;
        DWORD m_main2ThreadID;
        //scan thread
        HANDLE m_scanThreadHandles[10];
        DWORD m_scanThreadIDs[10];
        _threadPrama m_scanThreadParam[10];

        //working thread
		int m_nWorkingThread;		//working thread number
		HANDLE m_workingThreadHandles[10];
		DWORD m_workingThreadIDs[10];
        _threadPrama m_workingThreadParam[10];
	    //monitor thread
        HANDLE m_monitorThreadHandle;
        DWORD m_monitorThreadID;
        //second filtering thread
        HANDLE m_secondWorkingThreadHandles[10];
        DWORD m_secondWorkingThreadIDs[10];
        _threadPrama m_secondWorkingThreadParam[10];
        //event handle
        HANDLE m_monitorExitEventHandle;
        HANDLE m_monitorWorkEventHandle;
    public:
        //Locks
        ZHLock m_lkFrameMap;
        ZHLock m_lkResampleFrameMap;
        ZHLock m_lkPixelVec;
        ZHLock m_lkMiddleResult;
           
    public:
		bool m_doingScan;
        int m_iFilterSize;
        int m_iFilterFrame;
        int m_iImageWidth;
        int m_iImageHeight;
        float m_fSamplerThreshold;
        void saveResamplingDataToFile(char* fileName);
        void saveResamplingDataToImages(char* fileName);
        void _addResampling(Frame* _frm,int _index);
    private:
        std::map<int,FrameForResample*> m_vReSamplerSets;
        std::map<int,FrameForResample*>::iterator m_mapCurrentSearchframeIte;
        bool m_bIfResampler;
        ZHLog* m_log;
        float m_fMAXWEIGTSAMPLEADDUP;

    public:
        //other data from .pix files

        float m_fStartCameraFloat;
        float m_fCameraFloatStep;
        int m_nIndirectLightNumber;
        int m_nPhotonNum;
        bool m_bUseIrradianceCache;
        float m_fTolerance;

        float m_fIntensityScale;
        int m_nStartFrame;
        int m_nEndFrame;

        bool m_bSunLight;
        bool m_bSkyLight;
        bool m_bGradientIC;
        bool m_bBmcIntegration;

        DWORD m_SunLight;
        DWORD m_SkyLight;
        float m_fLightScale;
        float m_fSunlightScale;
        float m_fSkylightScale;

    public:
        int m_nTotalPixels;
        int m_nTotalResamplePixels;
        float m_fResampleRate;


    private:
        //current being filtering frame
        int m_nCurrentFilteringFrame;
        //current max index frame in frame map
        int m_nCurrentMaxFrame;
        //total frames for filtering
        int m_nTotalFrames;
        float m_fScale;
        //start index
        int m_startFilterIndex;

		//

        //from initialize function parameters
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
    };


}