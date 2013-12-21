
#ifndef _ZH_DLL
#define _ZH_DLL
#endif

#include "ZHSTD.h"
#include "FilterVideo2.h"
#include "Filter.h"
#include "PixelInfo.h"
#include "qmath.h"
#include "Frame.h"
#include "Singleton.h"
#include "Image.h"
#include "Watch.h"

namespace ZH
{


    FilterVideo2::FilterVideo2():
    m_iFilterSize(5),
        m_iFilterFrame(5),
        m_iImageWidth(0),
        m_iImageHeight(0),
        m_nWorkingThread(1),
        m_usePixelDistance(true),
        m_usePerceptionDistance(true),
        m_useNormalDistance(true),
        m_useDepthDistance(true),
        m_useMaterialDistance(true),
        //m_currentFrame(NULL),
        m_nCurrentFilteringFrame(0),
        m_nCurrentMaxFrame(0),
        m_nTotalFrames(0),
        m_img(NULL),
        m_resultFrame(NULL),
        m_fScale(0.4f),
        ExpTableSize(20000),
        G_ExpTable(NULL),
        m_nCurrentFrameFor2Filtering(0),
        m_bMotionBlur(true),
        _tempTor2(1.0f),
        m_fWeightAddup(NULL),
        m_fWeightAddupForDisplay(NULL),
        m_fCubic(NULL),
        m_startFilterIndex(0)
    {

        m_torPixelPositionDistance2	=	static_cast<float>(qmathPow((5-1)/2.0f * 1.414f /3.0f, 2.0f));
        m_torColorDistance2	=	m_torPixelPositionDistance2;
        m_torNormalDistance2	=	(1.0f/3.0f) * (1.0f/3.0f);
        m_torDepthDistance2	=	(1.0f/3.0f) * (1.0f/3.0f);
        m_torMaterialDistance2	=	m_torPixelPositionDistance2;
        // m_currentFrame    =  new Frame;
        m_img  =  Singleton<ImageUtility>::getSingletonPtr();

        //create event
        m_monitorExitEventHandle = CreateEvent(NULL,false,false,"MonitorExit");
        m_monitorWorkEventHandle   = CreateEvent(NULL,false,false,"MonitorWork");
        m_mainExitEventHandle = CreateEvent(NULL,false,false,"MainExit");
        //calculate exp table;
        G_ExpTable	=	new float[ExpTableSize];
        if(G_ExpTable==NULL)
        {
            printf("Exp table create failed!\n");
        }

        double	 pp	=	0.0f;
        double	 _step	=	5.0f/static_cast<float>((ExpTableSize -1));	
        for(int i=0;i<ExpTableSize;i++)	
        {	
            G_ExpTable[i]	=	qmathExp(-pp);
            pp+= _step;
        }

        //tor^2 =  -maxRange^2/(2*lnk), k is [0,1], exp. 0.1
        float maxRange  = static_cast<float>(m_iFilterFrame/2);
        float k   =  0.08f;
        _tempTor2   =  -maxRange*maxRange/(2.0f* log(k));

    }

    FilterVideo2::~FilterVideo2()
    {
        SAFE_DELETE_ARRAY(G_ExpTable);

        std::map<int,Frame*>::iterator t_ite;
        for(t_ite=  m_FrameMap.begin();t_ite!=m_FrameMap.end();t_ite++)
        {
            SAFE_DELETE(t_ite->second);
        }

        m_FrameMap.clear();

        std::map<int,CCol4*>::iterator t_ite2;
        for(t_ite2=  m_middleResultMap.begin();t_ite2!=m_middleResultMap.end();t_ite2++)
        {
            SAFE_DELETE_ARRAY(t_ite2->second);
        }

        m_middleResultMap.clear();


        SAFE_DELETE_ARRAY(m_resultFrame);

#ifdef FILTER_VIDEO_OUTPUT_WEIGHTADDUP
        SAFE_DELETE_ARRAY(m_fWeightAddup);
        SAFE_DELETE_ARRAY(m_fWeightAddupForDisplay);
#endif

#ifdef FILTER_VIDEO_OUTPUT_WEIGHTCUBIC
        SAFE_DELETE_ARRAY(m_fCubic);
#endif

    }

