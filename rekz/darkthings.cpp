

#include "darkrootgarden.h"


void darkroot::Cleanup (VkPipeline&                 pipeline,
                        //std::vector<Framebuffer>&         framebuffers,
                        std::vector<rokz::ImageView>&           imageviews,

                        rokz::Swapchain&                  swapchain,
                        VkSurfaceKHR&                     surf,
                        VkCommandPool&                    command_pool,
                        std::vector<rokz::FrameSync>&     syncs, 
                        std::vector<rokz::ShaderModule>&  shader_modules,
                        VkPipelineLayout&                 pipeline_layout,
                        //rokz::RenderPass&                 render_pass,
                        rokz::Image&                      msaa_color_image,
                        rokz::ImageView&                  msaa_color_imageview,

                        rokz::Image&                      depth_image,
                        rokz::ImageView&                  depth_imageview,

                        GLFWwindow*                       w,
                        rokz::Device&                     device,
                        VmaAllocator&                     allocator, 
                        VkInstance&                       inst) {

  //    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  rekz::CleanupSwapchain (imageviews,
                          msaa_color_image, msaa_color_imageview,
                          depth_image, depth_imageview,
                          swapchain, device, allocator);
   
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

  vmaDestroyAllocator(allocator);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
  glfwDestroyWindow(w);
}




