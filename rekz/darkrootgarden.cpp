
#include "darkrootgarden.h"
#include "dark_obj_pipeline.h"
#include "rokz/buffer.h"
#include "rokz/context.h"
#include "rokz/pipeline.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>
// 
#include "dark_obj_pipeline.h"
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
// EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT 
// ---------------------------------------------------------------------------------------
template<typename LoopTy>
inline bool RunApplicationLoop (LoopTy& apploop) {

  while (apploop.cond ()) {

    if (!apploop.loop ())  return false; 

  }

  return true;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
Glob::Glob()
  : instance()
  , device ()
  , depth_image()
  , depth_imageview()
  , msaa_samples ()
  , window()
  , swapchain_support_info()
  , surface(nullptr)
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
bool SetupDisplayWindow  (Glob& glob) {
  // glob.window, width, height, 
  rokz::CreateWindow (glob.window, kTestExtent.width , kTestExtent.height, "wut"); 
  glfwSetFramebufferSizeCallback (glob.window.glfw_window, rekz::win_event::on_resize ); 
  glfwSetKeyCallback (glob.window.glfw_window, rekz::win_event::on_keypress);
  glfwSetCursorPosCallback(glob.window.glfw_window, rekz::win_event::on_mouse_move);
  glfwSetMouseButtonCallback(glob.window.glfw_window, rekz::win_event::on_mouse_button);
  glfwSetCursorEnterCallback (glob.window.glfw_window, rekz::win_event::on_mouse_enter); 
                              
  glfwSetWindowUserPointer (glob.window.glfw_window, &glob.input_state);

  printf ("&glob.input_state : %p\n", (void*) &glob.input_state); 

  //glfwSetCursorEnterCallback(window, rokz);
  // typedef struct GLFWcursor GLFWcursor;
  // typedef void (* GLFWerrorfun)(int error_code, const char* description);
  // typedef void (* GLFWwindowposfun)(GLFWwindow* window, int xpos, int ypos);
  // typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);
  // typedef void (* GLFWwindowclosefun)(GLFWwindow* window);
  // typedef void (* GLFWwindowrefreshfun)(GLFWwindow* window);
  // typedef void (* GLFWwindowfocusfun)(GLFWwindow* window, int focused);
  // typedef void (* GLFWwindowiconifyfun)(GLFWwindow* window, int iconified);
  // typedef void (* GLFWwindowmaximizefun)(GLFWwindow* window, int maximized);
  // typedef void (* GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);
  // typedef void (* GLFWwindowcontentscalefun)(GLFWwindow* window, float xscale, float yscale);
  // typedef void (* GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);
  // typedef void (* GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);
  // typedef void (* GLFWcursorenterfun)(GLFWwindow* window, int entered);
  // typedef void (* GLFWscrollfun)(GLFWwindow* window, double xoffset, double yoffset);
  // typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
  // typedef void (* GLFWcharfun)(GLFWwindow* window, unsigned int codepoint);
  // typedef void (* GLFWcharmodsfun)(GLFWwindow* window, unsigned int codepoint, int mods);
  // typedef void (* GLFWdropfun)(GLFWwindow* window, int path_count, const char* paths[]);
  // typedef void (* GLFWmonitorfun)(GLFWmonitor* monitor, int event);
  // typedef void (* GLFWjoystickfun)(int jid, int event);
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupDynamicRenderingInfo (darkroot::Glob& glob) {

  rokz::RenderingInfoGroup& rig = glob.rendering_info_group;
  rig.clear_colors.resize (1);
  rig.color_attachment_infos.resize (1);

  rig.clear_colors[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  rig.clear_depth.depthStencil = {1.0f, 0};

  rokz::cx::AttachmentInfo (rig.color_attachment_infos[0],
                        glob.msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT,
                        glob.swapchain_group.imageviews[0].handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, rig.clear_colors[0]);

  rokz::cx::AttachmentInfo (rig.depth_attachment_info,
                        glob.depth_imageview.handle, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_NONE,
                        nullptr, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, rig.clear_depth);
  
  rig.render_area = { 
    VkOffset2D {0, 0}, glob.swapchain_group.swapchain.ci.imageExtent
  };

  rokz::cx::RenderingInfo (rig.ri, rig.render_area, 1, 0, rig.color_attachment_infos, &rig.depth_attachment_info, nullptr);
  return true;
}

// -----------------------------------------------------------------------------
// basically populates the AttachmentInfo
// -----------------------------------------------------------------------------
void UpdateDynamicRenderingInfo (darkroot::Glob& glob, uint32_t image_index) {
  //printf ("%s\n", __FUNCTION__); 
  rokz::RenderingInfoGroup& rig = glob.rendering_info_group;

  rokz::cx::AttachmentInfo (rig.color_attachment_infos[0],
                        glob.msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT, glob.swapchain_group.imageviews[image_index].handle,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
                        VK_ATTACHMENT_STORE_OP_STORE, rig.clear_colors[0]);

}


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
bool SetupRenderingAttachments (Glob& glob) { 

  rokz::SwapchainGroup& scg = glob.swapchain_group;

  //CreateMSAAColorImage -> (image, imageview)
  rekz::CreateMSAAColorImage  (glob.msaa_color_image, glob.msaa_color_imageview, glob.msaa_samples,
                               scg.swapchain.ci.imageFormat, glob.device.allocator.handle, glob.device.command_pool, 
                               glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent, glob.device);

  // CreateDepthBufferImage -> (image, imageview)
  rekz::CreateDepthBufferImage (glob.depth_image, glob.depth_imageview, glob.msaa_samples, glob.depth_format, 
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
  for (auto& ub : glob.vma_shared_uniforms) {
    rokz::cx::Destroy (ub, glob.device.allocator.handle); 
  }
  
  // dont bother freeing if pool is destroyed anyways
  //rokz::cx::Free   (glob.descrgroup_objs.descrsets, glob.descrgroup_objs.pool, glob.device.handle); 
  rokz::cx::Destroy (glob.polyd.descrgroup.pool, glob.device.handle); 

  rokz::cx::Destroy (glob.polys_dslo, glob.device.handle); 

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
           glob.surface,
           glob.device.command_pool.handle,
           glob.framesyncgroup.syncs, 
           glob.polys_pl.shader_modules,
           glob.polys_plo.handle,  
           glob.msaa_color_image, glob.msaa_color_imageview,

           glob.depth_image, glob.depth_imageview,

           glob.window.glfw_window,
           glob.device,
           glob.device.allocator.handle, 
           glob.instance.handle);
  //
  glfwTerminate();
}

  
// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------
glm::vec3& darkroot::unit_angle_xz (glm::vec3& v, float theta) {
  v.x = cos (theta) ;
  v.y = 0.0f;
  v.z = -sinf (theta) ;
  return v; 
}

glm::vec3& darkroot::unit_angle_yz (glm::vec3& v, float theta) {
  v.x = 0.0;
  v.y = cos (theta) ;
  v.z = -sinf (theta) ;
  return v; 
}
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateGlobals (Glob& glob, uint32_t current_frame, double dt) {

  glob.shared.dt             = dt;
  glob.shared.sim_time      += dt;
  glob.shared.current_frame  = current_frame;
  glob.shared.viewport_ext   = glob.swapchain_group.swapchain.ci.imageExtent;
  
  // update MVPTransofrm buffer
  // 
  //
    
  //float dtF = static_cast <float> (dt);
  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
  // printf ("m0 * v0 = <%f, %f, %f, %f>  \n",  v0.x, v0.y, v0.z, v0.w); 
  // printf ("v1 * m0 = <%f, %f, %f, %f>  \n",  v1.x, v1.y, v1.z, v1.w); 
  // printf ("m[3][0]=%f | m[3][1]=%f | m[3][2]=%f  \n",  m0[3][0], m0[3][1], m0[3][2] ); 
  rokz::MVPTransform mats; 
  mats.model = glm::rotate(posmat, glob.shared.sim_time * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

  mats.view = glm::rotate (glm::mat4(1), glob.polyd.view_orie.theta, glm::vec3(0.0f, 1.0f, 0.0f)) * 
    //glm::rotate (glm::mat4(1), glob.view_orie.phi, glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::translate (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0)); 

  // mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 1.0f, 20.0f);
  mats.proj[1][1] *= -1;

  // MVPTransform
  memcpy (rokz::cx::MappedPointer (glob.polyd.vma_poly_uniforms[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 
  
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateDarkUniforms (Glob& glob, uint32_t current_frame, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.shared.sim_time += dt;
  //  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);
  glob.shared.viewport_ext = glob.swapchain_group.swapchain.ci.imageExtent;
  
  //  float dtF = static_cast <float> (dt);
  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
  // printf ("m0 * v0 = <%f, %f, %f, %f>  \n",  v0.x, v0.y, v0.z, v0.w); 
  // printf ("v1 * m0 = <%f, %f, %f, %f>  \n",  v1.x, v1.y, v1.z, v1.w); 
  // printf ("m[3][0]=%f | m[3][1]=%f | m[3][2]=%f  \n",  m0[3][0], m0[3][1], m0[3][2] ); 
  rokz::MVPTransform mats; 
  mats.model = glm::rotate(posmat, glob.shared.sim_time * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

  mats.view = glm::rotate (glm::mat4(1), glob.polyd.view_orie.theta, glm::vec3(0.0f, 1.0f, 0.0f)) * 
    //glm::rotate (glm::mat4(1), glob.view_orie.phi, glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::translate (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0)); 

  // mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 1.0f, 20.0f);
  mats.proj[1][1] *= -1;

  // MVPTransform
  memcpy (rokz::cx::MappedPointer (glob.vma_shared_uniforms[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 


  // SceneObjParam
  if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rokz::cx::MappedPointer (glob.polyd.vma_poly_uniforms[current_frame]))) {
  
    glm::vec3 va, vb;
    unit_angle_xz (va, 5.0 * glob.shared.sim_time ); 
    unit_angle_xz (vb, 5.0 * glob.shared.sim_time + kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));

    //    glob.polyd.obj_theta[0] += mouse_dx * (float) dt * darkroot::k2Pi;
    
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    obj[0].modelmat = glm::rotate(model0, glob.polyd.obj_theta[0], glm::vec3(0.0f, -1.0f, 0.0f));
    //obj[0].modelmat = glm::rotate(model0, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    obj[1].modelmat = glm::rotate(model1, glob.shared.sim_time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
  }
}


// ---------------------------------------------------------------------------------------
//  encapsulate rendering loop
// ---------------------------------------------------------------------------------------
struct DarkLoop {

  Glob& glob;

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 600;

  const float Dt; 
  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);
  //
  // -------------------------------------------------------------
  DarkLoop (Glob& g, float dt ) :  glob(g), Dt (dt) { 

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
    return countdown && run && !glfwWindowShouldClose(glob.window.glfw_window); 
  }
  //
  // -------------------------------------------------------------
  bool loop () {

    glfwPollEvents(); 

    auto now = std::chrono::high_resolution_clock::now();    
    
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q)) {
      printf ("--> [q] pressed... quitting \n");
      run = false;
    }
    //
    rokz::SwapchainGroup& scg = glob.swapchain_group; 
    // get image index up here
    uint32_t image_index; 
    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 
    
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_reset_cb->ResetSwapchain (glob.window, glob.device.allocator, glob.device);
      printf ("===> %i <=== ]\n", __LINE__);
      return true;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {
      pipeline_assembly pa {
        glob.polys_pl, glob.polys_plo.handle, glob.polyd.descrgroup.descrsets[curr_frame] }; 

      //UpdateDarkUniforms (glob, curr_frame, Dt); 
      UpdateGlobals (glob, curr_frame, Dt);  

      // update data needed to record drawlist
      glob.polydraw->Prep (glob.shared, pa, glob.device); 

      // make sure the correct swapchain image is used
      UpdateDynamicRenderingInfo (glob, image_index);

      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here
      rokz::cx::FrameDrawBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                                image_index, glob.rendering_info_group.ri, glob.device);

      // EXECUTE DRAW LIST RECORDING 

      // for drawseq's
      glob.polydraw->Exec (glob.framesyncgroup.command_buffers[curr_frame], pa, glob.polyd.descrgroup.descrsets[curr_frame]);
      // thats all we are doing for now
      
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
  
  //rokz::CreateWindow_glfw (glob.glfwin);
  SetupDisplayWindow (glob); 

  // CREATE INSTANCE AND DEVICE
  //bool rekz::InitializeInstance (rokz::Instance& instance) {

  rokz::InitializeInstance (glob.instance); 
  rokz::cx::CreateSurface  (&glob.surface, glob.window.glfw_window, glob.instance.handle);
  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info, glob.surface, glob.device.physical.handle);

  rokz::ConfigureDevice  (glob.device.physical , VK_TRUE);

  // this does a lot of shit
  rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  
  // put these somwehere
  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rokz::InitializeSwapchain (scg, glob.swapchain_support_info, glob.surface,
                             kTestExtent, glob.device.physical, glob.device);

  //assert (false);
  if (!InitObjPipeline (glob.polys_pl, glob.polys_plo, glob.polys_dslo, dark_path,
                        glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
                        scg.swapchain.ci.imageFormat, glob.depth_format, glob.device)) {
    return false;
  }

  SetupRenderingAttachments (glob); // <-- this does all the additional  attachmentes

  glob.swapchain_reset_cb = CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
                                                     glob.depth_image, glob.depth_imageview,
                                                     glob.msaa_color_image, glob.msaa_color_imageview); 
  // for BeginRendering ()
  SetupDynamicRenderingInfo (glob); 
  // **
  // SetupObjResources (glob.polygons, glob.device);
  // SetupObjectTextureAndSampler
  //SetupObjResources (glob); <--- replaced by SetupPolygonData
  SetupPolygonData (glob.polyd, kMaxFramesInFlight, data_root, glob.device); 

  // FACT: the global uniforms are separate from the polygon uniforms
  SetupObjectUniforms (glob.vma_shared_uniforms, glob.polyd.vma_poly_uniforms, kMaxFramesInFlight, glob.device);

  // SetupObjDescriptorPool
  if (!rokz::MakeDescriptorPool(glob.polyd.descrgroup.pool, kMaxFramesInFlight, kObjDescriptorBindings, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }

  //
  if (!rokz::MakeDescriptorSets (glob.polyd.descrgroup.descrsets, glob.polyd.descrgroup.alloc_info, kMaxFramesInFlight,
                                 glob.polys_dslo.handle, glob.polyd.descrgroup.pool, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorSets \n"); 
    return false;
  }

  // Bind*DescriptorSets is part of a pipeline definition
  if (!BindObjectDescriptorSets (glob.polyd.descrgroup.descrsets, glob.vma_shared_uniforms,
                                 glob.polyd.vma_poly_uniforms, glob.polyd.imageview, glob.polyd.sampler,
                                 glob.polys_dslo, glob.device)) {
    printf ("[FAILED] --> BindObjectDescriptorSets \n"); 
    return false;
  }

  //glob.polygons = std::make_shared<DrawSequence> (69);
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

  // create draw list
  glob.polydraw = CreatePolygonDraw (glob.polyd);

  DarkLoop darkloop (glob, Dt ); 

  RunApplicationLoop (darkloop);

  vkDeviceWaitIdle(glob.device.handle);

  // end loop
  //ShutdownDarkroot ();

  // CLEAN UP
  CleanupDarkroot (glob); 
  printf ("===> %i <=== ]\n", __LINE__);
  
  //  globmem.reset ();
  return 0;
}