    bool FilterVideo2::initialize(
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
        bool _usePixelDistance,
        bool _usePerceptionDistance ,
        bool _useNormalDistance,
        bool _useDepthDistance,
        bool _usematerialDistance,
        bool _motionBlur
        )
    {
        if(_filterFrameNumber<=0||
            _filterBoxSize<=0||
            _workingThread<1||
            _filterFrameNumber%2==0||
            _filterBoxSize%2==0||
            pixelColorDataBaseFileName==NULL||
            pixelGeoDataBaseFileName==NULL||
            outputPath==NULL

            )
        {
            return false;
        }

        //reset some data
        m_nCurrentFilteringFrame  =  0;
        m_nCurrentMaxFrame    =  0;
        m_vColorFileName.clear();
        m_vGeoFileName.clear();
        m_nCurrentFrameFor2Filtering    = 0;

        //clear frameMap
        std::map<int,Frame*>::iterator t_ite;
        for(t_ite=  m_FrameMap.begin();t_ite!=m_FrameMap.end();t_ite++)
        {
            SAFE_DELETE(t_ite->second);
        }

        m_FrameMap.clear();
        //clear middle Map
        std::map<int,CCol4*>::iterator t_ite2;
        for(t_ite2=  m_middleResultMap.begin();t_ite2!=m_middleResultMap.end();t_ite2++)
        {
            SAFE_DELETE_ARRAY(t_ite2->second);
        }
        m_middleResultMap.clear();

        m_bMotionBlur                  =  _motionBlur;
        m_nWorkingThread			=	_workingThread;
        m_usePixelDistance			=	_usePixelDistance;
        m_usePerceptionDistance	=	_usePerceptionDistance;
        m_useNormalDistance		=	_useNormalDistance;
        m_useDepthDistance			=	_useDepthDistance;
        m_useMaterialDistance		=	_usematerialDistance;									

        m_iImageWidth		=	0;
        m_iImageHeight	=	0;
        m_iFilterFrame      =  _filterFrameNumber;
        m_iFilterSize			=	_filterBoxSize;

        if(_torPixelPositionDistance!=0)
            m_torPixelPositionDistance2	=	_torPixelPositionDistance * _torPixelPositionDistance;
        if(_torNormalDistance!=0)
            m_torNormalDistance2	=	_torNormalDistance *_torNormalDistance;
        if(_torColorDistance!=0)
            m_torColorDistance2	=	_torColorDistance * _torColorDistance;
        if(_torDepthDistance!=0)
            m_torDepthDistance2	=	_torDepthDistance * _torDepthDistance;
        if(_torMaterialDistance!=0)
            m_torMaterialDistance2	=	_torMaterialDistance * _torMaterialDistance;

        //output file directory
        strcpy(m_outputPath,outputPath);

        //check source data file
        if(_initializeSourceFile(pixelColorDataBaseFileName,pixelGeoDataBaseFileName)==false)
        {
            return false;
        }

        //reset m_nCurrentFrameFor2Filtering
        m_nCurrentFrameFor2Filtering    = m_startFilterIndex;

        //get width and height
        Frame* t_frame =  new Frame;
        if(t_frame!=NULL)
        {
            if(t_frame->initializeFromFile(m_vColorFileName[0].c_str(),m_vGeoFileName[0].c_str())==true)
            {
                m_iImageWidth     = t_frame->getFrameWidth();
                m_iImageHeight    = t_frame->getFrameHeight();
            }
            else
            {
                return false;
            }
        }
        SAFE_DELETE(t_frame);

#ifdef VIDEO_FILTER_OUTPUT_WEIGHTADDUP
        SAFE_DELETE_ARRAY(m_fWeightAddup);
        m_fWeightAddup   =  new float[m_iImageWidth * m_iImageHeight];
        if(m_fWeightAddup==NULL)
            return false;
        memset((void*)m_fWeightAddup,0,sizeof(float)*m_iImageWidth * m_iImageHeight);

        SAFE_DELETE_ARRAY(m_fWeightAddupForDisplay);
        m_fWeightAddupForDisplay   =  new float[m_iImageWidth * m_iImageHeight * 3];
        if(m_fWeightAddupForDisplay==NULL)
            return false;
        memset((void*)m_fWeightAddupForDisplay,0,sizeof(float)*m_iImageWidth * m_iImageHeight);
#endif

#ifdef VIDEO_FILTER_OUTPUT_WEIGHTCUBIC
        SAFE_DELETE_ARRAY(m_fCubic);
        m_fCubic   =  new float[_filterBoxSize * _filterBoxSize * _filterFrameNumber];
        if(m_fCubic==NULL)
            return false;
        memset((void*)m_fCubic,0,sizeof(float)*_filterBoxSize * _filterBoxSize * _filterFrameNumber);
#endif

        //reset result 
        SAFE_DELETE_ARRAY(m_resultFrame);
        m_resultFrame  =  new CCol4[m_iImageWidth*m_iImageHeight];

        //record total frame number
        m_nTotalFrames    =static_cast<int>(m_vColorFileName.size());

        return true;


    }

    int FilterVideo2::_initializeFileVec(const char* pixelDataBaseFileName,std::vector<std::string>& vec)
    {
        vec.clear();
        std::string _tempName   = pixelDataBaseFileName;
        char temp[MAX_PATH];
        strcpy(temp,pixelDataBaseFileName);

        int indexLen  =  4;
        int _dotPos    =  static_cast<int>(_tempName.find('.'));

        char _keyStr[10];

        int ii=0;
        for(;ii<indexLen;ii++)
        {
            _keyStr[ii]  =  pixelDataBaseFileName[_dotPos-indexLen+ii];
        }
        _keyStr[ii] =  '\0';


        int i  =  atoi(_keyStr);
        //record start filter index
        m_startFilterIndex =  i;

        FILE* _file = fopen(temp,"r");
        for(;_file!=NULL;)
        {
            fclose(_file);
            vec.push_back(temp);

            i++;
            sprintf(_keyStr,"%04d",i);

            //Replace
            int ii=0;
            for(;ii<indexLen;ii++)
            {
                temp[_dotPos-indexLen+ii]  =  _keyStr[ii];
            }

            _file = fopen(temp,"r");

        }


        return static_cast<int>(vec.size());

    }

