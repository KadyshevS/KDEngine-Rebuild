#pragma once

#include <random>

namespace KDE
{
	class Random
	{
	public:
		static void Init();

	//	Use carefully, uniform distribution created again for setting limits of generations
		static int CreateInt(int minVal, int maxVal);
	//	Use carefully, uniform distribution created again for setting limits of generations
		static float CreateFloat(float minVal, float maxVal);

		static int CreateIntFast();
		static int CreateFloatFast();
	private:
		static std::mt19937 m_MT19937;
		static std::uniform_int_distribution<int> u_IntDist;
		static std::uniform_real_distribution<float> u_RealDist;
	};
}