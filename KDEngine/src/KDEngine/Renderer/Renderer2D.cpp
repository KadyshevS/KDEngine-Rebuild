#include <kdpch.h>
#include "Renderer2D.h"

#include "RendererCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace KDE
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> defaultTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		KD_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->vertexArray = VertexArray::Create();

		float verticesSq[] =
		{
			-0.5f, 0.5f, 0.0f,	0.0f, 1.0f,
			-0.5f,-0.5f, 0.0f,	0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,	1.0f, 1.0f
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(verticesSq, sizeof(verticesSq));

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "inPos" },
			{ ShaderDataType::Float2, "inTexCoord" }
		};
		vertexBuffer->SetLayout(layout);
		s_Data->vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indicesSq[] = { 1, 0, 3,	 2, 1, 3 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indicesSq, sizeof(indicesSq));
		s_Data->vertexArray->SetIndexBuffer(indexBuffer);

		s_Data->defaultTexture = Texture2D::Create(1, 1);
		uint32_t m_DefaultData = 0xFFFFFFFF;
		s_Data->defaultTexture->SetData(&m_DefaultData, sizeof(uint32_t));
		
		s_Data->textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->textureShader->Bind();
		s_Data->textureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::Shutdown()
	{
		KD_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		KD_PROFILE_FUNCTION();

		s_Data->textureShader->Bind();
		s_Data->textureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMat());
	}
	void Renderer2D::EndScene()
	{
		KD_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->textureShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->textureShader->SetMat4("u_Transform", transform);
		s_Data->textureShader->SetFloat4("u_Color", color);
		s_Data->textureShader->SetFloat2("u_Scale", glm::vec2(10.0f));

		s_Data->defaultTexture->Bind();
		s_Data->vertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->vertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		KD_PROFILE_FUNCTION();

		s_Data->textureShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->textureShader->SetMat4("u_Transform", transform);
		s_Data->textureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		s_Data->textureShader->SetFloat2("u_Scale", glm::vec2(10.0f));

		texture->Bind();
		s_Data->vertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->vertexArray);
	}
}