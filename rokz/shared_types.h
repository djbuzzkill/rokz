

#ifndef ROKZ_SHARED_TYPES_INCLUDE
#define ROKZ_SHARED_TYPES_INCLUDE

#include "common.h"

namespace rokz { 
  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  typedef std::uint8_t  uint8;
  typedef std::uint16_t uint16; 
  typedef std::uint32_t uint32; 
  typedef std::uint64_t uint64; 
  
  typedef std::int8_t   int8;
  typedef std::int16_t  int16; 
  typedef std::int32_t  int32; 
  typedef std::int64_t  int64;
  typedef std::uint8_t  byte;

  typedef std::vector<byte> bytearray; 


  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  typedef std::optional<uint32_t> MaybeIndex;

  // ---------------------------------------------------
  struct MVPTransform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  constexpr size_t kSizeOf_MVPTransform = sizeof (rokz::MVPTransform);
  
  // --------------------------------------------------------
  struct QueueFamilyIndices {

    QueueFamilyIndices () : graphics (), present () {
    }
    
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------

  struct Window {
    Window () : glfw_window (nullptr) {
    }
    
    GLFWwindow*  glfw_window;
  };

  // --------------------------------------------------------
  struct Instance {

    Instance () : handle (VK_NULL_HANDLE), ci(), required_extensions (), app_info () {
    }
    
    VkInstance handle;
    VkInstanceCreateInfo ci;

    std::vector<const char*> required_extensions;
    std::vector<std::string> extension_strings; 

    std::vector<std::string> validation_strings; 
    std::vector<const char*> vals;

    VkApplicationInfo        app_info;
  };

  // --------------------------------------------------------
  struct PhysicalDevice {
    PhysicalDevice () : handle (VK_NULL_HANDLE),
      properties (),  features (), family_indices () {}
    
    
    VkPhysicalDevice           handle;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures   features;
    QueueFamilyIndices         family_indices;
  }; 

  // --------------------------------------------------------
  struct Device {

    Device () : handle (VK_NULL_HANDLE) , ci (), queue_ci () {
    }
      
    VkDevice                             handle;
    VkDeviceCreateInfo                   ci;
    std::vector<VkDeviceQueueCreateInfo> queue_ci;
    // device extensions strings
    std::vector<const char*> dxs; 
    std::vector<std::string> dxstrs; 
    // validation layers
    std::vector<const char*> vals; 
    std::vector<std::string> valstrs; 

  }; 
  
  // --------------------------------------------------------
  struct ShaderModule {

    ShaderModule () : handle (VK_NULL_HANDLE), bin (), ci () {
    }
    
    VkShaderModule                 handle; 
    rokz::bytearray                bin; 
    VkShaderModuleCreateInfo       ci;
  };

  // ------------------------------------------------------------------
  struct Image {

    Image () : handle (VK_NULL_HANDLE), ci (), mem (), alloc_info (), 
               alloc_ci (), allocation (),alloc_info_ () {}

    VkImage              handle;
    VkImageCreateInfo    ci;
    VkDeviceMemory       mem; 
    VkMemoryAllocateInfo alloc_info;
    
    // vmaCreateBuffer(glob.allocator, &buffer_info, &allocInfo, &buffer, &allocation, nullptr);
    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info_;
  
  }; 

  // ------------------------------------------------------------------
  struct ImageView {
    ImageView() : handle (VK_NULL_HANDLE), ci () { 
    }

    VkImageView             handle;
    VkImageViewCreateInfo   ci; 
  };


  // --------------------------------------------------------
  struct RenderSyncCreateInfo {

    RenderSyncCreateInfo () : semaphore (), fence () {
    }
    
    VkSemaphoreCreateInfo  semaphore;
    VkFenceCreateInfo      fence;

  }; 
  // --------------------------------------------------------
  struct RenderSync {

    RenderSync () :image_available_sem (), render_finished_sem (), in_flight_fen () {
    }
      
    VkSemaphore image_available_sem;
    VkSemaphore render_finished_sem;
    VkFence     in_flight_fen;

    RenderSyncCreateInfo ci;

  };
  
  // ---------------------------------------------------------------------
  struct Framebuffer {
    Framebuffer () : handle (VK_NULL_HANDLE), ci (), attachments () {
    }
    
    VkFramebuffer            handle; 
    VkFramebufferCreateInfo  ci;
    std::vector<VkImageView> attachments; 
  };

