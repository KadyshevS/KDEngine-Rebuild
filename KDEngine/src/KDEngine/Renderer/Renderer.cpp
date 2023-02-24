#include <kdpch.h>
#include "Renderer.h"

#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace KDE
{
	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		KD_PROFILE_FUNCTION();

		RendererCommand::Init();
		Renderer2D::Init();
	}
	void Renderer::Shutdown()
	{
		KD_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
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
		KD_PROFILE_FUNCTION();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMat);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vArr->Bind();
		RendererCommand::DrawIndexed(vArr);
	}
}