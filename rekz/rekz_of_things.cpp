
#include "rekz.h"              // 
//#include "rokz/attachment.h"
#include "grid_pipeline.h"
#include "rokz/attachment.h"


//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"
#include <glm/ext/quaternion_common.hpp>

using namespace rokz;


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
glm::vec3& rekz::unit_angle_xz (glm::vec3& v, float theta) {
  v.x = cos (theta) ;
  v.y = 0.0f;
  v.z = -sinf (theta) ;
  return v; 
}

glm::vec3& rekz::unit_angle_yz (glm::vec3& v, float theta) {
  v.x = 0.0;
  v.y = cos (theta) ;
  v.z = -sinf (theta) ;
  return v; 
}



// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------

void rekz::CleanupSwapchain (Vec<ImageView>&     sc_image_views,


                             Image&              msaa_color_image,
                             ImageView&          msaa_color_imageview,

                             Image&              depth_image,
                             ImageView&          depth_imageview,

                             Swapchain&          swapchain,
                             const Device&       device) {

  // for (auto fb : framebuffers) {
  //   vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  // }

  for (auto sc_imageview : sc_image_views) {
    vkDestroyImageView(device.handle, sc_imageview.handle, nullptr);
  }
  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);

  cx::Destroy (msaa_color_imageview, device.handle);
  cx::Destroy (msaa_color_image, device.allocator.handle);

  cx::Destroy (depth_imageview, device.handle);
  cx::Destroy (depth_image, device.allocator.handle);

}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool rekz::RecreateSwapchain(Swapchain& swapchain, const rokz::Window& win,
                             Vec<Image>& swapchain_images, Vec<ImageView>& imageviews,
                             Image& depth_image, ImageView& depth_imageview,
                             Image& msaa_color_image, ImageView& msaa_color_imageview,
                             const VmaAllocator& allocator, const Device& device) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(win.glfw_window, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(win.glfw_window, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (device.handle);
  
  CleanupSwapchain (imageviews, depth_image, depth_imageview,
                    msaa_color_image, msaa_color_imageview,
                    swapchain, device);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = cx::CreateSwapchain (swapchain, device);
  bool imageviews_res   = cx::CreateImageViews(imageviews, swapchain_images, device);
  
  //
  assert (false); // not yet tested
  // unless this changed --> msaa_color_image.ci.samples
  rokz::CreateMSAAColorTarget (msaa_color_image, msaa_color_imageview, msaa_color_image.ci.samples,
                               swapchain.ci.imageFormat, swapchain.ci.imageExtent, device);
  //
  assert (false); // not yet tested
  rokz::CreateDepthBufferTarget (depth_image, depth_imageview,  msaa_color_image.ci.samples,
                                 depth_image.ci.format, swapchain.ci.imageExtent, device);

  return (swapchain_res && imageviews_res); 
}

// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
rokz::SwapchainResetter::Ref rekz::CreateSwapchainResetter (Swapchain& sc, 
                                                           Vec<Image>& scis, Vec<ImageView>& scivs,
                                                           Image& dp, ImageView& div,
                                                           Image& mscim, ImageView& mscimv) {
  struct reset_def : public SwapchainResetter {

    // reset_def - basic msaa attachements

  public:

    reset_def (Swapchain& sc, Vec<Image>& scis, Vec<ImageView>& scivs, Image& dp, ImageView& dpiv, Image& mscim, ImageView&  mscimv)
      : SwapchainResetter (), swapchain (sc), swapchain_images (scis), swapchain_imageviews (scivs)
      , depth_image (dp), depth_imageview(dpiv), msaa_color_image(mscim), msaa_color_imageview(mscimv) { 
    }

    virtual bool Reset (const rokz::Window& win, const rokz::Allocator& allocator,  const rokz::Device& device) {

      return rekz::RecreateSwapchain (swapchain, win, 
                                      swapchain_images, swapchain_imageviews,
                                      depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                      msaa_color_image, msaa_color_imageview,
                                      allocator.handle, device);
    }
    
  protected:
    
    Swapchain&      swapchain;
    Vec<Image>&     swapchain_images;
    Vec<ImageView>& swapchain_imageviews;
    
    Image&          depth_image;
    ImageView&      depth_imageview;  //
    Image&          msaa_color_image;
    ImageView&      msaa_color_imageview; 
  };

  return std::make_shared<reset_def> (sc, scis, scivs, dp, div, mscim, mscimv); 
}

