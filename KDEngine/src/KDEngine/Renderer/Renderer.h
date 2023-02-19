#pragma once

#include "RendererCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace KDE
{
	class KD_API Renderer
	{
	public:
		static void Init();

		static void BeginScene(Ref<OrthographicCamera>& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vArr, const Ref<Shader>& shader, glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMat;
		};
		static SceneData* m_SceneData;
	};
}