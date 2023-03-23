
#include "rekz.h"              // 
//#include "rokz/attachment.h"
#include "grid_pipeline.h"
#include "rokz/attachment.h"


//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"
#include <glm/ext/quaternion_common.hpp>
#include <vulkan/vulkan_core.h>

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

void rekz::CleanupSwapchain (Vec<rc::ImageView::Ref>& sc_image_views,

                             rc::Image::Ref&     msaa_color_image,
                             rc::ImageView::Ref& msaa_color_imageview,

                             rc::Image::Ref&     depth_image,
                             rc::ImageView::Ref& depth_imageview,

                             VkSwapchainKHR&     swapchain,
                             const Device&       device) {

  // for (auto fb : framebuffers) {
  //   vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  // }

  for (auto sc_imageview : sc_image_views) {
    vkDestroyImageView(device.handle, sc_imageview->handle, nullptr);
  }
  vkDestroySwapchainKHR(device.handle, swapchain, nullptr);

  msaa_color_imageview.reset ();
  msaa_color_image.reset ();
  // cx::Destroy (msaa_color_imageview, device.handle);
  // cx::Destroy (msaa_color_image, device.allocator.handle);
  depth_imageview.reset ();
  depth_image.reset ();
  // cx::Destroy (depth_imageview, device.handle);
  // cx::Destroy (depth_image, device.allocator.handle);

}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool rekz::RecreateSwapchain (VkSwapchainKHR& swapchain, const VkSwapchainCreateInfoKHR& ci, const rokz::Window& win,
                              Vec<rc::Image::Ref>& swapchain_images, Vec<rc::ImageView::Ref>& imageviews,
                              rc::Image::Ref& depth_image, rc::ImageView::Ref& depth_imageview,
                              rc::Image::Ref& msaa_color_image, rc::ImageView::Ref& msaa_color_imageview,
                              const Device& device) {

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

  VkImageAspectFlagBits aspect = VK_IMAGE_ASPECT_COLOR_BIT;
  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = cx::CreateSwapchain (swapchain, ci, device);
  bool imageviews_res   = rc::CreateImageViews( imageviews, swapchain_images, ci.imageFormat, aspect, device);
  //
  assert (false); // not yet tested

  VkSampleCountFlagBits msaa_samples; 
  VkFormat depthformat; //  = depth_image.ci.format;
  VkFormat colorformat = ci.imageFormat; 
  
  // unless this changed --> msaa_color_image.ci.samples
  rc::CreateMSAAColorTarget (msaa_color_image, msaa_color_imageview, msaa_samples,
                             colorformat, ci.imageExtent, device);
  //

  rc::CreateDepthBufferTarget (depth_image, depth_imageview,  msaa_samples,
                                 depthformat, ci.imageExtent, device);

  return (swapchain_res && imageviews_res); 
}

// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
// rokz::SwapchainResetter::Ref rekz::CreateSwapchainResetter (Swapchain& sc, 
//                                                            Vec<Image>& scis, Vec<ImageView>& scivs,
//                                                            Image& dp, ImageView& div,
//                                                            Image& mscim, ImageView& mscimv) {
//   struct reset_def : public SwapchainResetter {

//     // reset_def - basic msaa attachements

//   public:

//     reset_def (Swapchain& sc, Vec<Image>& scis, Vec<ImageView>& scivs, Image& dp, ImageView& dpiv, Image& mscim, ImageView&  mscimv)
//       : SwapchainResetter (), swapchain (sc), swapchain_images (scis), swapchain_imageviews (scivs)
//       , depth_image (dp), depth_imageview(dpiv), msaa_color_image(mscim), msaa_color_imageview(mscimv) { 
//     }

//     virtual bool Reset (const rokz::Window& win, const rokz::Allocator& allocator,  const rokz::Device& device) {

//       return rekz::RecreateSwapchain (swapchain, win, 
//                                       swapchain_images, swapchain_imageviews,
//                                       depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
//                                       msaa_color_image, msaa_color_imageview,
//                                       allocator.handle, device);
//     }
    
//   protected:
    
//     Swapchain&      swapchain;
//     Vec<Image>&     swapchain_images;
//     Vec<ImageView>& swapchain_imageviews;
    
//     Image&          depth_image;
//     ImageView&      depth_imageview;  //
//     Image&          msaa_color_image;
//     ImageView&      msaa_color_imageview; 
//   };

//   return std::make_shared<reset_def> (sc, scis, scivs, dp, div, mscim, mscimv); 
// }



rokz::SwapchainResetter::Ref 
rekz::CreateSwapchainResetter (VkSwapchainKHR& sc, const VkSwapchainCreateInfoKHR& ci, 
                               Vec<rc::Image::Ref>& scis, Vec<rc::ImageView::Ref>& scivs,
                               rc::Image::Ref& dp, rc::ImageView::Ref& div,
                               rc::Image::Ref& mscim, rc::ImageView::Ref& mscimv)
{

  struct reset_ref_def : public SwapchainResetter {
    // reset_def - basic msaa attachements

  public:

    reset_ref_def (VkSwapchainKHR& sc, const VkSwapchainCreateInfoKHR& scci, Vec<rc::Image::Ref>& scis, Vec<rc::ImageView::Ref>& scivs, rc::Image::Ref& dp, rc::ImageView::Ref& dpiv, rc::Image::Ref& mscim, rc::ImageView::Ref&  mscimv)
      : SwapchainResetter (), swapchain (sc), ci (scci), swapchain_images (scis), swapchain_imageviews (scivs)
      , depth_image (dp), depth_imageview(dpiv), msaa_color_image(mscim), msaa_color_imageview(mscimv) { 
    }

    virtual bool Reset (const rokz::Window& win, const rokz::Allocator& allocator,  const rokz::Device& device) {

      return rekz::RecreateSwapchain (swapchain, ci, win, 
                                    swapchain_images, swapchain_imageviews,
                                    depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                    msaa_color_image, msaa_color_imageview,
                                    device);
    }
    
  protected:
    
    VkSwapchainKHR&                 swapchain;
    const VkSwapchainCreateInfoKHR& ci;
    
    Vec<rc::Image::Ref>&     swapchain_images;
    Vec<rc::ImageView::Ref>& swapchain_imageviews;
    
    rc::Image::Ref&          depth_image;
    rc::ImageView::Ref&      depth_imageview;  //
    rc::Image::Ref&          msaa_color_image;
    rc::ImageView::Ref&      msaa_color_imageview; 
  };

  return std::make_shared<reset_ref_def> (sc, ci, scis, scivs, dp, div, mscim, mscimv); 
} 
