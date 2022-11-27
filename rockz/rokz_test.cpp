
#include "rockz.h"              // 
#include "rokz/rokz.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "rokz/defaults.h"
//#include "rokz/rokz_funcs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


const size_t kMaxFramesInFlight = 2; 



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

struct Vertex_simple {

  glm::vec2 pos; 
  glm::vec3 col; 

}; 


struct PipelineLayout { 
  VkPipelineLayout            handle;
  VkPipelineLayoutCreateInfo  create_info;
};


const VkVertexInputBindingDescription& SimpleBindingDesc () {
  
  static  VkVertexInputBindingDescription desc;

  desc.binding = 0;
  desc.stride =  sizeof(Vertex_simple); 
  desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX ; 
    
  return desc;
}


const std::array<VkVertexInputAttributeDescription, 2>& SimpleAttributeDesc () {

  static  std::array<VkVertexInputAttributeDescription, 2> desc ;

  desc[0].binding = 0;
  desc[0].location = 0; 
  desc[0].format = VK_FORMAT_R32G32_SFLOAT;
  desc[0].offset = offsetof(Vertex_simple, pos);

  desc[1].binding = 0; 
  desc[1].location = 1;
  desc[1].format = VK_FORMAT_R32G32B32_SFLOAT; 
  desc[1].offset =  offsetof(Vertex_simple, col);

  return desc;
}


const Vertex_simple simple_verts[] = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const uint16_t simple_indices[] = {
    0, 1, 2, 2, 3, 0
};



struct BasicTransform3D {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
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

  VkGraphicsPipelineCreateInfo             pipeline;
  std::vector<VkFramebufferCreateInfo>     framebuffers; 
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
  VkPipelineVertexInputStateCreateInfo     vertex_input_state; 
  VkPipelineViewportStateCreateInfo        viewport_state;
  //

  std::vector<rokz::SyncCreateInfo> syncs; 
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

    
  rokz::BufferStruc vertex_buffer_user; 
  rokz::BufferStruc index_buffer_user;
    
  rokz::BufferStruc index_buffer_device; 
  rokz::BufferStruc vertex_buffer_device; 

  std::vector<rokz::BufferStruc>     uniform_buffers;
    
  std::vector<VkDynamicState>  dynamic_states; 
  VkCommandPool                command_pool; 

  std::vector<VkCommandBuffer> command_buffer; 
  std::vector<rokz::SyncStruc> syncs; 

  rokz::RenderPass             render_pass; 
  VkDescriptorSetLayout        descriptor_set_layout;    

  PipelineLayout              pipeline_layout; 
  VkPipeline                  pipeline; 
  // device + queues?
  GLFWwindow*                 glfwin;  // 
  VkSurfaceKHR                surface; // 
  VkDevice                    device;

  VkViewport                  viewport;
  VkRect2D                    scissor; 
    
  struct { VkQueue graphics; VkQueue present; } queues;

  rokz::QueueFamilyIndices         queue_fams;
  float                      queue_priority;
  CreateInfo                 create_info;
  // bool               enable_validation;
};


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool CreateDescriptorSetLayout (VkDescriptorSetLayout&              descriptor_set_layout,
                                VkDescriptorSetLayoutCreateInfo&    ci,
                                const VkDescriptorSetLayoutBinding& binding,
                                const VkDevice&                     device) {

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  ci.bindingCount = 1;
  ci.pBindings = &binding;

  if (vkCreateDescriptorSetLayout (device, &ci, nullptr, &descriptor_set_layout) != VK_SUCCESS) {
    printf ("failed to create descriptor set layout!");
    return false; 
  }

  return true; 
}



// ---------------------------------------------------------------------
// CreateGraphicsPipelineLayout 

