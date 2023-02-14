#pragma once

#include "RendererCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace KDE
{
	class Renderer
	{
	public:
		static void BeginScene(std::shared_ptr<OrthographicCamera>& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vArr, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMat;
		};
		static SceneData* m_SceneData;
	};
}