#pragma once
#include "math.h"

#define PI_D 6.2831852f
#define PI_H 1.5707963f

#define FP_EXP(e,p)                                                          \
{                                                                            \
    int _i;                                                                  \
    e = -1.44269504f * (float)0x00800000 * (p);                              \
    _i = (int)e + 0x3F800000;                                                \
    e = *(float *)&_i;                                                       \
}
#define FP_BITS(fp) (*(DWORD *)&(fp))


typedef union FastSqrtUnion
{
  float f;
  unsigned int i;
} FastSqrtUnion;


class CFastMath
{
private:
	static float m_fpSin [1001];
	static float m_fpCos [1001];
	static float m_fpAsin[1001];
	static float m_fpAcos[1001];
	static UINT  fast_sqrt_table[0x10000];

	void  _build_sqrt_table()
	{
		unsigned int i;
		FastSqrtUnion s;

		for (i = 0; i <= 0x7FFF; i++)
		{

			// Build a float with the bit pattern i as mantissa
			//  and an exponent of 0, stored as 127

			s.i = (i << 8) | (0x7F << 23);
			s.f = (float)sqrt(s.f);

			// Take the square root then strip the first 7 bits of
			//  the mantissa into the table

			fast_sqrt_table[i + 0x8000] = (s.i & 0x7FFFFF);

			// Repeat the process, this time with an exponent of 1, 
			//  stored as 128

			s.i = (i << 8) | (0x80 << 23);
			s.f = (float)sqrt(s.f);

			fast_sqrt_table[i] = (s.i & 0x7FFFFF);
		}
	}

	void  _build_sin_cos_asin_acon_table()
	{
		for (int i = 0; i <= 1000; i ++)
		{
			m_fpSin[i]  = sin (i / 1000.f * PI_D);
			m_fpCos[i]  = cos (i / 1000.f * PI_D);
			m_fpAsin[i] = asin(-1 + i / 1000.f * 2);
			m_fpAcos[i] = acos(-1 + i / 1000.f * 2);
		}
	}

public:
	void  fm_init(void)
	{
		_build_sqrt_table();
		_build_sin_cos_asin_acon_table();
	}

	float fm_sin(float fTheta)
	{
		int nIdx = (int) (fTheta / PI_D * 1000);
		if (nIdx > 1000 || nIdx < 0) return 0.f;
		return m_fpSin[nIdx];
	}

	float fm_cos(float fTheta)
	{
		int nIdx = (int) (fTheta / PI_D * 1000);
		if (nIdx > 1000 || nIdx < 0) return 0.f;
		return m_fpCos[nIdx];
	}

	float fm_asin(float fData)
	{
		if (fData > 1.f || fData < -1.f) return 0.f;
		int nIdx = (int)((fData + 1) * 500);
		return m_fpAsin[nIdx];
	}

	float fm_acos(float fData)
	{
		if (fData > 1.f || fData < -1.f) return 0.f;
		int nIdx = (int)((fData + 1) * 500);
		return m_fpAcos[nIdx];
	}

	float fm_sqrt(float n)
	{

		if (FP_BITS(n) == 0)
			return 0.0f;                 // check for square root of 0

		FP_BITS(n) = fast_sqrt_table[(FP_BITS(n) >> 8) & 0xFFFF] | ((((FP_BITS(n) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);

		return n;
	}
};