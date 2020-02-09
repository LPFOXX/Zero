project "ASSIMP"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"
    
    defines {
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
        "OPENDDL_STATIC_LIBARY",
        "OPENDDLPARSER_BUILD"
    }

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "Zero/vendor/assimp",
        "Zero/vendor/assimp/include",
        "Zero/vendor/assimp/code",
        "Zero/vendor/assimp/contrib/clipper",
        "Zero/vendor/assimp/contrib/irrXML",
        "Zero/vendor/assimp/contrib/openddlparser/include",        
        "Zero/vendor/assimp/contrib/poly2tri/poly2tri",        
        "Zero/vendor/assimp/contrib/rapidjson/include",        
        "Zero/vendor/assimp/contrib/zlib",
        "Zero/vendor/assimp/contrib/unzip",
        --"contrib/Melange/includes" -- to build C4D file support
    }

    files {
        ---- Assimp files
        "Zero/vendor/assimp/code/**.c",
        "Zero/vendor/assimp/code/**.cpp",
        "Zero/vendor/assimp/code/**.h",
        "Zero/vendor/assimp/code/**.hpp",

        ---- clipper files
        "Zero/vendor/assimp/contrib/clipper/*.cpp",
        ---- opendllparser files
        "Zero/vendor/assimp/contrib/openddlparser/code/*.cpp", 
        ---- poly2tri
        "Zero/vendor/assimp/contrib/poly2tri/poly2tri/common/*.cc",
        "Zero/vendor/assimp/contrib/poly2tri/poly2tri/sweep/*.cc",
        ---- zlib files
        "Zero/vendor/assimp/contrib/zlib/*.c",
        ---- unzip files
        "Zero/vendor/assimp/contrib/unzip/*.c",
        ---- irrXML files
        "Zero/vendor/assimp/contrib/irrXML/*.cpp",
    }

    removefiles "**/IFCReaderGen_4.cpp"

    filter "action:vs*"
        buildoptions {
            "/bigobj"
        }

        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "_SCL_SECURE_NO_WARNINGS"
        }
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
		runtime "Debug"
        symbols "on"
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
		runtime "Release"
        optimize "on"
    filter {}

project "FREETYPE"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    defines {
        "FT2_BUILD_LIBRARY"
    }

    filter "action:vs*"
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }
    filter {}
    
    includedirs {
        "Zero/vendor/freetype/include"
    }

    files {
        "Zero/vendor/freetype/src/autofit/autofit.c",
        "Zero/vendor/freetype/src/base/ftbase.c",
        "Zero/vendor/freetype/src/base/ftbbox.c",
        "Zero/vendor/freetype/src/base/ftbdf.c",
        "Zero/vendor/freetype/src/base/ftbitmap.c",
        "Zero/vendor/freetype/src/base/ftcid.c",
        "Zero/vendor/freetype/src/base/ftfntfmt.c",
        "Zero/vendor/freetype/src/base/ftfstype.c",
        "Zero/vendor/freetype/src/base/ftgasp.c",
        "Zero/vendor/freetype/src/base/ftglyph.c",
        "Zero/vendor/freetype/src/base/ftgxval.c",
        "Zero/vendor/freetype/src/base/ftinit.c",
        "Zero/vendor/freetype/src/base/ftlcdfil.c",
        "Zero/vendor/freetype/src/base/ftmm.c",
        "Zero/vendor/freetype/src/base/ftotval.c",
        "Zero/vendor/freetype/src/base/ftpatent.c",
        "Zero/vendor/freetype/src/base/ftpfr.c",
        "Zero/vendor/freetype/src/base/ftstroke.c",
        "Zero/vendor/freetype/src/base/ftsynth.c",
        "Zero/vendor/freetype/src/base/ftsystem.c",
        "Zero/vendor/freetype/src/base/fttype1.c",
        "Zero/vendor/freetype/src/base/ftwinfnt.c",
        "Zero/vendor/freetype/src/bdf/bdf.c",
        "Zero/vendor/freetype/src/bzip2/ftbzip2.c",
        "Zero/vendor/freetype/src/cache/ftcache.c",
        "Zero/vendor/freetype/src/cff/cff.c",
        "Zero/vendor/freetype/src/cid/type1cid.c",
        "Zero/vendor/freetype/src/gzip/ftgzip.c",
        "Zero/vendor/freetype/src/lzw/ftlzw.c",
        "Zero/vendor/freetype/src/pcf/pcf.c",
        "Zero/vendor/freetype/src/pfr/pfr.c",
        "Zero/vendor/freetype/src/psaux/psaux.c",
        "Zero/vendor/freetype/src/pshinter/pshinter.c",
        "Zero/vendor/freetype/src/psnames/psnames.c",
        "Zero/vendor/freetype/src/raster/raster.c",
        "Zero/vendor/freetype/src/sfnt/sfnt.c",
        "Zero/vendor/freetype/src/smooth/smooth.c",
        "Zero/vendor/freetype/src/truetype/truetype.c",
        "Zero/vendor/freetype/src/type1/type1.c",
        "Zero/vendor/freetype/src/type42/type42.c",
        "Zero/vendor/freetype/src/winfonts/winfnt.c"
    }

    filter "system:windows"
        files {
            "Zero/vendor/freetype/builds/windows/ftdebug.c"
        }
    filter {}

    filter "system:not windows"
        files {
            "Zero/vendor/freetype/src/base/ftdebug.c"
        }
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
		runtime "Debug"
        symbols "on"

        files {
           --- "Zero/vendor/freetype/src/gzip/infcodes.c"
        }
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
		runtime "Release"
        optimize "on"
    filter {}

