cmake_minimum_required (VERSION 3.1)

project (vulkania)



# include
include_directories (

  ${CMAKE_SOURCE_DIR}
j  )

add_definitions (-DCMAKE_EXPORT_COMPILE_COMMANDS=1)


add_compile_options (-g -O0)

#
link_directories (
  )

# libs
add_subdirectory (rokz)



# exe's
add_subdirectory (rockz)

#message (STATUS "${CMAKE_BINARY_DIR}")
#message (STATUS "${CMAKE_SOURCE_DIR}")
#message (STATUS "${CMAKE_CURRENT_BINARY_DIR}")
#message (STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
