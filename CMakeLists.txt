cmake_minimum_required(VERSION 3.25)
project(fb_demo)

set(CMAKE_CXX_STANDARD 20)

find_package(flatbuffers CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)

include(${flatbuffers_DIR}/BuildFlatBuffers.cmake)
find_program(FLATBUFFERS_FLATC_EXECUTABLE NAMES flatc)
build_flatbuffers(
        "schema.fbs"       # flatbuffers_schemas
        ""                  # schema_include_dirs
        fb_schema           # target
        ""                  # additional_dependencies
        "schema_includes"   # generated_includes_dir
        ""                  # binary_schemas_dir
        "")                 # copy_text_schemas_dir

add_executable(fb_demo main.cpp)
add_dependencies(fb_demo fb_schema)
target_link_libraries(fb_demo PRIVATE flatbuffers::flatbuffers zstd::libzstd_static)
