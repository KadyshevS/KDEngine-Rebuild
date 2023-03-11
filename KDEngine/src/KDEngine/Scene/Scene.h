#pragma once

#include "entt.hpp"

namespace KDE
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;
	};
}