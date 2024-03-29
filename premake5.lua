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
IncludeDirs["entt"] = "ZeroFramework/Zero/vendor/entt/src/entt"
IncludeDirs["glad"] = "ZeroFramework/Zero/vendor/glad/include"
IncludeDirs["glfw"] = "ZeroFramework/Zero/vendor/glfw/include"
IncludeDirs["glm"] = "ZeroFramework/Zero/vendor/glm/include"
IncludeDirs["imgui"] = "ZeroFramework/Zero/vendor/imgui"
IncludeDirs["lua"] = "ZeroFramework/Zero/vendor/lua/src"
IncludeDirs["rttr"] = "ZeroFramework/Zero/vendor/rttr/src"
IncludeDirs["sfml"] = "ZeroFramework/Zero/vendor/sfml/include"
IncludeDirs["spdlog"] = "ZeroFramework/Zero/vendor/spdlog/include"
IncludeDirs["stb_image"] = "ZeroFramework/Zero/vendor/stb_image/include"

include "ZeroFramework"

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
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
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.freetype}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.glfw}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.lua}",
		"%{IncludeDirs.rttr}",
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

project "ZeroEditor"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.hpp"
	pchsource "ZeroEditor/pch.cpp"

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
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.freetype}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.glfw}",
		"%{IncludeDirs.imgui}",
		"%{IncludeDirs.lua}",
		"%{IncludeDirs.rttr}",
		"%{IncludeDirs.sfml}",
		"%{IncludeDirs.spdlog}",
		"ZeroEditor"
	}

	files {
		"ZeroEditor/**.h",
		"ZeroEditor/**.hpp",
		"ZeroEditor/**.cpp",
		"ZeroEditor/**.inl",
	}

	links { 
		"ZeroFramework"
	}