    bool FilterVideo2::_initializeSourceFile(        
        const char* pixelColorDataBaseFileName,
        const char* pixelGeoDataBaseFileName)
    {
        if(pixelColorDataBaseFileName==NULL||pixelGeoDataBaseFileName==NULL)
            return false;

        m_vColorFileName.clear();
        m_vGeoFileName.clear();

        int _colFileNum  = _initializeFileVec(pixelColorDataBaseFileName, m_vColorFileName);
        int _geoFileNum = _initializeFileVec(pixelGeoDataBaseFileName, m_vGeoFileName);
        if(_colFileNum<m_iFilterFrame||_geoFileNum<m_iFilterFrame||_colFileNum!=_geoFileNum)
            return false;


        return true;
    }


    bool FilterVideo2::startFiltering()
    {
        //check enough frames for load
        if(static_cast<int>(m_vColorFileName.size())<m_iFilterFrame * 3)
            return false;

        //pre-load some frames
        int t_nFrameStackSize    =  m_iFilterFrame * 3;


        for(int i=0;i<t_nFrameStackSize;i++)
        {
            //frame
            Frame* t_pFrame  =  new Frame;
            if(t_pFrame->initializeFromFile(m_vColorFileName[i].c_str(),m_vGeoFileName[i].c_str())==false)
                return false;
            m_FrameMap.insert(std::make_pair(m_startFilterIndex+i,t_pFrame));
            m_nCurrentMaxFrame    =  m_startFilterIndex + i;
        }

        ////laugh the monitor thread
        m_monitorThreadHandle		=	(HANDLE)_beginthreadex(
            NULL,
            0,
            (THREADFUNC)(FilterVideo2::MonitorThread),
            (LPVOID)this,
            0,
            (THREADID)(&m_monitorThreadID)
            );

        if(m_monitorThreadHandle)
        {
            printf("Monitor Filtering thread launched!\n");
        }
        else
        {
            printf("Monitor Filtering thread launched failed!\n");
            return false;
        }

        //laugh the main thread
        m_mainThreadHandle		=	(HANDLE)_beginthreadex(
            NULL,
            0,
            (THREADFUNC)(FilterVideo2::MainThreadFunc),
            (LPVOID)this,
            0,
            (THREADID)(&m_mainThreadID)
            );

        if(m_mainThreadHandle)
        {
            printf("Main Filtering thread launched!\n");
        }
        else
        {
            printf("Main Filtering thread launched failed!\n");
            return false;
        }


        WaitForSingleObject(m_mainThreadHandle,INFINITE);
        CloseHandle(m_mainThreadHandle);

        return true;
    }


