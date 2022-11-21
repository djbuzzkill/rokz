
#include "rockz.h"              // 
#include "rokz/rokz.h"
#include <vulkan/vulkan_core.h>


//#include "skittlez/skittlez.h"
#include "glm/glm.hpp"
#include "rokz/rokz_funcs.h"



void look_at_this_shhhhi () {
  // auto tup = std::tuple{ 420, "wtf", 3.15f}; 
  // auto& [a1, b1, c1] = tup; 
  }



void SetupScene () {
  printf ("lolz\n"); 
}

void ShutdownScene () {
  printf ("lolz bai\n"); 
}


//
void UpdateInput (rokz::Glob& glob, double dt) {

}

  //
void UpdateScene (rokz::Glob& glob, double dt) {

  // for (int i = 0; i < 1000000; ++i ) {
      
  //   std::cos ( 3.1459265 * i * 0.000001); 
  // }
}

//
void RenderScene (rokz::Glob &glob, rokz::SyncStruc& sync, double dt) {
  
  vkWaitForFences(glob.device, 1, &sync.in_flight_fen, VK_TRUE, UINT64_MAX);
  vkResetFences(glob.device, 1, &sync.in_flight_fen);  
 
  uint32_t image_index;
  vkAcquireNextImageKHR (glob.device,
                         glob.swapchain,
                         UINT64_MAX,
                         sync.image_available_sem,
                         VK_NULL_HANDLE,
                         &image_index);

  vkResetCommandBuffer( glob.command_buffer, 0);
  rokz::RecordCommandBuffer(glob.command_buffer, glob.pipeline,
                            glob.create_info.swapchain.imageExtent,
                            glob.swapchain_framebuffers[image_index],
                            glob.render_pass, glob.device);

  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {sync.image_available_sem};

  VkSemaphore signal_semaphores[] = {sync.render_fnished_sem }; 

  VkPipelineStageFlags wait_stages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;

  submit_info.signalSemaphoreCount = 1; 
  submit_info.pSignalSemaphores = signal_semaphores; 

  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &glob.command_buffer;

  
  
  if (vkQueueSubmit(glob.queues.graphics, 1, &submit_info, sync.in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
  }

  
 VkPresentInfoKHR present_info{};
 VkSwapchainKHR swapchains[] = { glob.swapchain };

 present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
 present_info.waitSemaphoreCount = 1;
 present_info.pWaitSemaphores = signal_semaphores;
 present_info.swapchainCount  = 1;
 present_info.pSwapchains     = swapchains;
 present_info.pImageIndices   = &image_index;
 present_info.pResults        = nullptr; 


 vkQueuePresentKHR(glob.queues.present, &present_info);

 

}

//
//



int rokz_test_create (const std::vector<std::string>& args) {

  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 
  double      dt = 0.0;

  rokz::Glob glob; // something representing the app state

  auto rokz_path = std::filesystem::path ( "/home/djbuzzkill/owenslake");
  Default (glob); 
  
  glfwInit();

  rokz::CreateWindow_glfw (glob.glfwin);

  rokz::CreateInstance    (glob.instance, glob.app_info, glob.create_info.instance);

  rokz::CreateSurface (&glob.surface, glob.glfwin , glob.instance);

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

  
  printf (" present ind[%u]\n",  glob.queue_fams.present.value());
  rokz::CreateLogicalDevice (&glob.device, &glob.create_info.device, glob.physical_device); 
  printf (" graphics ind[%u]\n",  glob.queue_fams.graphics.value());

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
  // bool               CreateRenderPass (RenderPass &render_pass,
  //                                      VkFormat swapchain_format,
  //                                      const VkDevice &device); 
  
  // rokz::CreateRenderPass (glob.render_pass,
  //                         glob.create_info.renderpass,
  //                         glob.create_info.swapchain.imageFormat,
  //                         glob.device);


  rokz::CreateShaderModules (glob.shader_modules,
                             glob.create_info.shader_stages,
                             rokz_path,
                             glob.device);

  rokz::CreateColorBlendState (glob.color_blend_attachment_state, glob.create_info.color_blend); 

  
  rokz::CreateDynamicStates(glob.dynamic_states,
                            glob.create_info.dynamic_state); 
  
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


  rokz::CreateCommandBuffer(glob.command_buffer,
                            glob.create_info.command_buffer, 
                            glob.command_pool, 
                            glob.device);

  
  rokz::SyncStruc sync; 
  rokz::CreateSyncObjs(sync, glob.create_info, glob.device); 

  //
  SetupScene();
  //
  //
  const double time_per_frame_sec = 1.0 / 60.0;
  dt = time_per_frame_sec; // just do this for now
  
  std::chrono::microseconds time_per_frame_us(
                                              static_cast<size_t>(time_per_frame_sec * 1000000.0));
  
  std::chrono::duration<size_t, std::chrono::microseconds::period>
    time_per_frame(time_per_frame_us);
  
  size_t frame_counter = 0;

    // loop
  bool run = true;
  while (run && !glfwWindowShouldClose(glob.glfwin)) {

    glfwPollEvents(); 

    printf("frame[%zu]\n", frame_counter++);
    auto start = std::chrono::high_resolution_clock::now();
    
    // while (!glfwWindowShouldClose(glob.glfwin)) {
    // }
    UpdateInput(glob, dt);
    
    UpdateScene(glob, dt);
    
    RenderScene(glob, sync, dt);
    
    // how long did we take
    auto time_to_make_frame =
      std::chrono::high_resolution_clock::now() - start;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }
  }
    // end loop

  ShutdownScene();

  if (!run) {
    printf("config just exit atm\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }

  vkDeviceWaitIdle(glob.device);
  
    // CLEAN UP
  rokz::Cleanup(glob.pipeline, glob.swapchain_framebuffers, glob.swapchain,
                glob.surface, glob.command_buffer, glob.command_pool,
                sync, 
                glob.shader_modules, glob.pipeline_layout, glob.render_pass,
                glob.swapchain_imageviews, glob.glfwin, glob.device,
                glob.instance);
  // void Cleanup (VkSurfaceKHR& surf, GLFWwindow* w, VkDevice& dev,
  // VkInstance &vkinst);

    glfwTerminate();
  
  return 0;
}

int main (int argv, char** argc) {


  const std::vector<std::string> args (argc, argc + argv);

  rokz_test_create (args); 


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
  
