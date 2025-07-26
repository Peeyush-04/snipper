cmake_minimum_required(VERSION 3.14)
project(snipper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/external
)

# Define compile-time paths (escaped properly)
string(REPLACE "\\" "/" CONFIG_PATH "${PROJECT_SOURCE_DIR}/config/default_config.json")
string(REPLACE "\\" "/" SNIPPETS_PATH "${PROJECT_SOURCE_DIR}/data/snippets.db")
string(REPLACE "\\" "/" PROJECT_ROOT "${PROJECT_SOURCE_DIR}")

# Core Library
file(GLOB_RECURSE CORE_SOURCES
  src/app/*.cpp
  src/core/*.cpp
  src/models/*.cpp
  src/platform/*.cpp
  src/search/*.cpp
)
add_library(snipper_core STATIC ${CORE_SOURCES})
target_include_directories(snipper_core PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/external
)
target_compile_definitions(snipper_core PUBLIC
  DEFAULT_CONFIG_PATH=\"${CONFIG_PATH}\"
  DEFAULT_SNIPPETS_DB=\"${SNIPPETS_PATH}\"
  DEFAULT_PROJECT_ROOT=\"${PROJECT_ROOT}\"
)

# Link Windows-specific library
if (WIN32)
  target_link_libraries(snipper_core PUBLIC shlwapi)
endif()

# CLI Executable
file(GLOB_RECURSE CLI_SOURCES
  src/main.cpp
  src/cli/*.cpp
)
add_executable(snipper ${CLI_SOURCES})
target_link_libraries(snipper PRIVATE snipper_core)
target_compile_definitions(snipper PRIVATE
  DEFAULT_CONFIG_PATH=\"${CONFIG_PATH}\"
  DEFAULT_SNIPPETS_DB=\"${SNIPPETS_PATH}\"
  DEFAULT_PROJECT_ROOT=\"${PROJECT_ROOT}\"
)

# Output directory
set_target_properties(snipper PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin"
)
