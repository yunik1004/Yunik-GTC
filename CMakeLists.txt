cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 11)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set (CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
       CACHE STRING "")
endif ()

# You can change the project name
project ("Yunik-GTC")

# You can change the target name
set (YUNIK_GTC_TARGET yunik-gtc)

# OpenGL
find_package (OpenGL REQUIRED)
include_directories (${OPENGL_INCLUDE_DIRS})

# GLFW3
find_package (glfw3 REQUIRED)
include_directories (${GLFW_INCLUDE_DIRS})
link_libraries (${GLFW_LIBRARY_DIRS})

# GLEW
find_package (GLEW REQUIRED)
include_directories (${GLEW_INCLUDE_DIRS})

# GLM
find_package (glm REQUIRED)

# Add source to this project's executable.
file (GLOB_RECURSE YUNIK_GTC_CPP_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file (GLOB_RECURSE YUNIK_GTC_H_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

# Resource file for windows
set (YUNIK_GTC_WIN_RESOURCE_FILE ${PROJECT_SOURCE_DIR}/windows.rc)

add_executable (${YUNIK_GTC_TARGET}
	${YUNIK_GTC_CPP_FILES}
    ${YUNIK_GTC_H_FILES}
    ${YUNIK_GTC_WIN_RESOURCE_FILE}
)

target_link_libraries (${YUNIK_GTC_TARGET}
	${OPENGL_LIBRARIES}
	glfw
	${GLEW_LIBRARIES}
	glm
)
