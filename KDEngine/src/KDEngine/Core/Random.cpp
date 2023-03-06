#include <kdpch.h>
#include "Random.h"

namespace KDE
{
	std::mt19937 Random::m_MT19937;
	std::uniform_int_distribution<int> Random::u_IntDist;
	std::uniform_real_distribution<float> Random::u_RealDist;

	void Random::Init()
	{
		m_MT19937.seed(std::random_device()());
	}

	int Random::IntDist(int minVal, int maxVal)
	{
		KD_PROFILE_FUNCTION();

		std::uniform_int_distribution<int> Dist(minVal, maxVal);
		return Dist(m_MT19937);
	}
	int Random::Int()
	{
		KD_PROFILE_FUNCTION();

		return u_IntDist(m_MT19937);
	}

	float Random::FloatDist(float minVal, float maxVal)
	{
		KD_PROFILE_FUNCTION();

		std::uniform_real_distribution<float> Dist(minVal, maxVal);
		return Dist(m_MT19937);
	}
	int Random::Float()
	{
		KD_PROFILE_FUNCTION();

		return u_RealDist(m_MT19937);
	}
}