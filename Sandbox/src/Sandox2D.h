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
	KDE::Ref<KDE::Texture2D> m_Texture;

	KDE::Ref<KDE::OrthographicCameraController> m_CameraController;

	glm::vec3 m_QuadPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec2 m_QuadScale = { 1.0f, 1.0f };
	glm::vec4 u_Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	float m_QuadMoveSpeed = 5.0f;
	float m_QuadScaleSpeed = 3.0f;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
	std::vector<ProfileResult> m_ProfileResults;
};