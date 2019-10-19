

project (sfmlthread)

# Some files are compilated according to the platform used, so
# the SRCROOT is treated as an include directory (original SFML source code tree)
include_directories(
	${PROJECT_SOURCE_DIR}/include/
	${PROJECT_SOURCE_DIR}/src/
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "dir='${dir}'")
endforeach()

## SFML source files
FILE(GLOB SFML_SRC_FILES ${PROJECT_SOURCE_DIR}/src/SFML/System/*.cpp)

if(OS_WINDOWS)
	FILE(GLOB SFML_IMPL_FILES ${PROJECT_SOURCE_DIR}/src/SFML/System/Win32/*.cpp)
elseif(OS_LINUX)
	FILE(GLOB SFML_IMPL_FILES ${PROJECT_SOURCE_DIR}/src/SFML/System/Unix/*.cpp)
endif()

message("[ZR BUILD] SFML include directory: " ${PROJECT_SOURCE_DIR}/include/)
message("[ZR BUILD] SFML source files: " ${SFML_SRC_FILES})
message("[ZR BUILD] SFML platform implementation files: " ${SFML_IMPL_FILES})

add_library(sfmlthread STATIC ${SFML_SRC_FILES} ${SFML_IMPL_FILES})
target_compile_definitions(sfmlthread PUBLIC SFML_STATIC)

