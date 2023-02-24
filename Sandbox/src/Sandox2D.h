#pragma once

#include <KDEngine.h>

#include <random>

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
	KDE::Ref<KDE::OrthographicCameraController> m_Camera;

	KDE::Ref<KDE::Texture2D> m_Texture;
	KDE::Ref<KDE::Texture2D> m_BackTexture;
	KDE::Ref<KDE::Texture2D> m_PipeTexture;

	std::mt19937 mt;
	std::random_device rd;
	std::uniform_real_distribution<float> rng;

	glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
	float m_Rotation = 0.0f;

	float m_BirdUpSpeed = 1.0f;
	float m_BirdDownSpeed = 1.0f;
	float m_BirdRotSpeed = 360.0f * 2.0f;
	float m_PipesSpeed = 0.3f;

	float m_BirdRotLimitUp = 1.0f;
	float m_BirdRotLimitDown = -1.0f;

	float m_DistanceBetweenPipes = 0.8f;
	float m_PipesGap = 2.0f;

	float m_BirdSpeed = 0.8f;

	bool m_GameOver = false;

	struct Pipe
	{
		glm::vec3 position;
	};

	Pipe pipes[7];
};