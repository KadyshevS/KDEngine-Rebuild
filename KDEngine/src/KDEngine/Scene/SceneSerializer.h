#pragma once

#include <string>

#include "KDEngine/Renderer/EditorCamera.h"
#include "KDEngine/Core.h"
#include "Scene.h"

namespace KDE
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		SceneSerializer(const Ref<Scene>& scene, const Ref<EditorCamera>& editCamera);

		void Serialize(const std::string& src);
		bool Deserialize(const std::string& src);

	//	TODO:
	//	static void SerializeRuntime(const std::string& src);
	//	static void DeserializeRuntime(const std::string& src);
	private:
		Ref<Scene> m_Scene;
		Ref<EditorCamera> m_EditorCamera;
	};
}