cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MeshEngine")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party/glfw)

add_library(glad
    third_party/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    third_party/glad/include
)

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (MeshEngine   
    "src/main.cpp" 
    "src/viewer/renderer.cpp" 
    "src/viewer/renderer.h" 
    "src/mesh/mesh.h" 
    "src/mesh/obj_loader.h" 
    "src/mesh/obj_loader.cpp"
)

target_include_directories(MeshEngine PRIVATE
    third_party/glad/include
    third_party/tinyobjloader
    third_party/eigen
)

target_link_libraries(MeshEngine
    glfw
    glad
    opengl32
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MeshEngine PROPERTY CXX_STANDARD 20)
endif()

