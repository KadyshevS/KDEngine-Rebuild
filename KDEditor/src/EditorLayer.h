#pragma once

#include <KDEngine.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatisticsPanel.h"

#include "KDEngine/Renderer/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void LoadScene();
		void SaveSceneAs();
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;
		Ref<EditorCamera> m_EditorCamera;

		Entity m_SquareEntity;
		Entity m_SquareEntityRed;
		Entity m_CameraEntity;
		Entity m_CameraEntity2;

		float m_CameraOrthoSize = 10.0f;
		float m_CameraOrthoSize2 = 10.0f;

		ImGuizmo::OPERATION m_GizmoType = ImGuizmo::OPERATION(-1);

		glm::vec2 m_ViewportSize{0.0f, 0.0f};
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportPos{0.0f};
		glm::vec2 m_ViewportBounds[2];

		glm::vec2 m_MousePos{0.0f};

		int m_PixelData = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}