#pragma once

#include "RendererAPI.h"

namespace KDE
{
	class KD_API RendererCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vArr)
		{
			s_RendererAPI->DrawIndexed(vArr);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}