#include "kdpch.h"
#include "SceneSerializer.h"

#include "yaml-cpp/yaml.h"
#include "glm/glm.hpp"

#include "Entity.h"
#include "Components.h"

#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace KDE
{
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{}
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene, const Ref<EditorCamera>& editCamera)
		: m_Scene(scene), m_EditorCamera(editCamera)
	{}

	static void SerializeEntity(YAML::Emitter& out, Entity ent)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273";

		if (ent.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			
			auto& tag = ent.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}
		if (ent.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = ent.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}
		if (ent.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = ent.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthoSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthoNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthoFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}
		if (ent.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = ent.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& src)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";

		out << YAML::Key << "EditorCamera" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value << m_EditorCamera->GetPosition();
		out << YAML::Key << "Orientation" << YAML::Value << m_EditorCamera->GetOrientation();
		out << YAML::Key << "PerspectiveFOV" << YAML::Value << m_EditorCamera->GetFOV();
		out << YAML::Key << "PerspectiveNear" << YAML::Value << m_EditorCamera->GetNearClip();
		out << YAML::Key << "PerspectiveFar" << YAML::Value << m_EditorCamera->GetFarClip();
		out << YAML::EndMap;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		
		m_Scene->m_Registry.each(
			[&](auto entID)
			{
				Entity ent = { entID, m_Scene.get() };
				if (!ent)
					return;

				SerializeEntity(out, ent);
			}
		);
		out << YAML::EndSeq << YAML::EndMap;

		std::ofstream fout(src);
		fout << out.c_str();
	}
	bool SceneSerializer::Deserialize(const std::string& src)
	{
		std::ifstream stream(src);
		KD_CORE_ASSERT(stream.is_open(), "Failed to open file {0}", src.c_str());

		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		if (auto editCameraData = data["EditorCamera"]; editCameraData.IsDefined())
		{
			glm::vec3 editCamPos = editCameraData["Position"].as<glm::vec3>();
			glm::vec3 editCamOri = editCameraData["Orientation"].as<glm::vec3>();
			float editCamFov = editCameraData["PerspectiveFOV"].as<float>();
			float editCamNear = editCameraData["PerspectiveNear"].as<float>();
			float editCamFar = editCameraData["PerspectiveFar"].as<float>();
			m_EditorCamera->SetPosition(editCamPos);
			m_EditorCamera->SetOrientation(editCamOri);
			m_EditorCamera->SetFOV(editCamFov);
			m_EditorCamera->SetNearClip(editCamNear);
			m_EditorCamera->SetFarClip(editCamFar);
		}

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto ent : entities)
			{
				uint64_t uuid = ent["Entity"].as<uint64_t>();

				std::string name;
				auto tagComp = ent["TagComponent"];
				if (tagComp)
					name = tagComp["Tag"].as<std::string>();

				Entity deserEntity = m_Scene->CreateEntity(name);

				auto transformComp = ent["TransformComponent"];
				if (transformComp)
				{
					auto& tc = deserEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComp["Translation"].as<glm::vec3>();
					tc.Rotation = transformComp["Rotation"].as<glm::vec3>();
					tc.Scale = transformComp["Scale"].as<glm::vec3>();
				}

				auto camComp = ent["CameraComponent"];
				if (camComp)
				{
					auto& cc = deserEntity.AddComponent<CameraComponent>();

					const auto& cameraProps = camComp["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthoSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthoNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthoFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = camComp["Primary"].as<bool>();
					cc.FixedAspectRatio = camComp["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = ent["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}
}