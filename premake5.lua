workspace "KDEngine"
	architecture "x64"
	startproject "KDEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "KDEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "KDEngine/vendor/glad/include"
IncludeDir["ImGui"] = "KDEngine/vendor/imgui"
IncludeDir["glm"] = "KDEngine/vendor/glm"
IncludeDir["stb"] = "KDEngine/vendor/stb"
IncludeDir["entt"] = "KDEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "KDEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "KDEngine/vendor/ImGuizmo"

group "Dependencies"
	include "KDEngine/vendor/GLFW"
	include "KDEngine/vendor/glad"
	include "KDEngine/vendor/imgui"
	include "KDEngine/vendor/yaml-cpp"
group ""

project "KDEngine"
	location "KDEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	pchheader "kdpch.h"
	pchsource "KDEngine/src/kdpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb/stb_image.h",
		"%{prj.name}/vendor/stb/stb_image.cpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links 
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}
	
	filter "files:KDEngine/vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "KD_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"KDEngine/vendor/spdlog/include",
		"KDEngine/src",
		"KDEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"KDEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "KD_DIST"
		runtime "Release"
		optimize "on"
		
project "KDEditor"
	location "KDEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"KDEngine/vendor/spdlog/include",
		"KDEngine/src",
		"KDEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"KDEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "KD_DIST"
		runtime "Release"
		optimize "on"