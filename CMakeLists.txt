
cmake_minimum_required (VERSION 3.8)

project(Sandbox)

add_subdirectory("ZeroFramework")

include_directories(
	${PROJECT_SOURCE_DIR}/Sandbox
	${PROJECT_SOURCE_DIR}/ZeroFramework
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/freetype/include
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/assimp/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/freetype/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/glad/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/glfw/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/glm/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/imgui/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/spdlog/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/stb_image/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/sfml/include/
	${PROJECT_SOURCE_DIR}/ZeroFramework/Zero/vendor/sfml/src/
)

message("Sandbox include directories")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "dir='${dir}'")
endforeach()

file(GLOB SANDBOX_SRC_FILES ${PROJECT_SOURCE_DIR}/Sandbox/*.cpp)

add_executable(Sandbox ${SANDBOX_SRC_FILES})
target_link_libraries(Sandbox
	assimp
	freetype
	glfw
	spdlog
	sfmlthread
	Zero
)
