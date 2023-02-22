#include <kdpch.h>
#include "Renderer2D.h"

#include "RendererCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace KDE
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->vertexArray = VertexArray::Create();

		float verticesSq[] =
		{
			-0.5f, 0.5f, 0.0f,
			-0.5f,-0.5f, 0.0f,
			 0.5f,-0.5f, 0.0f,
			 0.5f, 0.5f, 0.0f,
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(verticesSq, sizeof(verticesSq));

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "inPos" }
		};
		vertexBuffer->SetLayout(layout);
		s_Data->vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indicesSq[] = { 1, 0, 3,	 2, 1, 3 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indicesSq, sizeof(indicesSq));
		s_Data->vertexArray->SetIndexBuffer(indexBuffer);

		s_Data->vertexArray->Unbind();
		vertexBuffer->Unbind();
		indexBuffer->Unbind();

		s_Data->shader = Shader::Create("assets/shaders/ColorSet.glsl");
	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->shader)->UploadUniformMat4(
			"u_ViewProjection", camera.GetViewProjectionMat()
		);
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->shader)->UploadUniformMat4(
			"u_Transform", glm::mat4(1.0f)
		);
	}
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->shader)->UploadUniformFloat3("u_Color", color);

		s_Data->vertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->vertexArray);
	}
}