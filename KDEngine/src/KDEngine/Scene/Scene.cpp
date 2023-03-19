#include "kdpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "KDEngine/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace KDE
{
	Scene::Scene() {}
	Scene::~Scene() {}

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
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto& ent : view)
		{
			auto& camComponent = view.get<CameraComponent>(ent);
			if (!camComponent.FixedAspectRatio)
				camComponent.Camera.SetViewportSize(width, height);
		}
	}
}