project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "Zero/vendor/glad/include"
    }

    files {
        "%{IncludeDirs.glad}/glad/glad.h",
        "%{IncludeDirs.glad}/KHR/khrplatform.h",
        "Zero/vendor/glad/src/glad.c"
    }

    filter "system:windows"
        systemversion "latest"
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"
    filter {}

project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "%{IncludeDirs.glfw}"
    }

    files {
		"%{IncludeDirs.glfw}/GLFW/glfw3.h",
		"%{IncludeDirs.glfw}/GLFW/glfw3native.h",
		"Zero/vendor/glfw/src/glfw_config.h",
		"Zero/vendor/glfw/src/context.c",
		"Zero/vendor/glfw/src/init.c",
		"Zero/vendor/glfw/src/input.c",
		"Zero/vendor/glfw/src/monitor.c",
		"Zero/vendor/glfw/src/vulkan.c",
		"Zero/vendor/glfw/src/window.c"
    }
    
	filter "system:linux"
        pic "On"
        
        systemversion "latest"
        staticruntime "On"

        files {
            "Zero/vendor/glfw/src/x11_init.c",
            "Zero/vendor/glfw/src/x11_monitor.c",
            "Zero/vendor/glfw/src/x11_window.c",
            "Zero/vendor/glfw/src/xkb_unicode.c",
            "Zero/vendor/glfw/src/posix_time.c",
            "Zero/vendor/glfw/src/posix_thread.c",
            "Zero/vendor/glfw/src/glx_context.c",
            "Zero/vendor/glfw/src/egl_context.c",
            "Zero/vendor/glfw/src/osmesa_context.c",
            "Zero/vendor/glfw/src/linux_joystick.c"
        }

        defines {
            "_GLFW_X11"
        }
    filter {}

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

		files {
			"Zero/vendor/glfw/src/win32_init.c",
			"Zero/vendor/glfw/src/win32_joystick.c",
			"Zero/vendor/glfw/src/win32_monitor.c",
			"Zero/vendor/glfw/src/win32_time.c",
			"Zero/vendor/glfw/src/win32_thread.c",
			"Zero/vendor/glfw/src/win32_window.c",
			"Zero/vendor/glfw/src/wgl_context.c",
			"Zero/vendor/glfw/src/egl_context.c",
			"Zero/vendor/glfw/src/osmesa_context.c"
		}

		defines { 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
        }
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
		runtime "Debug"
        symbols "on"
    filter {}

	filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"
    filter {}

project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"

    defines {
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"
    }

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "Zero/vendor/glfw/include",
        "Zero/vendor/glad/include",
        "Zero/vendor/imgui"
    }

    files {
        "Zero/vendor/imgui/*.h",
        "Zero/vendor/imgui/*.hpp",
        "Zero/vendor/imgui/*.cpp"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++11"
        staticruntime "On"
    filter {}

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++11"
        staticruntime "On"
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"
    filter {}

project "SFML"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    defines {
        "SFML_STATIC"
    }

    includedirs {
        "Zero/vendor/sfml/include",
        "Zero/vendor/sfml/src"
    }

    files {
        "Zero/vendor/sfml/src/SFML/System/*.cpp"
    }

    filter "system:windows"
        files {
            "Zero/vendor/sfml/src/SFML/System/Win32/*.hpp",
            "Zero/vendor/sfml/src/SFML/System/Win32/*.cpp"
        }
    filter {}

    filter "system:linux"
        files {
            "Zero/vendor/sfml/src/SFML/System/Unix/*.hpp",
            "Zero/vendor/sfml/src/SFML/System/Unix/*.cpp"
        }
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
		runtime "Debug"
        symbols "on"
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
		runtime "Release"
        optimize "on"
    filter {}

project "SPDLOG"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"

    defines {
        "SPDLOG_COMPILED_LIB",
        "SPDLOG_NO_EXCEPTIONS"
    }

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "Zero/vendor/spdlog/include"
    }

    files {
        "Zero/vendor/spdlog/src/spdlog.cpp",
        "Zero/vendor/spdlog/src/stdout_sinks.cpp",
        "Zero/vendor/spdlog/src/fmt.cpp",
        "Zero/vendor/spdlog/src/color_sinks.cpp",
        "Zero/vendor/spdlog/src/file_sinks.cpp",
        "Zero/vendor/spdlog/src/async.cpp"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
		runtime "Debug"
        symbols "on"
    filter {}

    filter "configurations:Release"
        defines { "NDEBUG" }
		runtime "Release"
        optimize "on"
    filter {}

project "ZeroFramework"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"
    
    defines {
        "GLM_FORCE_CXX11",
        "GLM_FORCE_CTOR_INIT",
        "GLM_FORCE_INLINE",

        "SFML_STATIC"
    }

    targetdir ("out/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "Zero",
        "Zero/Zero",
        "Zero/vendor/assimp/include",
        "Zero/vendor/freetype/include",
        "Zero/vendor/glad/include",
        "Zero/vendor/glfw/include",
        "Zero/vendor/glm/include",
        "Zero/vendor/imgui",
        "Zero/vendor/sfml/include",
        "Zero/vendor/spdlog/include",
        "Zero/vendor/stb_image/include"
    }

    files {
        "Zero/*.h",
        "Zero/*.hpp",
        "Zero/*.cpp",
		"Zero/Zero/**.h",
		"Zero/Zero/**.hpp",
		"Zero/Zero/**.cpp",
		"Zero/vendor/stb_image/**.h",
		"Zero/vendor/glm/glm/**.hpp",
		"Zero/vendor/glm/glm/**.inl"
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

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	pchheader "zr_pch.h"
    pchsource "Zero/zr_pch.cpp"
    
    links {
        "ASSIMP",
        "FREETYPE",
        "GLAD",
        "GLFW",
        "ImGui",
        "SFML",
        "SPDLOG"
    }
