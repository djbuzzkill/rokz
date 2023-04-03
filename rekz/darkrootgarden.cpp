
#include "darkrootgarden.h"


#include "rekz/rekz.h"
// #include "rokz/buffer.h"
// #include "rokz/context.h"
// #include "rokz/draw_sequence.h"
// #include "rokz/pipeline.h"
#include "rokz/global_descriptor.h"


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

const VkExtent2D    kTestExtent  = { 800, 600 };
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
  , swapchain_support_info()
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

bool LoadIndexBuffer_static ();
bool LoadVertexBuffer_static();

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
  rokz::cx::Destroy (glob.global_dslo, glob.device); 
  rokz::cx::Destroy (glob.object_dslo, glob.device); 

  glob.global_rc_uniform_bu.clear ();

  // dont bother freeing if pool is destroyed anyways
  //rokz::cx::Free   (glob.descrgroup_objs.descrsets, glob.descrgroup_objs.pool, glob.device.handle); 
  rokz::cx::Destroy (glob.global_uniform_de.pool, glob.device); 
  //glob.poly_objects_de; // ?!?! how r descriptors handled
  rokz::cx::Destroy (glob.poly_objects_de.pool, glob.device); 
  // ?  
  // polygons will make use of object descriptors

  // for (auto& buf : glob.poly_objects_bu) { 
  //   buf.reset ();
  //   //rokz::cx::Destroy  (buf, glob.device.allocator); 
  // }

  //
  rekz::CleanupSwapchain (glob.swapchain_group.imageviews,
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

    UpdateRunState () ;

    {
      char msg[64];
      sprintf (msg, "osd test :%i", countdown);
      glob.osdata.strings[0] = msg; 
    }
      
    
    rekz::UpdateViewAttitude (glob.shared.view_rot, glob.mouse_prev, glob.prev_inside, glob.input_state, 0.01f);
    rekz::UpdateViewPosition (glob.shared.view_pos, glob.shared.view_rot, glob.input_state, 0.1);

    //
    rc::SwapchainGroup& scg = glob.swapchain_group; 
    // get image index up here
    uint32_t image_index; 
    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain->handle, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 
    
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_resetter->Reset (glob.display, glob.device);
      printf ("===> %i <=== ]\n", __LINE__);
      return true;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {


      rokz::DrawSequence::PipelineAssembly
        papoly { glob.polys_pl, glob.polys_plo.handle }; 

      rokz::DrawSequence::PipelineAssembly
        pagrid { glob.grid_pl, glob.grid_plo.handle }; 

      rokz::DrawSequence::PipelineAssembly
        pa_osd  { glob.osd_pl, glob.osd_plo.handle }; 

      // ------------------------ Updaate b4 draw ----------------------------
      //
      //   update uniform here, but uniforms can also b update during DrawSeq::Prep ()
      // 
      //UpdateDarkUniforms (glob, curr_frame, Dt); 
      rokz::UpdateGlobals (glob.shared, glob.global_rc_uniform_bu [curr_frame], kTestExtent, Dt);

      // update data needed to record drawlist
      onscreen::UpdateOverlayDescriptors (glob.osdata.ubs[curr_frame],
                                          glob.osdata.strings, kTestExtent, Dt);  

      
      // make sure the correct swapchain image is used
      rokz::UpdateDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolorimageview->handle,
                                        glob.swapchain_group.imageviews[image_index]->handle);



      
      // ------------------------------- render pass start -------------------------------
      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here
      rc::FrameDrawBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                          image_index, glob.rendering_info_group.ri, glob.device);
      // EXECUTE DRAW LIST RECORDING 
      // for drawseq's
      // const std::vector<VkDescriptorSet> descrsets = {
      //   glob.global_uniform_de.descrsets[curr_frame], glob.objres_uniform_de.descrsets[curr_frame]
      // };
      rokz::DrawSequence::RenderEnv poly_re {
        papoly, glob.shared, glob.descriptormaps[curr_frame]
      };

      rokz::DrawSequence::RenderEnv grid_re {
        pagrid, glob.shared, glob.descriptormaps[curr_frame]
      };
      // !! there is nothing in shared_globals that is especially useful to overlay
      rokz::DrawSequence::RenderEnv osd_re  {
        pa_osd, glob.shared, glob.descriptormaps[curr_frame]
      };
        
      //
      // scene draw 
      glob.drawpoly->Prep (curr_frame, poly_re, glob.device); 
      glob.drawpoly->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, poly_re);
      //
      glob.drawgrid->Prep (curr_frame, grid_re, glob.device); 
      glob.drawgrid->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, grid_re);

      //
      // no more scene beyond here this is overlay now
      glob.osdraw->Prep (curr_frame, osd_re , glob.device); 
      glob.osdraw->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, osd_re); 

      
      // we are done, submit
      rc::FrameDrawEnd (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame], 
                    image_index, glob.framesyncgroup.syncs[curr_frame], glob.device);
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


