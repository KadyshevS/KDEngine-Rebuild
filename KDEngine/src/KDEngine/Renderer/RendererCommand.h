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

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport((int)x, (int)y, (int)width, (int)height);
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
		static Scope<RendererAPI> s_RendererAPI;
	};
}