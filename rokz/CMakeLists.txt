
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rokz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

#  
set (ROKZ_HEADER_FILES
  common.h
  rokz_types.h
  rokz_funcs.h
)

#
set (ROKZ_SOURCE_FILES
  rokz.cpp
)

#
add_library (rokz
  ${ROKZ_HEADER_FILES}
  ${ROKZ_SOURCE_FILES}
)

#
target_link_libraries (rokz
  zmq
  glfw
  vulkan
  dl
  pthread
  X11
  Xxf86vm
  Xrandr
  Xi
)
