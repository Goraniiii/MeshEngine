cmake_minimum_required(VERSION 3.16)

project(MeshEngine)

# =========================
# C++ Settings
# =========================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Qt Auto Settings
# =========================

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# =========================
# Find Qt6
# =========================

set(CMAKE_PREFIX_PATH
    "C:/Qt/6.11.1/msvc2022_64"
)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    OpenGL
    OpenGLWidgets
)

# =========================
# GLAD
# =========================

add_library(glad
    third_party/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    src
    
    third_party/glad/include
)

# =========================
# Executable
# =========================

qt_add_executable(MeshEngine

    # Main
    src/main.cpp

    # Qt Window
    src/ui/main_window.h
    src/ui/main_window.cpp

    # OpenGL Widget
    src/viewer/gl_widget.h
    src/viewer/gl_widget.cpp

    # Renderer
    src/viewer/renderer.h
    src/viewer/renderer.cpp

    # Camera
    src/viewer/camera.h
    src/viewer/camera.cpp

    # Shader
    src/viewer/shader.h
    src/viewer/shader.cpp

    # Mesh
    src/mesh/mesh.h
    src/mesh/mesh.cpp

    # Half Edge
    src/mesh/half_edge.h
    src/mesh/half_edge.cpp

    # OBJ Loader
    src/mesh/obj_loader.h
    src/mesh/obj_loader.cpp

    # Common
    src/common.h
    src/common.cpp

    # Scene
    src/core/scene.h
    src/core/scene.cpp

    # Main Window
    src/ui/main_window.h
    src/ui/main_window.cpp

    # GL Widget
    src/viewer/gl_widget.h
    src/viewer/gl_widget.cpp
    )

# =========================
# Include Directories
# =========================

target_include_directories(MeshEngine PRIVATE

    # GLAD
    third_party/glad/include

    # tinyobjloader
    third_party/tinyobjloader

    # Eigen
    third_party/eigen
)

# =========================
# Link Libraries
# =========================

target_link_libraries(MeshEngine PRIVATE

    # OpenGL Loader
    glad

    # Qt
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets

    # Windows OpenGL
    opengl32
)

# =========================
# Shader Copy
# =========================

file(COPY
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    DESTINATION
    "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_command(TARGET MeshEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:MeshEngine>/shaders
)

# =========================
# MSVC UTF-8
# =========================

if (MSVC)
    target_compile_options(MeshEngine PRIVATE /utf-8)
endif()

