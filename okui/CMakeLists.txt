
CMAKE_MINIMUM_REQUIRED(VERSION 3.17.2)

set (projname okui)
project (${projname})
#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

set (IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
#message (STATUS "${projname}:IMGUI_DIR -> ${IMGUI_DIR} ")


set (OKUI_HEADER_FILES
  okui.h
)

set (OKUI_SOURCE_FILES
  main.cpp
  imgui_demo.cpp
)

set (IMGUI_SOURCE_FILES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp)


set (IMGUI_HEADER_FILES
  ${IMGUI_DIR}/imgui.h
)

set (IMGUI_BACKEND_HEADER_FILES
  ${IMGUI_DIR}/backends/imgui_impl_glfw.h
  ${IMGUI_DIR}/backends/imgui_impl_vulkan.h
)

set (IMGUI_BACKEND_SOURCE_FILES
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

add_executable (
  ${projname}
  ${OKUI_HEADER_FILES}
  ${OKUI_SOURCE_FILES}
  ${IMGUI_SOURCE_FILES}
  ${IMGUI_HEADER_FILES}
  ${IMGUI_BACKEND_HEADER_FILES}
  ${IMGUI_BACKEND_SOURCE_FILES}

)


target_include_directories ( 
  ${projname}  

  PUBLIC /usr/include/shaderc
  /usr/include/freetype2

  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  
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





#add_executable(example_glfw_vulkan ${sources}

#target_link_libraries(example_glfw_vulkan ${LIBRARIES})
