
#include "milkshake.h"
#include "dorito_pipe.h"

#include "rokz/attachment.hpp"
#include "rokz/context.hpp"
#include "rokz/framebuffer.hpp"
#include "rokz/rc_image.hpp"
#include <vulkan/vulkan_core.h>

#include "grid_pipeline.h"
#include "onscreen_pipe.h"
#include "rokz/renderpass.hpp"
#include "rokz/rokz_types.hpp"



using namespace rokz; 
using namespace rekz; 


const VkExtent2D kDefaultDimensions { 1024, 768 }; 
// --------------------------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------------------------
bool setup_gbuf_framebuffers (milkshake::Glob& glob) {

  // glob.attachment.
  // do we need Nswapchainimages of framebuffers for geombuffer?

  // rc::RenderPass::Ref renderpass ;

  // for (auto v : glob.swapchain_group.views) { 

    
  //   Vec<rc::ImageView::Ref> views = {
  //   } 
    
  //     rc::CreateFramebuffer (views, renderpass, kDefaultDimensions, glob.device);
  // }
  
  // position color target
  //   normall color target
  //   albedo color target
  //   depth gbuff target 
  

  return false; 
}
// -- 
bool setup_lcomp_framebuffers (milkshake::Glob& glob) {


  // surface color target
  // depth lcmop target

  // bool rokz::CreateFramebuffers (std::vector<Framebuffer>&       framebuffers, 
  //                                const std::vector<ImageView>&   imageviews,
  //                                const RenderPass&               render_pass, 
  //                                const VkExtent2D&               swapchain_ext, 
  //                                const VkImageView&              msaa_color_imageview, 
  //                                const VkImageView&              depth_imageview, 
  //                                const Device&                   device) {
  //   printf ("%s\n", __FUNCTION__);

  //   framebuffers.resize (imageviews.size()); 

  //   for (size_t i = 0; i < imageviews.size(); i++) {

  //     framebuffers[i].attachments.clear ();

  //     // does this match renderpass 
  //     framebuffers[i].attachments.push_back (msaa_color_imageview);
  //     framebuffers[i].attachments.push_back (depth_imageview );
  //     framebuffers[i].attachments.push_back (imageviews[i].handle);

  //     CreateInfo (framebuffers[i].ci, swapchain_ext, render_pass, framebuffers[i].attachments); 
    
  //     if (vkCreateFramebuffer(device.handle, &framebuffers[i].ci, nullptr, &framebuffers[i].handle) != VK_SUCCESS) {
  //       printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
  //       return false;
  //     }

  //   }

  //   return true; 
  // }


  return false; 
}

