
#include "darkrootgarden.h"
#include "dark_obj_pipeline.h"
#include "rekz/rekz.h"
#include "rokz/buffer.h"
#include "rokz/context.h"
#include "rokz/draw_sequence.h"
#include "rokz/pipeline.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>
// 
#include "dark_obj_pipeline.h"
#include "grid_pipeline.h"
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const size_t kMaxFramesInFlight = darkroot::Glob::MaxFramesInFlight; 

const std::string   data_root =  "/home/djbuzzkill/owenslake/rokz/data"; // 
const VkExtent2D    kTestExtent  = { 800, 600 };
//
//
using namespace darkroot; 
// ---------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
Glob::Glob()
  : instance()
  , device ()
  , depth_image()
  , depth_imageview()
  , msaa_samples ()
  , swapchain_support_info()
  , shared ()
{ 
  // queues.graphics = {};
  // queues.present = {}; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool GenerateMipMaps (rokz::Image& image, bool generate_mipmap, uint32_t num_mipmaps) {
  assert (false); 
  return false; 
}

bool LoadIndexBuffer_static ();
bool LoadVertexBuffer_static();


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
//bool SetupDisplay  (Glob& glob) {
bool SetupDisplay (rokz::Display& display, rekz::InputState& input_state, const VkExtent2D& dim, const rokz::Instance& instance) { 
  
  // create GLFW window
  rokz::CreateWindow (display.window, dim.width, dim.height, "wut"); 
  glfwSetFramebufferSizeCallback (display.window.glfw_window, rekz::win_event::on_resize ); 
  glfwSetKeyCallback (display.window.glfw_window, rekz::win_event::on_keypress);
  glfwSetCursorPosCallback(display.window.glfw_window, rekz::win_event::on_mouse_move);
  glfwSetMouseButtonCallback(display.window.glfw_window, rekz::win_event::on_mouse_button);
  glfwSetCursorEnterCallback (display.window.glfw_window, rekz::win_event::on_mouse_enter); 
                              
  glfwSetWindowUserPointer (display.window.glfw_window, &input_state);

  // create surface
  return  rokz::cx::CreateSurface  (&display.surface, display.window.glfw_window, instance.handle);

}






// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupDynamicRenderingInfo (darkroot::Glob& glob) {

  rokz::RenderingInfoGroup& ri = glob.rendering_info_group;
  ri.clear_colors.resize (1);
  ri.color_attachment_infos.resize (1);

  ri.clear_colors[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  ri.clear_depth.depthStencil = {1.0f, 0};

  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],
                            glob.msaa_color_imageview.handle,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_AVERAGE_BIT,
                            glob.swapchain_group.imageviews[0].handle,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_STORE,
                            ri.clear_colors[0]);

  rokz::cx::AttachmentInfo (ri.depth_attachment_info,
                            glob.depth_imageview.handle,
                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_NONE,
                            nullptr,
                            VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                            ri.clear_depth);
  
  ri.render_area = { VkOffset2D {0, 0}, glob.swapchain_group.swapchain.ci.imageExtent };

  rokz::cx::RenderingInfo (ri.ri, ri.render_area, 1, 0, ri.color_attachment_infos, &ri.depth_attachment_info, nullptr);
  return true;
}

