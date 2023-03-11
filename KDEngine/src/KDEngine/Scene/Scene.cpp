#include "kdpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace KDE
{
	struct MeshComponent 
	{
		bool Value;
		MeshComponent() = default;
	};
	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	Scene::Scene()
	{
		entt::entity m_Entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(m_Entity, glm::mat4(1.0f));

		m_Registry.get<TransformComponent>(m_Entity);

		auto m_View = m_Registry.view<TransformComponent>();

		for (auto e : m_View)
		{
			TransformComponent& transform = m_View.get<TransformComponent>(e);
		}

		auto m_Group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
	}
	Scene::~Scene()
	{

	}
}