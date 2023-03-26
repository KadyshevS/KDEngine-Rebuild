#pragma once

#include "KDEngine/Core.h"
#include "KDEngine/Scene/Scene.h"
#include "KDEngine/Scene/Entity.h"

namespace KDE
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionEntity;
	};
}