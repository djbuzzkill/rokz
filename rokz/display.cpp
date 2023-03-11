
#include "display.h"
#include "window.h"

using namespace rokz;

bool rokz::SetupDisplay (rekz::Display& display, InputState& input_state, const VkExtent2D& dim, const Instance& instance) { 
  
  // create GLFW window
  rokz::CreateWindow (display.window, dim.width, dim.height, "wut"); 
  glfwSetFramebufferSizeCallback (display.window.glfw_window, rekz::win_event::on_resize ); 
  glfwSetKeyCallback (display.window.glfw_window, rekz::win_event::on_keypress);
  glfwSetCursorPosCallback(display.window.glfw_window, rekz::win_event::on_mouse_move);
  glfwSetMouseButtonCallback(display.window.glfw_window, rekz::win_event::on_mouse_button);
  glfwSetCursorEnterCallback (display.window.glfw_window, rekz::win_event::on_mouse_enter); 
                              
  glfwSetWindowUserPointer (display.window.glfw_window, &input_state);

  // create surface
  return  rokz::cx::CreateSurface  (&display.surface, display.window.glfw_window, instance.handle);

}
