#pragma once

#include "KDEngine/Renderer/RendererAPI.h"

namespace KDE
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;
		virtual void SetViewport(int x, int y, int width, int height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vArr, uint32_t indexCount = 0) override;
	};
}