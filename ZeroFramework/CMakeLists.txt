# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Zero")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(OS_WINDOWS 1)
	message("Compiling for Windows")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(OS_LINUX 1)
	message("Compiling for Linux")
endif()

set(GLFW_BUILD_EXAMPLES 0)
set(GLFW_BUILD_TESTS 0)
set(GLFW_BUILD_DOCS 0)

set(ASSIMP_ANDROID_JNIIOSYSTEM 0)
set(ASSIMP_ASAN 0)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT 0)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(ASSIMP_NO_EXPORT 1)
set(BUILD_DOCS OFF)
set(BUILD_SHARED_LIBS 0)
set(FREETYPE_NO_DIST OFF)
set(SKIP_INSTALL_HEADERS ON)

include_directories(
	${PROJECT_SOURCE_DIR}/Zero/
	${PROJECT_SOURCE_DIR}/Zero/vendor/assimp/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/freetype/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/glad/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/glfw/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/glm/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/imgui/
	${PROJECT_SOURCE_DIR}/Zero/vendor/spdlog/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/stb_image/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/sfml/include/
	${PROJECT_SOURCE_DIR}/Zero/vendor/sfml/src/
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "dir='${dir}'")
endforeach()

add_subdirectory("Zero/vendor/assimp")
add_subdirectory("Zero/vendor/freetype")
add_subdirectory("Zero/vendor/glfw")
add_subdirectory("Zero/vendor/spdlog")
add_subdirectory("Zero/vendor/sfml")

file(GLOB ZR_SRC_FILES
	${PROJECT_SOURCE_DIR}/Zero/Zero/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Core/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Core/Events/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/ImGui/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Platform/OpenGL/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Platform/Windows/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Renderer/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/vendor/glad/src/*.c
	${PROJECT_SOURCE_DIR}/Zero/vendor/imgui/*.cpp
)

add_library (Zero ${ZR_SRC_FILES})
target_compile_definitions(Zero PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
#target_compile_definitions(Zero PUBLIC FT2_BUILD_LIBRARY)
#target_compile_definitions(Zero PUBLIC STB_IMAGE_WRITE_IMPLEMENTATION)
#target_compile_definitions(Zero PUBLIC STB_IMAGE_IMPLEMENTATION)
#target_compile_definitions(Zero PUBLIC SPDLOG_COMPILED_LIB)
#target_compile_definitions(Zero PRIVATE GLM_FORCE_CXX11)
#target_compile_definitions(Zero PRIVATE GLM_FORCE_CTOR_INIT)
#target_compile_definitions(Zero PRIVATE GLM_FORCE_INLINE)

target_link_libraries(Zero
	assimp
	freetype
	glfw
	spdlog
	sfmlthread
)
