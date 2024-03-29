
#include "rekz.h"              // 
//#include "rokz/attachment.h"
#include "grid_pipeline.h"
#include "rokz/attachment.hpp"
#include "rokz/rokz_types.hpp"


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
glm::vec3  rekz::unit_angle_xz (float theta) {

  return   glm::vec3 ( cos (theta), 0.0,  -sinf (theta) ); 

}


glm::vec3 rekz::unit_angle_yz (float theta) {

  return glm::vec3 (0.0, cos (theta), -sinf (theta)); 
}


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void rekz::CleanupSwapchain (Vec<rc::ImageView::Ref>& sc_views,
                            Vec<rc::Image::Ref>&     color_images,
                            Vec<rc::ImageView::Ref>& color_views,

                            rc::Image::Ref&     depth_image,
                            rc::ImageView::Ref& depth_view,

                            rc::Swapchain::Ref& swapchain,
                            const Device&       device) {


  for (auto& sc_view : sc_views)  sc_view.reset ();

  for (auto& colimage : color_images)  colimage.reset ();

  for (auto& view : color_views) view.reset (); 

  depth_view.reset ();
  depth_image.reset ();

  swapchain.reset ();

}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void rekz::CleanupSwapchain (Vec<rc::ImageView::Ref>& sc_image_views,

                             rc::Image::Ref&     msaa_color_image,
                             rc::ImageView::Ref& msaa_color_imageview,

                             rc::Image::Ref&     depth_image,
                             rc::ImageView::Ref& depth_imageview,

                             rc::Swapchain::Ref& swapchain,
                             const Device&       device) {

  for (auto& sc_imageview : sc_image_views) 
    sc_imageview.reset ();
  
  msaa_color_imageview.reset ();
  msaa_color_image.reset ();

  depth_imageview.reset ();
  depth_image.reset ();

  swapchain.reset ();
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool rekz::RecreateSwapchain (rc::Swapchain::Ref& swapchain, const rokz::Display& display,
                              Vec<VkImage>& swapchain_images, Vec<rc::ImageView::Ref>& imageviews,
                              rc::Image::Ref& depth_image, rc::ImageView::Ref& depth_imageview,
                              rc::Image::Ref& msaa_color_image, rc::ImageView::Ref& msaa_color_imageview,
                              const Device& device) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(display.window.glfw_window, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(display.window.glfw_window, &width, &height);
    glfwWaitEvents();
  }

  const VkExtent2D newext { 800, 600 };

  vkDeviceWaitIdle (device.handle);
  
  CleanupSwapchain (imageviews, depth_image, depth_imageview,
                    msaa_color_image, msaa_color_imageview,
                    swapchain, device);

  rokz::SwapchainInfo supp;
  rokz::cx::QuerySwapchainInfo (supp, display.surface, device.physical.handle);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info,
  VkSwapchainCreateInfoKHR ci {};
  rokz::cx::CreateInfo_default (ci,
                                display.surface,
                                swapchain->family_indices,
                                newext,
                                supp);
  
  VkImageAspectFlagBits aspect = VK_IMAGE_ASPECT_COLOR_BIT;
  bool swapchain_res    = cx::CreateSwapchain (swapchain->handle, ci, device);
  rc::CreateImageViews(imageviews, swapchain_images, ci.imageFormat, aspect, device);

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

  return (swapchain_res && imageviews.size()); 
}

// -----------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------
rokz::SwapchainResetter::Ref 
rekz::CreateSwapchainResetter (rc::Swapchain::Ref& sc, rokz::Display& displ,  
                               Vec<VkImage>& scis, Vec<rc::ImageView::Ref>& scivs,
                               rc::Image::Ref& dp, rc::ImageView::Ref& div,
                               rc::Image::Ref& mscim, rc::ImageView::Ref& mscimv)
{

  struct reset_ref_def : public SwapchainResetter {
    
  public:

    reset_ref_def (rc::Swapchain::Ref& sc, rokz::Display& displ, Vec<VkImage>& scis, Vec<rc::ImageView::Ref>& scivs, rc::Image::Ref& dp, rc::ImageView::Ref& dpiv, rc::Image::Ref& mscim, rc::ImageView::Ref&  mscimv)
      : SwapchainResetter (), display (displ), swapchain (sc), swapchain_images (scis), swapchain_imageviews (scivs)
      , depth_image (dp), depth_imageview(dpiv), msaa_color_image(mscim), msaa_color_imageview(mscimv) { 
    }

    virtual bool Reset (const rokz::Device& device) {
      return rekz::RecreateSwapchain (swapchain, display, 
                                      swapchain_images, swapchain_imageviews,
                                      depth_image,      depth_imageview,  
                                      msaa_color_image, msaa_color_imageview,
                                      device);
    }

    // --------------- own no data -------------------
    rc::Swapchain::Ref&      swapchain;
    
    Vec<VkImage>&            swapchain_images;
    Vec<rc::ImageView::Ref>& swapchain_imageviews;
    rokz::Display&           display; 
    rc::Image::Ref&          depth_image;
    rc::ImageView::Ref&      depth_imageview;  //
    rc::Image::Ref&          msaa_color_image;
    rc::ImageView::Ref&      msaa_color_imageview; 
  };

  return std::make_shared<reset_ref_def> (sc, displ, scis, scivs, dp, div, mscim, mscimv); 
} 
