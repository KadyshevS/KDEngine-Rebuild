#pragma once

#include "entt.hpp"

#include "KDEngine/Core/Timestep.h"

namespace KDE
{
	class Entity;
	class EditorCamera;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCamera();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}