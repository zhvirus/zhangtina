#pragma once
#include "common.h"

#define PI_D 6.2831852f
#define PI_H 1.5707963f

class CLocalFrame
{
protected:
	float m_fpSin [1001];
	float m_fpCos [1001];
	float m_fpAsin[1001];
	float m_fpAcos[1001];

	float _my_sin(float fTheta)
	{
		int nIdx = (int) (fTheta / PI_D * 1000);
		if (nIdx > 1000 || nIdx < 0) return 0.f;
		return m_fpSin[nIdx];
	}
	float _my_cos(float fTheta)
	{
		int nIdx = (int) (fTheta / PI_D * 1000);
		if (nIdx > 1000 || nIdx < 0) return 0.f;
		return m_fpCos[nIdx];
	}
	float _my_asin(float fData)
	{
		if (fData > 1.f || fData < -1.f) return 0.f;
		int nIdx = (int)((fData + 1) * 500);
		return m_fpAsin[nIdx];
	}
	float _my_acos(float fData)
	{
		if (fData > 1.f || fData < -1.f) return 0.f;
		int nIdx = (int)((fData + 1) * 500);
		return m_fpAcos[nIdx];
	}

public:
	CVec3 m_vX;
	CVec3 m_vY;
	CVec3 m_vZ;

public:

	// Project the vector into the local frame to get the longitude (fPhi) and altitude (fTheta)
	void Vector_2_theta_phi (CVec3 vIn, float &fTheta, float &fPhi)
	{
		float fPz, fPx, fPy;
		fPz = vIn.Dot(m_vZ);
		if (fPz < 0.f) fPz = 0.f;
		fTheta = _my_acos(fPz);

		vIn = vIn - m_vZ * fPz;
		vIn.Normalize();

		fPx = vIn.Dot(m_vX);
		fPy = vIn.Dot(m_vY);
		if (fPy == 0.f)
		{
			fPhi = (fPx > 0.f)? 0.f : PI;
		}
		else
		{
			fPhi = (fPy > 0.f)? _my_acos(fPx) : PI_D - _my_acos(fPx);
		}
	}

	// Using the the longitude (fPhi) and altitude (fTheta) to calculate the direction by the local frame
	void Theta_phi_2_vector (float fTheta, float fPhi, CVec3 &vOut)
	{
		CVec3 vTemp;
		vTemp.x = _my_sin(fTheta) * _my_cos(fPhi);
		vTemp.y = _my_sin(fTheta) * _my_sin(fPhi);
		vTemp.z = _my_cos(fTheta);

		vOut = m_vX * vTemp.x + m_vY * vTemp.y + m_vZ * vTemp.z;
		vOut.Normalize();
	}

public:
	CLocalFrame(void)
	{
		m_vX = CVec3 (1, 0, 0);
		m_vY = CVec3 (0, 1, 0);
		m_vZ = CVec3 (0, 0, 1);

		for (int i = 0; i <= 1000; i ++)
		{
			m_fpSin[i]  = sin (i / 1000.f * PI_D);
			m_fpCos[i]  = cos (i / 1000.f * PI_D);
			m_fpAsin[i] = asin(-1 + i / 1000.f * 2);
			m_fpAcos[i] = acos(-1 + i / 1000.f * 2);
		}
	}
	virtual ~CLocalFrame(void){};
};
