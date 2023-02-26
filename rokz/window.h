
#ifndef ROKZ_WINDOW_H
#define ROKZ_WINDOW_H

#include "common.h"
#include "rokz_types.h"

namespace rokz {

  typedef void (*WindowEvent_OnResize)        (GLFWwindow* window, int width, int height); 
  typedef void (*WindowEvent_OnKeyPress)      (GLFWwindow* window, int key, int scancode, int action, int mods); 
  typedef void (*WindowEvent_OnMouseButtonCB) (GLFWwindow* window, int button, int action, int mods);
  typedef void (*WindowEvent_OnMouseMove)     (GLFWwindow* window, double xpos, double ypos); 
  typedef void (*WindowEvent_OnMouseEnter)    (GLFWwindow* window, int entered); 

  GLFWwindow* CreateWindow_glfw (GLFWwindow*& w); 

  //SwapchainSupportInfo& QuerySwapchainSupport (SwapchainSupportInfo& deets, const VkSurfaceKHR& surf, const VkPhysicalDevice& dev); 
  bool CreateWindow (Window& wnd, uint32_t w, uint32_t h, const std::string& title = std::string ("lol")); 
  

}

#endif