// -- 
bool setup_gbuf_renderpass  (milkshake::Glob& glob) {
  // bool rokz::CreateRenderPass (RenderPass&             render_pass,
  //                              VkFormat                swapchain_format,
  //                              VkSampleCountFlagBits   msaa_samples, 
  //                              const VkDevice&         device,
  //                              const VkPhysicalDevice& physdev) {

  //   // COLOR ATTACHMENT | VkAttachmentDescription 
  //   auto CO_n = ATTACH_COLOR; 
  //   render_pass.attach_desc[CO_n] = {}; 
  //   render_pass.attach_desc[CO_n].format         = swapchain_format ;
  //   render_pass.attach_desc[CO_n].samples        = msaa_samples;  // VK_SAMPLE_COUNT_1_BIT; // msaa samples
  //   render_pass.attach_desc[CO_n].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  //   render_pass.attach_desc[CO_n].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  //   render_pass.attach_desc[CO_n].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  //   render_pass.attach_desc[CO_n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  //   render_pass.attach_desc[CO_n].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  //   render_pass.attach_desc[CO_n].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // when msaa is used otherwise -> VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 

  //   // DEPTHSTENCIL ATTACHMENT | VkAttachmentDescription 
  //   auto DP_n = ATTACH_DEPTHSTENCIL;
  //   render_pass.attach_desc[DP_n] = {}; 
  //   // render_pass.attach_desc[dp_in].format         = depth_format;
  //   ut::FindDepthFormat (render_pass.attach_desc[DP_n].format, physdev);
  //   render_pass.attach_desc[DP_n].samples        = msaa_samples; // VK_SAMPLE_COUNT_1_BIT;
  //   render_pass.attach_desc[DP_n].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  //   render_pass.attach_desc[DP_n].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  //   render_pass.attach_desc[DP_n].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  //   render_pass.attach_desc[DP_n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  //   render_pass.attach_desc[DP_n].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  //   render_pass.attach_desc[DP_n].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  //   // COLOR RESOLVE ATTACHMENT | VkAttachmentDescription 
  //   auto CR_n = ATTACH_COLRESOLV; 
  //   render_pass.attach_desc[CR_n] = {}; 
  //   render_pass.attach_desc[CR_n].format         = swapchain_format ;
  //   render_pass.attach_desc[CR_n].samples        = VK_SAMPLE_COUNT_1_BIT; // msaa samples
  //   render_pass.attach_desc[CR_n].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  //   render_pass.attach_desc[CR_n].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  //   render_pass.attach_desc[CR_n].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  //   render_pass.attach_desc[CR_n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  //   render_pass.attach_desc[CR_n].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  //   render_pass.attach_desc[CR_n].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  //   //VkAttachmentReference color_attachment_ref{};
  //   render_pass.attach_ref[CO_n] = {};
  //   render_pass.attach_ref[CO_n].attachment = CO_n; // index
  //   render_pass.attach_ref[CO_n].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  //   // depth
  //   render_pass.attach_ref[DP_n] = {};
  //   render_pass.attach_ref[DP_n].attachment = DP_n;
  //   render_pass.attach_ref[DP_n].layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  //   // 
  //   render_pass.attach_ref[CR_n] = {};
  //   render_pass.attach_ref[CR_n].attachment = CR_n; // index
  //   render_pass.attach_ref[CR_n].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // <--- color opt is correct
  //                                        // = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  
  //   // SUBPASS,  VkSubpassDescription                 
  //   render_pass.subpass_descs.resize (1);
  //   render_pass.subpass_descs[0] = {}; 
  //   render_pass.subpass_descs[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  //   render_pass.subpass_descs[0].colorAttachmentCount    = 1;
  //   render_pass.subpass_descs[0].pColorAttachments       = &render_pass.attach_ref[CO_n];  // co_in, [cr_in] for msaa
  //   render_pass.subpass_descs[0].inputAttachmentCount    = 0;
  //   render_pass.subpass_descs[0].pInputAttachments       = nullptr;
  //   render_pass.subpass_descs[0].pDepthStencilAttachment = &render_pass.attach_ref[DP_n]; //nullptr;
  //   render_pass.subpass_descs[0].preserveAttachmentCount = 0;
  //   render_pass.subpass_descs[0].pPreserveAttachments    = nullptr;
  //   render_pass.subpass_descs[0].pResolveAttachments     = &render_pass.attach_ref[CR_n];
  //   render_pass.subpass_descs[0].flags = 0 ;
  //   //
  //   render_pass.dependencies.resize (1);
  //   render_pass.dependencies[0].srcSubpass    = VK_SUBPASS_EXTERNAL;
  //   render_pass.dependencies[0].dstSubpass    = 0;

  //   render_pass.dependencies[0].srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  //   render_pass.dependencies[0].srcAccessMask = 0;

  //   render_pass.dependencies[0].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  //   render_pass.dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  
  //   // CREATEINFO. gets passed back out
  //   render_pass.ci  = {}; 
  //   render_pass.ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

  //   render_pass.ci.attachmentCount = 3; // color + depthstencil + color resolv
  //   render_pass.ci.pAttachments = &render_pass.attach_desc[0];

  //   render_pass.ci.subpassCount = 1;
  //   render_pass.ci.pSubpasses   = &render_pass.subpass_descs[0];

  //   render_pass.ci.dependencyCount = render_pass.dependencies.size();
  //   render_pass.ci.pDependencies = &render_pass.dependencies[0]; 
  //   render_pass.ci.pNext = nullptr; 
  //   //
  //   if (vkCreateRenderPass(device, &render_pass.ci, nullptr, &render_pass.handle) != VK_SUCCESS) {
  //     printf ("[FAILED] %s create render pass\n", __FUNCTION__);
  //     return false; 
  //   }

  //   return true;
  // }

  
  HERE("HAI");
  return false;
}