    DWORD WINAPI FilterVideo2::MainThreadFunc(LPVOID param)
    {
        FilterVideo2* _object	=	(FilterVideo2*)(param);

        int _threadNum	=	_object->m_nWorkingThread;
        int _rowStep	=	static_cast<int>(_object->m_iImageHeight/_threadNum);


        //start time watch
        Watch::startWatch();

        int t_totalFrames   =  static_cast<int>(_object->m_vColorFileName.size());
        for(int render_index   =  _object->m_startFilterIndex ;render_index<_object->m_startFilterIndex + t_totalFrames;render_index++)
        {
            //set current frame
            _object->m_nCurrentFilteringFrame  =  render_index;

            //find current frame in frame map
            _object->m_currentFrameIte   =  _object->m_FrameMap.find(render_index);
            if(_object->m_currentFrameIte==_object->m_FrameMap.end())
            {
                printf(".............................Waiting for current frame %d to be load....\n",render_index);
                printf(".............................Waiting 2 second to retry.\n");
                SetEvent(_object->m_monitorWorkEventHandle);
                Sleep(3000);
                _object->m_currentFrameIte   =  _object->m_FrameMap.find(render_index);
                if(_object->m_currentFrameIte==_object->m_FrameMap.end())
                {
                    printf(".........................Retry failed! Current frame %d not found in frame map!\n",render_index);
                    break;
                }
            }


            //laugh threads to render
            for(int i=0; i<_threadNum;i++)
            {
                _object->m_workingThreadParam[i]._obj	=_object;
                _object->m_workingThreadParam[i].startRow	=	i * _rowStep;
                if(i<_threadNum-1)
                {	
                    _object->m_workingThreadParam[i].endRow	=	(i+1) * _rowStep;
                }
                else
                {
                    _object->m_workingThreadParam[i].endRow	=	_object->m_iImageHeight-1;
                }

                _object->m_workingThreadHandles[i] =	
                    (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    (THREADFUNC)(FilterVideo2::WorkingThread),
                    (LPVOID)(&(_object->m_workingThreadParam[i])),
                    0,
                    (THREADID)(&(_object->m_workingThreadIDs[i]))
                    );	

                if(_object->m_workingThreadHandles[i]!=NULL)
                {
                    SetThreadPriority(_object->m_workingThreadHandles[i],THREAD_PRIORITY_NORMAL);
                    printf("FilterVideo2::WorkingThread  %d  launched!\n",i);
                }

            }

            WaitForMultipleObjects (_threadNum, _object->m_workingThreadHandles, TRUE, INFINITE);

            //DWORD _tt;
            for(int j	=	0;j<_threadNum;j++)
            {
                if(_object->m_workingThreadHandles[j]!=NULL)
                {
                    CloseHandle(_object->m_workingThreadHandles[j]);
                    printf("FilterVideo2::WorkingThread  %d  exit!\n",j);
                    //GetExitCodeThread(_object->m_workingThreadHandles[j],&_tt);
                    //TerminateThread(_object->m_workingThreadHandles[j],_tt);
                }
            }


            //do some work after one frame is rendered.
            //let monitor thread to update.

            //=========================================
            //data output
#ifdef VIDEO_FILTER_OUTPUT_WEIGHTCUBIC               
            char t_weightsCubicPath[MAX_PATH];
            sprintf(t_weightsCubicPath,"%sweightFile%04d.wei",_object->m_outputPath,render_index);

            _object->_saveWeightCubicToFile(t_weightsCubicPath);
#endif
            //=========================================


#ifdef VIDEO_FILTER_OUTPUT_WEIGHTADDUP
            //save weightAppup
            char t_weightsPath[MAX_PATH];
            sprintf(t_weightsPath,"%sweightImage%04d.bmp",_object->m_outputPath,render_index);
            _object->_saveWeightAddupToImage(t_weightsPath,_object->m_fWeightAddup,_object->m_iImageWidth,_object->m_iImageHeight);
#endif

            SetEvent(_object->m_monitorWorkEventHandle);
            if(_object->m_bMotionBlur==false)
            {
                printf("Filtering frame  %d  finished!=========================>%d  ok\n ",render_index,render_index);
                //save result to file
                char t_resultPath[MAX_PATH];
                sprintf(t_resultPath,"%sFilteredImage%04d.bmp",_object->m_outputPath,render_index);
                _object->m_img->saveToFileFromColor4f(t_resultPath,_object->m_resultFrame,_object->m_iImageWidth,_object->m_iImageHeight,_object->m_fScale);      
            }
            else
            {
                //copy middle result to map for second filtering
                CCol4* t_middleResult   =  new CCol4[(_object->m_iImageHeight)*(_object->m_iImageWidth)];
                if(t_middleResult==NULL)
                {
                    printf("new t_middleResult failed!\n");
                    return 0;
                }

                memcpy((void*)t_middleResult,(void*)(_object->m_resultFrame),sizeof(CCol4)*(_object->m_iImageHeight)*(_object->m_iImageWidth));
                _object->m_middleResultMap.insert(std::make_pair(render_index,t_middleResult));

                //output one result
                if(render_index>=(_object->m_iFilterFrame + _object->m_startFilterIndex))
                {
                    _object->_secondFilterFunc();
                }

            }

            //if exit
            if(WaitForSingleObject(_object->m_mainExitEventHandle,0)!=WAIT_TIMEOUT)
            {
                SetEvent(_object->m_monitorExitEventHandle);
                _object->_clean();
                printf("Filtering exit!\n");
                return 0;
            }


        }

        //if motion blur ,we need to process some left frame in middle frame stack      
        if(_object->m_bMotionBlur==true)
        {
            while(_object->m_nCurrentFrameFor2Filtering<=(_object->m_nTotalFrames-1 + _object->m_startFilterIndex))
            {
                _object->_secondFilterFunc();
            }
        }

        //let monitor thread exit.
        printf("Filtering all finished!\n");
        float t_time   =  Watch::endWatch();
        printf("Filtering costs totally %f seconds, %f seconds per frame.\n",t_time,t_time/_object->m_nTotalFrames);

        SetEvent(_object->m_monitorExitEventHandle);
        return 0;
    }

    void FilterVideo2::_secondFilterFunc()
    {
        int t_nFront   =  m_nCurrentFrameFor2Filtering - static_cast<int>(m_iFilterFrame/2);
        int t_nBack    = m_nCurrentFrameFor2Filtering + static_cast<int>(m_iFilterFrame/2);

        memset((void*)m_resultFrame,0,sizeof(CCol4)*(m_iImageHeight)*(m_iImageWidth));

        //laugh threads to render
        int _rowStep  = static_cast<int>(m_iImageHeight/m_nWorkingThread);
        for(int i=0; i<m_nWorkingThread;i++)
        {
            m_secondWorkingThreadParam[i]._obj	=  this;
            m_secondWorkingThreadParam[i].startRow	=	i * _rowStep;
            m_secondWorkingThreadParam[i].front   =  t_nFront;
            m_secondWorkingThreadParam[i].back    = t_nBack;
            if(i<m_nWorkingThread-1)
            {	
                m_secondWorkingThreadParam[i].endRow	=	(i+1) * _rowStep;
            }
            else
            {
                m_secondWorkingThreadParam[i].endRow	=	m_iImageHeight-1;
            }

            m_secondWorkingThreadHandles[i] =	
                (HANDLE)_beginthreadex(
                NULL,
                0,
                (THREADFUNC)(FilterVideo2::secondWorkingThread),
                (LPVOID)(&(m_secondWorkingThreadParam[i])),
                0,
                (THREADID)(&(m_secondWorkingThreadIDs[i]))
                );	

            if(m_secondWorkingThreadHandles[i]!=NULL)
            {
                SetThreadPriority(m_secondWorkingThreadHandles[i],THREAD_PRIORITY_NORMAL);
                printf("FilterVideo2::secondWorkingThread  %d  launched!\n",i);
            }

        }

        WaitForMultipleObjects (m_nWorkingThread, m_secondWorkingThreadHandles, TRUE, INFINITE);

        //DWORD _tt;
        for(int j	=	0;j<m_nWorkingThread;j++)
        {
            if(m_secondWorkingThreadHandles[j]!=NULL)
            {
                CloseHandle(m_secondWorkingThreadHandles[j]);
                printf("FilterVideo2::secondWorkingThread  %d  exit!\n",j);
                //GetExitCodeThread(_object->m_workingThreadHandles[j],&_tt);
                //TerminateThread(_object->m_workingThreadHandles[j],_tt);
            }
        }



        //save result to file
        char t_resultPath[MAX_PATH];
        sprintf(t_resultPath,"%sFilteredImage%04d.bmp",m_outputPath,m_nCurrentFrameFor2Filtering);
        m_img->saveToFileFromColor4f(t_resultPath,m_resultFrame,m_iImageWidth,m_iImageHeight,m_fScale);      

        //if need to erase one frame from middleStack
        int _toBeRemoved  = m_nCurrentFrameFor2Filtering-static_cast<float>(m_iFilterFrame/2)-1;

        m_middleMapIte    = m_middleResultMap.find(_toBeRemoved);
        if(m_middleMapIte!=m_middleResultMap.end())
        {
            SAFE_DELETE_ARRAY(m_middleMapIte->second);
            m_middleResultMap.erase(m_middleMapIte);
        }

        printf("Filtering frame  %d  finished!=========================>%d  ok\n ",m_nCurrentFrameFor2Filtering,m_nCurrentFrameFor2Filtering);

        m_nCurrentFrameFor2Filtering+=1;


    }

