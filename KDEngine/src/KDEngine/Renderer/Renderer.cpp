#include <kdpch.h>
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace KDE
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RendererCommand::Init();
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMat = camera.GetViewProjectionMat();
	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexArray>& vArr, const Ref<Shader>& shader, glm::mat4 transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMat);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vArr->Bind();
		RendererCommand::DrawIndexed(vArr);
	}
}