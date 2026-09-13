cmake_minimum_required(VERSION 3.25)

project(CppClass VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(cmake/ProjectOptions.cmake)
if(BUILD_TESTING)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
endif()

# cpp_add_target(foo):
#   foo/include            headers (optional)
#   foo/src/*.cpp          static lib; src/main.cpp → "foo" exe linking it.
#                          no src/ → header-only INTERFACE lib.
#                          ponytail: main.cpp-only targets unsupported, keep main thin.
#   foo/tests/*.cpp        "foo_tests" gtest exe, links the lib.
function(cpp_add_target name)
  file(GLOB_RECURSE src CONFIGURE_DEPENDS
    "${name}/src/*.cpp" "${name}/src/*.cc" "${name}/src/*.cxx")
  file(GLOB_RECURSE tst CONFIGURE_DEPENDS
    "${name}/tests/*.cpp" "${name}/tests/*.cc" "${name}/tests/*.cxx")

  if(src)
    list(FILTER src EXCLUDE REGEX "/main\\.cpp$")
    add_library(${name}_lib STATIC ${src})
    target_include_directories(${name}_lib PUBLIC "${name}/include")
    cpp_target_options(${name}_lib)
  else()
    add_library(${name}_lib INTERFACE)
    target_include_directories(${name}_lib INTERFACE "${name}/include")
  endif()

  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}/src/main.cpp")
    add_executable(${name} "${name}/src/main.cpp")
    target_link_libraries(${name} PRIVATE ${name}_lib)
    cpp_target_options(${name})
  endif()

  if(tst)
    add_executable(${name}_tests ${tst})
    target_link_libraries(${name}_tests PRIVATE ${name}_lib GTest::gtest_main)
    cpp_target_options(${name}_tests)
    gtest_discover_tests(${name}_tests TEST_PREFIX "${name}.")
  endif()
endfunction()

# add exercises:
# cpp_add_target(smart_pointers)