    DWORD WINAPI FilterVideo2::secondWorkingThread(LPVOID param)
    {
        _threadPrama* t_param =  (_threadPrama*)param;
        FilterVideo2* _object   =  t_param->_obj;
        int startRow    = t_param->startRow;
        int endRow     = t_param->endRow;
        int t_nFront   =  t_param->front;
        int t_nBack    = t_param->back;

        CCol4* center  =  _object->m_middleResultMap[_object->m_nCurrentFrameFor2Filtering];
        CCol4 _tempTotalColor =  CCol4(0,0,0);
        float _tempTotalWeight  =  0.0f;
        float _tempWeight =  0.0f;

        float _tempExpVar;
        int _tempExpTableIndex;
        float _tempC = (_object->ExpTableSize-1)/5.0f;

        int index;
        for(int i=startRow;i<endRow;i++)
            for(int j=0;j<(_object->m_iImageWidth);j++)
            {
                _tempTotalColor =  CCol4(0,0,0);
                _tempTotalWeight  =  0.0f;
                index = i*_object->m_iImageWidth+j;

                for(int k  =  t_nFront ; k<=t_nBack; k++)
                {
                    if(k<(_object->m_startFilterIndex)||k>(_object->m_nTotalFrames-1+_object->m_startFilterIndex)) continue;

                    _tempExpVar	=	0.5f/(_object->_tempTor2) * (k-_object->m_nCurrentFrameFor2Filtering) * (k-_object->m_nCurrentFrameFor2Filtering);
                    if(_tempExpVar>5.0f)
                        _tempExpVar	=	5.0f;
                    _tempExpTableIndex	=	static_cast<int>(_tempExpVar * _tempC);
                    _tempWeight	=	_object->G_ExpTable[_tempExpTableIndex];

                    _tempTotalColor+= _tempWeight*((_object->m_middleResultMap[k])[index]);
                    _tempTotalWeight+=_tempWeight;
                }

                _object->m_resultFrame[index]    = _tempTotalColor/_tempTotalWeight;


            }

            return 0;
    }

    DWORD WINAPI FilterVideo2::MonitorThread(LPVOID param)
    {
        FilterVideo2* _object   =  (FilterVideo2*)param;
        for(;;)
        {
            WaitForSingleObject(_object->m_monitorWorkEventHandle,INFINITE);
            //check if need to exit the thread
            if(WaitForSingleObject(_object->m_monitorExitEventHandle,0)!=WAIT_TIMEOUT)
            {
                break;
            }

            printf("FilterVideo2::MonitorThread  updating activated!\n");
            //do update frame work
            int t_deltaFrames   = _object->m_nCurrentMaxFrame   -  _object->m_nCurrentFilteringFrame;
            int t_thresholdFrames = (_object->m_iFilterFrame) * 2;
            if(t_deltaFrames<=t_thresholdFrames)
            {

                std::map<int,Frame*>::iterator t_ite;
                for(t_ite=_object->m_FrameMap.begin();t_ite!=_object->m_FrameMap.end();)
                {
                    if((t_ite->first)<(_object->m_nCurrentFilteringFrame)-static_cast<int>(_object->m_iFilterFrame/2.0f))
                    {
                        //erase one frame
                        SAFE_DELETE(t_ite->second);
                        t_ite =  _object->m_FrameMap.erase(t_ite);


                        //insert new frame
                        if((_object->m_nCurrentMaxFrame)<(_object->m_nTotalFrames-1 + _object->m_startFilterIndex))
                        {               
                            Frame* t_frame =  new Frame;
                            if(t_frame==NULL)
                            {
                                printf("MonitorThread -->  Frame* t_frame =  new Frame  failed!\n ");
                                return false;
                            }
                            if(t_frame->initializeFromFile((_object->m_vColorFileName)[_object->m_nCurrentMaxFrame+1 - _object->m_startFilterIndex].c_str(),(_object->m_vGeoFileName)[_object->m_nCurrentMaxFrame+1-_object->m_startFilterIndex].c_str())==false)
                            {
                                printf("MonitorThread -->  t_frame->initializeFromFile  failed!\n ");
                                return false;
                            }            
                            (_object->m_FrameMap).insert(std::make_pair(_object->m_nCurrentMaxFrame+1,t_frame));
                            (_object->m_nCurrentMaxFrame)+=1;
                            printf("FilterVideo2::MonitorThread  loaded frame %d!\n",_object->m_nCurrentMaxFrame);
                        }
                        else
                        {
                            printf("FilterVideo2::MonitorThread  loaded work Finished!\n");
                            break;
                        }

                    }
                    else
                    {
                        t_ite++;
                    }
                }
            }


        }

        printf("FilterVideo2::MonitorThread  exits!\n");

        return 0;
    }

