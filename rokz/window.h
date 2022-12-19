
#ifndef ROKZ_WINDOW_H
#define ROKZ_WINDOW_H

#include "common.h"
#include "shared_types.h"

namespace rokz {



  GLFWwindow* CreateWindow_glfw (GLFWwindow*& w); 

  //SwapchainSupportInfo& QuerySwapchainSupport (SwapchainSupportInfo& deets, const VkSurfaceKHR& surf, const VkPhysicalDevice& dev); 
  bool CreateWindow (Window& wnd, uint32_t w, uint32_t h, const std::string& title = std::string ("lol")); 
  

}

#endif
