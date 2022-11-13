
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rockz)




#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

  
set (ROCKZ_HEADER_FILES
  rockz.h
)
	
set (ROCKZ_SOURCE_FILES
  rockz.cpp
)

add_executable (
  rockz
  ${ROCKZ_HEADER_FILES}
  ${ROCKZ_SOURCE_FILES}
)

target_link_libraries (
  rockz

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



