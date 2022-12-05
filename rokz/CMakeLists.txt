
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rokz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

#  
set (ROKZ_HEADER_FILES

  rokz.h
  context.h
  descriptor.h
  buffer.h
  image.h
  command.h
  shader.h
  texture.h
  uniform.h
  sampler.h
  window.h
  utility.h
  binary_IO.h
  defaults.h

  # rokz_types.h
  # rokz_funcs.h
  shared_types.h
  common.h
)

#
set (ROKZ_SOURCE_FILES





  context.cpp
  command.cpp
  descriptor.cpp
  buffer.cpp
  image.cpp
  shader.cpp
  texture.cpp
  uniform.cpp
  sampler.cpp
  window.cpp


  utility.cpp
  binary_IO.cpp
  defaults.cpp
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