// -- 
bool setup_lcomp_renderpass (milkshake::Glob& glob) {


  HERE("HAI");
  return false; 
}


// -- 
void cleanup_milkshake (milkshake::Glob& glob) {

  printf ("%s \n", __FUNCTION__); 


  // glob.gridbuff.reset (); //rekz::CleanupGridData    (glob.gridata, glob.device); 
  // glob.drawgrid.reset (); 

  // descriptor set layouts
  rokz::cx::Destroy (glob.grid_dslo, glob.device); 

  //  glob.global_rc_uniform_bu.clear ();

  // dont bother freeing if pool is destroyed anyways
  rokz::cx::Destroy (glob.grid_de.pool, glob.device); 


#ifdef DARKROOT_HIDE_OSD_PATH
  rokz::cx::Destroy (glob.osd_de.pool, glob.device); 
#endif
  // ?  
  // polygons will make use of object descriptors

  // for (auto& buf : glob.poly_objects_bu) { 
  //   buf.reset ();
  //   //rokz::cx::Destroy  (buf, glob.device.allocator); 
  // }

  //
  // rekz::CleanupSwapchain (glob.swapchain_group.imageviews,
  //                         glob.msaacolorimage, glob.msaacolorimageview,
  //                         glob.depthimage, glob.depthimageview,
  //                         glob.swapchain_group.swapchain, 
  //                         glob.device);

  // the the  poly stuff is done in Cleanup, grids is done here
  // for (auto shmod : glob.grid_pl.shader_modules) {
  //   vkDestroyShaderModule (glob.device.handle, shmod.handle, nullptr); 
  // }

  // vkDestroyPipelineLayout (glob.device.handle, glob.grid_plo.handle, nullptr);
  // Vec<VkPipeline> pipes = { 
  //   glob.polys_pl.handle, glob.grid_pl.handle }; 

  
  // darkroot::Cleanup (pipes,
  //                    glob.display.surface,
  //                    glob.device.command_pool.handle,
  //                    glob.framesyncgroup.syncs, 
  //                    glob.polys_pl.shader_modules,
  //                    glob.polys_plo.handle,  
  //                    glob.display,
  //                    glob.device,
  //                    glob.instance.handle);

  //
  glfwTerminate();
  
}

// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
struct MilkLoop {

  milkshake::Glob& glob;

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 6000;

