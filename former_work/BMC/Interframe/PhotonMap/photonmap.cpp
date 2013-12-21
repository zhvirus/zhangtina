#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "photonmap.h"

// ---------------------------------------------------------------------------
// Construction/Destruction
// ---------------------------------------------------------------------------
CPhotonMap::CPhotonMap( const int maxPhot )
{
	m_nStoredPhotons = 0;
	m_nPrevScale = 1;
	m_nMaxPhotons = maxPhot;
	
	m_photons = new CPhoton[m_nMaxPhotons + 1];
	if ( m_photons == NULL )
	{
		fprintf( stderr, "Out of memory initializing photon map.\n" );
		exit( EXIT_FAILURE );
	}
	
	m_bboxMin = CVector3f (1e8f, 1e8f, 1e8f);
	m_bboxMax = CVector3f (-1e8f, -1e8f, -1e8f);
	
	for ( int i = 0; i < 256; i++ )
	{
		double angle = (double) i * (1.0/256.0) * PI;
		m_fpCostheta[i] = (float)cos( angle );
		m_fpSintheta[i] = (float)sin( angle );
		m_fpCosphi[i] = (float)cos( 2.0 * angle );
		m_fpSinphi[i] = (float)sin( 2.0 * angle );
	}
}

CPhotonMap::~CPhotonMap()
{
	delete [] m_photons;
}

// ---------------------------------------------------------------------------
void CPhotonMap::writeToDisk( const char * fileName )
{
	FILE * file = fopen( fileName, "wb" );
	
	fwrite( m_photons, sizeof( CPhoton ), m_nStoredPhotons, file );
	fclose( file );
	
	printf( "Wrote %d m_photons to file %s.\n", m_nStoredPhotons, fileName );
}

// ---------------------------------------------------------------------------
void CPhotonMap::readFromDisk( const char * fileName, int numPhotons )
{
	FILE * file = fopen( fileName, "rb" );
	
	if ( file == NULL )
	{
		fprintf( stderr, "Failed to open photon map file %s.\n", fileName );
	}
	
	assert( fread( m_photons, sizeof( CPhoton ), numPhotons, file ) == (UINT)numPhotons );
	fclose( file );
	
	m_nStoredPhotons = numPhotons;
	m_nHalfStoredPhotons = m_nStoredPhotons/2 - 1;
	
	printf( "Read %d m_photons from file %s.\n", numPhotons, fileName );
}

// ---------------------------------------------------------------------------
// Return the direction of a photon
// ---------------------------------------------------------------------------
void CPhotonMap::photonDir( CVector3f * dir, const CPhoton * p ) const
{
	dir->x = m_fpSintheta[p->theta] * m_fpCosphi[p->phi];
	dir->y = m_fpSintheta[p->theta] * m_fpSinphi[p->phi];
	dir->z = m_fpCostheta[p->theta];
}

// ---------------------------------------------------------------------------
// Compute an irradiance estimate at a given surface position
// ---------------------------------------------------------------------------
void CPhotonMap::irradianceEstimate( CColor4f &irrad, const CVector3f pos,
			      const CVector3f normal, const float maxDist, const int nPhotons ) const
{
	irrad = CColor4f(0, 0, 0);
	
	NearestPhotons np;
	np.dist2 = new float [nPhotons + 1];
	np.index = (const CPhoton **) new pCPhoton [nPhotons + 1];
	
	np.pos = pos;
	np.max = nPhotons;
	np.found = 0;
	np.gotHeap = 0;
	np.dist2[0] = maxDist * maxDist;
	
	// locate the nearest photons
	locatePhotons( &np, 1 );

	// if less than 8 photons return
	if ( np.found < 8 )
	{
		delete [] np.dist2;
		delete [] np.index;
		return;
	}
	
	CVector3f dir;
	
	// Sum irradiance from all photons
	for ( int i = 1; i <= np.found; i++ )
	{
		const CPhoton * p = np.index[i];
		photonDir( &dir, p );
		if ( dir.Dot(normal) < 0.0f )
		{
			irrad.r() = irrad.r() + p->power.r();
			irrad.g() = irrad.g() + p->power.g();
			irrad.b() = irrad.b() + p->power.b();
		}
	}
	
	const float tmp = (float)(1.0/PI) / (np.dist2[0]); // estimate of density
	irrad *= tmp;
	
	delete [] np.dist2;
	delete [] np.index;
}


