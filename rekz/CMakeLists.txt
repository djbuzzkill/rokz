

CMAKE_MINIMUM_REQUIRED(VERSION 3.17.2)

project (rekz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")
set (REKZ_HEADER_FILES

  bumptool.hpp
  meshery.h
  rekz.h
  dark_types.h
  lumen_pipe.h
  marz.h
  darkrootgarden.h
  drawpolygon.h
  dark_obj_pipeline.h
  font_tool.h
  grid_pipeline.h
  landscape_pipeline.h
  mars_files.h
  marzdata.h
  onscreen_pipe.h
  onscreen_data.h
  onscreen_draw.h
  drawmarz.h
  image_loader.h
  image_tool.h
  milkshake.h
  pepper.h
)


set (REKZ_SOURCE_FILES

  bumptool.cpp
  drawgrid.cpp  
  darkrootgarden.cpp
  darkpolydata.cpp
  dark_obj_pipeline.cpp
  darkthings.cpp
  drawmarz.cpp
  drawwireframe.cpp
  drawpolygon.cpp
  lumen_pipe.cpp
  font_tool.cpp
  griddata.cpp
  grid_pipeline.cpp  
  image_loader.cpp
  landscape_pipeline.cpp
  marz.cpp
  marzdata.cpp
  mars_files.cpp
  milkshake.cpp
  milkshake_framebuffer.cpp
  milkshake_renderpass.cpp
  milkshake_attachment.cpp
  onscreen_data.cpp
  onscreen_draw.cpp
  onscreen_pipe.cpp
  pepper.cpp
  platonic.cpp
  rekz.cpp
  rekz_of_things.cpp
  stonk.cpp
  texture_tool.cpp
  tile_tool.cpp
  # test_rokz.cpp
)

add_executable (
  rekz
  ${REKZ_HEADER_FILES}
  ${REKZ_SOURCE_FILES}
)

target_link_libraries (
  rekz

# PRIVATE shaderc_combined shaderc SPIRV-Tools SPIRV-Tools-opt SPIRV

PUBLIC 

  freetype

  IL ILU
  zmq
  rokz
  glfw
  vulkan
  dl
  pthread
  X11
  Xxf86vm
  Xrandr
  Xi

)


target_include_directories (
  rekz
  
  PUBLIC /usr/include/shaderc

  /usr/include/freetype2/
)


