#pragma once


#include "ZHMathSTD.h"
#include "ZHMathBase.h"

namespace ZH
{

	class ZH_MATH_DLL Perlin
	{
	public:
		//���յ�PERLIN NOISE
		static float PerlinNoise_2D(float x, float y);

	private:
		static float persistence;
		static int Number_Of_Octaves;

		//һ������������
		static float Noise1(int x, int y);

		//һ���⻬����������
		static float SmoothNoise_1(int x, int y);
		//��ֵ����
		static float Cosine_Interpolate(float a, float b, float x);
		//��ֵ����������
		static float InterpolatedNoise_1(float x, float y);



	};
	




}