  // --------------------------------------------------------
  struct PipelineStateCreateInfo {
    PipelineStateCreateInfo () : shader_stages (), input_assembly (), vertexinputstate (), viewport_state (),
                                 rasterizer (), multisampling (), depthstencilstate (), colorblendstate (), dynamicstate () {
    }

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages; 
    VkPipelineInputAssemblyStateCreateInfo       input_assembly; 
    VkPipelineVertexInputStateCreateInfo         vertexinputstate;
    VkPipelineViewportStateCreateInfo            viewport_state;
    VkPipelineRasterizationStateCreateInfo       rasterizer;
    VkPipelineMultisampleStateCreateInfo         multisampling;
    VkPipelineDepthStencilStateCreateInfo        depthstencilstate;
    VkPipelineColorBlendStateCreateInfo          colorblendstate; 
    VkPipelineDynamicStateCreateInfo             dynamicstate;
  };


  // --------------------------------------------------------
  struct PipelineState {

    VkPipelineColorBlendAttachmentState color_blend_attachment;     

    PipelineStateCreateInfo ci;
  }; 

  // --------------------------------------------------------
  struct PipelineLayout {
    PipelineLayout () : handle (VK_NULL_HANDLE) , ci () {
    }
    
    VkPipelineLayout            handle;
    VkPipelineLayoutCreateInfo  ci;
  };


  // --------------------------------------------------------
  struct Pipeline {
    Pipeline () : handle (VK_NULL_HANDLE), ci (), layout (), state () {
    }
    
    VkPipeline                       handle; 
    VkGraphicsPipelineCreateInfo     ci;
    PipelineLayout                   layout; 
    PipelineState                    state;
    std::vector<rokz::ShaderModule>  shader_modules; 
    std::vector<VkDynamicState>      dynamic_states; 
  };

  // ----------------------------------------------------------
  struct SwapchainSupportInfo {
    SwapchainSupportInfo () : capabilities (), formats (), present_modes () {
    }
      
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };

  // ----------------------------------------------------------
  struct Swapchain {
    Swapchain () : handle (VK_NULL_HANDLE), ci (), family_indices () {
    }
    
    VkSwapchainKHR           handle;
    VkSwapchainCreateInfoKHR ci;
    std::vector<uint32_t>    family_indices; 
    //SwapchainSupportInfo     swapchain_support_info;
  }; 


  enum AttachmentIndex {
    ATTACH_COLOR = 0, 
    ATTACH_DEPTHSTENCIL = 1, 
    ATTACH_COLRESOLV = 2,
    ATTACH_UNUSED_3 = 3,
    ATTACH_UNUSED_4 = 4,
    ATTACH_UNUSED_5 = 5,
    ATTACHMENT_INDEX_MAX
  }; 

  
  // --------------------------------------------------------
  struct RenderPass {
    
    RenderPass () : handle (VK_NULL_HANDLE), ci(), attach_desc (),
                    attach_ref (), subpass_descs(), dependencies () {
    }
    
    VkRenderPass            handle; 
    VkRenderPassCreateInfo  ci;

    std::array<VkAttachmentDescription, ATTACHMENT_INDEX_MAX> attach_desc;
    std::array<VkAttachmentReference, ATTACHMENT_INDEX_MAX>   attach_ref;
    std::vector<VkSubpassDescription>                         subpass_descs;
    std::vector<VkSubpassDependency>                          dependencies;
  };

  // --------------------------------------------------------
  struct CommandPool {
    VkCommandPool           handle;
    VkCommandPoolCreateInfo ci; 
  };

  // --------------------------------------------------------
  struct CommandBufferGroup {

    std::vector<VkCommandBuffer> buffers; 
    VkCommandBufferAllocateInfo  alloc_info;
  };

  // --------------------------------------------------------
  struct FrameGroup {
    rokz::Swapchain                swapchain              ;// = glob.swapchain; 
    //  #image_views
    std::vector<rokz::Image>       swapchain_images       ;// = glob.swapchain_images; 
    std::vector<rokz::ImageView>   swapchain_imageviews   ;//= glob.swapchain_imageviews; 
    std::vector<rokz::Framebuffer> swapchain_framebuffers ;//= glob.swapchain_framebuffers; 

    VkCommandBufferAllocateInfo    command_buffer_alloc_info;
    rokz::RenderSyncCreateInfo     render_sync_create_info;
    // kMaxFramesInFlight
    std::vector<rokz::RenderSync> syncs;
    std::vector<VkCommandBuffer>  command_buffers;
  }; 
  
}

#endif
