workspace "KDEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "KDEngine"
	location "KDEngine"
	kind "SharedLib"
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
		"KDEngine/spdlog/include",
		"KDEngine/src"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"KD_PLATFORM_WINDOWS",
			"KD_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "KD_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KD_DIST"
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
		"KDEngine/spdlog/include",
		"KDEngine/src"
	}

	links
	{
		"KDEngine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"KD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "KD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "KD_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KD_DIST"
		optimize "On"