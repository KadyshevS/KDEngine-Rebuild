#pragma once

#include <random>

namespace KDE
{
	class Random
	{
	public:
		static void Init();

		static int IntDist(int minVal, int maxVal);
		static float FloatDist(float minVal, float maxVal);

		static int Int();
		static int Float();
	private:
		static std::mt19937 m_MT19937;
		static std::uniform_int_distribution<int> u_IntDist;
		static std::uniform_real_distribution<float> u_RealDist;
	};
}