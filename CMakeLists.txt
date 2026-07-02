CMAKE_MINIMUM_REQUIRED(VERSION 4.0)

project(vireon LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
#add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)


target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:ENGINE_DEBUG>
  $<$<CONFIG:Release>:ENGINE_OPTIMIZE>
)

if(NOT CMAKE_BUILD_TYPE)
    message("BUILD TYPE NOT SET. DEFAULTING TO DEBUG")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4

        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic

        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/api"
)


find_package(OpenGL REQUIRED)          
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

add_library(glad STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/gl.c"
)

target_include_directories(glad PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  glad
  glfw
  OpenGL::GL
  Freetype::Freetype
)

