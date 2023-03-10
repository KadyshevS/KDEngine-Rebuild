#pragma once

#include <KDEngine.h>

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
		Ref<Texture2D> m_Sheet;
		Ref<SubTexture2D> m_Kust;
		Ref<SubTexture2D> m_Tree;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize;

		OrthographicCameraController m_CameraController;
	};
}