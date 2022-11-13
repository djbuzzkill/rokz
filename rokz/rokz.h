
#ifndef ROKZ_INCLUDE
#define ROKZ_INCLUDE



#include <ctime>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <memory>
#include <map>
#include <set>
#include <array>

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <tuple>
#include <functional>
#include <optional>
#include <random>

#include <chrono>
#include <thread>
#include <iostream>


//#include <vulkan/vulkan.h> <--  GLFW_INCLUDE_VULKAN will do it for us
#define VK_USE_PLATFORM_XCB_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_core.h>



namespace rokz {

  typedef std::optional<uint32_t> MaybeIndex;


  struct QueueFamilyIndices {
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  //
  struct CreateInfo {
    VkInstanceCreateInfo     instance; // {};
    VkDeviceCreateInfo       device;
    VkDeviceQueueCreateInfo  queue;
    VkSwapchainCreateInfoKHR swapchain;
  };


  struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };   
  
  //
  struct Glob {
    
    VkApplicationInfo     app_info; // {};
    VkInstance            instance;

    VkPhysicalDevice      physical_device;
    VkPhysicalDeviceFeatures device_features;
    VkSwapchainKHR        swapchain;
    // device + queues?
    GLFWwindow*           glfwin;
    VkSurfaceKHR          surface;
    VkDevice              device;
    struct { VkQueue graphics; VkQueue present; } queues;

    QueueFamilyIndices    queue_fams;
    float                 queue_priority;
    
    CreateInfo            create_info;
    // bool               enable_validation;
  };

  bool
  Initialize (Glob& glob);

  // DEFAULTS
  VkSwapchainCreateInfoKHR&
  Default (VkSwapchainCreateInfoKHR& info, const VkSurfaceKHR& surf); 

  Glob& 
  Default (Glob& g); 
  
  VkDeviceQueueCreateInfo& 
  Default (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority);

  VkDeviceCreateInfo&
  Default (VkDeviceCreateInfo& info,
           VkDeviceQueueCreateInfo* quecreateinfo,
           VkPhysicalDeviceFeatures* devfeats); 
  
  QueueFamilyIndices&
  FindQueueFamilies (QueueFamilyIndices& queue_fams,
                     const VkSurfaceKHR& surf,
                     const VkPhysicalDevice& physdev);

  SwapchainSupportInfo&
  QuerySwapchainSupport (SwapchainSupportInfo& deets,
                         const VkSurfaceKHR& surf,
                         const VkPhysicalDevice& dev); 
  
  GLFWwindow*
  CreateWindow_glfw (GLFWwindow*& w);

  bool
  CheckValidationSupport (const std::vector<const char*>& validation_layers);

  int
  CreateInstance (Glob& glob);
  
  bool
  SelectPhysicalDevice (VkPhysicalDevice& physdev,
                        QueueFamilyIndices& queueind,
                        const VkSurfaceKHR& surf,
                        const VkInstance& inst);

  bool
  CreateLogicalDevice (VkDevice*                 device,
                       const VkDeviceCreateInfo* createinfo,
                       const VkPhysicalDevice&   physdev);

  void
  GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device); 

  void
  Cleanup (VkSwapchainKHR& swapchain,
           VkSurfaceKHR& surf,
           GLFWwindow* w,
           VkDevice& dev,
           VkInstance &inst);

  VkSurfaceFormatKHR
  ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

  VkPresentModeKHR
  ChooseSwapPresentMode  (const std::vector<VkPresentModeKHR>& available_modes);

  VkExtent2D
  ChooseSwapExtent       (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win);
  
  // TODO
  bool
  CreateSurface   (VkSurfaceKHR* surf, GLFWwindow* glfwin , const VkInstance& inst);

  bool
  CreateSwapchain (VkSwapchainKHR&         swapchain, 
                        VkSwapchainCreateInfoKHR& swapchaincreateinfo,
                        const VkSurfaceKHR&     surf,
                        const VkPhysicalDevice& physdev, 
                        const VkDevice&         dev, 
                        GLFWwindow*             glfwin);

  bool
  GetSwapChainImages (std::vector<VkImage> &swapchain_images,
                           VkSwapchainKHR& swapchain,
                           const VkDevice& dev);

  bool
  CreateImageViews (std::vector<VkImageView>& swapchain_imageviews);

  void
  CreateRenderPass ();

  bool
  CreatePipeline   (); 
  
} // namespace rokz

#endif
