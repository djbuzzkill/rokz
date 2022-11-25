
#include "rockz.h"              // 
#include "rokz/rokz.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "rokz/defaults.h"
#include "rokz/rokz_funcs.h"


#include <glm/glm.hpp>



const size_t kMaxFramesInFlight = 2; 



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

struct Vertex_simple {

  glm::vec2 pos; 
  glm::vec3 col; 

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


Vertex_simple simple_verts[] = {
  {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
  {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
  {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};


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
void ResizeCB (GLFWwindow* window, int width, int height) {
  *reinterpret_cast<bool*> (glfwGetWindowUserPointer(window)) = true;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupScene () {
  printf ("lolz\n"); 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void ShutdownScene () {
  printf ("lolz bai\n"); 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateInput (rokz::Glob& glob, double dt) {

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateScene (rokz::Glob& glob, double dt) {

  // for (int i = 0; i < 1000000; ++i ) {
      
  //   std::cos ( 3.1459265 * i * 0.000001); 
  // }
}

// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------
bool RenderFrame (rokz::Glob &glob, uint32_t curr_frame, std::vector<rokz::SyncStruc>& syncs, bool& resize, double dt) {

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
  rokz::RecordCommandBuffer(glob.command_buffer[curr_frame],
                            glob.pipeline,
                            glob.vertex_buffer_device.handle, //glob.vertex_buffer_user.handle, 
                            glob.create_info.swapchain.imageExtent,
                            glob.swapchain_framebuffers[image_index],
                            glob.render_pass, glob.device);

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

  rokz::Glob glob; // something representing the app state

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
  
  rokz::CreateGraphicsPipelineLayout(
      glob.pipeline_layout,
      glob.viewport,
      glob.scissor,
      glob.create_info,
      glob.create_info.swapchain.imageExtent,
      glob.device);

  rokz::CreateGraphicsPipeline (
      glob.pipeline,
      glob.create_info.pipeline,
      glob.create_info,
      glob.pipeline_layout,
      glob.render_pass,
      glob.device);

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
  
  rokz::CreateVertexBuffer_device (glob.vertex_buffer_device, 
                            sizeof(Vertex_simple),
                            sizeof(simple_verts) / sizeof(Vertex_simple),
                            glob.device, 
                            glob.physical_device); 

  void* transfer_ptr = nullptr; 
  if (rokz::MapBuffer ( &transfer_ptr, vb_transfer, glob.device)) {
    memcpy (transfer_ptr, simple_verts,  sizeof(simple_verts) ); 
    rokz::UnmapBuffer (vb_transfer, glob.device); 
  }

  rokz::MoveToBuffer_XB2DB  (glob.vertex_buffer_device, // device buffer
                             vb_transfer, // user buffer, 
                             sizeof(simple_verts),
                             glob.command_pool, 
                             glob.queues.graphics,
                             glob. device); 

  vkDestroyBuffer (glob.device, vb_transfer.handle, nullptr); 
  vkFreeMemory (glob.device, vb_transfer.mem, nullptr);

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
  
  // CLEAN UP
  rokz::Cleanup(glob.pipeline, glob.swapchain_framebuffers, glob.swapchain,
                glob.vertex_buffer_device, // glob.vertex_buffer_user, 
                glob.surface,
                glob.command_pool,
                glob.syncs, 
                glob.shader_modules, glob.pipeline_layout, glob.render_pass,
                glob.swapchain_imageviews, glob.glfwin, glob.device,
                glob.instance);

  glfwTerminate();
  
  return 0;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int main (int argv, char** argc) {


  const std::vector<std::string> args (argc, argc + argv);

  rokz_test_create (args); 

  printf ( "sizeof(simple_verts) [%zu] \n", sizeof(simple_verts)); 

  // printf ("current path: %s\n" , curr_path.string ().c_str()); 
  // printf ("current path: %s\n" , rel_path.string ().c_str());

  // for (auto path_it =  curr_path.begin (); path_it != curr_path.end (); ++path_it) {
  //   if (*path_it == std::filesystem::path ("rokz")) {
  //       printf ("FOUND \"rokz\"\n");

  //       auto cur_path = std::filesystem::path ( *path_it );

  //       auto new_path = cur_path /  "data";
  //       printf ("NEW Path[%s]\n", new_path.string().c_str());
        
  //   }
  // }
  

  return 0; 
}
  
