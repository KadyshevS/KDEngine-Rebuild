#pragma once

#include <KDEngine.h>

class Sandbox2D : public KDE::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() {}
	
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(KDE::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(KDE::Event& e) override;
private:
	KDE::Ref<KDE::ShaderLibrary> m_ShaderLibrary;

	KDE::Ref<KDE::VertexBuffer> m_SQVertexBuffer;
	KDE::Ref<KDE::IndexBuffer> m_SQIndexBuffer;
	KDE::Ref<KDE::VertexArray> m_SQVertexArray;

	KDE::Ref<KDE::Texture2D> m_Texture;

	KDE::Ref<KDE::OrthographicCameraController> m_CameraController;

	glm::vec2 m_QuadPosition = { 0.0f, 0.0f };
	glm::vec2 m_QuadScale = { 1.0f, 1.0f };

	glm::vec3 u_Color = { 0.0f, 1.0f, 0.0f };

	float m_QuadMoveSpeed = 5.0f;
	float m_QuadScaleSpeed = 3.0f;
};