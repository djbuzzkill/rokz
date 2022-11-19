
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
    VkRenderPassCreateInfo   renderpass; 
    VkGraphicsPipelineCreateInfo pipeline;


  // VkPipelineShaderStageCreateInfo vert_shader_stage_info {};
  // VkPipelineShaderStageCreateInfo frag_shader_stage_info {};
  // VkPipelineInputAssemblyStateCreateInfo input_assembly{};
  // VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};
  // VkPipelineViewportStateCreateInfo viewport_state_create_info{};
  // VkPipelineRasterizationStateCreateInfo rasterizer{};
  // VkPipelineMultisampleStateCreateInfo multisampling{};
  // VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_create_info {};
  // VkPipelineColorBlendStateCreateInfo color_blending_create_info{};
  // VkPipelineLayoutCreateInfo pipeline_layout_create_info{};







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
    
    VkRenderPass render_pass;
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
