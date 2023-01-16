


#ifndef DARKROOT_GARDEN_INCLUDE
#define DARKROOT_GARDEN_INCLUDE

#include "rokz/common.h"


#include "dark_types.h"


namespace darkroot {

  const float kPi = glm::pi<float> ();  

  //
  struct SceneObjParam {
    glm::mat4 modelmat;
    //  glm::mat4 unused0;
  };

  //
  const size_t SizeOf_SceneObjParam = sizeof (SceneObjParam); 

  const DarkrootMesh& DarkOctohedron (); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  namespace win_event { 
    void on_resize (GLFWwindow* window, int width, int height); 
    void on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods); 
    void on_mouse_enter (GLFWwindow* window, int entered);
    void on_mouse_move (GLFWwindow* window, double xpos, double ypos); 
    void on_mouse_button (GLFWwindow* window, int button, int action, int mods); 
  }

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  HalfEdge::BRep& BuildBoundaryRep (HalfEdge::BRep& brep, const DarkrootMesh& geom); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  uint32_t NumberOfComponents (VkFormat format);
  uint32_t SizeOfComponents (VkFormat format);


  void CleanupSwapchain (std::vector<rokz::ImageView>& sc_image_views,
                         rokz::Image&                msaa_color_image,
                         rokz::ImageView&            msaa_color_imageview,

                         rokz::Image&                depth_image,
                         rokz::ImageView&            depth_imageview,

                         rokz::Swapchain&            swapchain,
                         const rokz::Device&         device,
                         const VmaAllocator&         allocator); 

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
  
  bool RecreateSwapchain(rokz::Swapchain&  swapchain,
                         std::vector<rokz::Image>& swapchain_images, std::vector<rokz::ImageView>& imageviews,
                         rokz::Image& depth_image,           rokz::ImageView& depth_imageview,
                         rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,
                         const VmaAllocator& allocator, GLFWwindow* glfwin, const rokz::Device& device); 
}
#endif
