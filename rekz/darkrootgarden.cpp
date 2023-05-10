
#include "darkrootgarden.h"


#include "rekz/drawpolygon.h"
#include "rekz/onscreen_data.h"
#include "rekz/rekz.h"
// #include "rokz/buffer.h"
// #include "rokz/context.h"
// #include "rokz/draw_sequence.h"
// #include "rokz/pipeline.h"
#include "rokz/global_descriptor.hpp"


#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

#include <shaderc/shaderc.hpp>
#include <shaderc/status.h>
#include <vulkan/vulkan_core.h>
// 
#include "dark_obj_pipeline.h"
#include "grid_pipeline.h"
#include "onscreen_pipe.h"


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const size_t kMaxFramesInFlight = darkroot::Glob::MaxFramesInFlight; 

const VkExtent2D    kTestExtent  = { 1024, 768 };
//
//
using namespace darkroot; 
// ---------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------
Glob::Glob()
  : instance()
  , device ()
  // , depth_image()
  // , depth_imageview()
  , msaa_samples ()
  , swapchain_info()
  , shared ()
{ 
  // queues.graphics = {};
  // queues.present = {}; 
}

// ---------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------
bool GenerateMipMaps (rokz::Image& image, bool generate_mipmap, uint32_t num_mipmaps) {
  assert (false); 
  return false; 
}

// ---------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------
void CleanupDarkroot (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 

  rekz::CleanupPolygonData (glob.polyd, glob.device); 
  glob.drawpoly.reset ();
  glob.poly_objects_bu.clear ();

  glob.gridbuff.reset (); //rekz::CleanupGridData    (glob.gridata, glob.device); 
  glob.drawgrid.reset (); 
  // descriptor set layouts
  rokz::cx::Destroy (glob.grid_dslo, glob.device); 
  rokz::cx::Destroy (glob.object_dslo, glob.device); 

  glob.global_rc_uniform_bu.clear ();

  // dont bother freeing if pool is destroyed anyways
  rokz::cx::Destroy (glob.grid_de.pool, glob.device); 

  //glob.poly_objects_de; // ?!?! how r descriptors handled
  rokz::cx::Destroy (glob.poly_objects_de.pool, glob.device); 

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
  rekz::CleanupSwapchain (glob.swapchain_group.views,
                          glob.msaacolorimage, glob.msaacolorimageview,
                          glob.depthimage, glob.depthimageview,
                          glob.swapchain_group.swapchain, 
                          glob.device);

  // the the  poly stuff is done in Cleanup, grids is done here
  for (auto shmod : glob.grid_pl.shader_modules) {
    vkDestroyShaderModule (glob.device.handle, shmod.handle, nullptr); 
  }

  vkDestroyPipelineLayout (glob.device.handle, glob.grid_plo.handle, nullptr);
  Vec<VkPipeline> pipes = { 
    glob.polys_pl.handle, glob.grid_pl.handle }; 

  
  darkroot::Cleanup (pipes,
                     glob.display.surface,
                     glob.device.command_pool.handle,
                     glob.framesyncgroup.syncs, 
                     glob.polys_pl.shader_modules,
                     glob.polys_plo.handle,  
                     glob.display,
                     glob.device,
                     glob.instance.handle);

  //
  glfwTerminate();
  
}

// ---------------------------------------------------------------------------------------
//  encapsulate rendering loop
// ---------------------------------------------------------------------------------------
struct RootLoop {

  Glob& glob;

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 6000;

