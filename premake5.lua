workspace "Sandbox"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["assimp"] = "ZeroFramework/Zero/vendor/assimp/include"
IncludeDirs["freetype"] = "ZeroFramework/Zero/vendor/freetype/include"
IncludeDirs["glad"] = "ZeroFramework/Zero/vendor/glad/include"
IncludeDirs["glfw"] = "ZeroFramework/Zero/vendor/glfw/include"
IncludeDirs["glm"] = "ZeroFramework/Zero/vendor/glm/include"
IncludeDirs["imgui"] = "ZeroFramework/Zero/vendor/imgui"
IncludeDirs["sfml"] = "ZeroFramework/Zero/vendor/sfml/include"
IncludeDirs["spdlog"] = "ZeroFramework/Zero/vendor/spdlog/include"
IncludeDirs["stb_image"] = "ZeroFramework/Zero/vendor/stb_image/include"

--group "Dependencies"
--	include "ZeroFramework/Zero/vendor/assimp"
--	include "ZeroFramework/Zero/vendor/freetype"
--	include "ZeroFramework/Zero/vendor/glad"
--	include "ZeroFramework/Zero/vendor/glfw"
--	include "ZeroFramework/Zero/vendor/glm"
--	include "ZeroFramework/Zero/vendor/imgui"
--	include "ZeroFramework/Zero/vendor/sfml"
--	include "ZeroFramework/Zero/vendor/spdlog"
--	include "ZeroFramework/Zero/vendor/stb_image"00
--group ""

include "ZeroFramework"

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++11"
	staticruntime "on"
	
	targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Sandbox/pch.cpp"

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD",
		"SFML_STATIC"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"
	filter {}
	
	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "On"
	filter {}

	includedirs {
		"ZeroFramework",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.freetype}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.glfw}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.sfml}",
		"%{IncludeDirs.spdlog}",
		"Sandbox"
	}

	files {
		"Sandbox/**.h",
		"Sandbox/**.hpp",
		"Sandbox/**.cpp",
		"Sandbox/**.inl",
	}

	links { 
		"ZeroFramework"
	}
