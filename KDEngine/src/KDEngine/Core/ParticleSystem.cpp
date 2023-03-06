#include <kdpch.h>
#include "ParticleSystem.h"
#include <KDEngine/Renderer/Renderer2D.h>

#include "Random.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <KDEngine/Core.h>
#include <glad/glad.h>

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000);
}

void ParticleSystem::OnUpdate(KDE::Timestep ts)
{
	for (auto& p : m_ParticlePool)
	{
		if (!p.Active)
			continue;

		if (p.LifeRemaining <= 0.0f)
		{
			p.Active = false;
			continue;
		}

		p.LifeRemaining -= ts;
		p.Position += p.Velocity * (float)ts;
		p.Rotation += 180.0f * ts;
	}
}
void ParticleSystem::OnRender(KDE::OrthographicCamera& camera)
{
	for (auto& p : m_ParticlePool)
	{
		if (!p.Active)
			continue;

		// Fade away particles
		float life = p.LifeRemaining / p.LifeTime;
		glm::vec4 color = glm::lerp(p.ColorEnd, p.ColorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(p.SizeEnd, p.SizeBegin, life);

		KDE::Renderer2D::DrawRotatedQuad(p.Position, {size, size}, p.Rotation, color);
	}
}
void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