    DWORD WINAPI FilterVideo2::WorkingThread(LPVOID param)
    {
        _threadPrama* t_param =  (_threadPrama*)param;
        FilterVideo2* _object   =  t_param->_obj;
        int startRow    = t_param->startRow;
        int endRow     = t_param->endRow;

        for(int i=startRow;i<endRow;i++)
            for(int j=0;j<(_object->m_iImageWidth);j++)
            {
                _object->_filterOnePixel(i,j);
            }

            return 0;
    }


    void FilterVideo2::_filterOnePixel(int _row, int _col)
    {
        int t_halfFrames = static_cast<int>(m_iFilterFrame/2);
        int t_currentFrame =  m_nCurrentFilteringFrame;

        CCol4 t_weightPlusColor   =  CCol4(0.0f,0.0f,0.0f);
        float t_totalWeightsAddup  =  0.0f;

        //Iterator each frames
        for(int i=t_currentFrame - t_halfFrames;i<=t_currentFrame +t_halfFrames;i++)
        {
            //if out of frame range
            if(i<m_startFilterIndex||i>m_nTotalFrames-1+m_startFilterIndex) continue;

            CCol4 t_weightPlusColor_t;
            float t_totalWeightsAddup_t;
            _filterOnePixelBetweenTwoFrame(_row,_col,t_weightPlusColor_t,t_totalWeightsAddup_t,i);
            t_weightPlusColor+=t_weightPlusColor_t;
            t_totalWeightsAddup+=t_totalWeightsAddup_t;

        }

        //final color
        m_resultFrame[_row*m_iImageWidth+_col]   = t_weightPlusColor/t_totalWeightsAddup; 

#ifdef VIDEO_FILTER_OUTPUT_WEIGHTADDUP       
        m_fWeightAddup[_row*m_iImageWidth+_col] = t_totalWeightsAddup;
#endif

    }

