function(cpp_target_options target)
  target_compile_features(${target} PRIVATE cxx_std_23)

  target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Werror
  )

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${target} PRIVATE
      $<$<CONFIG:Debug>:-fsanitize=address,undefined;-fno-omit-frame-pointer>
    )
    target_link_options(${target} PRIVATE
      $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )
  endif()
endfunction()