// ---------------------------------------------------------------------------
// Compute an irradiance estimate at a given surface position using gaussian
// filter.
// Yanyun, Oct08, 2003
// ---------------------------------------------------------------------------
void CPhotonMap::GaussianIrradianceEstimate( CColor4f &irrad, const CVector3f pos,
			      const CVector3f normal, const float maxDist, const int nPhotons ) const
{
	irrad = CColor4f(0, 0, 0);
	
	NearestPhotons np;
	np.dist2 = new float [nPhotons + 1];
	np.index = (const CPhoton **) new pCPhoton [nPhotons + 1];
	
	np.pos = pos;
	np.max = nPhotons;
	np.found = 0;
	np.gotHeap = 0;
	np.dist2[0] = maxDist * maxDist;
	
	// locate the nearest photons
	locatePhotons( &np, 1 );

	// if less than 8 photons return
	if ( np.found < 8 )
	{
		delete [] np.dist2;
		delete [] np.index;
		return;
	}
	
	CVector3f dir;
	float r2 = maxDist * maxDist;
	// Sum irradiance from all photons
	for ( int i = 1; i <= np.found; i++ )
	{
		const CPhoton * p = np.index[i];
		photonDir( &dir, p );
		if ( dir.Dot(normal) < 0.0f )
		{
			CVector3f vTemp = p->pos;
			vTemp = vTemp - pos;
			float d2 = vTemp.Dot(vTemp);

			float fWeight = 0.918f * (1 - (1 - exp(-1.953f * d2 / (2*r2))) /
				(1 - exp(-1.953f)));
			irrad.r() = irrad.r() + p->power.r() * fWeight;
			irrad.g() = irrad.g() + p->power.g() * fWeight;
			irrad.b() = irrad.b() + p->power.b() * fWeight;
		}
	}
	
	const float tmp = (float)(1.0/PI) / (np.dist2[0]); // estimate of density
	irrad *= tmp;
	
	delete [] np.dist2;
	delete [] np.index;
}


// ---------------------------------------------------------------------------
// Chen Yanyun Ocr.31, 2002
// ---------------------------------------------------------------------------
void CPhotonMap::MY_irradianceEstimate( CColor4f &irrad, CVector3f pos,
			      const CVector3f normal, const float maxDist, const int nPhotons ) const
{
	irrad = CColor4f(0, 0, 0);
	
	NearestPhotons np;
	np.dist2 = new float [nPhotons + 1];
	np.index = (const CPhoton **) new pCPhoton [nPhotons + 1];
	
	np.pos = pos;
	np.max = nPhotons;
	np.found = 0;
	np.gotHeap = 0;
	np.dist2[0] = maxDist * maxDist;
	
	// locate the nearest photons
	locatePhotons( &np, 1 );

	// if less than 8 photons return
	if ( np.found < 8 )
	{
		delete [] np.dist2;
		delete [] np.index;
		return;
	}
	
	CVector3f dir;
	float fWeight;
	float fTotalWeight = 0;
	int nAvaliablePhoton = 0;
	
	// Sum irradiance from all photons
	for ( int i = 1; i <= np.found; i++ )
	{
		const CPhoton * p = np.index[i];
		photonDir( &dir, p );
		if ( dir.Dot(normal) < 0.0f )
		{
			nAvaliablePhoton ++;

			// Gaussian Filter
			CVector3f vTemp = p->pos;
			vTemp = vTemp - pos;
			float d2 = vTemp.Dot(vTemp);
			float d = (float) sqrt (d2);
			float w2 = maxDist * maxDist;
			float w = (float) sqrt( w2 );
			fWeight = (float)(1/(2.5*w)*exp(-0.5*d2/w2));
//			fTotalWeight += fWeight;

			irrad.r() = irrad.r() + p->power.r() * fWeight;
			irrad.g() = irrad.g() + p->power.g() * fWeight;
			irrad.b() = irrad.b() + p->power.b() * fWeight;
		}
	}
	
	float tmp = (float)(1.0/PI) / (np.dist2[0]); // estimate of density
//	tmp = tmp * nAvaliablePhoton / fTotalWeight;
	irrad *= tmp;
	
	delete [] np.dist2;
	delete [] np.index;
}

