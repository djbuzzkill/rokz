
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rekz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")
  
set (REKZ_HEADER_FILES
    rekz.h
    dark_types.h
    marz.h
    darkrootgarden.h
    dark_obj_pipeline.h
    grid_pipeline.h
    landscape_pipeline.h
    draw_mars.h
    image_loader.h
  )
	
set (REKZ_SOURCE_FILES
  texture_tool.cpp
  darkrootgarden.cpp
  dark_obj_pipeline.cpp
  darkthings.cpp
  platonic.cpp
  draw_mars.cpp
  drawwireframe.cpp
  drawpolygon.cpp
  darkpolydata.cpp
  marz.cpp
  landscape_pipeline.cpp
  grid_pipeline.cpp  
  drawgrid.cpp  
  image_loader.cpp
  # test_rokz.cpp
  rekz.cpp
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



