
cmake_minimum_required (VERSION 3.17.2)
project (vulkania)


# include
include_directories (

  ${CMAKE_SOURCE_DIR}
  )

set (OWENS_LAKE "$ENV{HOME}/owenslake")
message (STATUS "OWENS_LAKE -> ${OWENS_LAKE}")
message (STATUS "CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR}")

add_definitions (-DCMAKE_EXPORT_COMPILE_COMMANDS=1 )
add_definitions (-DOWENS_LAKE="${OWENS_LAKE}")

add_compile_options (-g -O0 -Wextra -Wshadow -Wpedantic -fexceptions -std=c++20) #  -Wall -Wpedantic -Werror -Wconversion

#
link_directories (
  )

# libs
add_subdirectory (rokz)

# exe's
add_subdirectory (rekz)

add_subdirectory (okui)

# add_subdirectory (/home/djbuzzkill/owenslake/gx/aframe aframe)



#message (STATUS "${CMAKE_BINARY_DIR}")
#message (STATUS "${CMAKE_SOURCE_DIR}")
#message (STATUS "${CMAKE_CURRENT_BINARY_DIR}")
#message (STATUS "${CMAKE_CURRENT_SOURCE_DIR}")


message (STATUS "${projname} - ${CMAKE_SOURCE_DIR}/rokz")
message (STATUS "${projname} - ${CMAKE_CURRENT_BINARY_DIR}")
message (STATUS "${projname} - ${CMAKE_CURRENT_SOURCE_DIR}")
