
cmake_minimum_required(VERSION 3.17.2)


project (rokz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

#  
set (ROKZ_HEADER_FILES

  allocation.hpp
  attachment.hpp
  binary_IO.hpp
  buffer.hpp
  command.hpp
  common.hpp
  context.hpp
  defaults.hpp
  draw_sequence.hpp
  descriptor.hpp
  image.hpp
  imagebuff.hpp
  pipeline.hpp
  framebuffer.hpp
  renderpass.hpp
  render.hpp
  shader.hpp
  texture.hpp
  uniform.hpp

  global_descriptor.hpp
  shared_descriptor.hpp
  window_handler.hpp
  input_state.hpp
  display.hpp
  vert_defs.hpp
  file.hpp
  frame.hpp
  shared_types.hpp
  synchronization.hpp
  rokz_types.hpp
  rc_types.hpp
  rc_buffer.hpp
  rc_image.hpp
  rc_attachment.hpp
  rc_swapchain.hpp
  rokz.hpp
  transfer_buffer.hpp
  transfer_image.hpp
  utility.hpp
  ut_offset.hpp
  sampler.hpp
  window.hpp
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

  transfer_buffer.cpp
  transfer_image.cpp

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
