
#ifndef DARKROOT_GARDEN_INCLUDE
#define DARKROOT_GARDEN_INCLUDE

#include "dark_types.h"


namespace darkroot {

  const float kPi = glm::pi<float> ();  
  const float k2Pi = 2 * glm::pi<float> ();  

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  uint32_t NumberOfComponents (VkFormat format);
  uint32_t SizeOfComponents   (VkFormat format);

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  void CleanupSwapchain (std::vector<rokz::ImageView>& sc_image_views,
                         rokz::Image&                msaa_color_image,
                         rokz::ImageView&            msaa_color_imageview,

                         rokz::Image&                depth_image,
                         rokz::ImageView&            depth_imageview,

                         rokz::Swapchain&            swapchain,
                         const rokz::Device&         device); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  void Cleanup (Vec<VkPipeline>&                  pipelines,
                VkSurfaceKHR&                     surface,
                VkCommandPool&                    command_pool,
                std::vector<rokz::FrameSync>&     syncs, 
                std::vector<rokz::ShaderModule>&  shader_modules,
                VkPipelineLayout&                 pipeline_layout,

                rokz::Display&                    display,
                rokz::Device&                     device,
                VkInstance&                       inst);
  

  
}
#endif
