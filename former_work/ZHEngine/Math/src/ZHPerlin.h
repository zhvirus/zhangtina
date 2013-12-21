#pragma once


#include "ZHMathSTD.h"
#include "ZHMathBase.h"

namespace ZH
{

	class ZH_MATH_DLL Perlin
	{
	public:
		//最终的PERLIN NOISE
		static float PerlinNoise_2D(float x, float y);

	private:
		static float persistence;
		static int Number_Of_Octaves;

		//一个噪声发生器
		static float Noise1(int x, int y);

		//一个光滑噪声发生器
		static float SmoothNoise_1(int x, int y);
		//插值函数
		static float Cosine_Interpolate(float a, float b, float x);
		//插值噪声发生器
		static float InterpolatedNoise_1(float x, float y);



	};
	




}