#pragma once

#include "RendererCommand.h"

namespace KDE
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vArr);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	};
}