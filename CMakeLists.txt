
cmake_minimum_required (VERSION 3.8)

project(Sandbox)

add_subdirectory("ZeroFramework")

include_directories(
	${PROJECT_SOURCE_DIR}/Sandbox
	${PROJECT_SOURCE_DIR}/ZeroFramework
)

file(GLOB SANDBOX_SRC_FILES ${PROJECT_SOURCE_DIR}/Sandbox/*.cpp)

add_executable(Sandbox ${SANDBOX_SRC_FILES})
target_link_libraries(Sandbox Zero glfw)