// ---------------------------------------------------------------------
bool CreateGraphicsPipelineLayout_default (
    PipelineLayout&                    pipeline_layout, 
    // VkPipelineLayout&                  pipeline_layout,
    // VkPipelineLayoutCreateInfo&        create_info,
    VkDescriptorSetLayout&             desc_set_layout, 
    VkDescriptorSetLayoutBinding&      desc_set_layout_binding, 
    VkViewport&                        viewport,
    VkRect2D&                          scissor,
    CreateInfo&                        create_info,
    const VkExtent2D&                  swapchain_extent,
    const VkDevice&                    device)
{
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stages_create_info = create_info.shader_stages;
  VkPipelineVertexInputStateCreateInfo&         vertex_input_state_info = create_info.vertex_input_state;
  VkPipelineInputAssemblyStateCreateInfo&       input_assembly = create_info.input_assembly; 
  VkPipelineRasterizationStateCreateInfo&       rasterizer = create_info.rasterizer;
  VkPipelineMultisampleStateCreateInfo&         multisampling = create_info.multisampling;
  VkPipelineDepthStencilStateCreateInfo&        depth_stencil_create_info = create_info.pipeline_depth_stencil; 
  //VkPipelineColorBlendStateCreateInfo&          color_blending_create_info = create_info.color_blending;
  VkPipelineViewportStateCreateInfo&            viewport_state_create_info = create_info.viewport_state;

  rokz::Init (input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  
  rokz::Init (viewport, swapchain_extent.width, swapchain_extent.height, 1.0f);

  rokz::Init (scissor,  VkOffset2D {0, 0}, swapchain_extent);

  rokz::Init (viewport_state_create_info, viewport, scissor);

  rokz::Init (rasterizer); 

  rokz::Init (multisampling); 

  // DEPTH/STENCIL
  rokz::Init (depth_stencil_create_info); 

  // index:0, uniform, vert stage
  rokz::Init (desc_set_layout_binding, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);

  assert (false); 
  VkDescriptorSetLayoutCreateInfo desc_set_layout_ci; 
  CreateDescriptorSetLayout (desc_set_layout,
                             desc_set_layout_ci,
                             desc_set_layout_binding,
                             device); 


                             // PIPELINE LAYOUT CREATE INFO 
  pipeline_layout.create_info = {};
  pipeline_layout.create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout.create_info.setLayoutCount = 1;            
  pipeline_layout.create_info.pSetLayouts = &desc_set_layout;         
  pipeline_layout.create_info.pushConstantRangeCount = 0;    
  pipeline_layout.create_info.pPushConstantRanges = nullptr; 

  if (vkCreatePipelineLayout (device, &pipeline_layout.create_info, nullptr, &pipeline_layout.handle) != VK_SUCCESS) {
    printf("failed to create pipeline layout!\n");
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// bool CreateGraphicsPipelineLayout (
//     VkPipelineLayout&            pipeline_layout,
//     VkPipelineLayoutCreateInfo&  create_info, 
//     const VkDescriptorSetLayout& ds_set_layout, 
//     const VkDevice&              device)
// {
//   // PIPELINE LAYOUT CREATE INFO << mostly empty for now
//   create_info = {};
//   create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//   create_info.setLayoutCount = 1;            
//   create_info.pSetLayouts = &desc_set_layout;         
//   create_info.pushConstantRangeCount = 0;    
//   create_info.pPushConstantRanges = nullptr; 

//   if (vkCreatePipelineLayout (device, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
//     printf("FAILED _create pipeline layout_\n");
//     return false;
//   }

//   return true;
// }

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// bool CreateGraphicsPipeline (
//     VkPipeline&                   pipeline,
//     VkGraphicsPipelineCreateInfo& create_info,

//     const VkPipelineLayout&       pipeline_layout,
//     const rokz::RenderPass&       render_pass, 
//     const VkDevice                device) {

//   // CreateGraphicsPipeline  OLD VERSION
//   // CreateGraphicsPipeline  OLD VERSION
//   // CreateGraphicsPipeline  OLD VERSION
//   // CreateGraphicsPipeline  OLD VERSION
//   create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//   create_info.stageCount          = 2;
//   create_info.pStages             = &create_info.shader_stages[0]; 
//   create_info.pVertexInputState   = &create_info.vertex_input_state; ;
//   create_info.pInputAssemblyState = &create_info.input_assembly;
//   create_info.pViewportState      = &create_info.viewport_state;
//   create_info.pRasterizationState = &create_info.rasterizer;
//   create_info.pMultisampleState   = &create_info.multisampling;
//   create_info.pDepthStencilState  = nullptr; // create_info.pipeline_depth_stencil;  
//   create_info.pColorBlendState    = &create_info.color_blend; 
//   create_info.pDynamicState       = &create_info.dynamic_state; 
//   create_info.layout              = pipeline_layout; 
//   create_info.renderPass          = render_pass.handle;
//   create_info.subpass             = 0;
//   create_info.basePipelineHandle  = VK_NULL_HANDLE; 
//   create_info.basePipelineIndex   = -1;              


//   if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline) != VK_SUCCESS) {
//     printf("failed to create graphics pipeline!");
//     return false;
//   }

//   return true; 
// }


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool CreateGraphicsPipeline (
    VkPipeline&                   pipeline,
    VkGraphicsPipelineCreateInfo& create_info,
    const PipelineLayout&         pipeline_layout,
    const VkRenderPass&           render_pass,
    const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
    const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
    const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
    const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
    const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
    const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
    const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
    const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
    const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 
    const VkDevice                                     device)
{
  create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  create_info.pNext               = nullptr;
  create_info.flags               = 0x0; 
  create_info.stageCount          = ci_shader_stages.size();
  create_info.pStages             = &ci_shader_stages[0]; 
  create_info.pVertexInputState   = ci_vertex_input_state; ;
  create_info.pInputAssemblyState = ci_input_assembly;
  create_info.pViewportState      = ci_viewport_state;
  create_info.pRasterizationState = ci_rasterizer;
  create_info.pMultisampleState   = ci_multisampling;
  create_info.pDepthStencilState  = ci_depthstencil; 
  create_info.pColorBlendState    = ci_colorblend; 
  create_info.pDynamicState       = ci_dynamic_state; 
  create_info.layout              = pipeline_layout.handle; 
  create_info.renderPass          = render_pass;
  create_info.subpass             = 0;
  create_info.basePipelineHandle  = VK_NULL_HANDLE; 
  create_info.basePipelineIndex   = -1;              

  if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS) {
    printf("failed to create graphics pipeline!");
    return false;
  }

  return true; 
  
}

// ------------------------------------------------------------------
//
// ------------------------------------------------------------------
int  CreateInstance (Glob& glob);


Glob& Default (Glob& g) {
  g.queue_priority = 1.0f;
  g.app_info            = {};
  g.device_features     = {};
  g.create_info.instance= {};
  g.create_info.device  = {};
  g.create_info.queue   = {};
  g.create_info.swapchain= {};
  g.create_info.imageview = {}; 
  g.create_info.renderpass = {};
  g.create_info.pipeline = {}; 

  g.queue_fams = {}; 
  g.queue_fams.graphics.reset();
  g.queue_fams.present.reset();

  return g; 
}



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void look_at_this_shhhhi () {
  // auto tup = std::tuple{ 420, "wtf", 3.15f}; 
  // auto& [a1, b1, c1] = tup; 

  glm::vec3 pos; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool CreateUniformBuffers (std::vector<rokz::BufferStruc>& uniform_buffers,
                           std::vector<void*> mapped_uniform_pointers, 
                           const VkDevice& device,
                           const VkPhysicalDevice& physdev) {


  mapped_uniform_pointers.resize  (kMaxFramesInFlight); 
  uniform_buffers.resize (kMaxFramesInFlight);
  for (size_t i = 0; i <  kMaxFramesInFlight; i++) {

    if (!rokz::CreateUniformBuffer (uniform_buffers[i], sizeof (BasicTransform3D), 1, device, physdev)) {
      printf (" [FAIL] CreateUniformbuffer in  CreateUniformbuffers\n"); 
      return false; 
    }

    rokz::MapBuffer (&mapped_uniform_pointers[i], uniform_buffers[i], device); 
  }
  //createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
  //vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);

  return true; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateUniformBuffers (std::vector<void*> uniform_pointers, uint32_t current_image, float dt) {

  assert (false); 
    static auto startTime = std::chrono::high_resolution_clock::now();

    BasicTransform3D mats; 
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool CreateDescriptorSet (VkDescriptorSetLayout&           desc_set_layout, 
                          VkDescriptorSetLayoutCreateInfo& create_info,
                          VkDescriptorSetLayoutBinding&    desc_set_layout_binding, 
                          const VkDevice&                  device) {
  

  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  create_info.bindingCount = 1;
  create_info.pBindings = &desc_set_layout_binding;

  if (vkCreateDescriptorSetLayout(device, &create_info, nullptr, &desc_set_layout) != VK_SUCCESS) {
    printf ("FAILED create descriptor set layout\n");
    return true; 
  }

  return true; 
}



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void ResizeCB (GLFWwindow* window, int width, int height) {
  *reinterpret_cast<bool*> (glfwGetWindowUserPointer(window)) = true;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupScene () {
  printf ("%s\n", __FUNCTION__); 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void ShutdownScene () {
  printf ("%s\n", __FUNCTION__); 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateInput (Glob& glob, double dt) {

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateScene (Glob& glob, double dt) {

  // for (int i = 0; i < 1000000; ++i ) {
      
  //   std::cos ( 3.1459265 * i * 0.000001); 
  // }
}

// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------
bool RenderFrame (Glob &glob, uint32_t curr_frame, std::vector<rokz::SyncStruc>& syncs, bool& resize, double dt) {

  vkWaitForFences(glob.device, 1, &syncs[curr_frame].in_flight_fen, VK_TRUE, UINT64_MAX);
    
  uint32_t image_index = 0;
  VkResult acquire_res = vkAcquireNextImageKHR (glob.device,
                         glob.swapchain,
                         UINT64_MAX,
                         syncs[curr_frame].image_available_sem,
                         VK_NULL_HANDLE,
                         &image_index);

  if (acquire_res == VK_ERROR_OUT_OF_DATE_KHR || acquire_res == VK_SUBOPTIMAL_KHR || resize) {
    resize = false; 
    return rokz::RecreateSwapchain (
        glob.swapchain, glob.create_info.swapchain, glob.swapchain_images,
        glob.swapchain_framebuffers, glob.create_info.framebuffers,
        glob.render_pass, glob.swapchain_imageviews, glob.surface,
        glob.physical_device, glob.device, glob.glfwin);
  } 
  else if (acquire_res != VK_SUCCESS) {
    printf("failed to acquire swap chain image!");
    return false;
  }

  vkResetFences (glob.device, 1, &syncs[curr_frame].in_flight_fen);
  
  vkResetCommandBuffer (glob.command_buffer[curr_frame], 0);
  // rokz::RecordCommandBuffer(glob.command_buffer[curr_frame],
  //                           glob.pipeline,
  //                           glob.vertex_buffer_device.handle, //glob.vertex_buffer_user.handle, 
  //                           glob.create_info.swapchain.imageExtent,
  //                           glob.swapchain_framebuffers[image_index],
  //                           glob.render_pass, glob.device);

  rokz::RecordCommandBuffer_indexed (glob.command_buffer[curr_frame],
                            glob.pipeline,
                            glob.vertex_buffer_device.handle, //glob.vertex_buffer_user.handle, 
                            glob.index_buffer_device.handle,
                            glob.create_info.swapchain.imageExtent,
                            glob.swapchain_framebuffers[image_index],
                            glob.render_pass, glob.device);

  //bool rokz::RecordCommandBuffer_indexed (VkCommandBuffer &command_buffer,

  
  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[]      = {syncs[curr_frame].image_available_sem};
  VkSemaphore signal_semaphores[]    = {syncs[curr_frame].render_fnished_sem }; 
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores    = wait_semaphores;

  submit_info.signalSemaphoreCount = 1; 
  submit_info.pSignalSemaphores    = signal_semaphores; 

  submit_info.pWaitDstStageMask  = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &glob.command_buffer[curr_frame];
  
  if (vkQueueSubmit (glob.queues.graphics, 1, &submit_info, syncs[curr_frame].in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
  }
  
 VkPresentInfoKHR present_info {};
 VkSwapchainKHR   swapchains[] = { glob.swapchain };

 present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
 present_info.waitSemaphoreCount = 1;
 present_info.pWaitSemaphores = signal_semaphores;
 present_info.swapchainCount  = 1;
 present_info.pSwapchains     = swapchains;
 present_info.pImageIndices   = &image_index;
 present_info.pResults        = nullptr;

 vkQueuePresentKHR (glob.queues.present, &present_info);

 return true; 
}



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int rokz_test_create (const std::vector<std::string>& args) {

  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 
  double      dt = 0.0;

  Glob glob; // something representing the app state

  auto rokz_path = std::filesystem::path ( "/home/djbuzzkill/owenslake");
  Default (glob); 
  
  glfwInit();

  bool fb_resize = false; 
  rokz::CreateWindow_glfw (glob.glfwin);
  glfwSetFramebufferSizeCallback (glob.glfwin, ResizeCB); 
  glfwSetWindowUserPointer (glob.glfwin, &fb_resize); 
  
  rokz::CreateInstance       (glob.instance, glob.app_info, glob.create_info.instance);
  rokz::CreateSurface        (&glob.surface, glob.glfwin , glob.instance);
  rokz::SelectPhysicalDevice (glob.physical_device, glob.queue_fams, glob.surface, glob.instance);

  // queue info
  //rokz:: QueueFamilyIndices fam_inds;

  //fam_inds.graphics =  glob.queue_fams.graphics;
  //fam_inds.present =  glob.queue_fams.present;
  glob.queue_priority = 1.0f;
  if (glob.queue_fams.graphics.has_value()) {
    printf ("HAS_VALUE:TRUE\n"); 
  }
  else  {
    printf ("HAS_VALUE:FALSE\n"); 
  }
  
  rokz::Default (glob.create_info.queue, glob.queue_fams.graphics.value() , &glob.queue_priority);

  // device info
  rokz::Default (glob.create_info.device, &glob.create_info.queue, &glob.device_features); 

  rokz::CreateLogicalDevice (&glob.device, &glob.create_info.device, glob.physical_device); 
  // get queue handle
  rokz::GetDeviceQueue (&glob.queues.graphics, glob.queue_fams.graphics.value(), glob.device);
  rokz::GetDeviceQueue (&glob.queues.present, glob.queue_fams.present.value(), glob.device);

  rokz::CreateSwapchain (glob.swapchain, glob.create_info.swapchain,
                         glob.surface, glob.physical_device,
                         glob.device,  glob.glfwin);

  rokz::GetSwapChainImages (glob.swapchain_images, glob.swapchain, glob.device); 

  rokz::CreateImageViews (glob.swapchain_imageviews,
                          glob.swapchain_images,
                          glob.create_info.swapchain.imageFormat, 
                          glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);

  rokz::CreateRenderPass (glob.render_pass,
                          glob.create_info.swapchain.imageFormat,
                          glob.device);

  rokz::CreateShaderModules (glob.shader_modules,
                             glob.create_info.shader_stages,
                             rokz_path,
                             glob.device);

  rokz::CreateColorBlendState (glob.color_blend_attachment_state, glob.create_info.color_blend); 

  rokz::CreateDynamicStates (glob.dynamic_states,
                            glob.create_info.dynamic_state); 


  // 
  rokz::Init (glob.create_info.vertex_input_state, SimpleBindingDesc (), SimpleAttributeDesc ()); 
  

  assert (false); 
  CreateGraphicsPipelineLayout_default;

  // CreateGraphicsPipelineLayout(
  //     glob.pipeline_layout,
  //     glob.viewport,
  //     glob.scissor,
  //     glob.create_info,
  //     glob.create_info.swapchain.imageExtent,
  //     glob.device);

  assert (false); 
  // CreateGraphicsPipeline (
  //     glob.pipeline,
  //     glob.create_info.pipeline,
  //     glob.create_info,
  //     glob.pipeline_layout,
  //     glob.render_pass,
  //     glob.device);

  rokz::CreateFramebuffers (glob.swapchain_framebuffers,
                            glob.create_info.framebuffers,
                            glob.render_pass,
                            glob.create_info.swapchain.imageExtent,
                            glob.swapchain_imageviews,
                            glob.device); 

  rokz::CreateCommandPool (glob.command_pool,
                           glob.create_info.command_pool,
                           glob.queue_fams,
                           glob.device);


  rokz::BufferStruc vb_transfer; 
  rokz::CreateVertexBuffer_transfer (vb_transfer,  
                            sizeof(Vertex_simple),
                            sizeof(simple_verts) / sizeof(Vertex_simple),
                            glob.device, 
                            glob.physical_device); 
  

  void* transfer_ptr = nullptr; 
  if (rokz::MapBuffer ( &transfer_ptr, vb_transfer, glob.device)) {
    memcpy (transfer_ptr, simple_verts,  sizeof(simple_verts) ); 
    rokz::UnmapBuffer (vb_transfer, glob.device); 
  }

  rokz::CreateVertexBuffer_device (glob.vertex_buffer_device, 
                            sizeof(Vertex_simple),
                            sizeof(simple_verts) / sizeof(Vertex_simple),
                            glob.device, 
                            glob.physical_device); 

  rokz::MoveToBuffer_XB2DB  (glob.vertex_buffer_device, // device buffer
                             vb_transfer, // user buffer, 
                             sizeof(simple_verts),
                             glob.command_pool, 
                             glob.queues.graphics,
                             glob. device); 

  rokz::DestroyBuffer  (vb_transfer, glob.device); 

  // INDEX BUFF
  rokz::BufferStruc ib_transfer;
  rokz::CreateIndexBuffer_transfer (ib_transfer,
                                    VK_INDEX_TYPE_UINT16,
                                    6,
                                    glob.device,
                                    glob.physical_device); 

  if (rokz::MapBuffer ( &transfer_ptr, ib_transfer, glob.device)) {
    memcpy (transfer_ptr, simple_indices,  sizeof(simple_indices) ); 
    rokz::UnmapBuffer (ib_transfer, glob.device); 
  }

  rokz::CreateIndexBuffer_device (glob.index_buffer_device,
                                  VK_INDEX_TYPE_UINT16,
                                  6,
                                  glob.device,
                                  glob.physical_device); 


  rokz::MoveToBuffer_XB2DB  (glob.index_buffer_device, // device buffer
                             ib_transfer, // user buffer, 
                             sizeof(simple_indices),
                             glob.command_pool, 
                             glob.queues.graphics,
                             glob.device); 

  rokz::DestroyBuffer  (ib_transfer, glob.device); 


  // rokz::CreateVertexBuffer (glob.vertex_buffer_user,  // glob.vertex_buffer_user
  //                           sizeof(Vertex_simple),
  //                           sizeof(simple_verts) / sizeof(Vertex_simple),
  //                           glob.device, 
  //                           glob.physical_device); 
  
  // rokz::MoveToBuffer_user_mem (glob.vertex_buffer_user, // glob.vertex_buffer_user
  //                               simple_verts,
  //                              sizeof(simple_verts),
  //                              glob.device); 
  
  
  // items per frames 
  glob.command_buffer.resize (kMaxFramesInFlight);
  glob.create_info.command_buffer.resize (kMaxFramesInFlight);
  glob.create_info.syncs.resize (kMaxFramesInFlight);
  glob.syncs.resize (kMaxFramesInFlight);
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    // sep
    rokz::CreateCommandBuffer(glob.command_buffer[i],
                              glob.create_info.command_buffer[i],
                              glob.command_pool, glob.device);

    rokz::CreateSyncObjs(glob.syncs[i], glob.create_info.syncs[i], glob.device);
  }

  //
  SetupScene();
  //
  const double time_per_frame_sec = 1.0 / 60.0;
  dt = time_per_frame_sec; // just do this for now
  
  std::chrono::microseconds time_per_frame_us(static_cast<size_t>(time_per_frame_sec * 1000000.0));
  
  std::chrono::duration<size_t, std::chrono::microseconds::period>
    time_per_frame(time_per_frame_us);
  
  //size_t    frame_counter = 0;
  bool        run       = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int  countdown = 60; 
  //
  while (countdown && run && !glfwWindowShouldClose(glob.glfwin)) {

    glfwPollEvents(); 
    auto start = std::chrono::high_resolution_clock::now();

    UpdateInput(glob, dt);
    
    UpdateScene(glob, dt);

    UpdateUniformBuffers; //  (std::vector<void*> uniform_pointers, uint32_t current_image, float dt);
    
    result = RenderFrame (glob, curr_frame, glob.syncs, fb_resize, dt);

    if (result == false)
      run = false;
    
    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - start;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }
    curr_frame = (curr_frame + 1) % kMaxFramesInFlight;

    countdown--; 
  }

  // end loop
  ShutdownScene();

  vkDeviceWaitIdle(glob.device);


  rokz::DestroyBuffer (glob.index_buffer_device, glob.device); 
  // CLEAN UP
  rokz::Cleanup(glob.pipeline, glob.swapchain_framebuffers, glob.swapchain,
                glob.vertex_buffer_device, // glob.vertex_buffer_user, 
                glob.surface,
                glob.command_pool,
                glob.syncs, 
                glob.shader_modules, glob.pipeline_layout.handle, glob.render_pass,
                glob.swapchain_imageviews, glob.glfwin, glob.device,
                glob.instance);

  glfwTerminate();
  
  return 0;
}








// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// bool rokz::CreateGraphicsPipelineLayout(
//     VkPipelineLayout&                  pipeline_layout,
//     VkViewport&                        viewport,
//     VkRect2D&                          scissor,
//     CreateInfo&                        create_info,
//     const VkExtent2D&                  swapchain_extent,
//     const VkDevice&                    device)
// {

//   VkPipelineLayoutCreateInfo&                   pipeline_layout_create_info = create_info.pipeline_layout; 
//   std::vector<VkPipelineShaderStageCreateInfo>& shader_stages_create_info = create_info.shader_stages;
//   VkPipelineVertexInputStateCreateInfo&         vertex_input_state_info = create_info.vertex_input_state;
//   VkPipelineInputAssemblyStateCreateInfo&       input_assembly = create_info.input_assembly; 
//   VkPipelineRasterizationStateCreateInfo&       rasterizer = create_info.rasterizer;
//   VkPipelineMultisampleStateCreateInfo&         multisampling = create_info.multisampling;
//   VkPipelineDepthStencilStateCreateInfo&        pipeline_depth_stencil_create_info = create_info.pipeline_depth_stencil; 
//   //VkPipelineColorBlendStateCreateInfo&          color_blending_create_info = create_info.color_blending;
//   VkPipelineViewportStateCreateInfo&            viewport_state_create_info = create_info.viewport_state;

  
//   // INPUT ASSEMBLY STATE
//   input_assembly = {};
//   input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//   input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//   input_assembly.primitiveRestartEnable = VK_FALSE;


//   // VIEWPORT 
//   viewport = {};
//   viewport.x = 0.0f;
//   viewport.y = 0.0f;
//   viewport.width = (float)swapchain_extent.width;
//   viewport.height = (float)swapchain_extent.height;
//   viewport.minDepth = 0.0f;
//   viewport.maxDepth = 1.0f;

//   // SCISSOR RECT
//   scissor = {}; 
//   scissor.offset = {0, 0};
//   scissor.extent = swapchain_extent;

//   // VkPipelineViewportStateCreateInfo
//   viewport_state_create_info = {};
//   viewport_state_create_info.sType =
//       VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//   viewport_state_create_info.viewportCount = 1;

//   viewport_state_create_info.pViewports = &viewport;
//   viewport_state_create_info.scissorCount = 1;
//   viewport_state_create_info.pScissors = &scissor;

//   // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
//   rasterizer = {};
//   rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//   rasterizer.depthClampEnable = VK_FALSE;
//   rasterizer.rasterizerDiscardEnable = VK_FALSE;
//   rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//   rasterizer.lineWidth = 1.0f;
//   rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
//   rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//   rasterizer.depthBiasEnable = VK_FALSE;
//   rasterizer.depthBiasConstantFactor = 0.0f; 
//   rasterizer.depthBiasClamp = 0.0f;          
//   rasterizer.depthBiasSlopeFactor = 0.0f;    

//   // MULTI SAMPLING
//   multisampling = {};
//   multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//   multisampling.sampleShadingEnable = VK_FALSE;
//   multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//   multisampling.minSampleShading = 1.0f;          
//   multisampling.pSampleMask = nullptr;            
//   multisampling.alphaToCoverageEnable = VK_FALSE; 
//   multisampling.alphaToOneEnable = VK_FALSE;      

//   // DEPTH/STENCIL
//   pipeline_depth_stencil_create_info = {};

//   // PIPELINE LAYOUT CREATE INFO << mostly empty for now
//   pipeline_layout_create_info = {};
//   pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//   pipeline_layout_create_info.setLayoutCount = 0;            
//   pipeline_layout_create_info.pSetLayouts = nullptr;         
//   pipeline_layout_create_info.pushConstantRangeCount = 0;    
//   pipeline_layout_create_info.pPushConstantRanges = nullptr; 

//   if (vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
//     printf("failed to create pipeline layout!\n");
//     return false;
//   }

//   return true;
// }

// // ---------------------------------------------------------------------
// //
// // ---------------------------------------------------------------------
// bool rokz::CreateGraphicsPipeline (
//     VkPipeline&                   pipeline,
//     VkGraphicsPipelineCreateInfo& pipeline_create_info,
//     const CreateInfo&             create_info,
//     const VkPipelineLayout&       pipeline_layout,
//     const RenderPass&           render_pass,
//     const VkDevice                device) {

//   pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//   pipeline_create_info.stageCount = 2;
//   pipeline_create_info.pStages = &create_info.shader_stages[0]; 
//   pipeline_create_info.pVertexInputState = &create_info.vertex_input_state; ;
//   pipeline_create_info.pInputAssemblyState = &create_info.input_assembly;
//   pipeline_create_info.pViewportState = &create_info.viewport_state;
//   pipeline_create_info.pRasterizationState = &create_info.rasterizer;
//   pipeline_create_info.pMultisampleState = &create_info.multisampling;
//   pipeline_create_info.pDepthStencilState = nullptr; // create_info.pipeline_depth_stencil;  
//   pipeline_create_info.pColorBlendState = &create_info.color_blend; 
//   pipeline_create_info.pDynamicState = &create_info.dynamic_state; 
//   pipeline_create_info.layout = pipeline_layout; 
//   pipeline_create_info.renderPass = render_pass.handle;
//   pipeline_create_info.subpass = 0;
//   pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE; 
//   pipeline_create_info.basePipelineIndex = -1;              

//   if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline) != VK_SUCCESS) {
//     printf("failed to create graphics pipeline!");
//     return false;
//   }

//   return true; 
  
// }

