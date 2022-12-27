cmake_minimum_required (VERSION 3.5)

project (RayTracer)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
set (include_dir "include")

file (GLOB source_files "${source_dir}/*.cpp" "${include_dir}/GLFW/*" "${include_dir}/GL/*")

add_executable (RayTracer ${source_files})

target_include_directories(RayTracer PRIVATE ${include_dir}/*)
target_link_libraries(${PROJECT_NAME} GL dl glfw)