// ---------------------------------------------------------------------------
// Chen Yanyun Nov. 10, 2002
// ---------------------------------------------------------------------------
void CPhotonMap::volumetricIrradianceEstimate( CColor4f &irrad, CVector3f pos,
			      const float maxDist, const int nPhotons) const
{
	irrad = CColor4f(0, 0, 0);
	
	NearestPhotons np;
	np.dist2 = new float [nPhotons + 1];
	np.index = (const CPhoton **) new pCPhoton [nPhotons + 1];
	
	np.pos = pos;
	np.max = nPhotons;
	np.found = 0;
	np.gotHeap = 0;
	np.dist2[0] = maxDist * maxDist;
	
	// locate the nearest photons
	locatePhotons( &np, 1 );

	// if less than 8 photons return
	if ( np.found < 2 )
	{
		delete [] np.dist2;
		delete [] np.index;
		return;
	}
	
	CVector3f dir;
	float fWeight;
	float fTotalWeight = 0;
	int nAvaliablePhoton = 0;
	
	// Sum irradiance from all photons
	for ( int i = 1; i <= np.found; i++ )
	{
		const CPhoton * p = np.index[i];
		photonDir( &dir, p );

		nAvaliablePhoton ++;
		
		// Gaussian Filter
		CVector3f vTemp = p->pos;
		vTemp = vTemp - pos;
		float d2 = vTemp.Dot(vTemp);
		float d = (float) sqrt (d2);
		float w2 = maxDist * maxDist;
		float w = (float) sqrt( w2 );
		fWeight = (float)(1/(2.5*w)*exp(-0.5*d2/w2));
		fTotalWeight += fWeight;
		
		irrad.r() = irrad.r() + p->power.r() * fWeight;
		irrad.g() = irrad.g() + p->power.g() * fWeight;
		irrad.b() = irrad.b() + p->power.b() * fWeight;
	}
	

	float tmp = (float)(1.0/PI) / (4 * np.dist2[0] * maxDist / 3); // estimate of density
	tmp = tmp * nAvaliablePhoton / fTotalWeight;
	irrad *= tmp;
	
	delete [] np.dist2;
	delete [] np.index;
}


