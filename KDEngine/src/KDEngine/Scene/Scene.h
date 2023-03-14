#pragma once

#include "entt.hpp"

#include "KDEngine/Core/Timestep.h"

namespace KDE
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};
}