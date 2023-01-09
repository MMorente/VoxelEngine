find_package(glad CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable (VoxelEngine "VoxelEngine.cpp" "VoxelEngine.h" "Window.h" "main.cpp" "Block.h" "Chunk.h" "Shader.h" "Light.h" "MeshLoader.h" "stb_image.h" "Camera.h")
add_library(STB_IMAGE "../stb_image.cpp" "Camera.h")
target_link_libraries(VoxelEngine glad::glad glfw glm::glm STB_IMAGE)
add_compile_definitions(SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VoxelEngine PROPERTY CXX_STANDARD 20)
endif()
