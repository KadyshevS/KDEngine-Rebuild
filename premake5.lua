workspace "KDEngine"
	architecture "x64"
	startproject "Sandbox"

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

include "KDEngine/vendor/GLFW"
include "KDEngine/vendor/glad"
include "KDEngine/vendor/imgui"

project "KDEngine"
	location "KDEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	pchheader "kdpch.h"
	pchsource "KDEngine/src/kdpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
	}

	links 
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"KD_PLATFORM_WINDOWS",
			"KD_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		defines "KD_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "KD_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "KD_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"KDEngine/src"
	}

	links
	{
		"KDEngine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"KD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "KD_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "KD_DIST"
		runtime "Release"
		optimize "On"