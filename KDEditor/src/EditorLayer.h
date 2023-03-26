#pragma once

#include <KDEngine.h>

#include "Panels/SceneHierarchyPanel.h"

namespace KDE
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;

		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_CameraEntity2;

		bool m_CameraSwitch = true;
		float m_CameraOrthoSize = 10.0f;
		float m_CameraOrthoSize2 = 10.0f;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportPos{ 0.0f };

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}