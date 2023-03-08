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
	KDE::Ref<KDE::Texture2D> m_Sheet;

	KDE::OrthographicCameraController m_CameraController;
	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
};