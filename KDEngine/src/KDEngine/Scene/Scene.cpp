#include "kdpch.h"
#include "Scene.h"

#include "Components.h"

#include "KDEngine/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace KDE
{
	Scene::Scene()
	{
		/*
			entt::entity m_Entity = m_Registry.create();
			m_Registry.emplace<TransformComponent>(m_Entity, glm::mat4(1.0f));

			m_Registry.get<TransformComponent>(m_Entity);

			auto m_View = m_Registry.view<TransformComponent>();

			for (auto e : m_View)
			{
				TransformComponent& transform = m_View.get<TransformComponent>(e);
			}

			auto m_Group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		*/
	}
	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto ent : group)
		{
			auto& transform = group.get<TransformComponent>(ent);
			auto& sprite = group.get<SpriteRendererComponent>(ent);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}