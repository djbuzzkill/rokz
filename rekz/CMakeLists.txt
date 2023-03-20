
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rekz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")
  
set (REKZ_HEADER_FILES
  meshery.h
  rekz.h
  dark_types.h
  marz.h
  darkrootgarden.h
  dark_obj_pipeline.h
  grid_pipeline.h
  landscape_pipeline.h
  mars_files.h
  draw_mars.h
  image_loader.h
  image_tool.h
)
	
set (REKZ_SOURCE_FILES
  drawgrid.cpp  
  darkrootgarden.cpp
  dark_obj_pipeline.cpp
  darkthings.cpp
  draw_mars.cpp
  drawwireframe.cpp
  drawpolygon.cpp
  darkpolydata.cpp
  griddata.cpp
  grid_pipeline.cpp  
  image_loader.cpp
  landscape_pipeline.cpp
  marz.cpp
  mars_files.cpp
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

  IL
  ILU

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



