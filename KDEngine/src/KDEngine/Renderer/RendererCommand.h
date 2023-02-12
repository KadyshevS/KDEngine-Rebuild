#pragma once

#include "RendererAPI.h"

namespace KDE
{
	class RendererCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vArr)
		{
			s_RendererAPI->DrawIndexed(vArr);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}