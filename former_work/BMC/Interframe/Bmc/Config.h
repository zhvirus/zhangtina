#pragma once
#include "Common.h"
#include "ArcBall.h"

class Config
{
public:


public:
	Config();
	~Config();
public:
	bool writeToFile(char* fileName);
	bool loadFromFile(char* fileName);
public:
	inline void setPreviewType(int type){m_ePreviewType	=	type;}
	inline int getPreviewType()const{return m_ePreviewType;}	
	
	inline void setLightScale(float scale){m_fLightScale	=	scale;}
	inline float getLightScale()const{return m_fLightScale;}	
	inline void setSunLight(bool enable){m_bSunLight	=	enable;}
	inline bool getSunLight()const {return m_bSunLight;}
	inline void setSkyLight(bool enable){m_bSkyLight	=	enable;}
	inline bool getSkyLight()const{return m_bSkyLight;}
	inline void setSunLightColor(const COLORREF& color){m_SunLightColor	=	color;}
	inline COLORREF getSunLightColor()const{return m_SunLightColor;}	
	inline void setSkyLightColor(const COLORREF& color){m_SkyLightColor	=	color;}
	inline COLORREF getSkyLightColor()const{return m_SkyLightColor;}
	inline void setSunLightScale(float scale){m_fSunLightScale	=	scale;}
	inline float getSunLightScale()const{return m_fSunLightScale;}
	inline void setSkyLightScale(float scale){m_fSkyLightScale	=	scale;}
	inline float getSkyLightScale()const{return m_fSkyLightScale;}
	
	inline void setPhotonNumber(int number){m_nPhotonNumber	=	number;}
	inline int getPhotonNumber()const{return m_nPhotonNumber;}
	inline void setSamplerRayNumber(int number){m_nSamplerRayNumber	=	number;}
	inline int getSamplerRayNumber()const{return m_nSamplerRayNumber;}
	inline void setSamplerLightRayNumber(int number){m_nSamplerLightRayNumber	=	number;}
	inline int getSamplerLightRayNumber()const {return m_nSamplerLightRayNumber;}
	inline void setIrradianceCache(bool enable){m_bIrradianceCache	=	enable;}
	inline bool getIrradianceCache()const{return m_bIrradianceCache;}
	inline void setTolerance(float tolerance){m_fTolerance	=	tolerance;}
	inline float getTolerance()const{return m_fTolerance;}
	inline void setGradient(bool gradient){m_bGradient	=	gradient;}
	inline bool getGradient()const{return m_bGradient;}
	inline void setBMC(bool BMC){m_bBMC	=	BMC;}
	inline bool getBMC()const {return m_bBMC;}

	inline void setImageWidth(int width){m_nImageWidth	=	width;}
	inline int getImageWidth()const{return m_nImageWidth;}
	inline void setImageHeight(int height){m_nImageHeight	=	height;}
	inline int getImageHeight()const{return m_nImageHeight;}
	inline void setThreadNumber(int number){m_nThreadNumber	=	number;}
	inline int getThreadNumber()const{return m_nThreadNumber;}
	inline void setShowResult(bool result){m_bShowResult	=	result;}
	inline bool getShowResult(){return m_bShowResult;}
	inline void setIntensityScale(float scale){m_fIntensityScale	=	scale;}
	inline float getIntensityScale()const{return m_fIntensityScale;}

