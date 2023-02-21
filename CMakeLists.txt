cmake_minimum_required(VERSION 3.12)

project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")

# Use FetchContent to download GLAD library
include(FetchContent)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.34
)
FetchContent_MakeAvailable(glad)

# Use FetchContent to download GLFW library
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.4
)
FetchContent_MakeAvailable(glfw)

# Use FetchContent to download GLM library
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Download imgui files
FetchContent_Declare(
    ImGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.84.2
)
FetchContent_MakeAvailable(imgui)

file(DOWNLOAD
    https://raw.githubusercontent.com/ocornut/imgui/c206a193737811193a0b48ef2d35fe028fa0996e/examples/README.txt
    ${EXTERNAL_DIR}/imgui/readme.txt)

# Download STB_IMAGE
file(DOWNLOAD
    https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
    ${EXTERNAL_DIR}/stb/stb_image.h)


# Add the executable
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
        "${EXTERNAL_DIR}/stb/*.h"
        "${EXTERNAL_DIR}/stb/*.cpp"
        "${EXTERNAL_DIR}/imgui/*.h"
        "${EXTERNAL_DIR}/imgui/*.cpp"
        ${IMGUI_SRCS}
        )
add_executable(MyExecutable ${all_SRCS})

# Link the executable to libraries
target_link_directories(MyExecutable PRIVATE ${EXTERNAL_DIR}/stb ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends ${glad_SOURCE_DIR})
target_link_libraries(MyExecutable PRIVATE glad ${GLFW_LIBRARIES} glm )

target_include_directories(MyExecutable PRIVATE ${glad_SOURCE_DIR}/include ${glfw_SOURCE_DIR}/include ${glm_SOURCE_DIR} ${imgui_SOURCE_DIR} ${EXTERNAL_DIR})
