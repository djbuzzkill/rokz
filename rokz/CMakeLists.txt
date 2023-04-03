
cmake_minimum_required(VERSION 3.17.2)


project (rokz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

#  
set (ROKZ_HEADER_FILES
  draw_sequence.h
  
  rokz.h
  context.h
  descriptor.h
  allocation.h
  attachment.h
  buffer.h
  image.h
  imagebuff.h
  command.h
  pipeline.h
  framebuffer.h
  renderpass.h
  render.h
  shader.h
  texture.h
  uniform.h
  sampler.h
  window.h
  binary_IO.h
  defaults.h

  global_descriptor.h
  shared_descriptor.h
  window_handler.h
  input_state.h
  display.h
  vert_defs.h
  file.h
  frame.h
  shared_types.h
  synchronization.h
  rokz_types.h
  rc_types.h
  rc_buffer.h
  rc_image.h
  rc_attachment.h
  rc_swapchain.h
  
  utility.h
  ut_offset.h
  common.h
)

#
set (ROKZ_SOURCE_FILES

  attachment.cpp
  context.cpp
  command.cpp
  defaults.cpp
  pipeline.cpp
  framebuffer.cpp
  renderpass.cpp
  render.cpp
  descriptor.cpp
  allocation.cpp
  buffer.cpp
  image.cpp
  shader.cpp
  texture.cpp
  uniform.cpp
  sampler.cpp
  window.cpp

  global_descriptor.cpp
  window_handler.cpp
  input_state.cpp
  display.cpp
  vert_defs.cpp
  frame.cpp
  utility.cpp
  binary_IO.cpp
  
  rc_attachment.cpp
  rc_buffer.cpp
  rc_types.cpp
  rc_image.cpp
  rc_swapchain.cpp
  rokz.cpp
  synchronization.cpp
)

#
add_library (rokz
  ${ROKZ_HEADER_FILES}
  ${ROKZ_SOURCE_FILES}
)

#
target_link_libraries (rokz

  PRIVATE shaderc_combined shaderc SPIRV-Tools SPIRV-Tools-opt SPIRV

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
