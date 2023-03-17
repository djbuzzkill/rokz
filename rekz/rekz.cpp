
#include "rekz.h"              // 
#include "rokz/attachment.h"


#include "rekz/grid_pipeline.h"


//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"
#include <glm/ext/quaternion_common.hpp>

using namespace rokz;


const Vec<VkDescriptorSetLayoutBinding> rekz::kObjDescriptorBindings = {
  { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::kMaxObjectCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 , VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
bool rekz::SetupGridData (GridData& gd, const Device& device) {

  void*  mem = nullptr;
  size_t szmem = 0;

  const uint16_t vertdim    = 11;
  const uint16_t totalverts = vertdim * vertdim;
  const float    dimsize    = 20.0f;
  const float    dimstep    = float (dimsize) / float (vertdim - 1);

  glm::vec3 zaxis_co  (1.0, 0.4, 0.8);
  glm::vec3 xaxis_co  (0.3, 0.5, 1.0);
  glm::vec3 origin_co (0.0, 0.4, 0.6);

  float dx = 1.0f / float (vertdim);
  
  const glm::vec3 voffs (-dimsize * 0.5f, 0.0f, -dimsize * 0.5f);

  Vec<GridVert> verts (vertdim * vertdim);
  Vec<uint16_t> inds (2 * totalverts);

  // -- vertices --
  for (uint16_t iz = 0; iz < vertdim; ++iz) {
    for (uint16_t ix = 0; ix < vertdim; ++ix) {
      verts[iz * vertdim + ix].pos = glm::vec3 (ix * dimstep, 0.0f, iz * dimstep) + voffs;
      verts[iz * vertdim + ix].col = glm::mix (origin_co, zaxis_co, ix * dx); 

    }
  } // move 2 vb
  Create_VB_device (gd.vb_device, &verts[0], verts.size () * sizeof(rekz::GridVert), device); 

  // -- indices --
  for (uint16_t iz = 0; iz < vertdim; ++iz) { // draw x lines
    for (uint16_t ix = 0; ix < vertdim; ++ix) { 
      inds[iz * vertdim + ix] = iz * vertdim + ix;   
    }}

  for (uint32_t ix = 0; ix < vertdim; ++ix) {  // draw z lines
    for (uint32_t iz = 0; iz < vertdim; ++iz) { 
      inds[totalverts + ix * vertdim + iz] = iz * vertdim + ix; 
    }
  } // move
  Create_IB_16_device (gd.ib_device, &inds[0], inds.size (), device); 
  
  return true; 
}


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
void rekz::CleanupGridData (GridData& gd, const Device& device) {

  rokz::Destroy (gd.vb_device, device.allocator.handle); 
  
}


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




// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//bool SetupRenderingAttachments (Glob& glob, rokz::SwapchainGroup& scg, const rokz::Device& device) { 
bool rekz::SetupRenderingAttachments (Image&          msaa_color_image       ,
                                      ImageView&      msaa_color_imageview   ,

                                      Image&          msaa_depth_image       ,
                                      ImageView&      msaa_depth_imageview   ,

                                      VkSampleCountFlagBits msaa_samples           ,
                                      VkFormat              swapchain_image_format ,
                                      VkFormat              msaa_depth_format      ,
                                      const VkExtent2D&     image_ext, 
                                      const Device& device) { 

  //CreateMSAAColorImage -> (image, imageview)
  rokz::CreateMSAAColorTarget (msaa_color_image, msaa_color_imageview, msaa_samples,
                               swapchain_image_format, image_ext, device);

  // CreateDepthBufferImage -> (image, imageview)
  rokz::CreateDepthBufferTarget (msaa_depth_image, msaa_depth_imageview, msaa_samples,
                                 msaa_depth_format, image_ext, device);

  return true;

}



// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void rekz::CleanupSwapchain (Vec<ImageView>&         sc_image_views,
                                 Image&              msaa_color_image,
                                 ImageView&          msaa_color_imageview,

                                 Image&              depth_image,
                                 ImageView&          depth_imageview,

                                 Swapchain&          swapchain,
                                 const Device&       device,
                                 const VmaAllocator& allocator) {

  // for (auto fb : framebuffers) {
  //   vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  // }

  for (auto sc_imageview : sc_image_views) {
    vkDestroyImageView(device.handle, sc_imageview.handle, nullptr);
  }

  cx::Destroy (msaa_color_image, allocator);
  cx::Destroy (msaa_color_imageview, device.handle);

  cx::Destroy (depth_image, allocator);
  cx::Destroy (depth_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool rekz::RecreateSwapchain(Swapchain&  swapchain, const rokz::Window& win, 
                             Vec<Image>& swapchain_images, Vec<ImageView>& imageviews,
                             Image& depth_image, ImageView& depth_imageview,
                             Image& multisamp_color_image, ImageView& multisamp_color_imageview,
                             const VmaAllocator& allocator, const Device& device) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(win.glfw_window, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(win.glfw_window, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (device.handle);

  CleanupSwapchain (imageviews,
                    depth_image, depth_imageview,
                    multisamp_color_image,
                    multisamp_color_imageview,
                    swapchain, device, allocator);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = cx::CreateSwapchain (swapchain, device);
  bool imageviews_res   = cx::CreateImageViews (imageviews, swapchain_images, device);

  assert (false);
  // rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,

  assert (false);
  //rokz::Image& depth_image, rokz::ImageView& depth_imageview,

  return (swapchain_res && imageviews_res); 
}

// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
rokz::ResetSwapchainCB::Ref rekz::CreateSwapchainResetter (Swapchain& sc, 
                                                           Vec<Image>& scis, Vec<ImageView>& scivs,
                                                           Image& dp, ImageView& div,
                                                           Image& mscim, ImageView& mscimv) {

  struct DefaultResetSwapchain : public ResetSwapchainCB {
  public:
  
    DefaultResetSwapchain (Swapchain& sc, 
                           Vec<Image>& scis, Vec<ImageView>& scivs,
                           Image& dp, ImageView& dpiv,  
                           Image& mscim, ImageView&  mscimv)
      : ResetSwapchainCB ()
      , swapchain (sc)
      , swapchain_images (scis)
      , swapchain_imageviews (scivs)
      , depth_image (dp)
      , depth_imageview(dpiv)
      , msaa_color_image(mscim)
      , msaa_color_imageview(mscimv) { 
    }
    
    //
    virtual bool ResetSwapchain (const rokz::Window& win, const rokz::Allocator& allocator,  const rokz::Device& device) {
      return rekz::RecreateSwapchain (swapchain, win, 
                                      swapchain_images, swapchain_imageviews,
                                      depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                      msaa_color_image, msaa_color_imageview,
                                      allocator.handle, device);
    }
    
  protected:
    
    Swapchain&              swapchain;
    Vec<Image>&     swapchain_images;
    Vec<ImageView>& swapchain_imageviews;
    
    Image&                  depth_image;
    ImageView&              depth_imageview;  //
    Image&                  msaa_color_image;
    ImageView&              msaa_color_imageview; 
  };


  return std::make_shared<DefaultResetSwapchain> (sc, scis, scivs, dp, div, mscim, mscimv); 
}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool rekz::SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                const rokz::ImageView&    msaa_color_imageview ,
                                const rokz::ImageView&    msaa_depth_imageview ,
                                const VkExtent2D&         image_extent) {

  // const rokz::ImageView& msaa_color_imageview = glob.msaa_color_imageview;
  // const rokz::ImageView& msaa_depth_imageview = glob.depth_imageview;
  // const VkExtent2D& image_extent              = glob.swapchain_group.swapchain.ci.imageExtent;
  ri.clear_colors.resize (1);
  ri.color_attachment_infos.resize (1);

  ri.clear_colors[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  ri.clear_depth.depthStencil = {1.0f, 0};

  cx::AttachmentInfo (ri.color_attachment_infos[0],
                            msaa_color_imageview.handle,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_AVERAGE_BIT,
                            VK_NULL_HANDLE, // swapchain_group.imageviews[i].handle <-- this will change
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_STORE,
                            ri.clear_colors[0]);

  cx::AttachmentInfo (ri.depth_attachment_info,
                            msaa_depth_imageview.handle,
                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_NONE,
                            nullptr,
                            VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                            ri.clear_depth);

  
  ri.render_area = { VkOffset2D {0, 0}, image_extent };

  cx::RenderingInfo (ri.ri, ri.render_area, 1, 0, ri.color_attachment_infos, &ri.depth_attachment_info, nullptr);
  return true;
}

// -----------------------------------------------------------------------------
// basically populates the AttachmentInfo
// -----------------------------------------------------------------------------
void rekz::UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                       const rokz::ImageView&    msaa_color_imageview ,
                                       const rokz::ImageView&    target_imageview) {
  //printf ("%s\n", __FUNCTION__); 
  cx::AttachmentInfo (ri.color_attachment_infos[0],
                        msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT, target_imageview.handle,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
                        VK_ATTACHMENT_STORE_OP_STORE, ri.clear_colors[0]);

}



// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
int mix_x (const Vec<std::string>& args) {

  ("mesh.h", "global_descriptor.h", "input_objects.h", "render_attachment.h");

  
  glm::vec3 cola (1.0f, 0.0f, 0.0f); 
  glm::vec3 colb (0.0f, 1.0f, 0.0f);

  glm::vec3 colz = glm::mix(cola, colb, 0.2f); // -> < 0.8, 0.2, 0.0>
  
  printf ("< r:%f, g:%f, b:%f >\n", colz.x, colz.y, colz.z);
  // 
  return 0; 
}

// rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info,
  //                              glob.surface,
  //                              glob.physical_device.handle);

  //rokz::SwapchainGroup& scg = glob.swapchain_group;
// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
int test_rokz (const Vec<std::string>& args); 
int test_rokz_hpp (const Vec<std::string>& args); 
int texture_tool (const Vec<std::string>& args); 
int test_ouput (const Vec<std::string>& args); 
int test_time (); 
int darkrootbasin (const Vec<std::string>& args);
int run_marz (const Vec<std::string>& args);
int mars_prelim (const Vec<std::string>& args);
int examine_TIFs (const Vec<std::string>& args); 
bool test_grid_geom_gen ();


// -------------------------------------------------------------------------------------------
int main (int argv, char** argc) {

  const Vec<std::string> args (argc, argc + argv);

  //mix_x (args);
  //darkrootbasin  (args);
  //marz_run  (args);
  examine_TIFs (args); 
  
  // test_rokz_hpp (args); 
  // texture_tool (args); 
  //test_ouput (args);
  //test_grid_geom_gen ();
  //test_time (); 
  //test_rokz (args); 
  
  printf ("lolz bai %s\n", __FUNCTION__); 
  return 0; 
}
