
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

  typedef std::optional<unsigned int> MaybeIndex;


  struct QueueFamilyIndices {
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  
  //
  struct CreateInfo {
    VkInstanceCreateInfo     instance; // {};
    VkDeviceCreateInfo       device;
    VkDeviceQueueCreateInfo  queue;
  };

  //
  struct Glob {
    
    VkApplicationInfo     app_info; // {};
    VkInstance            instance;

    VkPhysicalDevice      physical_device;
    VkPhysicalDeviceFeatures device_features;

    VkDevice              device;
    GLFWwindow*           glfwin;

    QueueFamilyIndices    queue_fams;
    float                 queue_priority;
    
    struct { VkQueue graphics; VkQueue present; } queues;

    VkSurfaceKHR          surface;
    
    CreateInfo            create_info;

    
    // bool               enable_validation;
  };

  Glob& 
    Default (Glob& g); 
  
  VkDeviceQueueCreateInfo& 
    Default (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority);

  VkDeviceCreateInfo&
    Default (VkDeviceCreateInfo& info,
             VkDeviceQueueCreateInfo* quecreateinfo,
             VkPhysicalDeviceFeatures* devfeats); 
  
  QueueFamilyIndices& FindQueueFamilies (QueueFamilyIndices& queue_fams,
                                         const VkSurfaceKHR& surf,
                                         const VkPhysicalDevice& physdev);

  GLFWwindow* CreateWindow_glfw (GLFWwindow*& w);
  bool CheckValidationSupport (const std::vector<const char*>& validation_layers);
  int  CreateInstance (Glob& glob);
  
  bool SelectPhysicalDevice (VkPhysicalDevice& physdev,
                             QueueFamilyIndices& queueind,
                             const VkSurfaceKHR& surf,
                             const VkInstance& inst);

  bool CreateLogicalDevice (VkDevice*                 device,
                            const VkDeviceCreateInfo* createinfo,
                            const VkPhysicalDevice&   physdev);

  void GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device); 

  void Cleanup (VkSurfaceKHR& surf, GLFWwindow* w, VkDevice& dev, VkInstance &vkinst);


  // TODO
  bool CreateSurface    (VkSurfaceKHR* surf, GLFWwindow* glfwin , const VkInstance& inst);
  void CreateSwapChain  (); 
  void CreateImageViews (); 
  void CreateRenderPass ();
  void CreatePipeline   (); 
  
} // namespace rokz

#endif
