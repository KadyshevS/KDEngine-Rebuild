#include <kdpch.h>
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

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
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMat);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vArr->Bind();
		RendererCommand::DrawIndexed(vArr);
	}
}