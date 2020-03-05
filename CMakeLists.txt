cmake_minimum_required(VERSION 3.0)

project(app CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_subdirectory(libs/glfw)

file(
    GLOB_RECURSE

    SRC

    src/*
)

file(
    GLOB_RECURSE

    HEADERS

    includes/*
)

find_package(Vulkan REQUIRED)
if(VULKAN_FOUND)
	include_directories(${Vulkan_INCLUDE_DIRS})
  message(STATUS "Include Vulkan Library Path : " ${Vulkan_INCLUDE_DIRS})
endif()

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
	include_directories(${FREETYPE_INCLUDE_DIRS})
  message(STATUS "Include FREETYPE Library Path : " ${FREETYPE_INCLUDE_DIRS})
endif()

find_package(assimp REQUIRED)
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIR})
  message(STATUS "Include Assimp Library Path : " ${ASSIMP_INCLUDE_DIRS})
endif()

add_executable(app ${SRC} ${HEADERS})

include_directories(includes)

target_link_libraries(app ${FREETYPE_LIBRARIES})
target_link_libraries(app glfw)
target_link_libraries(app ${OPENGL_gl_LIBRARY})
target_link_libraries(app ${ASSIMP_LIBRARIES})
target_link_libraries(app ${Vulkan_LIBRARY})
