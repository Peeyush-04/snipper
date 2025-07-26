cmake_minimum_required(VERSION 3.14)
project(snipper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/external
)

# Define compile-time paths
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

if (WIN32)
  target_link_libraries(snipper_core PUBLIC shlwapi)
endif()

# CLI Executable
file(GLOB_RECURSE CLI_SOURCES
  src/main.cpp
  src/cli/*.cpp
)

add_executable(snipper-cli ${CLI_SOURCES})
target_link_libraries(snipper-cli PRIVATE snipper_core)

target_compile_definitions(snipper-cli PRIVATE
  DEFAULT_CONFIG_PATH=\"${CONFIG_PATH}\"
  DEFAULT_SNIPPETS_DB=\"${SNIPPETS_PATH}\"
  DEFAULT_PROJECT_ROOT=\"${PROJECT_ROOT}\"
)

# Place CLI binary in scripts/
set(CLI_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/scripts")
file(MAKE_DIRECTORY ${CLI_OUTPUT_DIR})

set_target_properties(snipper-cli PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CLI_OUTPUT_DIR}"
)

# Post-build PATH setup for CLI
if(UNIX)
  add_custom_command(
    TARGET snipper-cli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running Linux/macOS PATH setup script..."
    COMMAND ${CMAKE_COMMAND} -E env bash "${PROJECT_SOURCE_DIR}/scripts/setup-path.sh"
    VERBATIM
  )
elseif(WIN32)
  add_custom_command(
    TARGET snipper-cli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running Windows PATH setup script..."
    COMMAND powershell -ExecutionPolicy Bypass -File "${PROJECT_SOURCE_DIR}/scripts/setup-path.ps1"
    VERBATIM
  )
endif()

# --- Tests -------------------------------------------------------------
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES
  tests/test_*.cpp
)

foreach(test_src IN LISTS TEST_SOURCES)
  get_filename_component(test_name ${test_src} NAME_WE)

  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name}
    PRIVATE snipper_core GTest::gtest GTest::gtest_main
  )

  target_include_directories(${test_name} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/external
  )

  target_compile_definitions(${test_name} PRIVATE
    DEFAULT_CONFIG_PATH=\"${CONFIG_PATH}\"
    DEFAULT_SNIPPETS_DB=\"${SNIPPETS_PATH}\"
    DEFAULT_PROJECT_ROOT=\"${PROJECT_ROOT}\"
  )

  set_target_properties(${test_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin"
  )

  add_test(NAME ${test_name} COMMAND ${PROJECT_SOURCE_DIR}/build/bin/${test_name})
endforeach()
