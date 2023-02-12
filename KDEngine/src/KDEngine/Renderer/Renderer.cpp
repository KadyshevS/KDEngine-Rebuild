#include <kdpch.h>
#include "Renderer.h"

namespace KDE
{
	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vArr)
	{
		vArr->Bind();
		RendererCommand::DrawIndexed(vArr);
	}
}