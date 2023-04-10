#include "kdpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "KDEngine/Renderer/Renderer2D.h"
#include "KDEngine/Renderer/EditorCamera.h"
#include <glm/glm.hpp>

namespace KDE
{
	Scene::Scene() {}
	Scene::~Scene() {}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity ent = { m_Registry.create(), this };
		ent.AddComponent<TransformComponent>();
		auto tag = ent.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;
		return ent;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		if (group.size() > 0)
		{
			Renderer2D::BeginScene(camera, camera.GetTransform());

			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.Transform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}
	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.Transform();
					break;
				}
			}
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		if (mainCamera && group.size() > 0)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.Transform(), sprite.Color);
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

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			bool primary = view.get<CameraComponent>(entity).Primary;
			if (primary)
				return Entity{ entity, this };
		}
		return {};
	}
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}