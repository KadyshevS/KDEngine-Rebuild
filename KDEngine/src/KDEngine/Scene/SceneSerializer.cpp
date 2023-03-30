#include "kdpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>

namespace KDE
{
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
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

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& src)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		
		m_Scene->m_Registry.each(
			[&](auto entityID)
			{
				Entity ent = { entityID, m_Scene.get() };
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
		return false;
	}
}