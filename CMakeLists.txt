cmake_minimum_required(VERSION 3.25)

project(SPR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
if(BUILD_TESTING)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
endif()

add_library(spr INTERFACE)
target_include_directories(spr INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

function(spr_add_problem name)
  set(header "${CMAKE_CURRENT_SOURCE_DIR}/include/spr/${name}.hpp")
  set(test_source "${CMAKE_CURRENT_SOURCE_DIR}/tests/${name}.cpp")

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
endfunction()

# Add one line per assignment, for example:
# spr_add_problem(p100)
spr_add_problem(p10189)