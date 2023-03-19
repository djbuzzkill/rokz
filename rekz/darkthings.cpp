

#include "darkrootgarden.h"


void darkroot::Cleanup (VkPipeline&                 pipeline,


                        VkSurfaceKHR&                     surf,
                        VkCommandPool&                    command_pool,
                        std::vector<rokz::FrameSync>&     syncs, 
                        std::vector<rokz::ShaderModule>&  shader_modules,
                        VkPipelineLayout&                 pipeline_layout,
                        rokz::Display&                    display,
                        rokz::Device&                     device,
                        VkInstance&                       inst) {
   
  vkDestroyPipeline (device.handle, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool (device.handle, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, syncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  //vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  vmaDestroyAllocator(device.allocator.handle);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
  glfwDestroyWindow(display.window.glfw_window);
}




