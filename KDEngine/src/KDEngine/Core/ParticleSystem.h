#pragma once

#include <glm/glm.hpp>

#include "Timestep.h"
#include <KDEngine/Renderer/OrthographicCamera.h>
#include <KDEngine/Renderer/Shader.h>

#include <vector>

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void OnUpdate(KDE::Timestep ts);
	void OnRender(KDE::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd, SizeVariation;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;

	uint32_t m_QuadVA = 0;
	std::unique_ptr<KDE::Shader> m_ParticleShader;
	int m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};