// ---------------------------------------------------------------------------
// Find the nearest photon in the photon map given the parameters in np
// ---------------------------------------------------------------------------
void CPhotonMap::locatePhotons( NearestPhotons * const np, const int index ) const
{
	const CPhoton * p = &m_photons[index];
	float dist1;
	
	if ( index < m_nHalfStoredPhotons )
	{
		dist1 = np->pos.at(p->plane) - p->pos.at(p->plane);
		
		if ( dist1 > 0.0 )	// if dist1 is positive search right plane
		{
			locatePhotons( np, 2*index+1 );
			if ( dist1 * dist1 < np->dist2[0] ) locatePhotons( np, 2*index );
		}
		else				// dist1 is negative search left first
		{
			locatePhotons( np, 2*index );
			if ( dist1 * dist1 < np->dist2[0] ) locatePhotons( np, 2*index+1 );
		}
	}
	
	// Compute squared distance between current photon and np->pos
	
	CVector3f vtemp = p->pos - np->pos;
	float dist2 = vtemp.Length();
	dist2 = dist2 * dist2;
	
	if ( dist2 < np->dist2[0] )
	{
		// Found a photon.  Insert in the candidate list.
		
		if ( np->found < np->max )
		{
			// Heap is not full.  Use array.
			np->found++;
			np->dist2[np->found] = dist2;
			np->index[np->found] = p;
		}
		else
		{
			int j, parent;
			
			if ( np->gotHeap == 0 )
			{
				// Build heap
				
				float dst2;
				const CPhoton * phot;
				int halfFound = np->found >> 1;
				for ( int k = halfFound; k >= 1; k-- )
				{
					parent = k;
					phot = np->index[k];
					dst2 = np->dist2[k];
					while ( parent <= halfFound )
					{
						j = parent + parent;
						if ( j < np->found && np->dist2[j] < np->dist2[j+1] )
							j++;
						if ( dst2 >= np->dist2[j] )	break;

						np->dist2[parent] = np->dist2[j];
						np->index[parent] = np->index[j];
						parent = j;
					}
					np->dist2[parent] = dst2;
					np->index[parent] = phot;
				}
				np->gotHeap = 1;
			}
			
			// Insert new photon into max heap
			// Delete largest element, insert new, and reorder the heap
			
			parent = 1;
			j = 2;
			while ( j <= np->found )
			{
				if ( j < np->found && np->dist2[j] < np->dist2[j+1] )
					j++;
				if ( dist2 > np->dist2[j] )	break;
				np->dist2[parent] = np->dist2[j];
				np->index[parent] = np->index[j];
				parent = j;
				j += j;
			}
			np->index[parent] = p;
			np->dist2[parent] = dist2;
			
			np->dist2[0] = np->dist2[1];
		}
	}
}

// ---------------------------------------------------------------------------
// Put a photon into the flat array that will form the final kd-tree
// ---------------------------------------------------------------------------
void CPhotonMap::store( const CColor4f power, const CVector3f pos, const CVector3f dir )
{
	if ( m_nStoredPhotons >= m_nMaxPhotons )
		return;
	
	m_nStoredPhotons++;
	CPhoton * const node = &m_photons[m_nStoredPhotons];
	
	node->pos = pos;
	for ( int i = 0; i < 3; i++ )
	{
		if ( node->pos.at(i) < m_bboxMin.at(i) )	m_bboxMin.at(i) = node->pos.at(i);
		if ( node->pos.at(i) > m_bboxMax.at(i) )	m_bboxMax.at(i) = node->pos.at(i);
	}
	node->power = power;
	
	int theta = (int) ( acos( dir.z ) * (256.0/PI) );
	if ( theta > 255 )
		node->theta = 255;
	else
		node->theta = (unsigned char) theta;
	
	int phi = (int) ( atan2( dir.y, dir.x ) * (256.0/(2.0*PI)) );

	if ( phi > 255 )
		node->phi = 255;
	else if ( phi < 0 )
		node->phi = (unsigned char) (phi+256);
	else
		node->phi = (unsigned char) phi;
}

// ---------------------------------------------------------------------------
// used to scale the power of all m_photons once they have emitted from the light
// source. Scale = 1/(#emitted photon).
// Call this function after each light source is processed
// ---------------------------------------------------------------------------
void CPhotonMap::scalePhotonPower( const float scale )
{
	for ( int i = m_nPrevScale; i <= m_nStoredPhotons; i++ )
	{
		m_photons[i].power *= scale;
	}
	m_nPrevScale = m_nStoredPhotons;
}

