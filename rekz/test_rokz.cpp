
#include "rekz.h"              // 
// #include "rokz/defaults.h"
// //#include "rokz/rokz_funcs.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>


const size_t kMaxFramesInFlight = 2; 


// #include "rokz/rokz.h"
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan_core.h>



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
struct Vertex_simple {

  glm::vec2 pos; 
  glm::vec3 col; 

}; 



const VkVertexInputBindingDescription kSimpleVertexBindingDesc =  {
  0,                            // binding    
  sizeof (Vertex_simple),       // stride      
  VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   
}; 

const std::vector<VkVertexInputAttributeDescription> kSimpleBindingAttributeDesc =  {

  VkVertexInputAttributeDescription {
    0,                             //  .location 
    0,                             //  .binding  
    VK_FORMAT_R32G32_SFLOAT,       //  .format   
    offsetof(Vertex_simple, pos),  //  .offset   
  },
  VkVertexInputAttributeDescription {
    1,                             //
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(Vertex_simple, col), 
  }

}; 

const Vertex_simple simple_verts[] = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const uint16_t simple_indices[] = {
    0, 1, 2, 2, 3, 0
};


struct StandardTransform3D {
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
  //

  std::vector<rokz::SyncCreateInfo> syncs; 
  // VkSemaphoreCreateInfo  semaphore;
  // VkFenceCreateInfo      fence;
  // //
  // VkPipelineInputAssemblyStateCreateInfo   input_assembly; 
  // VkPipelineVertexInputStateCreateInfo     vertex_input_state; 
  // VkPipelineViewportStateCreateInfo        viewport_state;
  // VkPipelineRasterizationStateCreateInfo   rasterizer;
  // VkPipelineMultisampleStateCreateInfo     multisampling; 
  // VkPipelineDepthStencilStateCreateInfo    depth_stencil_state; 
  // VkPipelineColorBlendStateCreateInfo      color_blend;
  // VkPipelineDynamicStateCreateInfo         dynamic_state;
};


struct DescriptorSetLayout { 
  VkDescriptorSetLayout           handle;    
  VkDescriptorSetLayoutCreateInfo ci;
}; 



// --------------------------------------------------------
struct Glob {

  Glob () = default;
  
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
  DescriptorSetLayout          descriptor_set_layout;
  std::vector<VkDescriptorSetLayoutBinding> desc_set_layout_bindings; 
  
  //PipelineLayout              pipeline_layout; 


  rokz::Pipeline              pipeline; 
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

  double                      sim_time; 
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
bool CreateGraphicsPipelineLayout_defaults (
    rokz::PipelineLayout&                 pipeline_layout, 
    // VkPipelineLayout&                  pipeline_layout,
    // VkPipelineLayoutCreateInfo&        create_info,
    VkViewport&                        viewport,
    VkRect2D&                          scissor,
    rokz::PipelineStateCreateInfo&     sci,
    const VkExtent2D&                  swapchain_extent,
    const VkDevice&                    device)
{
  rokz::Init (viewport, swapchain_extent.width, swapchain_extent.height, 1.0f);
  rokz::Init (scissor,  VkOffset2D {0, 0}, swapchain_extent);
  rokz::Init (sci.viewport_state, viewport, scissor);

  rokz::Init (sci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  rokz::Init (sci.rasterizer); 
  rokz::Init (sci.multisampling); 
  rokz::Init (sci.depthstencil); 

  // PIPELINE LAYOUT CREATE INFO 
  // pipeline_layout.create_info = {};
  // pipeline_layout.create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  // pipeline_layout.create_info.setLayoutCount = 1;            
  // pipeline_layout.create_info.pSetLayouts = &desc_set_layout;         
  // pipeline_layout.create_info.pushConstantRangeCount = 0;    
  // pipeline_layout.create_info.pPushConstantRanges = nullptr; 

  if (vkCreatePipelineLayout (device, &pipeline_layout.ci, nullptr, &pipeline_layout.handle) != VK_SUCCESS) {
    printf("failed to create pipeline layout!\n");
    return false;
  }

  return true;
}


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
    rokz::Pipeline&             pipeline,
    //const PipelineLayout&       pipeline_layout,
    const VkRenderPass&         render_pass,
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
  pipeline.ci.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline.ci.pNext               = nullptr;
  pipeline.ci.flags               = 0x0; 
  pipeline.ci.stageCount          = ci_shader_stages.size();
  pipeline.ci.pStages             = &ci_shader_stages[0]; 
  pipeline.ci.pVertexInputState   = ci_vertex_input_state; ;
  pipeline.ci.pInputAssemblyState = ci_input_assembly;
  pipeline.ci.pViewportState      = ci_viewport_state;
  pipeline.ci.pRasterizationState = ci_rasterizer;
  pipeline.ci.pMultisampleState   = ci_multisampling;
  pipeline.ci.pDepthStencilState  = ci_depthstencil; 
  pipeline.ci.pColorBlendState    = ci_colorblend; 
  pipeline.ci.pDynamicState       = ci_dynamic_state; 
  pipeline.ci.layout              = pipeline.layout.handle; 
  pipeline.ci.renderPass          = render_pass;
  pipeline.ci.subpass             = 0;
  pipeline.ci.basePipelineHandle  = VK_NULL_HANDLE; 
  pipeline.ci.basePipelineIndex   = -1;              

  if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline.ci, nullptr, &pipeline.handle) != VK_SUCCESS) {
    printf("failed to create graphics pipeline!");
    return false;
  }

  return true; 
  
}

