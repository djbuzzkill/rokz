

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

    QueueFamilyIndices () : graphics(), present () {
    }
    
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------

  struct Window {
    GLFWwindow*  glfw_window;
  };

  // --------------------------------------------------------
  struct Instance {
    VkInstance handle;
    VkInstanceCreateInfo ci;

    std::vector<const char*> required_extensions;
    VkApplicationInfo app_info;
  };

  // --------------------------------------------------------
  struct PhysicalDevice {
    VkPhysicalDevice           handle;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures   features;
    QueueFamilyIndices         family_indices;
  }; 

  // --------------------------------------------------------
  struct Device {
    VkDevice                handle;
    VkDeviceCreateInfo      ci;
    std::vector<VkDeviceQueueCreateInfo>  queue_ci;
  }; 
  


  // --------------------------------------------------------
  struct ShaderModule {
    VkShaderModule                 handle; 
    rokz::bytearray                bin; 
    VkShaderModuleCreateInfo       ci;
  };

  // ------------------------------------------------------------------
  struct Image {

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

    VkImageView           handle;
    VkImageViewCreateInfo ci; 

    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info_;
  };
  
  // ---------------------------------------------------------------------
  struct Framebuffer {
    VkFramebuffer           handle; 
    VkFramebufferCreateInfo ci;
    std::vector<VkImageView> attachments; 
  };

  // --------------------------------------------------------
  struct PipelineStateCreateInfo {
    std::vector<VkPipelineShaderStageCreateInfo> shader_stages; 
    VkPipelineInputAssemblyStateCreateInfo      input_assembly; 
    VkPipelineVertexInputStateCreateInfo        vertex_input_state;
    VkPipelineViewportStateCreateInfo           viewport_state;
    VkPipelineRasterizationStateCreateInfo      rasterizer;
    VkPipelineMultisampleStateCreateInfo        multisampling;
    VkPipelineDepthStencilStateCreateInfo       depthstencil;
    VkPipelineColorBlendStateCreateInfo         colorblend; 
    VkPipelineDynamicStateCreateInfo            dynamic_state;
  };

  // --------------------------------------------------------
  struct PipelineLayout { 
    VkPipelineLayout            handle;
    VkPipelineLayoutCreateInfo  ci;
  };


  // --------------------------------------------------------
  struct Pipeline { 
    VkPipeline                   handle; 
    VkGraphicsPipelineCreateInfo ci;
    PipelineLayout               layout; 
    PipelineStateCreateInfo      state_ci;
  };

  // ----------------------------------------------------------
  struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };

  // ----------------------------------------------------------
  struct Swapchain {
    VkSwapchainKHR           handle;
    VkSwapchainCreateInfoKHR ci;
    std::vector<uint32_t>    family_indices; 
    //SwapchainSupportInfo     swapchain_support_info;
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
      CORESOLV = 2,
      UNUSED_2 = 3,
      ATTACHMENT_INDEX_MAX
    }; 
  
    VkRenderPass            handle; 
    VkRenderPassCreateInfo  create_info;

    VkAttachmentDescription attach_desc[ATTACHMENT_INDEX_MAX];
    VkAttachmentReference   attach_ref[ATTACHMENT_INDEX_MAX];
    std::vector<VkSubpassDescription> subpass_descs;

    VkSubpassDependency dependancy;
  };

  // --------------------------------------------------------
  struct SyncCreateInfo {
    VkSemaphoreCreateInfo  semaphore;
    VkFenceCreateInfo      fence;
  }; 

  
  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  struct destructor {
    
    virtual ~destructor () = default;

  protected:
    destructor () = default;
  }; 



}

#endif
