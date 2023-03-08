#include <kdpch.h>
#include "Renderer2D.h"

#include "RendererCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace KDE
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TexScalingFactor;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadTextureShader;
		Ref<Texture2D> QuadDefaultTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 - default texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		KD_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "inPos" },
			{ ShaderDataType::Float4, "inColor" },
			{ ShaderDataType::Float2, "inTexCoord" },
			{ ShaderDataType::Float, "inTexIndex" },
			{ ShaderDataType::Float, "inTexScalingFactor" }
		};
		s_Data.QuadVertexBuffer->SetLayout(layout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] quadIndices;

		s_Data.QuadDefaultTexture = Texture2D::Create(1, 1);
		uint32_t m_DefaultData = 0xFFFFFFFF;
		s_Data.QuadDefaultTexture->SetData(&m_DefaultData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.QuadTextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.QuadDefaultTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}
	void Renderer2D::Shutdown()
	{
		KD_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		KD_PROFILE_FUNCTION();

		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMat());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}
	void Renderer2D::EndScene()
	{
		KD_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}
	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RendererCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Data.Stats.DrawCalls++;
	}
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		KD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f;
		const float scalingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TexScalingFactor = scalingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, float scalingFactor, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, scalingFactor, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, float scalingFactor, const glm::vec4& tintColor)
	{
		KD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr float sheetWidth = 2560, sheetHeight = 1664;
		constexpr float texWidth = 128, texHeight = 128;
		constexpr float texX = 0, texY = 3;

		constexpr glm::vec2 texCoords[4] =
		{
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f},
		};

		constexpr glm::vec4 inColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TexScalingFactor = scalingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, float scalingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, scalingFactor, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, float scalingFactor, const glm::vec4& tintColor)
	{
		KD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		constexpr glm::vec4 inColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TexScalingFactor = scalingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		KD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;
		const float scalingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TexScalingFactor = scalingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, float scalingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, scalingFactor, tintColor);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, float scalingFactor, const glm::vec4& tintColor)
	{
		KD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 inColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TexScalingFactor = scalingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subtexture, float scalingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, scalingFactor, tintColor);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subtexture, float scalingFactor, const glm::vec4& tintColor)
	{
		KD_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		constexpr glm::vec4 inColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TexScalingFactor = scalingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}