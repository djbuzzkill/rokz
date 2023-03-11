
#include "display.h"
#include "context.h"
#include "window.h"
#include "window_handler.h"


bool rokz::SetupDisplay (Display& display, InputState& input_state, const VkExtent2D& dim, const Instance& instance) { 
  
  // create GLFW window
  CreateWindow (display.window, dim.width, dim.height, "wut"); 
  glfwSetFramebufferSizeCallback (display.window.glfw_window, win_event::on_resize ); 
  glfwSetKeyCallback (display.window.glfw_window, win_event::on_keypress);
  glfwSetCursorPosCallback(display.window.glfw_window, win_event::on_mouse_move);
  glfwSetMouseButtonCallback(display.window.glfw_window, win_event::on_mouse_button);
  glfwSetCursorEnterCallback (display.window.glfw_window, win_event::on_mouse_enter); 
                              
  glfwSetWindowUserPointer (display.window.glfw_window, &input_state);

  // create surface
  return cx::CreateSurface (&display.surface, display.window.glfw_window, instance.handle);

}
