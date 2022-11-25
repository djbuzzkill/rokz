
#ifndef ROKZ_TYPES_INCLUDE
#define ROKZ_TYPES_INCLUDE


#include "common.h"


namespace rokz {
  //
  typedef std::optional<uint32_t> MaybeIndex;

  // --------------------------------------------------------
  struct QueueFamilyIndices {
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  // --------------------------------------------------------
  struct SyncStruc  {
    VkSemaphore image_available_sem;
    VkSemaphore render_fnished_sem;
    VkFence     in_flight_fen;
  };
  
  // --------------------------------------------------------
  struct RenderPass {
    enum AttachmentIndex {
      COLOR = 0, 
      DEPTHSTENCIL = 1, 
      UNUSED = 2,
    }; 
  
    VkRenderPass            handle; 
    VkRenderPassCreateInfo  create_info;

    VkAttachmentDescription attach_desc[3];
    VkAttachmentReference   attach_ref[3];

    std::vector<VkSubpassDescription> subpass_descs;

    VkSubpassDependency dependancy;
  };

  struct SyncCreateInfo {
    VkSemaphoreCreateInfo  semaphore;
    VkFenceCreateInfo      fence;
  }; 


  struct BufferStruc {
    VkBuffer              handle;
    VkDeviceMemory        mem; 
    VkBufferUsageFlags    usage_flags; 
    VkMemoryPropertyFlags mem_prop_flags;
    VkBufferCreateInfo    create_info; 
    VkMemoryAllocateInfo  alloc_info;
    VkIndexType          index_type;
  }; 
  
  // --------------------------------------------------------
  struct CreateInfo {
    VkInstanceCreateInfo                     instance; 
    VkDeviceCreateInfo                       device;
    VkDeviceQueueCreateInfo                  queue;
    VkSwapchainCreateInfoKHR                 swapchain;
    VkImageViewCreateInfo                    imageview; 
    VkRenderPassCreateInfo                   renderpass; 

    VkCommandPoolCreateInfo                  command_pool; 
    std::vector<VkCommandBufferAllocateInfo> command_buffer; 

    VkPipelineLayoutCreateInfo               pipeline_layout; 
    VkGraphicsPipelineCreateInfo             pipeline;
    std::vector<VkFramebufferCreateInfo>     framebuffers; 
    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
    VkPipelineVertexInputStateCreateInfo     vertex_input_state; 
    VkPipelineViewportStateCreateInfo        viewport_state;
    //

    std::vector<SyncCreateInfo> syncs; 
    // VkSemaphoreCreateInfo  semaphore;
    // VkFenceCreateInfo      fence;
    // //
    VkPipelineInputAssemblyStateCreateInfo   input_assembly; 
    VkPipelineDynamicStateCreateInfo         dynamic_state;
    VkPipelineRasterizationStateCreateInfo   rasterizer;
    VkPipelineMultisampleStateCreateInfo     multisampling; 
    VkPipelineDepthStencilStateCreateInfo    pipeline_depth_stencil; 
    VkPipelineColorBlendStateCreateInfo      color_blend;
  };

  // --------------------------------------------------------
  struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };

  // --------------------------------------------------------
  struct Glob {
    VkApplicationInfo            app_info; // {};
    VkInstance                   instance;

    VkPhysicalDevice             physical_device;
    VkPhysicalDeviceFeatures     device_features;
    VkSwapchainKHR               swapchain;

    std::vector<VkImage>         swapchain_images;
    std::vector<VkImageView>     swapchain_imageviews;
    std::vector<VkFramebuffer>   swapchain_framebuffers;
    std::vector<VkShaderModule>  shader_modules; 

    VkPipelineColorBlendAttachmentState color_blend_attachment_state;     

    
    BufferStruc vertex_buffer_user; 
    BufferStruc index_buffer_user;
    
    BufferStruc index_buffer_device; 
    BufferStruc vertex_buffer_device; 
    
    std::vector<VkDynamicState>  dynamic_states; 
    VkCommandPool                command_pool; 

    std::vector<VkCommandBuffer> command_buffer; 
    std::vector<SyncStruc>       syncs; 

    RenderPass                  render_pass; 
    VkDescriptorSetLayout       descriptor_layout;    

    VkPipelineLayout            pipeline_layout; 
    VkPipeline                  pipeline; 
    // device + queues?
    GLFWwindow*                 glfwin;  // 
    VkSurfaceKHR                surface; // 
    VkDevice                    device;

    VkViewport                  viewport;
    VkRect2D                    scissor; 
    
    struct { VkQueue graphics; VkQueue present; } queues;

    QueueFamilyIndices         queue_fams;
    float                      queue_priority;
    CreateInfo                 create_info;
    // bool               enable_validation;
  };


} // namespace rokz

#endif
