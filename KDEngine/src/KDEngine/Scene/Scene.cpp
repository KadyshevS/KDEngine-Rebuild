#include "kdpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity ent = { m_Registry.create(), this };

		auto& tag = ent.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;

		ent.AddComponent<TransformComponent>();

		return ent;
	}
	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto& ent : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(ent);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto ent : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(ent);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}
}