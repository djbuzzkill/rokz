

#include "darkrootgarden.h"
#include "rokz/utility.hpp"
#include <vulkan/vulkan_core.h>





using namespace rokz;

// -----------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------
void darkroot::Cleanup (Vec<VkPipeline>&         pipelines,
                        VkSurfaceKHR&            surf,
                        VkCommandPool&           command_pool,
                        Vec<rokz::FrameSync>&    syncs, 
                        Vec<rokz::ShaderModule>& shader_modules,
                        VkPipelineLayout&        pipeline_layout,
                        rokz::Display&           display,
                        rokz::Device&            device,
                        VkInstance&              inst) {

  for (auto& p : pipelines) {
    vkDestroyPipeline (device.handle, p, nullptr);
  }
  
  vkDestroySurfaceKHR (inst, surf, nullptr);
  surf = VK_NULL_HANDLE;
  
  vkDestroyCommandPool (device.handle, command_pool, nullptr);
  command_pool = VK_NULL_HANDLE;
  
  rokz::cx::Destroy (syncs, device); 


  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  //vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  glfwDestroyWindow(display.window.glfw_window);

  vmaDestroyAllocator(device.allocator.handle);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
}






