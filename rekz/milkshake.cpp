
#include "milkshake.h"
#include "lumen_pipe.h"

#include "rokz/attachment.hpp"
#include "rokz/context.hpp"
#include "rokz/descriptor.hpp"
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
struct LoopFrame {

  milkshake::Glob& glob;

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 6000;

  const float Dt; 
  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);

  

  LoopFrame (milkshake::Glob& g, double dt) : glob (g), Dt (dt) {
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

      cx::FrameDrawingBegin (glob.swapchain_group, glob.sync[curr_frame].commandbuf,
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

      
      cx::FrameDrawingEnd (glob.swapchain_group,
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
struct milkshake::ConfigureFeatures : public VkPhysicalDeviceFeatures2 {
  
  ConfigureFeatures (const rokz::Device& dev) {

    separate_depth_stencil_layout_feature.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES;
    separate_depth_stencil_layout_feature.pNext                       = VK_NULL_HANDLE;
    separate_depth_stencil_layout_feature.separateDepthStencilLayouts = VK_TRUE;

    sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2; 
    pNext    = &separate_depth_stencil_layout_feature; 
    features = dev.physical.features2.features;             
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
  ConfigureFeatures milkfeats (glob.device);

  // this does a lot of shit
  //rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  rokz::InitializeDevice (glob.device,
                          milkfeats, glob.device.physical, glob.instance);
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
  rokz::DefineDescriptorSetLayout (glob.grid_dslo,
                                   grid::kDescriptorBindings, glob.device); 

  rokz::DefineDescriptorSetLayout (glob.lumen_dslo,
                                   lumen::kDescriptorBindings, glob.device); 

  // ---------------- INIT GRID PIPELINE  ---------------------
  rokz::DefineDescriptorSetLayout (glob.grid_dslo,
                                   grid::kDescriptorBindings, glob.device); 

  glob.grid.pipe.dslos.push_back (glob.grid_dslo.handle);
  if (!rekz::grid::InitPipeline (glob.grid.pipe,  glob.grid.plo, VK_NULL_HANDLE,
                                 glob.grid.pipe.dslos , pipe_path, kDefaultDimensions,
                                 glob.msaa_samples, scg.format, glob.depth.format,
                                 glob.device)) { 
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
  LoopFrame  milkloop (glob, Dt); 
  rokz::FrameLoop  (milkloop);


  HERE(" milkloope exited "); 
  vkDeviceWaitIdle(glob.device.handle);

//   // CLEAN UP
   cleanup_milkshake (glob); 


  printf ("res path:%s\n", OWENS_LAKE); 
  
  return 0; 
}



// drwx------   7 djbuzzkill djbuzzkill  4096 Jun  6 22:29 ..
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep 25  2022 0blayout-20190703.527
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep 12  2022 ace-link-20220901.1710
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Oct 31  2022 ace-window-20220911.358
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:05 all-the-icons-20230505.1635
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 all-the-icons-dired-2.0
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 all-the-icons-ibuffer-1.3.0
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 all-the-icons-ivy-0.5.0
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 all-the-icons-ivy-rich-1.9.0
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 annalist-20190929.207
// drwxr-xr-x   6 djbuzzkill djbuzzkill  4096 Jul 23  2022 archives
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 async-20230528.622
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 avy-20230420.404
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 bind-key-20230203.2004
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 bui-20210108.1141
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 cargo-20230125.1253
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 cargo-mode-20230213.401
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 cfrs-20220129.1149
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 closql-20230520.1519
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 cmake-font-lock-20230304.2223
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug 14  2022 cmake-ide-20210610.1525
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 cmake-mode-20230422.828
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug 14  2022 cmake-project-20171121.1115
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 command-log-mode-20160413.447
// drwxr-xr-x   4 djbuzzkill djbuzzkill  4096 Jun  5 20:05 company-20230416.2034
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 compat-29.1.4.1
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:05 compat-29.1.4.1.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:25 counsel-20230410.1815
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:25 counsel-projectile-20211004.2003
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:05 dap-mode-20230604.1120
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 dash-20230502.1324
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:17 dired-icon-0.5
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 doom-modeline-20230530.754
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:05 doom-themes-20230221.610
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 eglot-1.15
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:05 eglot-1.15.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 eldoc-1.14.0
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:04 eldoc-1.14.0.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 elisp-refs-20230419.405
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 emacsql-20230417.1448
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 emacsql-sqlite-20230225.2205
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 epl-20180205.2049
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug  8  2022 eshell-fringe-status-20170117.2316
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep  8  2022 eshell-git-prompt-20220830.2000
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug  8  2022 eshell-outline-20201121.620
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug  8  2022 eshell-prompt-extras-20210925.110
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug  8  2022 eshell-toggle-20220718.729
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug  8  2022 esh-help-20190905.22
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 eterm-256color-20210224.2241
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 evil-20230606.2141
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 evil-collection-20230530.1639
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 evil-nerd-commenter-20221207.1245
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 external-completion-0.1
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:04 external-completion-0.1.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 f-20230116.1032
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:09 flycheck-32
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:18 flycheck-clang-analyzer-20211214.648
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:12 flycheck-clang-tidy-0.3.0
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 15:59 flymake-1.3.4
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  6 15:59 flymake-1.3.4.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 forge-20230522.1054
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 general-20230311.1229
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 ghub-20230513.1619
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 git-commit-20230521.2344
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jan  3 21:39 glsl-mode-20210808.1945
// drwx------   3 djbuzzkill djbuzzkill  4096 Jun  6 22:29 gnupg
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 goto-chg-20220107.1733
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 helm-20230606.1113
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 helm-core-20230606.612
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:17 helm-icons-20230506.432
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 helm-xref-20211017.1334
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 helpful-20230504.1546
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 ht-20230214.1632
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Oct 31  2022 hydra-20220910.1206
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep 12  2022 iedit-20220216.717
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 ivy-20230410.1815
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep 12  2022 ivy-posframe-20211217.234
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 ivy-rich-20230425.1422
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 jsonrpc-1.0.17
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:04 jsonrpc-1.0.17.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug 14  2022 levenshtein-20090830.1040
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 lispy-20230314.1738
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 lsp-docker-20230508.2051
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 22:25 lsp-ivy-20220831.1823
// drwxr-xr-x   2 djbuzzkill djbuzzkill 12288 Jun  5 20:04 lsp-mode-20230602.1742
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 lsp-treemacs-20230504.516
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 lsp-ui-20230116.2024

// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 lua-mode-20221218.605
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 lv-20200507.1518
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:57 magit-20230606.1402
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 magit-section-20230521.1803
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  6 22:17 major-mode-icons-20220210.1404
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 markdown-mode-20230522.147
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Sep  8  2022 mode-icons-20200920.2031
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep  8  2022 modern-cpp-font-lock-20210405.1155
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:05 nerd-icons-20230603.1813
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 org-bullets-20200317.1740
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Sep 12  2022 pamparam-20210105.1513
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Oct 31  2022 pfuture-20220913.1401
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 pkg-info-20150517.1143
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 popup-20221231.1634
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 posframe-20230601.1529
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 powerline-20221110.1956
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 project-0.9.8
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:04 project-0.9.8.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  6 21:56 projectile-20230606.509
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 python-mode-20230515.722
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 pyvenv-20211014.707
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 rainbow-delimiters-20210515.1254
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 rustic-20230130.912
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 rust-mode-20230112.27
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep  8  2022 s-20220902.1511
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 shrink-path-20190208.1335
// drwxr-xr-x   6 djbuzzkill djbuzzkill  4096 Jun  5 20:04 sly-20230523.1254
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jul 23  2022 spinner-1.7.4
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jul 23  2022 spinner-1.7.4.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 swiper-20230410.1815
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Aug 12  2022 tabbar-20180726.1735
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep  8  2022 tabbar-ruler-20160802.307
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 transient-20230602.2121
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 treemacs-20230522.1925
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 treepy-20230528.750
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 use-package-20230426.2324
// drwxr-xr-x   3 djbuzzkill djbuzzkill  4096 Jun  5 20:04 vterm-20230417.424
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 which-key-20230330.2121
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 with-editor-20230511.2142
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep 12  2022 worf-20220102.835
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 xref-1.6.3
// -rw-r--r--   1 djbuzzkill djbuzzkill   154 Jun  5 20:04 xref-1.6.3.signed
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:03 xterm-color-20230321.3
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Jun  5 20:04 yaml-20230312.250
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Oct 31  2022 yasnippet-20200604.246
// drwxr-xr-x   2 djbuzzkill djbuzzkill  4096 Sep 12  2022 zoutline-20220102.835
