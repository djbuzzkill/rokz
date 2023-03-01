
#ifndef ROKZ_WINDOW_H
#define ROKZ_WINDOW_H

#include "common.h"
#include "rokz_types.h"

namespace rokz {

  // typedef void (* GLFWerrorfun)(int error_code, const char* description);
  // typedef void (* GLFWwindowposfun)(GLFWwindow* window, int xpos, int ypos);
  // typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);
  // typedef void (* GLFWwindowclosefun)(GLFWwindow* window);
  // typedef void (* GLFWwindowrefreshfun)(GLFWwindow* window);
  // typedef void (* GLFWwindowfocusfun)(GLFWwindow* window, int focused);
  // typedef void (* GLFWwindowiconifyfun)(GLFWwindow* window, int iconified);
  // typedef void (* GLFWwindowmaximizefun)(GLFWwindow* window, int maximized);
  // typedef void (* GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);
  // typedef void (* GLFWwindowcontentscalefun)(GLFWwindow* window, float xscale, float yscale);
  // typedef void (* GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);
  // typedef void (* GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);
  // typedef void (* GLFWcursorenterfun)(GLFWwindow* window, int entered);
  // typedef void (* GLFWscrollfun)(GLFWwindow* window, double xoffset, double yoffset);
  // typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
  // typedef void (* GLFWcharfun)(GLFWwindow* window, unsigned int codepoint);
  // typedef void (* GLFWcharmodsfun)(GLFWwindow* window, unsigned int codepoint, int mods);
  // typedef void (* GLFWdropfun)(GLFWwindow* window, int path_count, const char* paths[]);
  // typedef void (* GLFWmonitorfun)(GLFWmonitor* monitor, int event);
  // typedef void (* GLFWjoystickfun)(int jid, int event);
  
  //SwapchainSupportInfo& QuerySwapchainSupport (SwapchainSupportInfo& deets, const VkSurfaceKHR& surf, const VkPhysicalDevice& dev); 
  bool CreateWindow (Window& wnd, uint32_t w, uint32_t h, const std::string& title = std::string ("lol")); 
  

}

#endif
