
#ifndef ROKZ_TYPES_INCLUDE
#define ROKZ_TYPES_INCLUDE


#include "common.h"


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
    VkImageViewCreateInfo    imageview; 

  };

  struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };


  // Context
  struct Glob {
    
    VkApplicationInfo     app_info; // {};
    VkInstance            instance;

    VkPhysicalDevice      physical_device;
    VkPhysicalDeviceFeatures device_features;
    VkSwapchainKHR        swapchain;
    std::vector<VkImage>  swapchain_images;
    std::vector<VkImageView> swapchain_imageviews;
    std::vector<VkShaderModule> shader_modules; 
    
    VkPipelineLayout pipeline_layout; 
    
    // device + queues?
    GLFWwindow*           glfwin;  // 
    VkSurfaceKHR          surface; // surface is related to a window?
    VkDevice              device;
    
    struct { VkQueue graphics; VkQueue present; } queues;

    QueueFamilyIndices    queue_fams;
    float                 queue_priority;
    
    CreateInfo            create_info;
    // bool               enable_validation;
  };


} // namespace rokz

#endif
