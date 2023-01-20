
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rekz)
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")
  
set (REKZ_HEADER_FILES
    rekz.h
    dark_types.h
    darkrootgarden.h
    mars_types.h
  )
	
set (REKZ_SOURCE_FILES
  texture_tool.cpp
  rekz_win_handler.cpp
  darkrootgarden.cpp
  darkthings.cpp

  marscape.cpp
#  mars_window_handler.cpp
  mars_things.cpp
  
  #  test_rokz_hpp.cpp
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



