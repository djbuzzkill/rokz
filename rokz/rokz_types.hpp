
#ifndef ROKZ_TYPES_INCLUDE
#define ROKZ_TYPES_INCLUDE


#include "common.h"
#include "shared_types.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace rokz {

  //-------------------------------------------------------------------------------------
  // not sure what to do with these yet
  //-------------------------------------------------------------------------------------
  struct QueueFamilyIndices {

    QueueFamilyIndices () : graphics (), present () {
    }
    
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  struct Window {

    Window () : glfw_window (nullptr) {
    }
    
    GLFWwindow*  glfw_window;
  };

  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  struct Instance {

    Instance () : handle (VK_NULL_HANDLE), ci(), required_extensions (), app_info () {
    }
    
    VkInstance           handle;
    VkInstanceCreateInfo ci;
    Vec<const char*>     required_extensions;
    Vec<std::string>     extension_strings; 

    Vec<std::string>     validation_strings; 
    Vec<const char*>     vals;
    VkApplicationInfo    app_info;
  };

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  struct PhysicalDevice {
    PhysicalDevice () : handle (VK_NULL_HANDLE),
      properties (),  features2 (), family_indices () {}
    
    
    VkPhysicalDevice           handle;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures2  features2;
    QueueFamilyIndices         family_indices;
  }; 

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  struct Allocator {
    VmaAllocator           handle;
    VmaAllocatorCreateInfo ci;  
  };
  
  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  struct CommandPool {
    VkCommandPool           handle;
    VkCommandPoolCreateInfo ci; 
  };

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  struct Device {

    Device () : handle (VK_NULL_HANDLE) , ci (), queue_ci (), physical() {
    }
      
    VkDevice                     handle;
    VkDeviceCreateInfo           ci;
    Vec<VkDeviceQueueCreateInfo> queue_ci;

    CommandPool                  command_pool;
    Allocator                    allocator;

    struct { VkQueue graphics; VkQueue present; } queues;
    struct { float graphics; float present; } priority;

    PhysicalDevice physical;
    
    // device extensions strings
    Vec<const char*> dxs; 
    Vec<std::string> dxstrs; 
    // validation layers
    Vec<const char*> vals; 
    Vec<std::string> valstrs; 

  }; 
  
  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  typedef std::vector<uint32> spvcode;

  struct ShaderModule {

    ShaderModule (): handle (VK_NULL_HANDLE), ci () {
    }

    VkShaderModule                 handle; 
    VkShaderModuleCreateInfo       ci;
    spvcode                        spv;
  };

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  struct Buffer {
    VkBuffer handle ; 
    VkBufferCreateInfo      ci; 
    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info;
  }; 

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  struct Image {

    Image () : handle (VK_NULL_HANDLE), ci (),
               //, mem (), alloc_info (), 
               alloc_ci (), allocation (),alloc_info () {}

    VkImage              handle;
    VkImageCreateInfo    ci;
    //VkDeviceMemory       mem; 
    //    VkMemoryAllocateInfo alloc_info;
    
    // vmaCreateBuffer(glob.allocator, &buffer_info, &allocInfo, &buffer, &allocation, nullptr);
    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info;
  
  }; 

  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
  struct ImageView {
    ImageView() : handle (VK_NULL_HANDLE), ci () { 
    }

    VkImageView             handle;
    VkImageViewCreateInfo   ci; 
  };

  // ---------------------------------------------------------------------
  // ?? where should these go
  // ---------------------------------------------------------------------
  struct DescriptorPool {

    VkDescriptorPool                  handle;
    std::vector<VkDescriptorPoolSize> sizes;
    VkDescriptorPoolCreateInfo        ci;
  };

  // ---------------------------------------------------------------------
  // ?? a DrawSequence should own these
  // ---------------------------------------------------------------------
  struct DescriptorSetLayout { 
    VkDescriptorSetLayout                     handle;    
    VkDescriptorSetLayoutCreateInfo           ci;
    // dslo doesnt have to keep its own bindings
    // std::vector<VkDescriptorSetLayoutBinding> bindings; 
  }; 

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------

  struct DescriptorGroup {

    DescriptorPool               pool; // <-- put pool in here
    VkDescriptorSetAllocateInfo  alloc_info;
    // these are the final thing created
    std::vector<VkDescriptorSet> descrsets;

    //DescriptorSetLayout descriptor; <-- should be part of pipeline definition

  }; 

  // ------------------------------------------------------------------------
  struct Sampler {
    VkSampler           handle; 
    VkSamplerCreateInfo ci;
  };
  
  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct Framebuffer {
    Framebuffer () : handle (VK_NULL_HANDLE), ci (), attachments () {
    }
    
    VkFramebuffer            handle; 
    VkFramebufferCreateInfo  ci;
    std::vector<VkImageView> attachments; 
  };


  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct PipelineStateCreateInfo {
    PipelineStateCreateInfo () : shader_stages(), colorblendstate(), input_assembly(), vertexinputstate(),
                                 viewport_state(), rasterizer(), multisampling(), depthstencilstate(), dynamicstate() {
    }

    std::vector<VkPipelineShaderStageCreateInfo>     shader_stages; 

    VkPipelineColorBlendStateCreateInfo              colorblendstate; 
    VkPipelineInputAssemblyStateCreateInfo           input_assembly; 
    VkPipelineVertexInputStateCreateInfo             vertexinputstate;
    VkPipelineViewportStateCreateInfo                viewport_state;
    VkPipelineTessellationStateCreateInfo            tesselation; 
    VkPipelineRasterizationStateCreateInfo           rasterizer;
    VkPipelineMultisampleStateCreateInfo             multisampling;
    VkPipelineDepthStencilStateCreateInfo            depthstencilstate;
    VkPipelineDynamicStateCreateInfo                 dynamicstate;
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct ViewportState {

    struct VS {
      VkViewport viewport; VkRect2D scissor; };    
  
    Vec<VS> vps;
    // std::vector<VkViewport> viewports;
    // std::vector<VkRect2D>   scissors;
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct PipelineState {

    PipelineStateCreateInfo ci;
    Vec<VkPipelineColorBlendAttachmentState> colorblend_attachments;     
    ViewportState                            viewport;
    Vec<VkDynamicState>                      dynamics; 

  }; 

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct PipelineLayout {
    PipelineLayout () : handle (VK_NULL_HANDLE) , ci () {
    }
    
    VkPipelineLayout            handle;
    VkPipelineLayoutCreateInfo  ci;
  };

  // ------------------------------------------------------------------------------------------
  struct ShaderStageDef {
    std::string           entrypoint;
    systempath              fqsource;
    VkShaderStageFlagBits stage; 
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct Pipeline {
    Pipeline () : handle (VK_NULL_HANDLE), ci (), state () {
    }
    
    VkPipeline                       handle; 
    VkGraphicsPipelineCreateInfo     ci;
    //PipelineLayout                   layout; // move to pipeline-def

    PipelineState                    state;
    Vec<rokz::ShaderModule>  shader_modules; 
    Vec<ShaderStageDef>     shader_stage_defs;

    
    // 
    Vec<VkDescriptorSetLayout> dslos;
    // EXTENSIONS
    struct { 
      struct {
        VkPipelineRenderingCreateInfoKHR ci; 
        Vec<VkFormat> color_formats;
        VkFormat              depth_format;
      } pipeline_rendering;
    } ext;
  
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct SwapchainSupportInfo {
    SwapchainSupportInfo () : capabilities (), formats (), present_modes () {
    }
      
    VkSurfaceCapabilitiesKHR capabilities;
    Vec<VkSurfaceFormatKHR>  formats;
    Vec<VkPresentModeKHR>    present_modes;    
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct Swapchain {
    Swapchain () : handle (VK_NULL_HANDLE), ci (), family_indices () {
    }
    
    VkSwapchainKHR           handle;
    VkSwapchainCreateInfoKHR ci;
    Vec<uint32_t>            family_indices; 
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

    Vec<VkSubpassDescription>                         subpass_descs;
    Vec<VkSubpassDependency>                          dependencies;
  };

  // --------------------------------------------------------------------
  // VK 1.3 | dynamic rendering 
  // --------------------------------------------------------------------
  struct RenderingInfoGroup {

    VkRenderingInfo                ri;
    Vec<VkRenderingAttachmentInfo> color_attachment_infos;
    VkRenderingAttachmentInfo      depth_attachment_info;

    Vec<VkClearValue>              clear_colors;  // = {{0.0f, 0.0f, 0.0f, 1.0f}};
    VkClearValue                   clear_depth; //  = {{
    VkRect2D                       render_area; 
  };

  // --------------------------------------------------------
  struct CommandBufferGroup {

    std::vector<VkCommandBuffer> buffers; 
    VkCommandBufferAllocateInfo  alloc_info;
  };

  // --------------------------------------------------------
  struct SwapchainGroup {
    rokz::Swapchain        swapchain    ;// = glob.swapchain; 
    Vec<rokz::Image>       images       ;// = glob.swapchain_images; 
    Vec<rokz::ImageView>   imageviews   ;//= glob.swapchain_imageviews; 
    Vec<rokz::Framebuffer> framebuffers ;//= glob.swapchain_framebuffers; 
    // VkCommandBufferAllocateInfo    command_buffer_alloc_info;
  }; 

  // --------------------------------------------------------
  struct FrameSyncCreateInfo {

    FrameSyncCreateInfo () : semaphore (), fence () {
    }
    
    VkSemaphoreCreateInfo  semaphore;
    VkFenceCreateInfo      fence;

  }; 
  // --------------------------------------------------------
  struct FrameSync {
    // This should actually be called FrameSync
    FrameSync () : image_available_sem (), render_finished_sem (), in_flight_fen () {
    }
    
    VkSemaphore image_available_sem;
    VkSemaphore render_finished_sem;
    VkFence     in_flight_fen;

    FrameSyncCreateInfo ci;
  };
  // ------------------------------------------------------------------------------------------
  struct FrameSyncGroup {
    //
    VkCommandBufferAllocateInfo  command_buffer_alloc_info;
    // kMaxFramesInFlight
    Vec<VkCommandBuffer> command_buffers;
    Vec<FrameSync> syncs;
  }; 

} // namespace rokz

#endif
