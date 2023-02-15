#include <kdpch.h>
#include "Renderer.h"

namespace KDE
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(std::shared_ptr<OrthographicCamera>& camera)
	{
		m_SceneData->ViewProjectionMat = camera->GetViewProjectionMat();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vArr, const std::shared_ptr<Shader>& shader, glm::mat4 transform)
	{
		shader->Bind();
		shader->UploadUniformMat4(m_SceneData->ViewProjectionMat, "u_ViewProjection");
		shader->UploadUniformMat4(transform, "u_Transform");

		vArr->Bind();
		RendererCommand::DrawIndexed(vArr);
	}
}