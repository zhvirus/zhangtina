#pragma once
#include "ZHDecl.h"
#include "Common.h"


namespace ZH
{


class ZH_DLL PixelInfo
{
	public:
		explicit PixelInfo(int width, int height);
		~PixelInfo();
		bool setPixelInfo(int offset, 
                                float depth,
                                int materialID,
                                const CVec3& normal,
                                const CVec3& pos,
                                const CVec3& tex,
                                int triangleID,
                                float _densityScale=0.4f, 
                                int samplerNum = 200);
		bool clearAllData();
	public:
		int m_width;
		int m_height;
		int m_totalPixels;
		CVec3* m_normal;
        CVec3* m_pos;
        CVec3* m_tex;
        int* m_triangleID;
		float* m_depth;
		int* m_materialID;
        int* m_samplerNumber;//Direct Light number

        float m_fIntensityScale;
        float m_fCurCameraFloat;
        float m_fCameraFloatStep;
        int m_nTotalFrames;
        int m_nCurrentFrame;
        int m_nStartFrame;
        int m_nEndFrame;
        int m_nIndirectLightNumber;
        int m_nPhotonNum;
        float m_fTolerance; 

        bool m_bUseIrradianceCache;
        bool m_bSunLight;
        bool m_bSkyLight;
        bool m_bGradientIC;
        bool m_bBmcIntegration;

        DWORD m_SunLight;
        DWORD m_SkyLight;
        float m_fLightScale;
        float m_fSunlightScale;
        float m_fSkylightScale;

        float m_fReserved[5];

       
	public:
		bool saveDepthToImageFile(char* fileName);
		bool saveNormalToImageFile(char* fileName);
		bool savePixelInfoToFile(char* fileName);
		bool loadPixelInfoFromFile(char* fileName);
	private:
		ImageUtility* m_imageUtility;
};


class PixelIndex
{
    public:
        int m_nNormalIndex;
        int m_nPositionIndex;
        int m_nTextureIndex;
        int m_nTriangleIDIndex;
        int m_nDepthIndex;
        int m_nMaterialIndex;
        int m_nSamplerNumberIndex;        
};

class ZH_DLL PixelValue
{
    public:
        PixelValue()
        {
            m_nRow=0;
            m_nCol=0;
            PixelIndex _val;
            m_vIndex.push_back(_val);
            m_nIndexNumber  = static_cast<int>(m_vIndex.size());
        }

        ~PixelValue(){}
    public:
        int m_nRow;
        int m_nCol;
        int m_nIndexNumber;
        std::vector<PixelIndex> m_vIndex;
};


class ZH_DLL PixelInformation
{
public:
    explicit PixelInformation(int width, int height);
    ~PixelInformation();
    bool setPixelInfo(
        int offset,
        float depth,
        int materialID,
        const CVec3& normal,
        const CVec3& pos,
        const CVec3& tex,
        int triangleID,
        int samplerNum=200);

    bool searchForAntiAliasPoints();
public:
    int m_width;
    int m_height;
    int m_totalPixels;
    PixelValue* m_pPixelValue;
    CVec3* m_normal;
    CVec3* m_pos;
    CVec3* m_tex;
    int* m_triangleID;
    float* m_depth;
    int* m_materialID;
    int* m_samplerNumber;//Direct Light number

    float m_fIntensityScale;
    float m_fCurCameraFloat;
    float m_fCameraFloatStep;
    int m_nTotalFrames;
    int m_nCurrentFrame;
    int m_nStartFrame;
    int m_nEndFrame;
    int m_nIndirectLightNumber;
    int m_nPhotonNum;
    float m_fTolerance; 

    bool m_bUseIrradianceCache;
    bool m_bSunLight;
    bool m_bSkyLight;
    bool m_bGradientIC;
    bool m_bBmcIntegration;

    DWORD m_SunLight;
    DWORD m_SkyLight;
    float m_fLightScale;
    float m_fSunlightScale;
    float m_fSkylightScale;

    private:
        bool m_bDoneAntiAliasing;

};

}