  const float Dt; 
  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);

  void UpdateRunState  () {
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q)) {
      printf ("--> [q] pressed... quitting \n");
      run = false;
    }

  } 
  
  //
  // -------------------------------------------------------------
  RootLoop (Glob& g, float dt) : glob(g), Dt(dt) { 

    then = std::chrono::high_resolution_clock::now(); 

    time_per_frame = std::chrono::microseconds (static_cast<size_t>(Dt * 1000000.0));
    
    printf ( "\nBegin run for [%i] frames.. \n\n", countdown); 

  }
  // while (cond()) loop()
  // -------------------------------------------------------------
  bool cond () {
    return countdown && run && !glfwWindowShouldClose(glob.display.window.glfw_window); 
  }
  //
  // -------------------------------------------------------------
  bool loop () {

    auto now = std::chrono::high_resolution_clock::now();    
    
    glfwPollEvents(); 

    UpdateRunState ();

    { 
      char msg[64];
      sprintf (msg, "OSD test: %i", countdown);
      glob.osdata.strings[0] = msg;
    }
      
    UpdateViewAttitude (glob.shared.view_rot, glob.mouse_prev, glob.prev_inside, glob.input_state, 0.01f);
    UpdateViewPosition (glob.shared.view_pos, glob.shared.view_rot, glob.input_state, 0.1);

    //
    rc::SwapchainGroup& scg = glob.swapchain_group; 
    // get image index up here
    uint32_t image_index;

    // -- no_frame_sync -- 
    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain->handle, image_index,
                                                  glob.framesyncgroup.syncs[curr_frame].in_flight_fence, 
                                                  glob.framesyncgroup.syncs[curr_frame].image_available_sem, 
                                                  glob.device); 
    //VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain->handle, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 

    // -------------------- ---------------------
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_resetter->Reset (glob.display, glob.device);
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
      rokz::UpdateGlobals (glob.shared, glob.global_rc_uniform_bu [curr_frame], kTestExtent, Dt);

      // did not work as planned
      //onscreen::UpdateOSD (glob.global_rc_uniform_bu[curr_frame], glob.osdata.strings, kTestExtent, Dt);  
      
      // make sure the correct swapchain image is used

      rokz::UpdateDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolorimageview->handle,
                                        glob.swapchain_group.views[image_index]->handle);

      // ------------------------------- render pass start -------------------------------
      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here
      cx::FrameDrawBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                          image_index, glob.rendering_info_group.ri, glob.device);
      // EXECUTE DRAW LIST RECORDING 
      // for drawseq's
      // const std::vector<VkDescriptorSet> descrsets = {
      //   glob.global_uniform_de.descrsets[curr_frame], glob.objres_uniform_de.descrsets[curr_frame]
      // };
      rokz::DrawSequence::RenderEnv poly_re {
        glob.polys_pl, glob.polys_plo.handle, glob.shared, 
      };

      rokz::DrawSequence::RenderEnv grid_re {
        glob.grid_pl, glob.grid_plo.handle, glob.shared, 
      };

      rokz::DrawSequence::RenderEnv osd_re  {
        glob.osd_pl, glob.osd_plo.handle, glob.shared, 
      };
      
      //
      // polygons
      glob.drawpoly->Prep (curr_frame, poly_re, glob.device); 
      glob.drawpoly->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, poly_re);
      // grid
      glob.drawgrid->Prep (curr_frame, grid_re, glob.device); 
      glob.drawgrid->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, grid_re);
      //
      // no more scene beyond here this is overlay now
      glob.osdraw->Prep (curr_frame, osd_re , glob.device); 
      glob.osdraw->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, osd_re); 

      // -- we are done, submit
      cx::FrameDrawEnd (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame], 
                        image_index, 
                        glob.framesyncgroup.syncs[curr_frame].image_available_sem,
                        glob.framesyncgroup.syncs[curr_frame].render_finished_sem,
                        glob.framesyncgroup.syncs[curr_frame].in_flight_fence, 
                        glob.device);
    }
    
    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }

    curr_frame = (curr_frame + 1) % kMaxFramesInFlight;
    then = now; // std::chrono::high_resolution_clock::now(); 
    countdown--; 

    return true;
  }
};

// #define  DARKROOT_HIDE_OSD_PATH 1

