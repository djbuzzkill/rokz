
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
using namespace milkshake;

// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
bool setup_lcomp_attachments (Glob& glob); 
bool setup_gbuff_attachments (Glob& glob); 

bool setup_gbuff_framebuffer  (Glob& glob);
bool setup_lcomp_framebuffers (Glob& glob); 

bool setup_gbuff_renderpass (Glob& glob);
bool setup_lcomp_renderpass (Glob& glob); 

// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
void cleanup_milkshake (Glob& glob) {

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
rokz::SwapchainResetter::Ref CreateResetMilkshake (rokz::Display& display,
                                                   rc::Attachment& position,
                                                   rc::Attachment& normal,
                                                   rc::Attachment& albedo,
                                                   rc::Attachment& gbuff_depth,
                                                   rc::Attachment& lcomp_depth,
                                                   const VkExtent2D& ext2d) {
  //
  struct milkshake_resetter : public SwapchainResetter {
    rokz::Display&  display; 
    rc::Attachment& position;
    rc::Attachment& normal;
    rc::Attachment& albedo;
    rc::Attachment& gbuff_depth;
    rc::Attachment& lcomp_depth;
    const VkExtent2D&    ext2d;  
    
  public:

    milkshake_resetter (rokz::Display& disp, 
                        rc::Attachment& pos, rc::Attachment& norm, rc::Attachment& alb,
                        rc::Attachment& gdep, rc::Attachment& ldep, const VkExtent2D& ext)
      
      : display (disp), position (pos), normal (norm), albedo(alb), 
        gbuff_depth(gdep), lcomp_depth(ldep), ext2d (ext)  {
    }
    //

    virtual bool Reset (const rokz::Device& device) {
      HERE("TODO"); 
      assert (false); 
      return false; 
    }
  };
  
  return std::make_shared<milkshake_resetter> (display, position, normal, albedo,
                                               gbuff_depth, lcomp_depth, ext2d); 
}

// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
struct MilkshakeDeviceFeatures : public VkPhysicalDeviceFeatures2 {
  
  MilkshakeDeviceFeatures (const rokz::PhysicalDevice& physdev) : VkPhysicalDeviceFeatures2 {} {

    separate_depth_stencil_layout_feature.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES;
    separate_depth_stencil_layout_feature.pNext                       = VK_NULL_HANDLE;
    separate_depth_stencil_layout_feature.separateDepthStencilLayouts = VK_TRUE;

    sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2; 
    pNext    = &separate_depth_stencil_layout_feature; 
    features = physdev.features2.features;             
  }
  
  // ext structs
  VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures separate_depth_stencil_layout_feature {}; 
 // VK_KHR_synchronization2
 // VK_KHR_timeline_semaphore
 // VK_EXT_color_write_enable
 // VK_EXT_extended_dynamic_state3
 // VK_EXT_separate_stencil_usage
 // VK_EXT_transform_feedback
  
}; 

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
  rokz::cx::QuerySwapchainSupport (glob.swapchain_info, glob.display.surface, glob.device.physical.handle);

  // -- replace rokz::ConfigureFeatures  (feats, glob.device.physical);
  MilkshakeDeviceFeatures milkfeats (glob.device.physical);

  // this does a lot of shit
  //rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  rokz::InitializeDevice2 (glob.device, milkfeats, glob.device.physical, glob.instance);
  // put these somwehere

  // 1 sample
  glob.msaa_samples = VK_SAMPLE_COUNT_1_BIT;  // rokz::ut::MaxUsableSampleCount (glob.device.physical); 

  rokz::ut::FindDepthFormat (glob.depth.format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_info, glob.display.surface,
                            kDefaultDimensions, glob.device.physical, glob.device);

  glob.swapchain_resetter = 
      CreateResetMilkshake (glob.display,
                            glob.gbuff.attachment.position,
                            glob.gbuff.attachment.normal,
                            glob.gbuff.attachment.albedo,
                            glob.gbuff.attachment.depth,
                            glob.lcomp.attachment.depth,
                            kDefaultDimensions); 
                                                  
  // rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
  //                                                        glob.depth_image, glob.depth_imageview,
  //                                                        glob.msaa_color_image, glob.msaa_color_imageview); 

  // create render pass 
  //  
  //const size_t NuberOfColorTargets = 2; 
  setup_gbuff_attachments (glob); 
  setup_gbuff_renderpass  (glob);
  setup_gbuff_framebuffer (glob); 

  HERE("B4 Lcomp things");  
  setup_lcomp_attachments (glob); 
  setup_lcomp_renderpass  (glob);
  setup_lcomp_framebuffers(glob); 
  HERE("4f Lcomp things");  
  // for BeginRendering ()
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
                                 scg.format, glob.depth.format, glob.device)) { 
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

  HERE ("407");
  assert (false); 
  HERE ("409");
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


