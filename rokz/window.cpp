
#include "window.hpp"






// -------------------------------------------------------------------------------------
//                 
// -------------------------------------------------------------------------------------
GLFWwindow* CreateWindow_glfw (GLFWwindow*& wn, uint32_t wd, uint32_t ht, const std::string& title) {
  printf ("%s\n", __FUNCTION__); 
  
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
  wn = glfwCreateWindow(wd, ht, title.c_str(), nullptr, nullptr);
 
  return wn; 
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


