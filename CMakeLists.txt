cmake_minimum_required(VERSION 3.27)

project(sokolkol)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    lib/sokol
    lib/cglm/include
)

add_executable(${PROJECT_NAME}
    src/main.c
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32 gdi32)
endif (WIN32)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw GL X11 Xi Xcursor dl pthread m)
endif (UNIX)
