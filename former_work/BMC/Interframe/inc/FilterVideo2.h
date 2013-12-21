#pragma once
#include "ZHDecl.h"
#include "Common.h"

namespace ZH
{
    class ZH_DLL FilterVideo2
    {	
        typedef unsigned (WINAPI *THREADFUNC)(LPVOID lpThreadParam);
        typedef unsigned *THREADID;

        struct _threadPrama
        {
            FilterVideo2* _obj;
            int startRow;
            int endRow;
            int front;
            int back;
        };

    public:
        FilterVideo2();
        ~FilterVideo2();
    public:
        bool initialize(
            const char* pixelColorDataBaseFileName,
            const char* pixelGeoDataBaseFileName,
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
            bool _motionBlur=true);


        bool startFiltering();
        void stopFiltering();
        inline int getFilterBoxSize()const{return m_iFilterSize;}
        inline int getFilterFrameNum()const{return m_iFilterFrame;}
        inline int getImageWidth()const{return m_iImageWidth;}
        inline int getImageHeight()const{return m_iImageHeight;}
    private:
        bool _initializeSourceFile(        
            const char* pixelColorDataBaseFileName,
            const char* pixelGeoDataBaseFileName);
        int _initializeFileVec(const char* pixelDataBaseFileName,std::vector<std::string>& vec);
    private:
        static DWORD WINAPI MainThreadFunc(LPVOID param);
        static DWORD WINAPI WorkingThread(LPVOID param);
        static DWORD WINAPI secondWorkingThread(LPVOID param);
        static DWORD WINAPI MonitorThread(LPVOID param);
        //file vector
        std::vector<std::string> m_vColorFileName;
        std::vector<std::string> m_vGeoFileName;
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

        float* m_fWeightAddup;//weightAddup data array
        float* m_fWeightAddupForDisplay;
        bool _saveWeightAddupToImage(char* _fileName,float* _data,int _width,int _height);

        //data output
        float * m_fCubic;
        void _saveWeightCubicToFile(char* fileName);
        void _clean();
        void _secondFilterFunc();       
        void _filterOnePixel(int _row, int _col);
        void _filterOnePixelBetweenTwoFrame(int _row,int _col,CCol4& WeightPlusColor,float& Weights,int _nBiasFrame);
    public:    
        ImageUtility* m_img;

        //thread

    private:
        //main thread
        HANDLE m_mainThreadHandle;
        DWORD m_mainThreadID;
        HANDLE m_mainExitEventHandle;
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
        int m_iFilterSize;
        int m_iFilterFrame;
        int m_iImageWidth;
        int m_iImageHeight;

    private:
        //current being filtering frame
        int m_nCurrentFilteringFrame;
        //current max index frame in frame map
        int m_nCurrentMaxFrame;
        //total frames
        int m_nTotalFrames;
        float m_fScale;
        //start index
        int m_startFilterIndex;

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