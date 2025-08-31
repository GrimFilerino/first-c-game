cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(FirstGame C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find SDL2 and its extensions
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    lib/glad/src/glad.c
    lib/microui/microui.c
    src/main.c
    src/**/*.c
)

add_executable(${PROJECT_NAME} ${SOURCES}) 

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/glad/include
    ${CMAKE_SOURCE_DIR}/lib/microui
    ${CMAKE_SOURCE_DIR}/lib/linmath
    ${CMAKE_SOURCE_DIR}/lib/stb_image
    ${CMAKE_SOURCE_DIR}/lib/SDL3/include
    ${OPENGL_INCLUDE_DIR}
    src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/SDL3/lib/linux/libSDL3.a 
    ${CMAKE_SOURCE_DIR}/lib/SDL3/lib/linux/libSDL3_image.a 
    ${CMAKE_SOURCE_DIR}/lib/SDL3/lib/linux/libSDL3_mixer.a 
    ${CMAKE_SOURCE_DIR}/lib/SDL3/lib/linux/libSDL3_ttf.a 
    ${OPENGL_LIBRARIES}
    m
)

