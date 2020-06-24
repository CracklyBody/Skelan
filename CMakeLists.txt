cmake_minimum_required(VERSION 3.5)

project(skeletallib)

# Find source files
file(GLOB SOURCES src/*.cpp)

FIND_PACKAGE(OpenGL)
# Include header files
include_directories(include)

# Create shared library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
LINK_DIRECTORIES(${PROJECT_NAME}/lib/*.lib)
TARGET_LINK_LIBRARIES(skeletallib ../lib/SOIL ../lib/assimp-vc142-mtd opengl32 ../lib/glfw3dll ../lib/glew32)
# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

# Install library headers
file(GLOB HEADERS include/*.h)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})