// ---------------------------------------------------------------------------
// Create a left-balanced kd-tree from the flat photon array
// This function should be called before the photon map is used for rendering.
// ---------------------------------------------------------------------------
void CPhotonMap::balance( void )
{
	int i;

	if ( m_nStoredPhotons > 1 )
	{
		CPhoton ** pa1 = new pCPhoton [m_nStoredPhotons + 1];
		CPhoton ** pa2 = new pCPhoton [m_nStoredPhotons + 1];
		
		for (i = 0; i <= m_nStoredPhotons; i++)
			pa2[i] = &m_photons[i];
		
		balanceSegment( pa1, pa2, 1, 1, m_nStoredPhotons );
		delete [] pa2;
		
		int d, j=1, foo=1;
		CPhoton fooPhoton = m_photons[j];
		
		for (i = 1; i <= m_nStoredPhotons; i++)
		{
			d = pa1[j] - m_photons;
			pa1[j] = NULL;
			if ( d != foo )
				m_photons[j] = m_photons[d];
			else
			{
				m_photons[j] = fooPhoton;
				
				if ( i < m_nStoredPhotons)
				{
					for ( ; foo <= m_nStoredPhotons; foo++ )
					{
						if ( pa1[foo] != NULL )
							break;
					}
					fooPhoton = m_photons[foo];
					j = foo;
				}
				continue;
			}
			j = d;
		}
		delete [] pa1;
	}
	
	m_nHalfStoredPhotons = m_nStoredPhotons/2 - 1;
}

#define swap( ph, a, b ) { CPhoton * ph2 = ph[a]; ph[a] = ph[b]; ph[b] = ph2; }

// ---------------------------------------------------------------------------
// split the photon array into two separate pieces around the median with all
// photon below the median in the lower half and all photon above the median 
// in the upper half. The comparison criteria is the axis (indicate by the
// axis parameter)
// ---------------------------------------------------------------------------
void CPhotonMap::medianSplit( CPhoton ** p, const int start, const int end,
		       const int median, const int axis )
{
	int left = start;
	int right = end;
	int i, j;
	
	while ( right > left )
	{
		const float v = p[right]->pos.at(axis);
		i = left - 1;
		j = right;
		for (;;)
		{
			while ( p[++i]->pos.at(axis) < v )	;
			while ( p[--j]->pos.at(axis) > v && j > left ) ;
			if ( i >= j )	break;
			swap( p, i, j );
		}
		
		swap( p, i, right );
		if ( i >= median )	right = i - 1;
		if ( i <= median )	left = i + 1;
	}
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void CPhotonMap::balanceSegment( CPhoton ** pbal, CPhoton ** porg, const int index,
			  const int start, const int end )
{
	int median = 1;
	
	while ( (4*median) <= (end - start + 1) )
		median += median;
	
	if ( (3*median) <= (end - start + 1) )
	{
		median += median;
		median += start - 1;
	}
	else	median = end - median + 1;
	
	int axis = 2;
	if ( (m_bboxMax.x - m_bboxMin.x) > (m_bboxMax.y - m_bboxMin.y) &&
		(m_bboxMax.x - m_bboxMin.x) > (m_bboxMax.z - m_bboxMin.z) )
		axis = 0;
	else if ( (m_bboxMax.y - m_bboxMin.y) > (m_bboxMax.z - m_bboxMin.z) )
		axis = 1;
	
	medianSplit( porg, start, end, median, axis );
	
	pbal[index] = porg[median];
	pbal[index]->plane = axis;
	
	if ( median > start )
	{
		if ( start < median - 1 )
		{
			const float tmp = m_bboxMax.at(axis);
			m_bboxMax.at(axis) = pbal[index]->pos.at(axis);
			balanceSegment( pbal, porg, 2*index, start, median-1 );
			m_bboxMax.at(axis) = tmp;
		} 
		else	pbal[2*index] = porg[start];
	}
	
	if ( median < end )
	{
		if ( median + 1 < end )
		{
			const float tmp = m_bboxMin.at(axis);
			m_bboxMin.at(axis) = pbal[index]->pos.at(axis);
			balanceSegment( pbal, porg, 2*index+1, median+1, end );
			m_bboxMin.at(axis) = tmp;
		} 
		else
		{
			pbal[2*index+1] = porg[end];
		}
	}
}