    void FilterVideo2::_filterOnePixelBetweenTwoFrame(int _row,int _col,CCol4& WeightPlusColor,float& Weights,int _nBiasFrame)
    {
        if(_row<0||_row>m_iImageHeight-1||_col<0||_col>m_iImageWidth -1)
            return;

        int posRow,posCol;
        float totalWeights = 0.0f;
        CColor4f finalColor;
        finalColor.r()	=	0.0f;
        finalColor.g()	=	0.0f;
        finalColor.b()	=	0.0f;

        float wPixel,wColor,wNormal,wDepth,wMaterial,wFinal;
        float distance2=0.0f;

        //index
        int centerIndex  =  m_iImageWidth*_row+_col;
        int biasIndex;

        //current frame
        Frame* _centerFrame =  m_currentFrameIte->second;

        //find the bias frame data
        std::map<int,Frame*>::iterator t_biasFrameIte   =  m_FrameMap.find(_nBiasFrame);
        if(t_biasFrameIte==m_FrameMap.end())
        {
            printf("......................................Waiting for bias frame %d to be load....\n",_nBiasFrame);
            SetEvent(m_monitorWorkEventHandle);
            Sleep(3000);
            t_biasFrameIte   =  m_FrameMap.find(_nBiasFrame);
            if(t_biasFrameIte==m_FrameMap.end())
            {
                printf("......................................Bias frame %d not found in frame map\n",_nBiasFrame);
                return;
            }
        }

        //bias frame
        Frame* _biasFrame    =  t_biasFrameIte->second;

        //two point color
        //////RGB space
        CColor4f	centerColor	=	_centerFrame->m_colInfo[centerIndex];
        CColor4f	biasColor;
        //////CIElab space
        CColor4f   CIElab_centerColor;
        CColor4f   CIElab_biasColor;

        CVec3 centerNormal, biasNormal;
        float centerDepth, biasDepth;
        int centerMaterialID, biasMaterialID;

        //temp normal dot
        float tempDot;
        float tempExpVar;
        int tempExpTableIndex;

        for(int iBlockRow	=	0; iBlockRow<m_iFilterSize; iBlockRow++)
            for(int iBlockCol	=	0; iBlockCol<m_iFilterSize; iBlockCol++)
            {
                posRow	=	_row	- static_cast<int>(m_iFilterSize/2.0f) + iBlockRow;
                posCol	=	_col	- static_cast<int>(m_iFilterSize/2.0f) + iBlockCol;	

                //if out of range
                if(posRow<0||posRow>m_iImageHeight-1||posCol<0||posCol>m_iImageWidth-1)
                {
                    continue;
                }

                biasIndex   =  m_iImageWidth*posRow+posCol;

                biasColor		=	(_biasFrame->m_colInfo)[biasIndex];			

                //pixel position distance weight calculation
                if(m_usePixelDistance)
                {
                    distance2	=	static_cast<float>(
                        (posRow	-	_row) * (posRow	-	_row) + 
                        (posCol	-	_col) * (posCol	-	_col)+
                        (m_nCurrentFilteringFrame - _nBiasFrame) * (m_nCurrentFilteringFrame - _nBiasFrame)
                        );
                    //distance2  =  m_pixelDistTable[iBlockRow*m_iFilterSize+iBlockCol];
                    tempExpVar	=	0.5f/m_torPixelPositionDistance2 * distance2;
                    if(tempExpVar>5.0f)
                        tempExpVar	=	5.0f;
                    tempExpTableIndex	=	static_cast<int>(tempExpVar/5.0f*(ExpTableSize-1));
                    wPixel	=	G_ExpTable[tempExpTableIndex];
                    //wPixel	=	static_cast<float>(qmathExp(-0.5f/m_torPixelPositionDistance2 * distance2));
                }
                else
                {
                    wPixel	=	1.0f;
                }


                //color perception distance weight calculation
                if(m_usePerceptionDistance)
                {
                    CIElab_centerColor = (_centerFrame->m_colCIEInfo)[centerIndex];
                    CIElab_biasColor    = (_biasFrame->m_colCIEInfo)[biasIndex];

                    distance2	=	
                        (CIElab_centerColor.r() - CIElab_biasColor.r()) * (CIElab_centerColor.r() - CIElab_biasColor.r())+
                        (CIElab_centerColor.g() - CIElab_biasColor.g()) * (CIElab_centerColor.g() - CIElab_biasColor.g())+
                        (CIElab_centerColor.b() - CIElab_biasColor.b()) *  (CIElab_centerColor.b() - CIElab_biasColor.b());
                    distance2  /=(CIElab_centerColor.r()*CIElab_centerColor.r()+
                        CIElab_centerColor.g()*CIElab_centerColor.g()+
                        CIElab_centerColor.b()*CIElab_centerColor.b());

                    tempExpVar	=	0.5f/m_torColorDistance2 * distance2;
                    if(tempExpVar>5.0f)
                        tempExpVar	=	5.0f;
                    tempExpTableIndex	=	static_cast<int>(tempExpVar/5.0f*(ExpTableSize-1));
                    wColor	=	G_ExpTable[tempExpTableIndex];
                    //wColor	=	static_cast<float>(qmathExp(-0.5f/m_torColorDistance2 * distance2));
                }
                else
                {
                    wColor	=	1.0f;
                }

                //normal  distance weight calculation
                if(m_useNormalDistance)
                {
                    centerNormal	=	(_centerFrame->m_geoInfo->m_normal)[centerIndex];
                    biasNormal	=	(_biasFrame->m_geoInfo->m_normal)[biasIndex];
                    if(biasNormal==CVec3(0,0,0))
                    {
                        if(centerNormal==biasNormal)
                            wNormal = 1.0f;
                        else
                            wNormal = 0.0f;
                    }
                    else if(centerNormal==CVec3(0,0,0))
                    {
                        wNormal   =  0.0f;
                    }
                    else
                    {
                        centerNormal.Normalize();
                        biasNormal.Normalize();
                        tempDot    = centerNormal.Dot(biasNormal);
                        if(tempDot<=0)
                        {
                            wNormal   =  0.0f;
                        }
                        else
                        {
                            distance2	=	1.0f - tempDot;
                            distance2 *=distance2;

                            tempExpVar	=	0.5f/m_torNormalDistance2 * distance2;
                            if(tempExpVar>5.0f)
                                tempExpVar	=	5.0f;
                            tempExpTableIndex	=	static_cast<int>(tempExpVar/5.0f*(ExpTableSize-1));
                            wNormal	=	G_ExpTable[tempExpTableIndex];

                            //wNormal	=	static_cast<float>(qmathExp(-0.5f/m_torNormalDistance2 * distance2));
                        }

                    }


                }
                else
                {
                    wNormal	=	1.0f;
                }


                //depth distance weight calculation
                if(m_useDepthDistance)
                {
                    centerDepth	=	(_centerFrame->m_geoInfo->m_depth)[centerIndex];
                    biasDepth	=	(_biasFrame->m_geoInfo->m_depth)[biasIndex];
                    distance2	=	(centerDepth	-	biasDepth)	;
                    distance2 *=distance2;

                    tempExpVar	=	0.5f/m_torDepthDistance2 * distance2;
                    if(tempExpVar>5.0f)
                        tempExpVar	=	5.0f;
                    tempExpTableIndex	=	static_cast<int>(tempExpVar/5.0f*(ExpTableSize-1));
                    wDepth	=	G_ExpTable[tempExpTableIndex];

                    //wDepth	=	static_cast<float>(qmathExp(-0.5f/m_torDepthDistance2 * distance2));
                }
                else
                {
                    wDepth	=	1.0f;
                }


                //material distance weight calculation
                if(m_useMaterialDistance)
                {
                    centerMaterialID	=	(_centerFrame->m_geoInfo->m_materialID)[centerIndex];
                    biasMaterialID	=	(_biasFrame->m_geoInfo->m_materialID)[biasIndex];
                    if(centerMaterialID == biasMaterialID)
                        wMaterial	=	1.0f;
                    else
                        wMaterial	=	0.0f;
                }
                else
                {
                    wMaterial	=	1.0f;
                }


                wFinal	=	wPixel * wColor * wNormal * wDepth * wMaterial;
                finalColor.r()	+=	biasColor.r() * wFinal;
                finalColor.g()	+=	biasColor.g() * wFinal;
                finalColor.b() +=	biasColor.b() * wFinal;
                totalWeights	+=wFinal;

#ifdef VIDEO_FILTER_OUTPUT_WEIGHTCUBIC
                //=================================
                //data output
                if(_col==128&&_row==128)
                {
                    int index   =  (_nBiasFrame -   (m_nCurrentFilteringFrame - static_cast<int>(m_iFilterFrame/2.0f)))*m_iFilterSize*m_iFilterSize+iBlockRow*m_iFilterSize+iBlockCol;
                    m_fCubic[index]    =  wFinal;
                }

                //=================================
#endif

            }


            WeightPlusColor = finalColor;
            Weights =  totalWeights; 

            return;


    }