  const float Dt; 
  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);

  

  MilkLoop (milkshake::Glob& g, double dt) : glob (g), Dt (dt) {
  }
  
  bool cond () { return false; } 
  bool loop () {



    auto now = std::chrono::high_resolution_clock::now();    
    
    glfwPollEvents(); 

    //UpdateRunState ();

    // { 
    //   char msg[64];
    //   sprintf (msg, "OSD test: %i", countdown);
    //   glob.osdata.strings[0] = msg;
    // }
      
    // UpdateViewAttitude (glob.shared.view_rot, glob.mouse_prev, glob.prev_inside, glob.input_state, 0.01f);
    // UpdateViewPosition (glob.shared.view_pos, glob.shared.view_rot, glob.input_state, 0.1);

    //
    rc::SwapchainGroup& scg = glob.swapchain_group; 
    // get image index up here
    uint32_t image_index; 
    VkResult acquireres = rokz::cx::AcquireFrame  (scg.swapchain->handle, image_index, 
                                                   glob.sync[curr_frame].inflight, glob.sync[curr_frame].sem.image_available,
                                                   glob.device); 

    // -------------------- ---------------------
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_resetter->Reset (glob.device);
      HERE ("#$A%"); 
      return true;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {
      // ------------------------ Updaate b4 draw ----------------------------
      //
      //   update uniform here, but uniforms can also b update during DrawSeq::Prep ()
      // 
      //UpdateDarkUniforms (glob, curr_frame, Dt); 

      //rokz::UpdateGlobals (glob.shared, glob.global_rc_uniform_bu [curr_frame], kDefaultDimensions, Dt);

      // did not work as planned
      //onscreen::UpdateOSD (glob.global_rc_uniform_bu[curr_frame], glob.osdata.strings, kDefaultDimensions, Dt);  
      
      // make sure the correct swapchain image is used

      assert (false); 
      // rokz::UpdateDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolorimageview->handle,
      //                                   glob.swapchain_group.imageviews[image_index]->handle);

      // ------------------------------- render pass start -------------------------------
      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here

      assert (false);
      // cx::FramePassBegin;
      
      // rokz::DrawSequence::RenderEnv poly_re {
      //   glob.polys_pl, glob.polys_plo.handle, glob.shared, 
      // };
      // rokz::DrawSequence::RenderEnv grid_re {
      //   glob.grid_pl, glob.grid_plo.handle, glob.shared, 
      // };
      // rokz::DrawSequence::RenderEnv osd_re  {
      //   glob.osd_pl, glob.osd_plo.handle, glob.shared, 
      // };

      cx::FrameDrawBegin (glob.swapchain_group, glob.sync[curr_frame].commandbuf,
                          image_index, glob.rendering_info_group.ri, glob.device);

      // EXECUTE DRAW LIST RECORDING 
      // for drawseq's
      // const std::vector<VkDescriptorSet> descrsets = {
      //   glob.global_uniform_de.descrsets[curr_frame], glob.objres_uniform_de.descrsets[curr_frame]
      // };

      assert (false); 

      // DRAW GEOM BUFF 


      assert (false);

      // TRANSITION IMAGES 

      // BIND DESCRIPTORS

      // cx::FrameNextPass; 

      // DRAW LIGHT PASS

      // glob.drawpoly->Prep (curr_frame, poly_re, glob.device); 
      // glob.drawpoly->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, poly_re);
      // // grid
      // glob.drawgrid->Prep (curr_frame, grid_re, glob.device); 
      // glob.drawgrid->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, grid_re);
      // //
      // // no more scene beyond here this is overlay now
      // glob.osdraw->Prep (curr_frame, osd_re , glob.device); 
      // glob.osdraw->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, osd_re); 

      
      cx::FrameDrawEnd (glob.swapchain_group,
                        glob.sync[curr_frame].commandbuf, 
                        image_index,
                        glob.sync[curr_frame].sem.image_available,
                        glob.sync[curr_frame].sem.lightpass, // render_finished ??
                        glob.sync[curr_frame].inflight,  
                        //glob.framesyncgroup.syncs[curr_frame],
                        glob.device);
    }
    
    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }

    curr_frame = (curr_frame + 1) % milkshake::kMaxFramesInFlight; 
    then = now; // std::chrono::high_resolution_clock::now(); 
    countdown--; 

    return false;
  } 
 
};


// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
bool setup_color_render_attachments (milkshake::Glob& glob) {

  using namespace milkshake;

  const Device& device = glob.device; 
  const VkSampleCountFlagBits sample_bit_count = VK_SAMPLE_COUNT_1_BIT; 
  const VkImageUsageFlags color_target_usage   = rokz::kColorTargetUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 
  const VkImageUsageFlags depth_target_usage   = rokz::kDepthStencilUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 

  glob.msaa_samples = sample_bit_count; 
  
  Vec<rc::Attachment>& colorattach = glob.attachment.color; 
  rc::Attachment&      depthattach = glob.attachment.depth;

  { // position
    VkImageCreateInfo ci {}; 

    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

    colorattach[COLATT_POSITION].format = VK_FORMAT_R16G16B16A16_SFLOAT; 
    colorattach[COLATT_POSITION].image  = rc::CreateImage (ci, device);
    colorattach[COLATT_POSITION].view   ; 
    // create imageview...
    assert (false); 
  }

  { // normal
    VkImageCreateInfo ci {}; 

    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);
    colorattach[COLATT_NORMAL].format  =  VK_FORMAT_R16G16B16A16_SFLOAT;
    colorattach[COLATT_NORMAL].image =  rc::CreateImage (ci, device);
    colorattach[COLATT_NORMAL].view; 
    // create imageview...
    assert (false); 
  }


  { // albedo
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

    colorattach[COLATT_ALBEDO].format = VK_FORMAT_R16G16B16A16_SFLOAT;
    colorattach[COLATT_ALBEDO].image  = rc::CreateImage (ci, device);
    colorattach[COLATT_ALBEDO].view ; 

    // create imageview...
    assert (false); 
  }

  // { // specular
            
  // }
  
  // { // depth 
  //   VkImageCreateInfo ci {}; 
  //   rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);
  
  //   cx::CreateInfo_2D (ci, glob.depth_format, depth_target_usage, sample_bit_count, 
  //                      kDefaultDimensions.width, kDefaultDimensions.height);

  //   //glob.msaa_samples rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  //   depthattach.image = rc::CreateImage (ci, device); 

  //   // create imageview...
  //   assert (false); 
  // }

  return true; 
}

// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
rokz::SwapchainResetter::Ref CreateResetMilkshake (rokz::Display& display, Vec<rc::Attachment>& colors, rc::Attachment& depth,
                                                   const VkExtent2D& ext2d) {
  //
  struct milkshake_resetter : public SwapchainResetter {
    rokz::Display&       display; 
    Vec<rc::Attachment>& colors;     
    rc::Attachment&      depth;
    const VkExtent2D&    ext2d;  
    
  public:

    milkshake_resetter (rokz::Display& disp, Vec<rc::Attachment>& cols, rc::Attachment& dep, const VkExtent2D& ext)
      : display (disp), colors (cols), depth (dep), ext2d (ext)  {
    }
    //

    virtual bool Reset (const rokz::Device& device) {
      HERE("TODO"); 
      assert (false); 
      return false; 
    }
  };
  
  return std::make_shared<milkshake_resetter> (display, colors, depth, ext2d); 
}
// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
int milkshake::run (const Vec<std::string>& args) {
  HERE("hai");

  milkshake::Glob glob; 

  rc::SwapchainGroup&                     scg  = glob.swapchain_group;
  Arr<per_frame_set, kMaxFramesInFlight>& sync = glob.sync; 


  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath pipe_path = OWENS_LAKE"/rokz/pipeline"; //
  systempath data_path = OWENS_LAKE"/rokz/data";     // 

  //Default (glob); 
  glfwInit();
  
  rokz::InitializeInstance (glob.instance); 

  rekz::SetupDisplay (glob.display, glob.input_state, "milkshake", kDefaultDimensions , glob.instance); 
  
  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //

  // 
  rokz::cx::QuerySwapchainSupport (glob.swapchain_info, glob.display.surface, glob.device.physical.handle);

  VkPhysicalDeviceFeatures2 f2 {};
  rokz::ConfigureFeatures  (f2, glob.device.physical);
  // this does a lot of shit
  //rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  rokz::InitializeDevice (glob.device, f2, glob.device.physical, glob.instance);
  // put these somwehere

  // 1 sample
  glob.msaa_samples = VK_SAMPLE_COUNT_1_BIT;  // rokz::ut::MaxUsableSampleCount (glob.device.physical); 

  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_info, glob.display.surface,
                            kDefaultDimensions, glob.device.physical, glob.device);
  //  
  //const size_t NuberOfColorTargets = 2; 
  setup_color_render_attachments (glob); 
  
  glob.swapchain_resetter =
      CreateResetMilkshake (glob.display, glob.attachment.color, glob.attachment.depth, kDefaultDimensions); 
                                                  
  // rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
  //                                                        glob.depth_image, glob.depth_imageview,
  //                                                        glob.msaa_color_image, glob.msaa_color_imageview); 

  // create render pass 
  
  // for BeginRendering ()

  setup_gbuf_renderpass (glob);
  setup_lcomp_renderpass (glob);

  setup_gbuf_framebuffers (glob); 
  setup_lcomp_framebuffers (glob); 

  //rokz::SetupDynamicRenderingInfo;//  (glob.rendering_info_group, glob.msaa_color_imageview->handle,
                                  // glob.depth_imageview->handle, scg.extent); 

  // define first 
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, grid::kDescriptorBindings, glob.device); 

  rokz::DefineDescriptorSetLayout (glob.dorito_dslo, dorito::kDescriptorBindings, glob.device); 


  // ---------------- INIT GRID PIPELINE  ---------------------
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, grid::kDescriptorBindings, glob.device); 

  glob.grid.pipe.dslos.push_back (glob.grid_dslo.handle);
  if (!rekz::grid::InitPipeline (glob.grid.pipe,  glob.grid.plo, glob.grid.pipe.dslos , pipe_path,
                                 kDefaultDimensions, glob.msaa_samples,
                                 scg.image_format, glob.depth_format, glob.device)) { 
    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }

  // ---------------- INIT OSD PIPELINE  ---------------------
  // rokz::DefineDescriptorSetLayout (glob.osd_dslo, onscreen::kDescriptorBindings, glob.device); 

  // glob.osd_pl.dslos.push_back (glob.osd_dslo.handle); 
  // if (!onscreen::InitPipeline (glob.osd_pl, glob.osd_plo, glob.osd_pl.dslos, dark_path,
  //                              kDefaultDimensions, glob.msaa_samples,
  //                              scg.image_format, glob.depth_format,  glob.device)) {
  //   printf ("[FAILED] --> OSD pipeline \n"); 
  //   return false; 
  // }
  //
  rokz::SetupGlobalUniforms (glob.global_bu, kMaxFramesInFlight, glob.device); 

  // pipelines...

  // data...

  // descriptor sets..

  // draw drawlists

  // create frame syncs
  assert (false); 
  // fsg.command_buffers.resize (kMaxFramesInFlight);
  // fsg.syncs.resize           (kMaxFramesInFlight);
  // rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.device.command_pool.handle); 

  // for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
  //   // ^^ 'CreateCommandBuffers' should be called, we call it 
  //   rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], fsg.command_buffer_alloc_info, glob.device.handle);
  //   rokz::cx::CreateFrameSync (fsg.syncs[i], fsg.syncs[i].ci, glob.device.handle);
  // } 

  
  const double time_per_frame_sec = 1.0 / 60.0;
  double Dt = time_per_frame_sec; // just do this for now
  // auto t0 = std::chrono::high_resolution_clock::now(); 
  // std::chrono::system_clock::time_point then = t0; 
  MilkLoop milkloop (glob, Dt); 
  rokz::FrameLoop  (milkloop);


  HERE(" milkloope exited "); 
  vkDeviceWaitIdle(glob.device.handle);

//   // CLEAN UP
   cleanup_milkshake (glob); 


  printf ("res path:%s\n", OWENS_LAKE); 
  
  return 0; 
}