// ------------------------------------------------------------------------------------------
// main()
// ------------------------------------------------------------------------------------------
int darkrootbasin (const std::vector<std::string>& args) {
 
  printf ( " Mv = v is the correct order \n"); 
  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 
  Glob glob; // *globmem; // something representing the app state
  rc::SwapchainGroup& scg = glob.swapchain_group;
  
  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath dark_path = OWENS_LAKE"/rokz/pipeline"; //
  systempath data_path = OWENS_LAKE"/rokz/data";     // 

  glfwInit();
  // CREATE INSTANCE AND DEVICE
  //bool rekz::InitializeInstance (rokz::Instance& instance) {

  rokz::InitializeInstance (glob.instance); 

  rokz::SetupDisplay (glob.display, "darkroot garden", glob.input_state, kTestExtent, glob.instance); 
  //SetupDisplay (glob); 

  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_info, glob.display.surface, glob.device.physical.handle);

  VkPhysicalDeviceFeatures2 features2 {};  
  //rokz::ConfigureDevice (glob.device.physical, VK_TRUE);
  rokz::ConfigureFeatures (features2, glob.device.physical);

  // this does a lot of shit
  rokz::InitializeDevice (glob.device, features2, glob.device.physical, glob.instance);
  
  // put these somwehere else
  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_info, glob.display.surface,
                           kTestExtent, glob.device.physical, glob.device);

  // ---------------- INIT POLYGON PIPELINE ---------------------
  rokz::DefineDescriptorSetLayout (glob.object_dslo, obz::kDescriptorBindings, glob.device); 

  glob.polys_pl.dslos.push_back (glob.object_dslo.handle);
  if (!rekz::InitObjPipeline (glob.polys_pl, glob.polys_plo, glob.polys_pl.dslos, dark_path,
                              kTestExtent, glob.msaa_samples,
                              scg.image_format, glob.depth_format, glob.device)) {
    printf ("[FAILED] --> InitObjPipeline \n"); 
    return false;
  }


  // ---------------- INIT GRID PIPELINE  ---------------------
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, grid::kDescriptorBindings, glob.device); 

  glob.grid_pl.dslos.push_back (glob.grid_dslo.handle);
  if (!rekz::grid::InitPipeline (glob.grid_pl,  glob.grid_plo, glob.grid_pl.dslos , dark_path,
                                 kTestExtent, glob.msaa_samples,
                                 scg.image_format, glob.depth_format, glob.device)) { 
    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }

  // ---------------- INIT OSD PIPELINE  ---------------------
  rokz::DefineDescriptorSetLayout (glob.osd_dslo, onscreen::kDescriptorBindings, glob.device); 

  glob.osd_pl.dslos.push_back (glob.osd_dslo.handle); 
  if (!onscreen::InitPipeline (glob.osd_pl, glob.osd_plo, glob.osd_pl.dslos, dark_path,
                               kTestExtent, glob.msaa_samples,
                               scg.image_format, glob.depth_format,  glob.device)) {
    printf ("[FAILED] --> OSD pipeline \n"); 
    return false; 
  }

  //
  rc::SetupMSAARenderingAttachments (glob.msaacolorimage,
                                     glob.msaacolorimageview, 
                                     
                                     glob.depthimage,       
                                     glob.depthimageview,
                            
                                     glob.msaa_samples,
                                     scg.image_format,
                                     glob.depth_format,          
                                     kTestExtent,
                                     glob.device); // <-- this does all the additional  attachmentes
  //
  glob.swapchain_resetter = rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.views,
                                                           glob.depthimage, glob.depthimageview,
                                                           glob.msaacolorimage, glob.msaacolorimageview); 

  // for BeginRendering ()
  rokz::SetupDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolorimageview->handle,
                                   glob.depthimageview->handle, scg.extent); 


  //  --------------------------- GLOBAL ---------------------------
  rokz::SetupGlobalUniforms (glob.global_rc_uniform_bu, kMaxFramesInFlight, glob.device); 
  // global buffer shared across all descriptor sets 


  // ----------------- GRID -------------------
  size_t vertoffs = 0;
  size_t indoffs = 0;
  glob.gridbuff = rekz::SetupGridData (vertoffs, indoffs, 11, 11, 20.0, 20.0, glob.device); 
  if (!rokz::MakeDescriptorPool(glob.grid_de.pool, kMaxFramesInFlight, grid::kDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }

  // 
  if (!rokz::MakeDescriptorSets (glob.grid_de.descrsets, glob.grid_de.alloc_info, kMaxFramesInFlight,
                                 glob.grid_dslo.handle, glob.grid_de.pool, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorSets \n"); 
    return false;
  }

  if ( !grid::BindDescriptorResources (glob.grid_de.descrsets, glob.global_rc_uniform_bu, glob.device)) {
    printf ("[FAILED] --> grid::BindDescriptorResources \n"); 
    return false; 
  }
  // draw list 
  glob.drawgrid = rekz::CreateDrawGrid (glob.gridbuff, glob.grid_de, vertoffs, indoffs);

  // -------------- polygon obz --------------------
  // setup data
  rekz::SetupPolygonData (glob.polyd, kMaxFramesInFlight, data_path, glob.device); 

  rekz::SetupObjectUniforms (glob.poly_objects_bu, kMaxFramesInFlight, glob.device);
  // SetupObjDescriptorPool
  if (!rokz::MakeDescriptorPool(glob.poly_objects_de.pool, kMaxFramesInFlight, rekz::obz::kDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }

  // POLYGONS
  if (!rokz::MakeDescriptorSets (glob.poly_objects_de.descrsets, glob.poly_objects_de.alloc_info,
                                 kMaxFramesInFlight, glob.object_dslo.handle,
                                 glob.poly_objects_de.pool, glob.device)) {
    printf ("[FAILED|%i] --> MakeDescriptorSets \n", __LINE__ ); 
    return false;
  }

  if (!rekz::BindObjectDescriptorResources (glob.poly_objects_de.descrsets, glob.global_rc_uniform_bu, 
                                            glob.poly_objects_bu, glob.polyd.imageviews,
                                            glob.polyd.sampler, glob.object_dslo, glob.device)) {
    printf ("[FAILED] --> BindObjectDescriptorSets \n"); 
    return false;
  } 
  //  draw list
  glob.drawpoly = rekz::polyob::CreateDrawPoly (glob.polyd, glob.poly_objects_bu, glob.poly_objects_de);

  //  -------------------------- OSD --------------------------
  onscreen::SetupData (glob.osdata, kMaxFramesInFlight, glob.device); 

  if (!rokz::MakeDescriptorPool (glob.osd_de.pool, kMaxFramesInFlight, onscreen::kDescriptorBindings, glob.device)) {
    printf ("[FAILED|%i] --> MakeDescriptorPool \n", __LINE__ ); 
    return false;
  }
  //
  if (!rokz::MakeDescriptorSets (glob.osd_de.descrsets, glob.osd_de.alloc_info, kMaxFramesInFlight, 
                                 glob.osd_dslo.handle, glob.osd_de.pool, glob.device)) {

    printf ("[FAILED|%i] --> MakeDescriptorSets \n", __LINE__ ); 
    return false;
  }

  //
  if (!rekz::onscreen::BindDescriptorResources (glob.osd_de.descrsets,
                                                glob.global_rc_uniform_bu,
                                                glob.osdata.texture.view, 
                                                glob.osdata.texture.sampler,
                                                glob.osd_dslo, glob.device)) {
    printf ("[FAILED|%i] --> onscreen::BindDescriptorResources \n", __LINE__ ); 
    return false;
  }
  // draw
  glob.osdraw = onscreen::CreateDrawText (glob.osdata, glob.osd_de.descrsets); 
  //#endif
    //rekz::CreateDrawGrid (glob.gridata); 
  // items per frames 
  //scg.command_buffer_group.buffers.resize (kMaxFramesInFlight);
  rokz::FrameSyncGroup& fsg = glob.framesyncgroup;
  // !! 
  fsg.command_buffers.resize (kMaxFramesInFlight);
  fsg.syncs.resize           (kMaxFramesInFlight);
  rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.device.command_pool.handle); 

  for (size_t i = 0; i < Glob::MaxFramesInFlight; ++i) {
    // ^^ 'CreateCommandBuffers' should be called, we call it 

    // command buffer for dispatching this frame
    rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], fsg.command_buffer_alloc_info, glob.device.handle);

    // creates image_available, render_finished,and in_flight_fence
    rokz::cx::CreateFrameSync (fsg.syncs[i], fsg.syncs[i].ci, glob.device.handle);
  } 

  // RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION 
  // RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION 
  const double time_per_frame_sec = 1.0 / 60.0;
  double Dt = time_per_frame_sec; // just do this for now
  
  auto t0 = std::chrono::high_resolution_clock::now(); 
  std::chrono::system_clock::time_point then = t0; 

  RootLoop rootloop (glob, Dt ); 
  rokz::FrameLoop  (rootloop);

  vkDeviceWaitIdle(glob.device.handle);
  // CLEAN UP
  CleanupDarkroot (glob); 
  HERE("bai");
  return 0;


// VkSubpassDependency dependency{};
// dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
// dependency.dstSubpass = 0;
// //make sure the first pass has complete the color output in G-buffers and in depth buffer
// dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
// //make sure the second pass will wait the first pass before execute the fragment shader which read the G-buffers of the first pass
// dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
// //the first pass goal is the write in the attachments (G-buffers) & depth buffer 
// dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
// //the second pass goal is to write in the lit final scene attachment 
// dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


}

