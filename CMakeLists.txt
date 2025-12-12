cmake_minimum_required(VERSION 3.16...3.25)

project(
        GamePhysicsTemplate
        VERSION 0.1.0
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

option(USE_WAYLAND "Use Wayland instead of X11" OFF)
option(USE_X11 "Use X11 instead of Wayland" OFF)
if (USE_WAYLAND AND USE_X11)
    message(FATAL_ERROR "You can't use both Wayland and X11 at the same time")
endif ()

if (UNIX AND NOT APPLE)
    if (DEFINED ENV{XDG_SESSION_TYPE})
        if ($ENV{XDG_SESSION_TYPE} STREQUAL "x11")
            set(GLFW_BUILD_X11 TRUE)
            set(GLFW_BUILD_WAYLAND FALSE)
            message(" Using X11 (detected via XDG_SESSION_TYPE)")
        else ()
            set(GLFW_BUILD_X11 FALSE)
            set(GLFW_BUILD_WAYLAND TRUE)
            message("Using Wayland (detected via XDG_SESSION_TYPE)")
        endif ()
    else ()
        set(GLFW_BUILD_WAYLAND TRUE)
        set(GLFW_BUILD_X11 FALSE)
        message("Odd Configuration detected, XDG_SESSION_TYPE not set! If you are on WSL2 this is expected, otherwise make sure your display settings are sane.")
    endif ()
endif (UNIX AND NOT APPLE)

# Compatibility fixes for GLFW
# Older versions used GLFW_USE_WAYLAND and GLFW_USE_X11
# Newer versions use GLFW_BUILD_WAYLAND and GLFW_BUILD_X11
# We need to make sure that the correct one is set for older versions
if (DEFINED GLFW_USE_WAYLAND)
    unset(GLFW_USE_WAYLAND CACHE)
    set(GLFW_BUILD_WAYLAND TRUE)
    set(GLFW_BUILD_X11 FALSE)
elseif (DEFINED GLFW_USE_X11)
    unset(GLFW_USE_X11 CACHE)
    set(GLFW_BUILD_X11 TRUE)
    set(GLFW_BUILD_WAYLAND FALSE)
endif ()

if (USE_WAYLAND)
    set(GLFW_BUILD_WAYLAND TRUE)
    set(GLFW_BUILD_X11 FALSE)
elseif (USE_X11)
    set(GLFW_BUILD_X11 TRUE)
    set(GLFW_BUILD_WAYLAND FALSE)
endif ()


option(DEV_MODE "Set up development helper settings" ON)
add_subdirectory(thirdparty/webgpu)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glfw3webgpu)
add_subdirectory(thirdparty/imgui)

option(USE_HIGH_PERFORMANCE_GPU "Use high performance GPU, most likely a discrete GPU, should be left ON except for trouble shooting purposes." ON)
if (USE_HIGH_PERFORMANCE_GPU)
    add_compile_definitions(WGPU_GPU_HIGH_PERFORMANCE="ON")
endif ()
configure_file(resources/config.h.in config/config.h)

add_executable(Template
        src/implementations.cpp
        src/main.cpp
        src/Renderer.h
        src/Renderer.cpp
        src/Primitives.h
        src/Primitives.cpp
        thirdparty/stb_image.h
        src/ResourceManager.h
        src/ResourceManager.cpp
        src/Simulator.cpp
        src/Camera.h
        src/Camera.cpp
        src/Colormap.h
        src/Colormap.cpp
        src/PathFinder.cpp
        Scenes/Single_Step_MassSpring.cpp
		Scenes/Single_Step_MassSpring.h
        Scenes/Euler_Simulation.cpp
        Scenes/Euler_Simulation.h
        Scenes/Midpoint_Simulation.cpp
        Scenes/Midpoint_Simulation.h
        Scenes/Complex_Simulation.cpp
        Scenes/Complex_Simulation.h
		Scenes/Single_Step.cpp
		Scenes/Single_Step.h
        Scenes/Simulation.cpp
        Scenes/Simulation.h
        Scenes/Collision.cpp
        Scenes/Collision.h
        Scenes/Complex_Simulation_Rb.cpp
        Scenes/Complex_Simulation_Rb.h
        Scenes/PDE_Single_Step.cpp
        Scenes/PDE_Single_Step.h
        Scenes/PDE_Explicit_Simulation.cpp
        Scenes/PDE_Explicit_Simulation.h
        Scenes/PDE_Implicit_Simulation.cpp
        Scenes/PDE_Implicit_Simulation.h
        Scenes/Rigidbody.cpp
        Scenes/Rigidbody.h
)

if (UNIX AND NOT APPLE)
    if (DEFINED ENV{XDG_SESSION_TYPE})
        if ($ENV{XDG_SESSION_TYPE} STREQUAL "x11")
            target_compile_definitions(Template PUBLIC _GLFW_X11=1)
            target_compile_definitions(imgui PUBLIC _GLFW_X11=1)
            target_compile_definitions(glfw3webgpu PUBLIC _GLFW_X11=1)
        else ()
            target_compile_definitions(Template PUBLIC _GLFW_WAYLAND=1)
            target_compile_definitions(imgui PUBLIC _GLFW_WAYLAND=1)
            target_compile_definitions(glfw3webgpu PUBLIC _GLFW_WAYLAND=1)
            message("Using Wayland!")
        endif ()
    else ()
        target_compile_definitions(Template PUBLIC _GLFW_WAYLAND=1)
        target_compile_definitions(imgui PUBLIC _GLFW_WAYLAND=1)
        target_compile_definitions(glfw3webgpu PUBLIC _GLFW_WAYLAND=1)
    endif ()
endif (UNIX AND NOT APPLE)

target_compile_definitions(Template PRIVATE
        GLM_FORCE_RIGHT_HANDED
        GLM_FORCE_DEPTH_ZERO_TO_ONE
)

file(GLOB_RECURSE SCENE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.cpp
)

file(GLOB_RECURSE SCENE_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.h
)
target_sources(Template PRIVATE ${SCENE_SOURCES} ${SCENE_HEADERS})

file(GLOB_RECURSE PIPELINE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pipelines/*.cpp
)

file(GLOB_RECURSE PIPELINE_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pipelines/*.h
)
target_sources(Template PRIVATE ${PIPELINE_SOURCES} ${PIPELINE_HEADERS})

file(GLOB_RECURSE UTIL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp
)

file(GLOB_RECURSE UTIL_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.h
)
target_sources(Template PRIVATE ${UTIL_SOURCES} ${UTIL_HEADERS})

target_include_directories(Template PRIVATE .)
target_include_directories(Template PRIVATE src)
target_include_directories(Template PRIVATE thirdparty)
target_include_directories(Template PRIVATE ${CMAKE_BINARY_DIR})
# message(STATUS  ${CMAKE_CURRENT_SOURCE_DIR})
# message(STATUS  ${CMAKE_BINARY_DIR})

target_link_libraries(Template PRIVATE glfw webgpu glfw3webgpu imgui)

set_target_properties(Template PROPERTIES
        CXX_STANDARD 17
)

target_copy_webgpu_binaries(Template)

if (MSVC)
    # Ignore a warning that GLM requires to bypass
    # Disable warning C4201: nonstandard extension used: nameless struct/union
    target_compile_options(Template PUBLIC /wd4201)
    # Disable warning C4305: truncation from 'int' to 'bool' in 'if' condition
    target_compile_options(Template PUBLIC /wd4305)
endif (MSVC)
