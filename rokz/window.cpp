
#include "window.h"






// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
GLFWwindow* rokz::CreateWindow_glfw (GLFWwindow*& w) {

  printf ("%s\n", __FUNCTION__); 
  
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
  w = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
 
  return w; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateWindow (Window& wnd, uint32_t w, uint32_t h, const std::string& title) {
  printf ("%s\n", __FUNCTION__); 

  if (wnd.glfw_window) {
    printf ("%s [Window::glfw_window] NOT NULL \n", __FUNCTION__); 
  }

  //wnd.glfw_window = nullptr;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  
  wnd.glfw_window = glfwCreateWindow(w, h, title.c_str (), nullptr, nullptr); 

  if (wnd.glfw_window != nullptr)  {
    return true; 
  }
  

  printf ("[FAILED] %s \n", __FUNCTION__); 
  return false;
  
}


