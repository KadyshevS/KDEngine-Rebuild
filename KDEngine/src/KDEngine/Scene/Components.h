#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace KDE
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
		
		operator const char* () const { return Tag.c_str(); }
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		KDE::SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator bool () { return Primary; }
		operator const bool& () const { return Primary; }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> CreateInstanceFunc;
		std::function<void()> DestroyInstanceFunc;

		std::function<void(ScriptableEntity*)> OnCreateFunc;
		std::function<void(ScriptableEntity*)> OnDestroyFunc;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunc;

		template<typename T>
		void Bind()
		{
			CreateInstanceFunc = [&]() { Instance = new T(); };
			DestroyInstanceFunc = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunc = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunc = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunc = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		}
	};
}