// -----------------------------------------------------------------------------
// basically populates the AttachmentInfo
// -----------------------------------------------------------------------------
void UpdateDynamicRenderingInfo (darkroot::Glob& glob, uint32_t image_index) {
  //printf ("%s\n", __FUNCTION__); 
  rokz::RenderingInfoGroup& ri = glob.rendering_info_group;

  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],
                        glob.msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT, glob.swapchain_group.imageviews[image_index].handle,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
                        VK_ATTACHMENT_STORE_OP_STORE, ri.clear_colors[0]);

}

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
bool SetupRenderingAttachments (Glob& glob) { 

  rokz::SwapchainGroup& scg = glob.swapchain_group;

  //CreateMSAAColorImage -> (image, imageview)
  rekz::CreateMSAAColorTarget (glob.msaa_color_image, glob.msaa_color_imageview, glob.msaa_samples,
                               scg.swapchain.ci.imageFormat, glob.device.allocator.handle, glob.device.command_pool, 
                               glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent, glob.device);

  // CreateDepthBufferImage -> (image, imageview)
  rekz::CreateDepthBufferTarget (glob.depth_image, glob.depth_imageview, glob.msaa_samples, glob.depth_format, 
                                 glob.device.command_pool, glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent,
                                 glob.device.allocator.handle, glob.device);

  return true;

}



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void CleanupDarkroot (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 

  CleanupPolygonData (glob.polyd, glob.device.allocator.handle);


  // create by pipeline, but freed here
  for (auto& ub : glob.global_uniform_bu) {
    rokz::Destroy (ub, glob.device.allocator.handle); 
  }

  {
    assert (false); 
    //rokz::cx::Destroy (glob.objres_dslo, glob.device.handle); 
    //
    // for (auto buf : glob.object_uniform_bu) {  
    //   rokz::Destroy (buf, glob.device.allocator.handle);
    // }
  }

  
  // dont bother freeing if pool is destroyed anyways
  //rokz::cx::Free   (glob.descrgroup_objs.descrsets, glob.descrgroup_objs.pool, glob.device.handle); 
  rokz::cx::Destroy (glob.global_uniform_de.pool, glob.device.handle); 


  rokz::cx::Destroy (glob.global_dslo, glob.device.handle); 

  
  // moved to DrawPolygon
  // rokz::cx::Destroy (glob.texture_image, glob.device.allocator.handle);
  // rokz::cx::Destroy (glob.sampler, glob.device.handle); 
  // rokz::cx::Destroy (glob.texture_imageview, glob.device.handle);

  assert (false); // >>>>>>>>>>>
  // rokz::cx::Destroy (glob.vma_vb_device, glob.device.allocator.handle);
  // rokz::cx::Destroy (glob.vma_ib_device, glob.device.allocator.handle);
  // <<<<<<<<<<
  Cleanup (glob.polys_pl.handle,
           glob.swapchain_group.imageviews,

           glob.swapchain_group.swapchain,
           glob.display.surface,
           glob.device.command_pool.handle,
           glob.framesyncgroup.syncs, 
           glob.polys_pl.shader_modules,
           glob.polys_plo.handle,  
           glob.msaa_color_image, glob.msaa_color_imageview,

           glob.depth_image, glob.depth_imageview,

           glob.display.window.glfw_window,
           glob.device,
           glob.device.allocator.handle, 
           glob.instance.handle);
  //
  glfwTerminate();
}

  

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateGlobals (Glob& glob, uint32_t current_frame, double dt) {

  //
  //  SharedGlobals
  {
    glob.shared.dt             = dt;
    glob.shared.sim_time      += dt;
    glob.shared.current_frame  = current_frame;
    glob.shared.viewport_ext   = glob.swapchain_group.swapchain.ci.imageExtent;
  }    
  
  // 
  { // MVPTransform buffer
    rokz::MVPTransform* mvp =
      reinterpret_cast<rokz::MVPTransform*>(rokz::cx::MappedPointer (glob.global_uniform_bu[current_frame]));
  
    if (mvp) {
    
      glm::mat4 posmat = glm::translate (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
      mvp->model = glm::mat4(1.0); //  posmat; //  glm::rotate(posmat, glob.shared.sim_time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      const float aspf = rekz::ViewAspectRatio (glob.swapchain_group.swapchain.ci.imageExtent.width, glob.swapchain_group.swapchain.ci.imageExtent.height);

      mvp->view = glm::rotate (glm::mat4(1), glob.shared.view_ypr.x, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::translate (glm::mat4(1.0), glob.shared.view_pos); 

      //glm::vec3 (0.0, .5, -5.0));
      // mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      mvp->proj  = glm::perspective(glm::radians(45.0f), aspf , 1.0f, 100.0f);
      mvp->proj[1][1] *= -1;
    }
  }
  
}



// ---------------------------------------------------------------------------------------
//  encapsulate rendering loop
// ---------------------------------------------------------------------------------------
struct RootLoop {

  Glob& glob;

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 600;

  const float Dt; 
  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);

  void UpdateInput () {

    const float move_rate = 0.05f;

    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q)) {
      printf ("--> [q] pressed... quitting \n");
      run = false;
    }

    glob.shared.view_pos.y = -2.05f;

    if (glob.input_state.keys.count (GLFW_KEY_F)) {
      glob.shared.view_pos.z -= move_rate;
    }    
    if (glob.input_state.keys.count (GLFW_KEY_V)) {
      glob.shared.view_pos.z += move_rate;
    }    
    if (glob.input_state.keys.count (GLFW_KEY_D)) {
      glob.shared.view_pos.x -= move_rate;
    }    
    if (glob.input_state.keys.count (GLFW_KEY_G)) {
      glob.shared.view_pos.x += move_rate;
    }    


  } 
  //
  // -------------------------------------------------------------
  RootLoop (Glob& g, float dt ) :  glob(g), Dt (dt) { 

    run        = true;
    curr_frame = 0; 
    result     = false;
    countdown  = 600;
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

    glfwPollEvents(); 

    auto now = std::chrono::high_resolution_clock::now();    
    

    UpdateInput () ;
    
    //
    rokz::SwapchainGroup& scg = glob.swapchain_group; 
    // get image index up here
    uint32_t image_index; 
    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 
    
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_reset_cb->ResetSwapchain (glob.display.window, glob.device.allocator, glob.device);
      printf ("===> %i <=== ]\n", __LINE__);
      return true;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {

      printf  ("---> [%i]\n", __LINE__); 
      rokz::DrawSequence::PipelineAssembly papoly {
        glob.polys_pl, glob.polys_plo.handle, glob.descriptormaps[curr_frame] }; 

      printf  ("---> [%i]\n", __LINE__); 
      rokz::DrawSequence::PipelineAssembly pagrid {
        glob.grid_pl, glob.grid_plo.handle, glob.descriptormaps[curr_frame] }; 

      printf  ("---> [%i]\n", __LINE__); 
  
      //UpdateDarkUniforms (glob, curr_frame, Dt); 
      
      UpdateGlobals (glob, curr_frame, Dt);  

      printf  ("---> [%i]\n", __LINE__); 
      // update data needed to record drawlist

      // make sure the correct swapchain image is used
      UpdateDynamicRenderingInfo (glob, image_index);
      printf  ("---> [%i]\n", __LINE__); 

      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here
      rokz::cx::FrameDrawBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                                image_index, glob.rendering_info_group.ri, glob.device);
      // EXECUTE DRAW LIST RECORDING 

      // for drawseq's
      // const std::vector<VkDescriptorSet> descrsets = {
      //   glob.global_uniform_de.descrsets[curr_frame], glob.objres_uniform_de.descrsets[curr_frame]
      // };
      
      
      glob.drawpoly->Prep (glob.shared, papoly, glob.device); 
      glob.drawpoly->Exec (glob.framesyncgroup.command_buffers[curr_frame],
                           glob.shared, papoly, glob.descriptormaps[curr_frame]);


      glob.drawgrid->Prep (glob.shared, pagrid, glob.device); 
      glob.drawgrid->Exec (glob.framesyncgroup.command_buffers[curr_frame],
                           glob.shared, pagrid, glob.descriptormaps[curr_frame]);

      printf  ("---> [%i]\n]", __LINE__); 

      // we are done, submit
      rokz::cx::FrameDrawEnd (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame], 
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
int darkroot_basin (const std::vector<std::string>& args) {
    
  printf ( " Mv = v is the correct order \n"); 

  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 

  Glob  glob; // *globmem; // something representing the app state
  rokz::SwapchainGroup& scg = glob.swapchain_group;
  
  glob.polyd.obj_theta[0] =   0.0;
  glob.polyd.obj_theta[1] =   0.0;

  glob.prev_x = 0;
  glob.prev_y = 0;

  //std::shared_ptr<Glob> globmem = std::make_shared<Glob> ();

  auto dark_path = std::filesystem::path ( "/home/djbuzzkill/owenslake/rokz/");
  //Default (glob); 
  
  glfwInit();
  glob.input_state.fb_resize = false; 
  
  // CREATE INSTANCE AND DEVICE
  //bool rekz::InitializeInstance (rokz::Instance& instance) {

  rokz::InitializeInstance (glob.instance); 

  //rokz::CreateWindow_glfw (glob.glfwin);

  SetupDisplay (glob.display, glob.input_state, kTestExtent, glob.instance); 
  //SetupDisplay (glob); 

  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info, glob.display.surface, glob.device.physical.handle);

  rokz::ConfigureDevice  (glob.device.physical , VK_TRUE);

  // this does a lot of shit
  rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  
  // put these somwehere
  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rokz::InitializeSwapchain (scg, glob.swapchain_support_info, glob.display.surface,
                             kTestExtent, glob.device.physical, glob.device);
  //assert (false);

  rokz::DefineDescriptorSetLayout (glob.global_dslo, rekz::kGlobalDescriptorBindings, glob.device); 
  rokz::DefineDescriptorSetLayout (glob.object_dslo, rekz::kObjDescriptorBindings, glob.device); 

  // polygon pipeline uses both descriptor sets
  glob.polys_pl.dslos.push_back (glob.global_dslo.handle);
  glob.polys_pl.dslos.push_back (glob.object_dslo.handle);

  if (!rekz::InitObjPipeline ( glob.polys_pl, glob.polys_plo, glob.polys_pl.dslos, dark_path,
                              glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
                              scg.swapchain.ci.imageFormat, glob.depth_format, glob.device)) {
    printf ("[FAILED] --> InitObjPipeline \n"); 
    return false;
  }

  // grid only uses globals
  glob.grid_pl.dslos.push_back (glob.global_dslo.handle);
  if (!rekz::InitGridPipeline (glob.grid_pl,  glob.grid_plo, glob.grid_pl.dslos , dark_path,
                               glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
                               scg.swapchain.ci.imageFormat, glob.depth_format, glob.device)) { 

    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }
                               

  SetupRenderingAttachments (glob); // <-- this does all the additional  attachmentes

  glob.swapchain_reset_cb = CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
                                                     glob.depth_image, glob.depth_imageview,
                                                     glob.msaa_color_image, glob.msaa_color_imageview); 
  //
  // for BeginRendering ()
  SetupDynamicRenderingInfo (glob); 
  //


  //
  // setup object data >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  SetupPolygonData (glob.polyd, kMaxFramesInFlight, data_root, glob.device); 

  // should 
  //
  
  rekz::SetupGridData (glob.gridata, glob.device); 
  // setup object data <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //

  //
  // GLOBAL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  rekz::SetupGlobalUniforms (glob.global_uniform_bu, kMaxFramesInFlight, glob.device); 
  
  printf (" [HERE] --> %i \n", __LINE__); 
  if (!rokz::MakeDescriptorPool(glob.global_uniform_de.pool, kMaxFramesInFlight, rekz::kGlobalDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }
  printf (" [HERE] --> %i \n", __LINE__); 
  // 
  if (!rokz::MakeDescriptorSets (glob.global_uniform_de.descrsets, glob.global_uniform_de.alloc_info,
                                 kMaxFramesInFlight,
                                 glob.global_dslo.handle, glob.global_uniform_de.pool, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorSets \n"); 
    return false;
  }


  rekz::BindGlobalDescriptorResources (glob.global_uniform_de.descrsets, glob.global_uniform_bu, glob.device);
  // Bind*DescriptorSets is part of a pipeline definition
  // if (!rekz::BindGridDescriptorResources (glob.global_uniform_de.descrsets, glob.global_uniform_bu, glob.device)) {
  //   printf ("[FAILED] --> BindGridDescriptorResources \n"); 
  // }
  // // GLOBAL <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
  //
  

  //
  // object descriptor set 
  rekz::SetupObjectUniforms (glob.poly_objects_bu, kMaxFramesInFlight, glob.device);
  // SetupObjDescriptorPool
  //if (!rokz::MakeDescriptorPool(glob.global_uniform_de.pool, kMaxFramesInFlight, rekz::kObjDescriptorBindings, glob.device)) {

  printf (" [HERE] --> %i \n", __LINE__); 
  if (!rokz::MakeDescriptorPool(glob.poly_objects_de.pool, kMaxFramesInFlight, rekz::kObjDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }
  printf (" [HERE] --> %i \n", __LINE__); 
  // ?? who owns descriptor sets
  // POLYGONS
  if (!rokz::MakeDescriptorSets (glob.poly_objects_de.descrsets, glob.poly_objects_de.alloc_info, kMaxFramesInFlight,
                                 glob.object_dslo.handle, glob.poly_objects_de.pool, glob.device)) {
    printf ("[FAILED|%i] --> MakeDescriptorSets \n", __LINE__ ); 
    return false;
  }

  printf (" [HERE] --> %i \n", __LINE__); 
  // Bind*DescriptorSets is part of a pipeline definition

  
  // object params are bound here but we never bound the global
  if (!rekz::BindObjectDescriptorResources (glob.poly_objects_de.descrsets, glob.poly_objects_bu,
                                       glob.polyd.imageview, glob.polyd.sampler, glob.object_dslo,
                                       glob.device)) {
    printf ("[FAILED] --> BindObjectDescriptorSets \n"); 
    return false;
  } 
  printf (" [HERE] --> %i \n", __LINE__); 


  //
  // 
  for (size_t iframe = 0; iframe < kMaxFramesInFlight; ++iframe) { 
    rokz::DrawSequence::DescriptorMap& descrmap = glob.descriptormaps[iframe];
    descrmap["Global"] = glob.global_uniform_de.descrsets[iframe];
  }


  //
  // create draw list
  glob.drawpoly = rekz::CreatePolygonDraw (glob.polyd, glob.poly_objects_bu, glob.poly_objects_de);
  //
  glob.drawgrid = rekz::CreateDrawGrid (glob.gridata); 
  //

  
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
  printf ("===> %i <=== ]\n", __LINE__);
  
  //  globmem.reset ();
  return 0;
}