	inline void setApplyFilter(bool enable){m_CheckBox_ApplyFilter	=	enable;}
	inline bool getApplyFilter()const{return m_CheckBox_ApplyFilter;}
	inline void setPixelFilterEnable(bool enable){m_CheckBox_Pixel	=	enable;}
	inline bool getPixelFilterEnable()const{return m_CheckBox_Pixel;}
	inline void setPerceptionFilterEnable(bool enable){m_CheckBox_Perception	=	enable;}
	inline bool getPerceptionFilterEnable()const{return m_CheckBox_Perception;}
	inline void setNormalFilterEnable(bool enable){m_CheckBox_Normal	=	enable;}
	inline bool getNormalFilterEnable()const{return m_CheckBox_Normal;}
	inline void setDepthFilterEnable(bool enable){m_CheckBox_Depth	=	enable;}
	inline bool getDepthFilterEnable()const{return m_CheckBox_Depth;}
	inline void setMaterialEnable(bool enable){m_CheckBox_Material	=	enable;}
	inline bool getMaterialEnable()const {return m_CheckBox_Material;}
	inline void setFilterBoxSize(int size){m_Edit_FilterBoxSize	=	size;}
	inline int getFilterBoxSize()const{return m_Edit_FilterBoxSize;}
	inline void setPixelFilterC(float C){m_Edit_Pixel	=	C;}
	inline float getPixelFilterC()const{return m_Edit_Pixel;}
	inline void setPerceptionFilterC(float C){m_Edit_Perception	=	C;}
	inline float getPerceptionFilterC()const{return m_Edit_Perception;}
	inline void setNormalFilterC(float C){m_Edit_Normal	=	C;}
	inline float getNormalFilterC()const{return m_Edit_Normal;}
	inline void setDepthFilterC(float C){m_Edit_Depth	=	C;}
	inline float getDepthFilterC()const{return m_Edit_Depth;}
	inline void setMaterialFiterC(float C){m_Edit_Material	=	C;}
	inline float getMaterialFilterC()const{return m_Edit_Material;}

	inline void setDirectLightEnable(bool enable){ m_cbDirect_light	=	enable;}
	inline bool getDirectLightEnable()const{return m_cbDirect_light;}
	inline void setIndirectLightEnable(bool enable){ m_cbIndirect_light	=	enable;}
	inline float getIndirectLightEnable()const{return m_cbIndirect_light;}
	inline void setMirrorEffectEnable(bool enable){m_cbMirror_effect	=	enable;}
	inline bool getMirrorEffectEnable()const{return m_cbMirror_effect;}
	inline void setCausticEffectEnable(bool enable){m_cbCaustic_effect	=	enable;}
	inline bool getCausticEffectEnable()const {return m_cbCaustic_effect;}
	inline void setDirectionalLightEnable(bool enable){m_cbDirectional_light	=	enable;}
	inline bool getDirectionalLightEnable()const{return m_cbDirectional_light;}
	inline void setPhotonMapTotallyEnable(bool enable){m_cbTotally_photonmap	=	enable;}
	inline bool getPhotonMapTotallyEnable()const{return m_cbTotally_photonmap;}


private:
	//preview
	int	 m_ePreviewType;

	//Light source energy setting
	float					m_fLightScale;
	bool					m_bSunLight;
	bool					m_bSkyLight;
	COLORREF			m_SunLightColor;
	COLORREF			m_SkyLightColor;
	float					m_fSunLightScale;
	float					m_fSkyLightScale;

	//Quality
	int			m_nPhotonNumber;
	int			m_nSamplerRayNumber;
	int			m_nSamplerLightRayNumber;
	bool		m_bIrradianceCache;
	float		m_fTolerance;
	bool		m_bGradient;
	bool		m_bBMC;

	//Ray tracing
	int m_nImageWidth;
	int m_nImageHeight;
	int m_nThreadNumber;
	bool m_bShowResult;
	float m_fIntensityScale;

	//Filter
	bool m_CheckBox_ApplyFilter;
	bool m_CheckBox_Pixel;
	bool m_CheckBox_Perception;
	bool m_CheckBox_Normal;
	bool m_CheckBox_Depth;
	bool m_CheckBox_Material;
	int m_Edit_FilterBoxSize;
	float m_Edit_Pixel;
	float m_Edit_Perception;
	float m_Edit_Normal;
	float m_Edit_Depth;
	float m_Edit_Material;

	//Render Feature
    bool m_cbDirect_light;
    bool m_cbIndirect_light;
    bool m_cbMirror_effect;
    bool m_cbCaustic_effect;
    bool m_cbDirectional_light;
    bool m_cbTotally_photonmap;

public:

    float m_fViewAngle_trace;		// View angle
    CVec3 m_vEye_trace;			// Eye position
    CVec3 m_vCent_trace;			// View center
    CVec3 m_vUpright_trace;		// Upright
    float m_fNear_trace;			// Near culling plane
    float m_fFar_trace;			// Far culling plane   

    CArcBall    m_arcBall;

    float  m_fDistScale;
    float  m_fDistance;
    CVec3   m_vTrans;
    CVec3   m_vCent;
    CVec3   m_vEye;

};

