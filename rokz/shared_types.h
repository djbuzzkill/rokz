

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
    PipelineStateCreateInfo () : shader_stages (), input_assembly (), vertex_input_state (), viewport_state (),
                                 rasterizer (), multisampling (), depthstencil (), colorblend (), dynamic_state () {
    }

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages; 
    VkPipelineInputAssemblyStateCreateInfo       input_assembly; 
    VkPipelineVertexInputStateCreateInfo         vertex_input_state;
    VkPipelineViewportStateCreateInfo            viewport_state;
    VkPipelineRasterizationStateCreateInfo       rasterizer;
    VkPipelineMultisampleStateCreateInfo         multisampling;
    VkPipelineDepthStencilStateCreateInfo        depthstencil;
    VkPipelineColorBlendStateCreateInfo          colorblend; 
    VkPipelineDynamicStateCreateInfo             dynamic_state;
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
    Pipeline () : handle (VK_NULL_HANDLE), ci (), layout (), state_ci () {
    }
    
    VkPipeline                       handle; 
    VkGraphicsPipelineCreateInfo     ci;
    PipelineLayout                   layout; 
    PipelineStateCreateInfo          state_ci;
    std::vector<rokz::ShaderModule>  shader_modules; 
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
  // --------------------------------------------------------
  struct SyncStruc  {

    SyncStruc () :image_available_sem (), render_fnished_sem (), in_flight_fen () {
    }
      
    VkSemaphore image_available_sem;
    VkSemaphore render_fnished_sem;
    VkFence     in_flight_fen;
  };
  

  enum AttachmentIndex {
    ATTACH_COLOR = 0, 
    ATTACH_DEPTHSTENCIL = 1, 
    ATTACH_COLRESOLV = 2,
    ATTACH_UNUSED_2 = 3,
    ATTACHMENT_INDEX_MAX
  }; 

  // --------------------------------------------------------
  struct RenderPass {
    
    RenderPass () : handle (VK_NULL_HANDLE), create_info(), attach_desc (),
                    attach_ref (), subpass_descs(), dependancy () {
    }
    
    VkRenderPass            handle; 
    VkRenderPassCreateInfo  create_info;

    std::array<VkAttachmentDescription, ATTACHMENT_INDEX_MAX> attach_desc;
    std::array<VkAttachmentReference, ATTACHMENT_INDEX_MAX>   attach_ref;
    std::vector<VkSubpassDescription>    subpass_descs;
    VkSubpassDependency                  dependancy;
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
  struct SyncCreateInfo {

    SyncCreateInfo () : semaphore (), fence () {
    }
    
    VkSemaphoreCreateInfo  semaphore;
    VkFenceCreateInfo      fence;
  }; 

  


}

#endif
