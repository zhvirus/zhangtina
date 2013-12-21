// Color.h: interface for the CColor4f class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __PHOTON_MAP_FROM_Henrik_Wann_Jensen__2002_10_18
#define __PHOTON_MAP_FROM_Henrik_Wann_Jensen__2002_10_18

#include "Color.h"
#include "vector3f.h"


//////////////////////////////////////////////////////////////////////////////
// This is the photon 
// The power is not compressed so the size is 28 bytes
typedef struct CPhoton
{
	CVector3f pos;				// photon position
	short plane;				// splitting plane for kd-tree
	unsigned char theta, phi;	// incoming direction
	CColor4f power;				// photon power (uncompressed)
} CPhoton, *pCPhoton; 

//////////////////////////////////////////////////////////////////////////////
// This structure is used to locate the nearest photon
typedef struct NearestPhotons
{
	int max;
	int found;
	int gotHeap;
	CVector3f pos;
	float *dist2;
	const CPhoton **index;
} NearestPhotons;

//////////////////////////////////////////////////////////////////////////////
// This is the photon map class
class CPhotonMap
{
public:
	CPhotonMap( const int maxPhot );
	virtual ~CPhotonMap();
	
	void store(	const CColor4f power,				// photon power 
			const CVector3f pos,					// photon position
			const CVector3f dir );				// photon direction

	void scalePhotonPower( const float scale );	// scale : 1/(number of emitted photon
	
	void balance( void );						// balance the kd-tree

	void irradianceEstimate( CColor4f &irrad,	// return irradiance
				const CVector3f pos,			// surface position
				const CVector3f normal,			// surface normal at pos
				const float maxDist,			// max distance to look for m_photons
				const int nPhotons ) const;		// number of photon to use

	// Yanyun Oct08, 2003 
	void GaussianIrradianceEstimate( CColor4f &irrad,	// return irradiance
				const CVector3f pos,			// surface position
				const CVector3f normal,			// surface normal at pos
				const float maxDist,			// max distance to look for m_photons
				const int nPhotons ) const;		// number of photon to use

	void MY_irradianceEstimate( CColor4f &irrad,	// Chen Yanyun Oct31, 2002 return irradiance
				CVector3f pos,					// Chen Yanyun Oct31, 2002 surface position
				const CVector3f normal,			// Chen Yanyun Oct31, 2002 surface normal at pos
				const float maxDist,			// Chen Yanyun Oct31, 2002 max distance to look for m_photons
				const int nPhotons ) const;		// Chen Yanyun Oct31, 2002 number of photon to use

	void volumetricIrradianceEstimate( CColor4f &irrad,	// Chen Yanyun Nov10, 2002 return irradiance
				CVector3f pos,					// Chen Yanyun Nov10, 2002 surface position
				const float maxDist,			// Chen Yanyun Nov10, 2002 max distance to look for m_photons
				const int nPhotons) const;		// Chen Yanyun Nov10, 2002 number of photon to use

	void locatePhotons( NearestPhotons * const np,	// np is used to locate the m_photons
			const int index ) const;			// call with index = 1

	void photonDir(	CVector3f *dir,				// direction of photon (returned)
			const CPhoton * p ) const;			// the photon

	void writeToDisk( const char * fileName );
	void readFromDisk( const char * fileName, int numPhotons ); 
	CPhoton * m_photons;
	
protected:
	void balanceSegment( CPhoton **pbal, CPhoton **porg, const int index,
		const int start, const int end );
	void medianSplit( CPhoton **p, const int start, const int end,
		const int median, const int axis );
	
	int m_nStoredPhotons;
	int m_nHalfStoredPhotons;
	int m_nMaxPhotons;
	int m_nPrevScale;
	float m_fpCostheta[256];
	float m_fpSintheta[256];
	float m_fpCosphi[256];
	float m_fpSinphi[256];
	CVector3f m_bboxMin;
	CVector3f m_bboxMax;
}; 

#endif //__PHOTON_MAP_FROM_Henrik_Wann_Jensen__2002_10_18