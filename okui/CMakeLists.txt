
CMAKE_MINIMUM_REQUIRED(VERSION 3.17.2)

set (projname okui)
project (${projname})
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")
  
set (OKUI_HEADER_FILES
  okui.h
)


set (OKUI_SOURCE_FILES
  main.cpp
)



add_executable (
  ${projname}
  ${OKUI_HEADER_FILES}
  ${OKUI_SOURCE_FILES}
)


target_include_directories ( 
  ${projname}  
  
  PUBLIC
  ${CMAKE_SOURCE_DIR}/rokz

  /usr/include/shaderc
  /usr/include/freetype2
)

target_link_libraries (
  ${projname}
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

message (STATUS "${projname} - ${CMAKE_SOURCE_DIR}/rokz")
message (STATUS "${projname} - ${CMAKE_CURRENT_BINARY_DIR}")
message (STATUS "${projname} - ${CMAKE_CURRENT_SOURCE_DIR}")
