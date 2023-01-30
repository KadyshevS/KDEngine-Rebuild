#include <kdpch.h>
#include "Layer.h"

namespace KDE
{
	Layer::Layer(const std::string& name)
		: m_DebugName(name)
	{}
	Layer::~Layer() {}
}