#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace KDE
{
	class KD_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(int x, int y, int width, int height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vArr) = 0;

		inline static const API& GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}