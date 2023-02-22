#pragma once

#include "OrthographicCamera.h"

#include <glm/glm.hpp>

namespace KDE
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	private:
	};
}