// ------------------------------------------------------------------
//
// ------------------------------------------------------------------

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

  g.queue_fams.graphics.reset();
  g.queue_fams.present.reset();

  g.sim_time = 0.0; 
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

    if (!rokz::CreateUniformBuffer (uniform_buffers[i], sizeof (StandardTransform3D), 1, device, physdev)) {
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
void UpdateUniformBuffers (Glob& glob, uint32_t current_image, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.sim_time += dt;
  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);

  float sim_timef = glob.sim_time;
  
  float asp =  glob.create_info.swapchain.imageExtent.width /  glob.create_info.swapchain.imageExtent.height;
    
  StandardTransform3D mats; 
  mats.model = glm::rotate(glm::mat4(1.0f), sim_timef * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  mats.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 0.1f, 10.0f);



  //auto currentTime = std::chrono::high_resolution_clock::now();
    //float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
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


  UpdateUniformBuffers (glob, image_index, dt); 

  
  vkResetCommandBuffer (glob.command_buffer[curr_frame], 0);
  // rokz::RecordCommandBuffer(glob.command_buffer[curr_frame],
  //                           glob.pipeline,
  //                           glob.vertex_buffer_device.handle, //glob.vertex_buffer_user.handle, 
  //                           glob.create_info.swapchain.imageExtent,
  //                           glob.swapchain_framebuffers[image_index],
  //                           glob.render_pass, glob.device);

  rokz::RecordCommandBuffer_indexed (glob.command_buffer[curr_frame],
                            glob.pipeline.handle,
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
int test_rokz (const std::vector<std::string>& args) {

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
                             glob.pipeline.state_ci.shader_stages,
                             rokz_path,
                             glob.device);

  rokz::CreateColorBlendState (glob.color_blend_attachment_state, glob.pipeline.state_ci.colorblend); 

  rokz::CreateDynamicStates (glob.dynamic_states, glob.pipeline.state_ci.dynamic_state); 

  // 
  rokz::Init (glob.pipeline.state_ci.vertex_input_state, kSimpleVertexBindingDesc , kSimpleBindingAttributeDesc ); 
  
  // CreateGraphicsPipelineLayout(
  //     glob.pipeline_layout,
  //     glob.viewport,
  //     glob.scissor,
  //     glob.create_info,
  //     glob.create_info.swapchain.imageExtent,
  //     glob.device);
  CreateGraphicsPipelineLayout_defaults;
  //glob.create_info.swapchain.imageExtent.width;
  rokz::Init (glob.viewport,
              glob.create_info.swapchain.imageExtent.width,
              glob.create_info.swapchain.imageExtent.height,
              1.0f);
  rokz::Init (glob.scissor, VkOffset2D {0, 0}, glob.create_info.swapchain.imageExtent);



  rokz::PipelineStateCreateInfo& sci = glob.pipeline.state_ci;
  rokz::Init (sci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  rokz::Init (sci.viewport_state, glob.viewport, glob.scissor);
  rokz::Init (sci.rasterizer); 
  //rokz::Init (sci.colorblend);
  rokz::Init (sci.multisampling); 
  rokz::Init (sci.depthstencil); 

  //
  glob.desc_set_layout_bindings.resize (1); 
  rokz::Init (glob.desc_set_layout_bindings[0],
              0,
              VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
              VK_SHADER_STAGE_VERTEX_BIT);

  rokz::CreateDescriptorSetLayout (glob.descriptor_set_layout.handle,
                                   glob.descriptor_set_layout.ci,
                                   glob.desc_set_layout_bindings,
                                   glob.device); 


  rokz::CreateGraphicsPipelineLayout (glob.pipeline.layout.handle,
                                      glob.pipeline.layout.ci,
                                      glob.descriptor_set_layout.handle,
                                      glob.device);
  
  // CreateGraphicsPipeline (
  //     glob.pipeline,
  //     glob.create_info.pipeline,
  //     glob.create_info,
  //     glob.pipeline_layout,
  //     glob.render_pass, 
 //     glob.device);

  bool pipeline_res =  CreateGraphicsPipeline (
    glob.pipeline,
    //    glob.pipeline.layout, // const PipelineLayout&         pipeline_layout,
    glob.render_pass.handle,     // const VkRenderPass&           render_pass,
    sci.shader_stages, //const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
    &sci.input_assembly, //const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
    &sci.vertex_input_state, // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
    &sci.viewport_state, //const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
    &sci.rasterizer, //const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
    &sci.multisampling, //const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
    nullptr, // &ci.depth_stencil_state,  //const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
    &sci.colorblend, //const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
    &sci.dynamic_state,//const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 
    glob.device);    //const VkDevice                           b          device)

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
  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 60; 

  //
  auto t0 = std::chrono::high_resolution_clock::now(); 

  auto then = t0; 
  while (countdown && run && !glfwWindowShouldClose(glob.glfwin)) {

    glfwPollEvents(); 
    //start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();    
    //dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds>(then - now).count (); 
    dt = std::chrono::duration<double, std::chrono::seconds::period>(now - then).count();
    
    UpdateInput(glob, dt);
    
    UpdateScene(glob, dt);

    result = RenderFrame (glob, curr_frame, glob.syncs, fb_resize, dt);

    if (result == false)
      run = false;
    
    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }
    curr_frame = (curr_frame + 1) % kMaxFramesInFlight;

    then = now; // std::chrono::high_resolution_clock::now(); 
    countdown--; 
  }

  // end loop
  ShutdownScene();

  vkDeviceWaitIdle(glob.device);


  rokz::DestroyBuffer (glob.index_buffer_device, glob.device); 
  // CLEAN UP
  rokz::Cleanup(glob.pipeline.handle, glob.swapchain_framebuffers, glob.swapchain,
                glob.vertex_buffer_device, // glob.vertex_buffer_user, 
                glob.surface,
                glob.command_pool,
                glob.syncs, 
                glob.shader_modules, glob.pipeline.layout.handle, glob.render_pass,
                glob.swapchain_imageviews, glob.glfwin, glob.device,
                glob.instance);

  glfwTerminate();
  
  return 0;
}




int  test_time () {

  double dt = 0.0;

  const double time_per_frame_sec = 1.0 / 60.0;
  dt = time_per_frame_sec; // just do this for now
  
  std::chrono::microseconds time_per_frame_us(static_cast<size_t>(time_per_frame_sec * 1000000.0));
  
  std::chrono::duration<size_t, std::chrono::microseconds::period>
    time_per_frame(time_per_frame_us);

  Glob fake;
  Default (fake);

  
  size_t curr_frame = 0;   
  auto t0 = std::chrono::high_resolution_clock::now(); 
  auto then = t0; 
  int countdown = 720; 


  double counter_f64 = 0.0;
  
  while (countdown) {

    //start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();

    //  std::chrono::milliseconds
    //dt =
    //std::cout << "microseconds: " << time_per_frame_us << std::endl;
    //    auto auto_dt = -0.000001 *std::chrono::duration_cast<std::chrono::milliseconds> (then - now);

    auto seconds = std::chrono::duration_cast<std::chrono::seconds> (then - now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (then - now);
    auto us = std::chrono::duration_cast<std::chrono::microseconds> (then - now);
    std::cout << "ms: " << -ms.count () << std::endl ;
    std::cout << "us: " << us.count () << std::endl ;
    // auto auto_dt =  -0.000001 * us.count ();    
    // << "secs: " << seconds.count () << std::endl;
    //auto_dt
    dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds> (then - now).count ();     

    counter_f64 += dt ;
    if (counter_f64 += 60.0) {
      counter_f64 = counter_f64 - 60.0;
    }
      
    
    std::cout << "us: " << us.count () << std::endl ;

    UpdateScene (fake, dt); 

    //std::chrono::duration<std::chrono::seconds>;
    //dt =     std::chrono::duration< double>  (then - now); 
    //dt = ;

    double _ = 0.0;
    for (int i = 0; i < 373511; ++i) {
      _ += std::sqrt ( std::cos (i * 0.0123) * std::cos (i * 0.0123) + std::sin (i * 0.0123) * std::sin (i * 0.0123) );
    }
    
    // how long did we take
    // auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
    // if (time_to_make_frame < time_per_frame) {
    //   auto sleep_time = time_per_frame - time_to_make_frame;
    //   std::this_thread::sleep_for(sleep_time);
    // }
    // curr_frame = (curr_frame + 1) % kMaxFramesInFlight;

    then = now; // std::chrono::high_resolution_clock::now(); 

    countdown--; 
  }

  return 0; 
}




