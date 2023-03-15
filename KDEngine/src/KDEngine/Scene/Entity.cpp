#include "kdpch.h"
#include "Entity.h"

namespace KDE
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{}
}