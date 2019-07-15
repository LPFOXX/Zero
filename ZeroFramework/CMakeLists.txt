# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Zero")

set(GLFW_BUILD_EXAMPLES 0)
set(GLFW_BUILD_TESTS 0)
set(GLFW_BUILD_DOCS 0)

add_subdirectory("Zero/vendor/glfw")

include_directories(
	${PROJECT_SOURCE_DIR}/Zero
	${PROJECT_SOURCE_DIR}/Zero/vendor/imgui
	${PROJECT_SOURCE_DIR}/Zero/vendor/glad/include
	${PROJECT_SOURCE_DIR}/Zero/vendor/glm/include
	${PROJECT_SOURCE_DIR}/Zero/vendor/stb_image/include
)

file(GLOB ZR_SRC_FILES
	${PROJECT_SOURCE_DIR}/Zero/Zero/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Events/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/ImGui/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Platform/OpenGL/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Platform/Windows/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/Zero/Renderer/*.cpp
	${PROJECT_SOURCE_DIR}/Zero/vendor/glad/src/*.c
	${PROJECT_SOURCE_DIR}/Zero/vendor/imgui/*.cpp
)

add_library (Zero ${ZR_SRC_FILES})
target_compile_definitions(Zero PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
#target_compile_definitions(Zero PRIVATE GLM_FORCE_CXX11)
#target_compile_definitions(Zero PRIVATE GLM_FORCE_CTOR_INIT)
#target_compile_definitions(Zero PRIVATE GLM_FORCE_INLINE)

target_link_libraries(Zero glfw)