// ------------------------------------------------------------------------------------------
// main()
// ------------------------------------------------------------------------------------------
int darkrootbasin (const std::vector<std::string>& args) {
 
  printf ( " Mv = v is the correct order \n"); 
  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 

  Glob  glob; // *globmem; // something representing the app state
  rc::SwapchainGroup& scg = glob.swapchain_group;
  
  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath dark_path = "/home/djbuzzkill/owenslake/rokz/pipeline"; //
  systempath data_path = "/home/djbuzzkill/owenslake/rokz/data";     // 

  glfwInit();
  // CREATE INSTANCE AND DEVICE
  //bool rekz::InitializeInstance (rokz::Instance& instance) {

  rokz::InitializeInstance (glob.instance); 

  rokz::SetupDisplay (glob.display, "darkroot garden", glob.input_state, kTestExtent, glob.instance); 
  //SetupDisplay (glob); 

  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info, glob.display.surface, glob.device.physical.handle);

  VkPhysicalDeviceFeatures2 features2 {};  
  //rokz::ConfigureDevice (glob.device.physical, VK_TRUE);
  rokz::ConfigureFeatures (features2, glob.device.physical);

  // this does a lot of shit
  rokz::InitializeDevice (glob.device, features2, glob.device.physical, glob.instance);
  
  // put these somwehere else
  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_support_info, glob.display.surface,
                           kTestExtent, glob.device.physical, glob.device);
  // define first

  //rekz::kGlobalDescriptorBindings
  rokz::DefineDescriptorSetLayout (glob.global_dslo, rekz::kGlobalDescriptorBindings, glob.device); 
  // rekz::obz::kDescriptorBindings
  rokz::DefineDescriptorSetLayout (glob.object_dslo, rekz::obz::kDescriptorBindings, glob.device); 

  rokz:: DefineDescriptorSetLayout(glob.osd_dslo, onscreen::kDescriptorBindings, glob.device); 



  // polygon pipeline uses both descriptor sets
  glob.polys_pl.dslos.push_back (glob.global_dslo.handle);
  glob.polys_pl.dslos.push_back (glob.object_dslo.handle);
  if (!rekz::InitObjPipeline (glob.polys_pl, glob.polys_plo, glob.polys_pl.dslos, dark_path,
                              kTestExtent, glob.msaa_samples,
                              scg.image_format, glob.depth_format, glob.device)) {
    printf ("[FAILED] --> InitObjPipeline \n"); 
    return false;
  }
  // grid only uses globals
  glob.grid_pl.dslos.push_back (glob.global_dslo.handle);
  if (!rekz::InitGridPipeline (glob.grid_pl,  glob.grid_plo, glob.grid_pl.dslos , dark_path,
                               kTestExtent, glob.msaa_samples,
                               scg.image_format, glob.depth_format, glob.device)) { 
    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }

  
  glob.osd_pl.dslos.push_back (glob.osd_dslo.handle); 
  if (!onscreen::InitPipeline (glob.osd_pl,  glob.osd_plo,  glob.osd_pl.dslos, dark_path,
                               kTestExtent, glob.msaa_samples, scg.image_format, glob.device)) {
    printf ("[FAILED] --> OSD pipeline \n"); 
    return false; 
  }

  //glob.ods_plo;



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
  glob.swapchain_resetter = rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
                                                           glob.depthimage, glob.depthimageview,
                                                           glob.msaacolorimage, glob.msaacolorimageview); 
  //
  // for BeginRendering ()
  rokz::SetupDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolorimageview->handle,
                                   glob.depthimageview->handle, scg.extent); 

  //
  // setup object data >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  rekz::SetupPolygonData (glob.polyd, kMaxFramesInFlight, data_path, glob.device); 


  // text overlay
  onscreen::SetupData (glob.osdata, glob.device); 
  glob.osdraw = onscreen::CreateDrawText (glob.osdata); 
  //rekz::SetupGridData (glob.gridata, glob.device); 
  // setup object data <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //

  size_t vertoffs = 0;
  size_t indoffs = 0;
  glob.gridbuff = rekz::SetupGridData (vertoffs, indoffs, 11, 11, 20.0, 20.0, glob.device); 
  glob.drawgrid = rekz::CreateDrawGrid (glob.gridbuff, vertoffs, indoffs);
  //
  // GLOBAL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  rokz::SetupGlobalUniforms (glob.global_rc_uniform_bu, kMaxFramesInFlight, glob.device); 
  
  if (!rokz::MakeDescriptorPool(glob.global_uniform_de.pool, kMaxFramesInFlight, rokz::kGlobalDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }

  // 
  if (!rokz::MakeDescriptorSets (glob.global_uniform_de.descrsets, glob.global_uniform_de.alloc_info,
                                 kMaxFramesInFlight,
                                 glob.global_dslo.handle, glob.global_uniform_de.pool, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorSets \n"); 
    return false;
  }
  
  if (!rokz::BindGlobalDescriptorResources (glob.global_uniform_de.descrsets, glob.global_rc_uniform_bu, glob.device)) {
    printf ("[FAILED] --> BindGridDescriptorResources \n"); 
  }


  //
  // object descriptor set 
  rekz::SetupObjectUniforms (glob.poly_objects_bu, kMaxFramesInFlight, glob.device);
  // SetupObjDescriptorPool
  if (!rokz::MakeDescriptorPool(glob.poly_objects_de.pool, kMaxFramesInFlight, rekz::obz::kDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }

  // ?? who owns descriptor sets
  // POLYGONS
  if (!rokz::MakeDescriptorSets (glob.poly_objects_de.descrsets, glob.poly_objects_de.alloc_info, kMaxFramesInFlight,
                                 glob.object_dslo.handle, glob.poly_objects_de.pool, glob.device)) {
    printf ("[FAILED|%i] --> MakeDescriptorSets \n", __LINE__ ); 
    return false;
  }

  // Bind*DescriptorSets is part of a pipeline definition
  if (!rekz::BindObjectDescriptorResources (glob.poly_objects_de.descrsets, glob.poly_objects_bu,
                                            glob.polyd.imageviews, glob.polyd.sampler, glob.object_dslo,
                                            glob.device)) {
    printf ("[FAILED] --> BindObjectDescriptorSets \n"); 
    return false;
  } 

  //
  //
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
  if (!rekz::onscreen::BindDescriptorResources ( glob.osd_de.descrsets, glob.osdata.ubs,
                                                 glob.osdata.texture.view, 
                                                 glob.osdata.texture.sampler,
                                                 glob.osd_dslo, glob.device)) {
    printf ("[FAILED|%i] --> onscreen::BindDescriptorResources \n", __LINE__ ); 
    return false;
  }

  // 
  for (size_t iframe = 0; iframe < kMaxFramesInFlight; ++iframe) { 
    rokz::DrawSequence::DescriptorMap& descrmap = glob.descriptormaps[iframe];
    // ?? 
    descrmap["Global"] = glob.global_uniform_de.descrsets[iframe];
    // ?? is this the best place to store and pass the descr to the drawlist
    // ?? should the drawlist store this directly
    // ?? drawpolygon references directly
    descrmap["osd"]    = glob.osd_de.descrsets[iframe]; // <-- should we do this here?
    
  }

  //
  // create draw list
  glob.drawpoly = rekz::CreatePolygonDraw (glob.polyd, glob.poly_objects_bu, glob.poly_objects_de);
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
    rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], fsg.command_buffer_alloc_info, glob.device.handle);
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
}