    bool FilterVideo2::_saveWeightAddupToImage(char* _fileName,float* _data,int _width,int _height)
    {
        if(_fileName==NULL||_data==NULL||_width<=0||_height<=0)
            return false;

        float t_max   = 0.0f;
        for(int i=0;i<_width*_height;i++)
        {
            if(_data[i]>t_max) t_max  =  _data[i];
        }

        if(t_max==0.0f)
        {
            memset((void*)(m_fWeightAddupForDisplay),0,sizeof(float) * m_iImageHeight * m_iImageWidth);
        }
        else
        {
            float t_tColor;
            int index;
            for(int i=0;i<_width*_height;i++)
            {
                index = i* 3;
                t_tColor  = _data[i]/t_max;
                if(t_tColor<0.25f)
                {
                    m_fWeightAddupForDisplay[index]     =  0.0f;
                    m_fWeightAddupForDisplay[index+1] =  t_tColor/0.25f;
                    m_fWeightAddupForDisplay[index+2] =  1.0f;

                }
                else if(t_tColor<0.5f)
                {
                    m_fWeightAddupForDisplay[index]     =  0.0f;
                    m_fWeightAddupForDisplay[index+1] =  1.0f;
                    m_fWeightAddupForDisplay[index+2] =  2.0f - 4 * t_tColor;//1.0f - (t_tColor-0.25f)/0.25f;
                }
                else if(t_tColor<0.75f)
                {
                    m_fWeightAddupForDisplay[index]     =  (t_tColor-0.5f)/0.25f;
                    m_fWeightAddupForDisplay[index+1] =  1.0f;
                    m_fWeightAddupForDisplay[index+2] =  0.0f;
                }
                else
                {
                    m_fWeightAddupForDisplay[index]     =  1.0f;
                    m_fWeightAddupForDisplay[index+1] =  4.0f - 4.0f * t_tColor;//1 - (t_tColor-0.75f)/0.25f;
                    m_fWeightAddupForDisplay[index+2] =  0.0f;
                }



            }

        }

        return(m_img->saveToFileFromFloat3(_fileName,m_fWeightAddupForDisplay,_width,_height));
    }


    void FilterVideo2::_saveWeightCubicToFile(char* fileName)
    {
        if(fileName==NULL)
            return;

        std::ofstream of(fileName,std::ios::out);
        if(of.fail())
        {
            return;
        }

        for(int i=0;i<m_iFilterFrame;i++)
        {
            for(int j=0;j<m_iFilterSize;j++)
            {
                for(int k=0;k<m_iFilterSize;k++)
                {
                    of<<m_fCubic[i*m_iFilterSize*m_iFilterSize+j*m_iFilterSize+k]<<" ";
                }
                of<<std::endl;
            }
            of<<std::endl;

        }

    }

    void FilterVideo2::stopFiltering()
    {
        SetEvent(m_mainExitEventHandle);
    }

    void FilterVideo2::_clean()
    {
        //reset some data
        m_vColorFileName.clear();
        m_vGeoFileName.clear();

        //clear frameMap
        std::map<int,Frame*>::iterator t_ite;
        for(t_ite=  m_FrameMap.begin();t_ite!=m_FrameMap.end();t_ite++)
        {
            SAFE_DELETE(t_ite->second);
        }

        m_FrameMap.clear();
        //clear middle Map
        std::map<int,CCol4*>::iterator t_ite2;
        for(t_ite2=  m_middleResultMap.begin();t_ite2!=m_middleResultMap.end();t_ite2++)
        {
            SAFE_DELETE_ARRAY(t_ite2->second);
        }
        m_middleResultMap.clear();								





    }



}
