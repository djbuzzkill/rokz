
#ifndef ROKZ_TYPES_INCLUDE
#define ROKZ_TYPES_INCLUDE


#include "common.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"


//#include "shared_types.h"

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


  // -----------------------------------------------------------------------------------
  // 
  // -----------------------------------------------------------------------------------
  typedef std::optional<uint32_t> MaybeIndex;




  // -----------------------------------------------------------------------------------
  // not sure what to do with these yet
  // -----------------------------------------------------------------------------------
  struct VkObj {
    //what needs VkDevice for cleanup?
  protected :

    VkObj (VkDevice& dev) : device (dev) {
    }

    VkDevice& device;
  }; 

  struct VMARes {
    // what needs allocator for cleanup?
  protected :
    
    VMARes (VmaAllocator& a) : allocator (a) {
    }
  
    VmaAllocator& allocator; 
  }; 

  //  -----------------------------------------------------------------------------------
  struct MVPTransform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  constexpr size_t kSizeOf_MVPTransform = sizeof (rokz::MVPTransform);
  
  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  struct QueueFamilyIndices {

    QueueFamilyIndices () : graphics (), present () {
    }
    
    MaybeIndex graphics;
    MaybeIndex present; 
  };

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------

  struct Window {

    Window () : glfw_window (nullptr) {
    }
    
    GLFWwindow*  glfw_window;
  };

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
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

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  struct PhysicalDevice {
    PhysicalDevice () : handle (VK_NULL_HANDLE),
      properties (),  features (), family_indices () {}
    
    
    VkPhysicalDevice           handle;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures   features;
    QueueFamilyIndices         family_indices;
  }; 

  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
  struct Allocator {
    VmaAllocator           handle;
    VmaAllocatorCreateInfo ci;  
  };
  
  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
  struct CommandPool {
    VkCommandPool           handle;
    VkCommandPoolCreateInfo ci; 
  };

  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
  struct Device {

    Device () : handle (VK_NULL_HANDLE) , ci (), queue_ci (), physical() {
    }
      
    VkDevice                             handle;
    VkDeviceCreateInfo                   ci;
    std::vector<VkDeviceQueueCreateInfo> queue_ci;

    CommandPool                          command_pool;
    Allocator                            allocator;

    struct { VkQueue graphics; VkQueue present; } queues;
    struct { float graphics; float present; } priority;

    PhysicalDevice physical;
    
    // device extensions strings
    std::vector<const char*> dxs; 
    std::vector<std::string> dxstrs; 
    // validation layers
    std::vector<const char*> vals; 
    std::vector<std::string> valstrs; 

  }; 
  
  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
  struct ShaderModule {

    ShaderModule () : handle (VK_NULL_HANDLE), bin (), ci () {
    }
    
    VkShaderModule                 handle; 
    rokz::bytearray                bin; 
    VkShaderModuleCreateInfo       ci;
  };

  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
  struct Buffer {
    VkBuffer                handle;
    VkBufferCreateInfo      ci; 
    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info;
  }; 

  // ---------------------------------------------------------------------
  // ---------------------------------------------------------------------
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

  struct ViewportState {
    std::vector<VkViewport> viewports;
    std::vector<VkRect2D>   scissors;
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct PipelineState {

    PipelineStateCreateInfo ci;
    std::vector<VkPipelineColorBlendAttachmentState> colorblend_attachments;     
    ViewportState                                    viewport;
    std::vector<VkDynamicState>                      dynamics; 

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

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  struct LayoutGroup {

    LayoutGroup () : pipeline(), descriptor () {
    }
    // how  it looks        
    PipelineLayout      pipeline; 
    DescriptorSetLayout descriptor;
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
    std::vector<rokz::ShaderModule>  shader_modules; 
    // EXTENSIONS
    struct { 
      struct {
        VkPipelineRenderingCreateInfoKHR ci; 
        std::vector<VkFormat> color_formats;
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
      
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
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

  // --------------------------------------------------------------------
  // VK 1.3 | dynamic rendering 
  // --------------------------------------------------------------------
  struct RenderingInfoGroup {

    VkRenderingInfo                        ri;
    std::vector<VkRenderingAttachmentInfo> color_attachment_infos;
    VkRenderingAttachmentInfo              depth_attachment_info;

    std::vector<VkClearValue>              clear_colors;  // = {{0.0f, 0.0f, 0.0f, 1.0f}};
    VkClearValue                           clear_depth; //  = {{

    VkRect2D                               render_area; 
  };

  // // --------------------------------------------------------
  // struct CommandPool {
  //   VkCommandPool           handle;
  //   VkCommandPoolCreateInfo ci; 
  // };

  // --------------------------------------------------------
  struct CommandBufferGroup {

    std::vector<VkCommandBuffer> buffers; 
    VkCommandBufferAllocateInfo  alloc_info;
  };

  // --------------------------------------------------------
  struct SwapchainGroup {
    rokz::Swapchain                swapchain    ;// = glob.swapchain; 
    std::vector<rokz::Image>       images       ;// = glob.swapchain_images; 
    std::vector<rokz::ImageView>   imageviews   ;//= glob.swapchain_imageviews; 
    std::vector<rokz::Framebuffer> framebuffers ;//= glob.swapchain_framebuffers; 

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
    FrameSync () : image_available_sem (), render_finished_sem (), in_flight_fen () {    }
    
    VkSemaphore image_available_sem;
    VkSemaphore render_finished_sem;
    VkFence     in_flight_fen;

    FrameSyncCreateInfo ci;

  };
  // --------------------------------------------------------
  struct FrameSyncGroup {
    //
    VkCommandBufferAllocateInfo  command_buffer_alloc_info;
    // kMaxFramesInFlight
    std::vector<VkCommandBuffer> command_buffers;
    std::vector<FrameSync> syncs;
  }; 



} // namespace rokz

#endif
