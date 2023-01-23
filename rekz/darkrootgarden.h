
#ifndef DARKROOT_GARDEN_INCLUDE
#define DARKROOT_GARDEN_INCLUDE

#include "dark_types.h"


namespace darkroot {

  const float kPi = glm::pi<float> ();  
  const float k2Pi = 2 * glm::pi<float> ();  

  const size_t        kSceneObjCount = 128;
  //
  //
  const size_t SizeOf_SceneObjParam = sizeof (SceneObjParam); 

  const DarkMesh& DarkOctohedron (); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  uint32_t NumberOfComponents (VkFormat format);
  uint32_t SizeOfComponents (VkFormat format);


  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  void CleanupSwapchain (std::vector<rokz::ImageView>& sc_image_views,
                         rokz::Image&                msaa_color_image,
                         rokz::ImageView&            msaa_color_imageview,

                         rokz::Image&                depth_image,
                         rokz::ImageView&            depth_imageview,

                         rokz::Swapchain&            swapchain,
                         const rokz::Device&         device,
                         const VmaAllocator&         allocator); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  void Cleanup (VkPipeline&                 pipeline,
                //std::vector<Framebuffer>&         framebuffers,
                std::vector<rokz::ImageView>&           imageviews,

                rokz::Swapchain&                  swapchain,
                VkSurfaceKHR&                     surf,
                VkCommandPool&                    command_pool,
                std::vector<rokz::RenderSync>&     syncs, 
                std::vector<rokz::ShaderModule>&  shader_modules,
                VkPipelineLayout&                 pipeline_layout,
                rokz::Image&                      msaa_color_image,
                rokz::ImageView&                  msaa_color_imageview,

                rokz::Image&                      depth_image,
                rokz::ImageView&                  depth_imageview,

                GLFWwindow*                       w,
                rokz::Device&                     device,
                VmaAllocator&                     allocator, 
                VkInstance&                       inst);
  
  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  bool RecreateSwapchain(rokz::Swapchain&  swapchain,
                         std::vector<rokz::Image>& swapchain_images, std::vector<rokz::ImageView>& imageviews,
                         rokz::Image& depth_image,           rokz::ImageView& depth_imageview,
                         rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,
                         const VmaAllocator& allocator, GLFWwindow* glfwin, const rokz::Device& device); 


  //void SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent); 

}
#endif
