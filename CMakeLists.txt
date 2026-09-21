cmake_minimum_required(VERSION 3.25)

project(SPR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
if(BUILD_TESTING)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
endif()

add_library(spr INTERFACE)
target_include_directories(spr INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_custom_target(export)

function(spr_add_problem name)
  set(header "${CMAKE_CURRENT_SOURCE_DIR}/include/spr/${name}.hpp")
  set(test_source "${CMAKE_CURRENT_SOURCE_DIR}/tests/${name}.cpp")
  set(submission "${CMAKE_BINARY_DIR}/submissions/${name}.cpp")
  file(GLOB_RECURSE project_headers CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

  if(NOT EXISTS "${header}")
    message(FATAL_ERROR "Missing solution header: ${header}")
  endif()

  if(BUILD_TESTING)
    add_executable(${name}_test "${test_source}")
    target_link_libraries(${name}_test PRIVATE spr GTest::gtest_main)
    target_compile_options(${name}_test PRIVATE
      -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
      target_compile_options(${name}_test PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined;-fno-omit-frame-pointer>)
      target_link_options(${name}_test PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
    endif()

    gtest_discover_tests(${name}_test TEST_PREFIX "${name}.")
  endif()

  add_custom_command(
    OUTPUT "${submission}"
    COMMAND "${Python3_EXECUTABLE}"
      "${CMAKE_CURRENT_SOURCE_DIR}/scripts/export_submission.py"
      --source "${header}"
      --include-root "${CMAKE_CURRENT_SOURCE_DIR}/include"
      --output "${submission}"
      --namespace "spr::${name}"
      --compiler "${CMAKE_CXX_COMPILER}"
    DEPENDS ${project_headers} "${CMAKE_CURRENT_SOURCE_DIR}/scripts/export_submission.py"
    COMMENT "Exporting and C++11-checking ${name}"
    VERBATIM)

  add_custom_target(${name}_export DEPENDS "${submission}")
  add_dependencies(export ${name}_export)
endfunction()

# Add one line per assignment, for example:
# spr_add_problem(p100)
