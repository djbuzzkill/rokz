
#include "marscape.h"
#include "rekz/mars_types.h"
// 
#include "grid_pipeline.h"
#include "rekz/rekz.h"
#include "rokz/allocation.h"
#include "rokz/buffer.h"
#include <vulkan/vulkan_core.h>


//#define ROKZ_USE_VMA_ALLOCATION 1
// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------
glm::vec3& mars::unit_angle_xz (glm::vec3& v, float theta) {
  v.x = cos (theta) ;
  v.y = 0.0f;
  v.z = -sinf (theta) ;
  return v; 
}


using namespace mars;
// --------------------------------------------------------------------


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupMarsWindow (rokz::Window& window, void* user_pointer) {

  glfwInit();
  rokz::CreateWindow (window, kWinExtent.width , kWinExtent.height, "wut"); 

  glfwSetFramebufferSizeCallback (window.glfw_window, rekz::win_event::on_resize ); 
  glfwSetKeyCallback (window.glfw_window, rekz::win_event::on_keypress);
  glfwSetCursorPosCallback(window.glfw_window, rekz::win_event::on_mouse_move);
  glfwSetMouseButtonCallback(window.glfw_window, rekz::win_event::on_mouse_button);
  glfwSetCursorEnterCallback (window.glfw_window, rekz::win_event::on_mouse_enter); 

  glfwSetWindowUserPointer (window.glfw_window, user_pointer);

  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupGridUniforms (Glob& glob) {
  printf ("%s", __FUNCTION__);

  VkDevice const&            device        = glob.device.handle;
  VkPhysicalDevice const&    physdev       = glob.physical_device.handle;
  std::vector<rokz::Buffer>& uniform_buffs = glob.uniform_mvp;
  
  uniform_buffs.resize (kMaxFramesInFlight);
  for (size_t i = 0; i <  kMaxFramesInFlight; i++) {

    rokz::cx::CreateInfo_uniform (uniform_buffs[i].ci, rokz::kSizeOf_MVPTransform, 1); 
    rokz::cx::AllocCreateInfo_mapped (uniform_buffs[i].alloc_ci); 
    if (!rokz::cx::CreateBuffer (uniform_buffs[i], glob.device.allocator.handle)) {
      printf (" --> [FAIL]  create MVPTransform \n"); 
      return false; 
    }

  }

  printf (" --> [true] \n"); 
  return true; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupGridResources (Glob& glob) {

  float     kGridDimSize = 100.0f;
  uint32_t  kGridTiles   = 10; 

  float grid_offset = kGridDimSize / 2; 
  float grid_step_size = kGridDimSize / kGridTiles; 
  
  uint32_t grid_vert_dim  = kGridTiles + 1;                  
  uint32_t num_grid_verts = grid_vert_dim * grid_vert_dim;

  // VERTEX BUFFER
  {
    rokz::Buffer vb_stage;
    rokz::cx::CreateInfo_VB_stage (vb_stage.ci,  sizeof(rekz::GridVert), num_grid_verts) ;
    rokz::cx::AllocCreateInfo_stage (vb_stage.alloc_ci);
    rokz::cx::CreateBuffer (vb_stage, glob.device.allocator.handle);
    if (void* mappedp = rokz::cx::MappedPointer (vb_stage)) {
      rekz::GridVert* vertp = reinterpret_cast<rekz::GridVert*> (mappedp);
      
      for (uint32_t iz = 0; iz < grid_vert_dim; ++iz) {
        for (uint32_t ix = 0; ix < grid_vert_dim; ++ix) {

          vertp[grid_vert_dim * iz + ix].pos.x =  (grid_step_size * ix) - grid_offset;
          vertp[grid_vert_dim * iz + ix].pos.y =  0.0f;
          vertp[grid_vert_dim * iz + ix].pos.z =  (grid_step_size * iz) - grid_offset;

          vertp[grid_vert_dim * iz + ix].col = glm::vec3 (0.2f, 0.2f, 0.9f); 
        }
      }
        
      rokz::cx::UnmapMemory (vb_stage.allocation, glob.device.allocator.handle); 
    }
    //
    rokz::cx::CreateInfo_VB_device (glob.vma_vb_device.ci, sizeof(rekz::GridVert), num_grid_verts);
    rokz::cx::AllocCreateInfo_device (glob.vma_vb_device.alloc_ci); 
    rokz::cx::CreateBuffer (glob.vma_vb_device, glob.device.allocator.handle); 
    //rokz::Transfer_2_Device;
    rokz::cx::MoveToBuffer_XB2DB (glob.vma_vb_device, vb_stage, sizeof(rekz::GridVert) * num_grid_verts, 
                                  glob.device.command_pool.handle, glob.device.queues.graphics, glob.device.handle); 

    rokz::Destroy (vb_stage, glob.device.allocator.handle); 
  }
  
  
  uint32_t num_grid_inds = (2 * grid_vert_dim) + (2 * grid_vert_dim); 
  // INDEX BUFFER
  {
    rokz::Buffer ib_stage;
    rokz::cx::CreateInfo_IB_16_stage (ib_stage.ci, num_grid_inds); 
    rokz::cx::AllocCreateInfo_stage (ib_stage.alloc_ci);
    rokz::cx::CreateBuffer (ib_stage, glob.device.allocator.handle);
    if (void* mappedp = rokz::cx::MappedPointer (ib_stage)) {
      uint16_t* indexp = reinterpret_cast<uint16_t*> (mappedp); 
      // Z goes x-row 
      // |  |  0  1  2  3  4  5  6  7  8  9 10
      // v  | 11 12 13 14 15 16 17 18 19 20 21
      for (uint32_t iz = 0; iz < grid_vert_dim; ++iz) {
        *indexp++ = iz * grid_vert_dim;
        *indexp++ = (iz * grid_vert_dim) + kGridTiles;
      }

      // X--> x goes z-col
      //  0   1   2
      //  |   |   |
      //  V   v   V
      // 110 111 112
      for (uint32_t ix = 0; ix < grid_vert_dim; ++ix) {
        *indexp++ = ix;
        *indexp++ = kGridTiles * grid_vert_dim + ix;
      }

      rokz::cx::UnmapMemory (ib_stage.allocation, glob.device.allocator.handle); 
    
    }  

    rokz::cx::CreateInfo_IB_16_device (glob.vma_ib_device.ci, num_grid_inds); 
    rokz::cx::AllocCreateInfo_device (glob.vma_ib_device.alloc_ci);
    rokz::cx::CreateBuffer (glob.vma_ib_device, glob.device.allocator.handle);
    // upload 
    rokz::cx::MoveToBuffer_XB2DB (glob.vma_ib_device, ib_stage, sizeof (uint16_t) * num_grid_inds, 
                                  glob.device.command_pool.handle, glob.device.queues.graphics, glob.device.handle); 
    
    rokz::Destroy (ib_stage, glob.device.allocator.handle); 
  }

  if (!SetupGridUniforms (glob)) {
    printf ("[FAILED] --> SetupObjectUniforms \n"); 
    return false;
  }

  return false; 
    
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateGridUniforms (Glob& glob, uint32_t current_frame, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.sim_time += dt;
  //  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);

  float sim_timef = glob.sim_time;
  float dtF = static_cast <float> (dt);
  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  //glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));

  rokz::MVPTransform mats;  
  mats.model = glm::mat4(1.0); // <-- always at origin
  mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 1.0f, 20.0f);
  mats.proj[1][1] *= -1;

  memcpy (rokz::cx::MappedPointer (glob.uniform_mvp[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupDynamicRenderingInfo (mars::Glob& glob) {

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
                        glob.msaa_depth_imageview.handle, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_NONE,
                        nullptr, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, rig.clear_depth);
  
  rig.render_area = { 
    VkOffset2D {0, 0}, glob.swapchain_group.swapchain.ci.imageExtent
  };

  rokz::cx::RenderingInfo (rig.ri, rig.render_area, 1, 0, rig.color_attachment_infos, &rig.depth_attachment_info, nullptr);
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupRenderAttachments (Glob& glob) {

  rokz::SwapchainGroup& scg = glob.swapchain_group;
  
  if (!rekz::CreateDepthBufferTarget (glob.msaa_depth_image,    
                                      glob.msaa_depth_imageview,
                                      glob.msaa_samples,
                                      glob.depth_format,
                                      glob.device.command_pool,                                      
                                      glob.device.queues.graphics, 
                                      kWinExtent,
                                      glob.device.allocator.handle, 
                                      glob.device))
    {
      // depth buffer attachment failed
      return false; 
    }

  if (!rekz::CreateMSAAColorTarget (glob.msaa_color_image, 
                                    glob.msaa_color_imageview, 
                                    glob.msaa_samples,
                                    scg.swapchain.ci.imageFormat,
                                    glob.device.allocator.handle, 
                                    glob.device.command_pool,                                      
                                    glob.device.queues.graphics, 
                                    kWinExtent,
                                    glob.device)) {
    // msaa color target failed 
    return false; 
  }

  return true; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void CleanupMars (Glob& glob) {

  
  printf ("%s \n", __FUNCTION__); 

  for (auto& ub : glob.uniform_mvp) {
    rokz::Destroy (ub, glob.allocator); 
  }

  assert (false); // rokz::cx::Destroy (glob.terrain_pipeline.descrgroup
  //rokz::cx::Destroy (glob.terrain_pipeline.descrgroup, glob.device.handle); 

  rokz::cx::Destroy (glob.descriptor_pool, glob.device.handle); 

  assert (false); // rokz::cx::Destroy (glob.grid_pipeline.descrgroup, 
  //rokz::cx::Destroy (glob.grid_pipeline.descrgroup, glob.device.handle); 

  rokz::Destroy (glob.vma_vb_device, glob.allocator);
  rokz::Destroy (glob.vma_ib_device, glob.allocator);
  
  assert (false); // Cleanup
  // Cleanup (glob.terrain_pipeline.pipeline.handle,
  //          glob.swapchain_group.framebuffers, glob.swapchain_group.imageviews,

  //          glob.swapchain_group.swapchain,
  //          glob.surface,
  //          glob.command_pool.handle,
  //          glob.framesyncgroup.syncs, 
  //          glob.terrain_pipeline.pipeline.shader_modules,
  //          glob.terrain_pipeline.pipeline.layout.handle, 
  //          glob.render_pass,

  //          glob.msaa_color_image, glob.msaa_color_imageview,

  //          glob.msaa_depth_image, glob.msaa_depth_imageview,

  //          glob.window.glfw_window,
  //          glob.device,
  //          glob.allocator, 
  //          glob.instance.handle);

  glfwTerminate();

}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupMarsTexturesAndImageViews (Glob& glob) {

  // SetupDarkTexture (glob); 
  // SetupDarkTextureImageView (glob); 

  "height textures";

  "normal textures";

  "color textures"; 
    
  return false;
}




// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupGlobalDescriptorPool (Glob& glob) {

  assert (false);
  return false;
}


bool SetupTerrainDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

  assert (false);
  return false;
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupMarsUniforms (mars::Glob& glob) {
  printf ("%s", __FUNCTION__);

  VkDevice const&          device = glob.device.handle;
  VkPhysicalDevice const&  physdev = glob.physical_device.handle;

  std::vector<rokz::Buffer>& uniform_mvp_transform = glob.uniform_mvp;
  //std::vector<rokz::Buffer>& objparams   = glob.vma_objparam_buffs;
  
  uniform_mvp_transform.resize (kMaxFramesInFlight);
  
  for (size_t i = 0; i <  kMaxFramesInFlight; i++) {
    rokz::cx::CreateInfo_uniform     (uniform_mvp_transform[i].ci, rokz::kSizeOf_MVPTransform, 1); 
    rokz::cx::AllocCreateInfo_mapped (uniform_mvp_transform[i].alloc_ci); 

    if (!rokz::cx::CreateBuffer (uniform_mvp_transform[i], glob.allocator)) {
      printf (" --> [FAIL]  create MVPTransform \n"); 
      return false; 
    }
  }

  printf (" --> [true] \n"); 
  return true; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateMarsUniforms (mars::Glob& glob, uint32_t current_frame, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.sim_time += dt;
  //  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);

  float sim_timef = glob.sim_time;

  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
  // printf ("m0 * v0 = <%f, %f, %f, %f>  \n",  v0.x, v0.y, v0.z, v0.w); 
  // printf ("v1 * m0 = <%f, %f, %f, %f>  \n",  v1.x, v1.y, v1.z, v1.w); 
  // printf ("m[3][0]=%f | m[3][1]=%f | m[3][2]=%f  \n",  m0[3][0], m0[3][1], m0[3][2] ); 
  rokz::MVPTransform mats; 
  mats.model = glm::rotate(posmat, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
  mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 1.0f, 20.0f);
  mats.proj[1][1] *= -1;

  memcpy (rokz::cx::MappedPointer (glob.uniform_mvp[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 
 
}




// ---------------------------------------------------------------------
// RecordDarkCommandBuffer_indexed
// ---------------------------------------------------------------------
bool RecordDynamicRender (Glob& glob, 
                          VkCommandBuffer        &command_buffer,
                          const rokz::Pipeline&        pipeline,
                          const VkDescriptorSet& desc_set, 
                          const VkBuffer&        vertex_buffer, 
                          const VkBuffer&        index_buffer, 
                          const VkExtent2D&      ext2d,
                          const VkFramebuffer&   framebuffer,
                          const rokz::RenderPass&      render_pass,
                          const VkDevice&        device) {

  return false;
}
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool RenderFrame (mars::Glob&           glob,
                      uint32_t&               image_index,
                      bool&                   resize,
                      rokz::RenderPass&       renderpass, 
                      const rokz::Pipeline&   pipeline,
                      const VkDescriptorSet&  descr_set, 
                      uint32_t                curr_frame,
                      double dt) {


  return false;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
mars::Glob& InitMarsGlob (mars::Glob& glob) {

  glob.dt       = 0.0;
  glob.sim_time = 0.0;
  glob.input_state.fb_resize = false; 

  return glob;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int mars_run (const std::vector<std::string>& args) {
  printf ("%s\n", __FUNCTION__);

  Glob glob; //
  InitMarsGlob (glob);
  rokz::SwapchainGroup&  scg   = glob.swapchain_group;
  rokz::FrameSyncGroup&  fsg   = glob.framesyncgroup; 
  
  auto mars_path = std::filesystem::path ("/home/djbuzzkill/owenslake/rokz/");
  


  SetupMarsWindow (glob.window, &glob.input_state); 


  rokz::InitializeInstance (glob.instance); 
  rokz::cx::CreateSurface  (&glob.surface, glob.window.glfw_window, glob.instance.handle);
  rokz::cx::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance.handle);

  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info, glob.surface, glob.physical_device.handle);

  rokz::ConfigureDevice ( glob.physical_device , VK_TRUE); 
  rokz::InitializeDevice (glob.device,glob.physical_device, glob.instance);

  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.physical_device); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.physical_device.handle);
  
  //
  rokz::InitializeSwapchain (scg, glob.swapchain_support_info, glob.surface,
                             kWinExtent, glob.physical_device, glob.device);


#ifdef MARSCAPE_CALL_SETUP_GRID_PIPELINE_
  
  rekz::SetupGridPipeline (glob.grid_pipeline,
                           mars_path,
                           kWinExtent, //const rokz::Swapchain& swapchain,
                           glob.msaa_samples, 
                           glob.swapchain_group.swapchain.ci.imageFormat,
                           glob.depth_format,
                           sizeof(PushConstants), 
                           glob.device);
#endif

  // SetupRenderAttachments
  if (!SetupRenderAttachments (glob)) {
    printf ("[FAILED|%i] -> SetupRenderAttachments \n", __LINE__);
    return false; 
  }
      
  if (! SetupGridResources (glob) ) {
    return false; 
  }
  
#ifdef MARS_ENABLE_TERRAIN_PIPELINE  


  SetupTerrainPipeline (glob.terrain_pipeline,
                        glob.terrain_pipeline.pipeline.state.viewport,
                        glob.render_pass,
                        mars::data_root,
                        glob.swapchain_group.swapchain,
                        glob.msaa_samples,
                        glob.device); 
  //SetupTerrainPipeline (glob.terrain_pipeline, glob.viewport_state, glob.render_pass, dark_path, glob.swapchain_group.swapchain);

  SetupTerrainResources (glob); 
  "Setup Index+Vertex Buffers";

  SetupTerrainSamplers (glob);

  
#endif //  MARS_ENABLE_TERRAIN_PIPELINE

  

  // for BeginRendering ()
  SetupDynamicRenderingInfo (glob) ; 
  

  

  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);
  if (!SetupMarsUniforms (glob)) {
    printf ("[FAILED] --> SetupDarkUniforms \n"); 
    return false;
  }


  if (!SetupMarsTexturesAndImageViews (glob)) {
    printf ("[FAILED] --> SetupMarsTexturesAndImageViews \n"); 
    return false;
  }
  
  
  if (!SetupGlobalDescriptorPool (glob)) {
    printf ("[FAILED] --> SetupGlobalDescriptorPool \n"); 
    return false;
  }


#ifdef MARS_TERRAIN_ENABLE 
  if (!SetupTerrainDescriptorSets (glob.obj_pipeline,
                                   glob.uniform_mvp,
                                   glob.vma_objparam_buffs,
                                   glob.texture_imageview,
                                   glob.sampler,
                                   glob.descr_pool, 
                                   glob.device)) {
    printf ("[FAILED] --> SetupTerrainDescriptorSets \n"); 
    return false;
  }
#endif
  
#ifdef MARSCAPE_CALL_SETUP_GRID_DESCRIPTORSETS  
  if (!rekz::SetupGridDescriptorSets (glob.descrgroup_grid, glob.uniform_mvp,
                                      glob.pipeline_def_grid.layout.descriptor, glob.device)) {
    printf ("[FAILED] --> SetupTerrainDescriptorSets \n"); 
    return false;
  }
#endif
  
  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);

  //swapchain_group.command_buffer_group.buffers.resize (kMaxFramesInFlight);

  fsg.syncs.resize (kMaxFramesInFlight);
  fsg.command_buffers.resize (kMaxFramesInFlight);
  
  rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.command_pool.handle); 

  // 
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    // sep
    rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], 
                              fsg.command_buffer_alloc_info,
                              glob.device.handle);

    rokz::cx::CreateFrameSync (fsg.syncs[i], fsg.syncs[i].ci, glob.device.handle);
  }

  // SetupDarkroot ();

  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);
  const double time_per_frame_sec = 1.0 / 60.0;
  glob.dt = time_per_frame_sec; // just do this for now
  
  std::chrono::microseconds time_per_frame_us(static_cast<size_t>(time_per_frame_sec * 1000000.0));
  
  std::chrono::duration<size_t, std::chrono::microseconds::period>
    time_per_frame(time_per_frame_us);

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 60;
  
  //printf ( "\nBegin run for [%i] frames.. \n\n", countdown); 
  //
  auto t0 = std::chrono::high_resolution_clock::now(); 
  auto then = t0; 

  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);
  while (countdown && run && !glfwWindowShouldClose(glob.window.glfw_window)) {

    glfwPollEvents(); 
    //start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();    
    //dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds>(then - now).count (); 
    glob.dt = std::chrono::duration<double, std::chrono::seconds::period>(now - then).count();
    
    //    result = RenderFrame (glob, curr_frame, fb_resize, glob.dt);
    uint32_t image_index; 

assert (false); // RenderFrame
    // if (RenderFrame (glob, image_index, glob.fb_resize, glob.render_pass, glob.grid_pipeline.pipeline,
    //                  glob.grid_pipeline.descrgroup.descrsets[curr_frame], curr_frame, glob.dt)) {

    // }
    // else {
    //   run = false;
    // }
    
    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }

    curr_frame = (curr_frame + 1) % kMaxFramesInFlight;
    then = now; // std::chrono::high_resolution_clock::now(); 
    countdown--; 
  }

  
  vkDeviceWaitIdle(glob.device.handle);
  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);


  // CLEAN UP
  CleanupMars (glob); 

  printf ("%[LEAVING] --> %s\n", __FUNCTION__);
  return 0; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------


int mars_prelim (const std::vector<std::string>& args) {
  printf ("%s\n", __FUNCTION__);
  // const std::string colt = tile_root + "/rando_col_" + index_2_str(iY, iX) + "_3uc.col"; 
  // const std::string hgtt = tile_root + "/rando_hgt_" + index_2_str(iY, iX) + "_1f.hgt"; 
  // const std::string nrmt = tile_root + "/rando_nrm_" + index_2_str(iY, iX) + "_3f.nrm"; 
  std::string tile_root = "/home/djbuzzkill/owenslake/data/awsum/rando/tile/";
  
  std::string color_tile =  "rando_col_1_1_3uc.col";
  std::string height_tile = "rando_hgt_3_1_1f.hgt";
  std::string normal_tile = "rando_nrm_5_1_3f.nrm"; 

  const int col_dim = 128;
  const int hgt_dim = 256;
  const int nrm_dim = 256;

  const int num_col_pxls = col_dim * col_dim;
  const int num_hgt_pxls = hgt_dim * hgt_dim;
  const int num_nrm_pxls = nrm_dim * nrm_dim;

  uint32_t mars_terrain_num_X_tiles      = 10;
  uint32_t mars_terrain_num_Y_tiles      = 10;

  float mars_terrain_easting_Tile_step   = 256.0;
  float mars_terrain_northing_Tile_step  = 256.0;
  float mars_tearrain_height_offset      = 0.0;
  float mars_terrain_height_range        = 100.0;

  // std::vector<RGBu8> colors (col_dim * col_dim, RGBu8{});
  // rokz::ReadStreamRef col_rs = rokz::CreateReadFileStream  (tile_root + color_tile); 
  // col_rs->Read (&colors[0], col_dim * col_dim * sizeof (RGBu8)); 
  // for (int iy = 0; iy < col_dim; ++iy) {
  //   for (int ix = 0; ix < col_dim; ++ix) {
  //     int ind = iy * col_dim + ix; 
  //     printf ( "col[%i] (%u, %u, %u)\n", ind, colors[ind].r, colors[ind].g, colors[ind].b); 
  //   }
  // }
                                        
  std::vector<float> heights (hgt_dim * hgt_dim, 0.0); 
  if ( rokz::ReadStreamRef hgt_rs = rokz::CreateReadFileStream  (tile_root + height_tile)) {  

  hgt_rs->Read (&heights[0], hgt_dim * hgt_dim * sizeof(float));
  for (int iy = 0; iy < hgt_dim; ++iy) {
    for (int ix = 0; ix < hgt_dim; ++ix) {
      int ind = iy * hgt_dim + ix; 
      printf ( "heights[%i] --> %f\n", ind, heights[ind] ); 
    }
  }
  printf ( "max height %f\n ", *std::max_element (heights.begin (), heights.end ()));
  printf ( "min height %f\n ", *std::min_element (heights.begin (), heights.end ()));
  }
  
  // std::vector<glm::vec3> normals (nrm_dim * nrm_dim, glm::vec3{});
  // rokz::ReadStreamRef nrm_rs = rokz::CreateReadFileStream  (tile_root + normal_tile);
  // nrm_rs->Read (&normals[0], nrm_dim * nrm_dim * sizeof(glm::vec3)); 
  // for (int iy = 0; iy < nrm_dim; ++iy) {
  //   for (int ix = 0; ix < nrm_dim; ++ix) {

  //     int ind = iy * nrm_dim + ix; 
  //     printf ( "normal[%i] --> <%f, %f, %f> \n", ind, normals[ind].x, normals[ind].y, normals[ind].z);
  //   }
  // }


  return 0; 
}









#ifdef MARS_TERRAIN_ENABLE 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  const VkVertexInputBindingDescription kVertexBindingDesc =  {
    0,                            // binding    
    sizeof (MarsVert),       // stride      
    VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   
  }; 

  const std::vector<VkVertexInputAttributeDescription> kVertexInputAttributeDesc = {

    VkVertexInputAttributeDescription { // pos
      0,                             // .location 
      0,                             // .binding  
      VK_FORMAT_R32G32B32_SFLOAT,    // .format   
      offsetof(MarsVert, pos),  // .offset   
    },
    VkVertexInputAttributeDescription { // color
      1,                              
      0, 
      VK_FORMAT_R32G32B32_SFLOAT,
      offsetof(MarsVert, nrm), 
    },

    VkVertexInputAttributeDescription { // color
      2,                              
      0, 
      VK_FORMAT_R32G32B32_SFLOAT,
      offsetof(MarsVert, col), 
    },
    VkVertexInputAttributeDescription { // tex coord
      3,                             
      0, 
      VK_FORMAT_R32G32_SFLOAT,
      offsetof(MarsVert, txc0), 
    }
  }; 

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupTerrainPipeline (rekz::PipelineGroup& pipelinegroup,
                          const rokz::ViewportState& vps,
                          const rokz::RenderPass& renderpass,
                          const std::filesystem::path& fspath,
                          const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          const rokz::Device& device) {

  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);

  //rokz::FrameGroup& swapchain_group = glob.swapchain_group;
  if (!SetupTerrainShaderModules (pipelinegroup.pipeline, fspath, device)) {
    printf ("[FAILED] --> SetupTesselationShaderModules \n");
    return false;
  }

  pipelinegroup.pipeline.state.colorblend_attachments.resize (1);
  rokz::ColorBlendState_default (pipelinegroup.pipeline.state.colorblend_attachments[0]); 
  rokz::DynamicState_default    (pipelinegroup.pipeline.state.dynamics); 

  rokz::PipelineStateCreateInfo& psci = pipelinegroup.pipeline.state.ci;
  rokz::CreateInfo (psci.tesselation, 4); // <-- is 4 points right? 
  rokz::CreateInfo (psci.dynamicstate, pipelinegroup.pipeline.state.dynamics); 
  rokz::CreateInfo (psci.dynamicstate, pipelinegroup.pipeline.state.dynamics); 
  rokz::CreateInfo (psci.vertexinputstate, kVertexBindingDesc, mars::kVertexInputAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, vps);
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_PATCH_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, pipelinegroup.pipeline.state.colorblend_attachments); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 

  SetupTerrainDescriptorLayout (pipelinegroup.descrgroup, device); 

  //
  rokz::CreateGraphicsPipelineLayout (pipelinegroup.pipeline.layout.handle,
                                      pipelinegroup.pipeline.layout.ci,
                                      sizeof(mars::PushConstants), 
                                      pipelinegroup.descrgroup.dslayout.handle,
                                      device.handle);


  //
  rokz::CreateInfo (pipelinegroup.pipeline.ci,
                    pipelinegroup.pipeline.layout.handle,
                    
                    renderpass.handle,                    
                    psci.shader_stages,       //const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
                    &psci.input_assembly,     //const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                    &psci.vertexinputstate, // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                    &psci.viewport_state,     //const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                    &psci.tesselation,                 // tesselation 
                    &psci.rasterizer,         //const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
                    &psci.multisampling,      //const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
                    &psci.depthstencilstate,       //const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
                    &psci.colorblendstate,         //const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
                    &psci.dynamicstate);     // const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 

  if (!rokz::CreateGraphicsPipeline (pipelinegroup.pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }

  return true;

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupTerrainShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {

  printf ("%s \n", __FUNCTION__); 
  
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  std::vector<rokz::ShaderModule>&              shader_modules            = pipeline.shader_modules;
  // 1 VERTEX SHADER
  // 2 TESS CONTROLLER
  // 3 TESS EVALUATOR
  // 4 FRAGMENT SHADER 
  shader_modules.resize  (4);
  shader_stage_create_infos.resize(4);
  //
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"mars/pipeline/terrain/shader/mars_terrain_vert.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[0], vert_file_path.string(), device.handle)) {
    printf ("[FAILED] -->  \n", vert_file_path.string().c_str()); 
    return false; }
  rokz::CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].handle); 

  //
  // TESS CONTROL
  std::filesystem::path tesco_file_path  = fspath/"mars/pipeline/terrain/shader/mars_terrain_tesc.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[1], tesco_file_path.string(), device.handle)) {
    printf ("[FAILED] -->  \n", tesco_file_path.string().c_str()); 
    return false; }
  rokz::CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, shader_modules[1].handle); 

  //
  // TESSS EVAL
  std::filesystem::path tesev_file_path  = fspath/"mars/pipeline/terrain/shader/mars_terrain_tese.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[2], tesev_file_path.string(), device.handle)) {
    printf ("[FAILED] -->  \n", tesev_file_path.string().c_str()); 
    return false;
  }
  rokz::CreateInfo (shader_stage_create_infos[2], VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, shader_modules[2].handle); 

  //
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"mars/pipeline/terrain/shader/mars_terrain_frag.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[3], frag_file_path.string(), device.handle)) {
    printf ("[FAILED] -->  \n", frag_file_path.string().c_str()); 
    return false; }
  rokz::CreateInfo (shader_stage_create_infos[3], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[3].handle); 
  //
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupTerrainDescriptorSets (rekz::PipelineGroup& pipelinegroup,

                                const std::vector<rokz::Buffer>& uniform_mvp,
                                const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                const rokz::ImageView& texture_imageview, 
                                const rokz::Sampler&   sampler, 

                                const rokz::DescriptorPool& descpool,
                                const rokz::Device&         device)
{

  // printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

  rokz::DescriptorGroup& dg = pipelinegroup.descrgroup;
 
  // use same layout for both allocations
  std::vector<VkDescriptorSetLayout> descrlos (kMaxFramesInFlight, dg.dslayout.handle);
  // could have also said: 
  //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
  // but that wouldnt work
  rokz::cx::AllocateInfo (dg.alloc_info , descrlos, descpool);
  
  if (!rokz::cx::AllocateDescriptorSets (dg.descrsets, kMaxFramesInFlight, dg.alloc_info, device.handle)) {
    printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
    return false;
  }
  //

  struct notype {}; 
  
  for (uint32_t flight = 0; flight < kMaxFramesInFlight; flight++) {

    // MVPTransform
    VkDescriptorBufferInfo mvp_info{};
    mvp_info.buffer     = 0; // uniform_mvp[i].handle;
    mvp_info.offset     = 0;
    mvp_info.range      = sizeof (rokz::MVPTransform); 

    // mar::ViewParams
    VkDescriptorBufferInfo view_info{};
    view_info.buffer     = 0; // uniform_mvp[i].handle;
    view_info.offset     = 0;
    view_info.range      = sizeof (mars::ViewParams); 
    //  mars::PatchParams
    std::vector<VkDescriptorBufferInfo> patch_params  (kPatchCount, VkDescriptorBufferInfo {});
    for (size_t ipatch = 0; ipatch < patch_params.size (); ++ipatch) { 
      patch_params[ipatch].buffer   = vma_objparam_buffs[flight].handle; //
      patch_params[ipatch].offset   = ipatch * sizeof(mars::PatchParams) ;         // min_uniform_buffer_offset_alignment ??
      patch_params[ipatch].range    = sizeof(mars::PatchParams) ;            //glob.vma_objparam_buffs[i].ci.size;
    }
    
    // height maps
    std::vector<VkDescriptorImageInfo> hgt_map_infos  (kPatchCount, VkDescriptorImageInfo {});
    for (size_t ipatch = 0; ipatch < kPatchCount; ++ipatch) { 
      hgt_map_infos[ipatch].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
      hgt_map_infos[ipatch].imageView   = texture_imageview.handle;
      hgt_map_infos[ipatch].sampler     = sampler.handle;
    }

    //  normal maps
    std::vector<VkDescriptorImageInfo>  normal_map_infos (kPatchCount, VkDescriptorImageInfo {}); 
    for (size_t ipatch = 0; ipatch < kPatchCount; ++ipatch ) { 
      normal_map_infos[ipatch].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
      normal_map_infos[ipatch].imageView   = texture_imageview.handle;
      normal_map_infos[ipatch].sampler     = sampler.handle;
    }
    
    // color maps
    std::vector<VkDescriptorImageInfo>  color_map_infos (kPatchCount, VkDescriptorImageInfo {}); 
    for (size_t ipatch = 0; ipatch < kPatchCount; ++ipatch) {  
      color_map_infos[ipatch].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
      color_map_infos[ipatch].imageView   = texture_imageview.handle;
      color_map_infos[ipatch].sampler     = sampler.handle;

    }

    // MVP
    std::array<VkWriteDescriptorSet, 6>  descr_writes {};
    descr_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descr_writes[0].pNext            = nullptr;    
    descr_writes[0].dstSet           = dg.descrsets[flight];
    descr_writes[0].dstBinding       = 0;
    descr_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descr_writes[0].dstArrayElement  = 0;
    descr_writes[0].descriptorCount  = 1;
    descr_writes[0].pBufferInfo      = &mvp_info;
    descr_writes[0].pImageInfo       = nullptr; 
    descr_writes[0].pTexelBufferView = nullptr; 
    // ViewParams
    descr_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descr_writes[1].pNext            = nullptr;    
    descr_writes[1].dstSet           = dg.descrsets[flight];
    descr_writes[1].dstBinding       = 1;
    descr_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descr_writes[1].dstArrayElement  = 0;
    descr_writes[1].descriptorCount  = 1;
    descr_writes[1].pBufferInfo      = &mvp_info;
    descr_writes[1].pImageInfo       = nullptr; 
    descr_writes[1].pTexelBufferView = nullptr; 
    // per Patch
    descr_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descr_writes[2].pNext            = nullptr;    
    descr_writes[2].dstSet           = dg.descrsets[flight];
    descr_writes[2].dstBinding       = 2;
    descr_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descr_writes[2].dstArrayElement  = 0;
    descr_writes[2].descriptorCount  = patch_params.size();
    descr_writes[2].pBufferInfo      = &patch_params[0];
    descr_writes[2].pImageInfo       = nullptr; 
    descr_writes[2].pTexelBufferView = nullptr; 
    // height maps
    descr_writes[3].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descr_writes[3].pNext            = nullptr;    
    descr_writes[3].dstSet           = dg.descrsets[flight];
    descr_writes[3].dstBinding       = 3;
    descr_writes[3].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descr_writes[3].dstArrayElement  = 0;
    descr_writes[3].descriptorCount  = hgt_map_infos.size();
    descr_writes[3].pBufferInfo      = nullptr; 
    descr_writes[3].pImageInfo       = &hgt_map_infos[0];
    descr_writes[3].pTexelBufferView = nullptr; 
    // normal maps
    descr_writes[4].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descr_writes[4].pNext            = nullptr;    
    descr_writes[4].dstSet           = dg.descrsets[flight];
    descr_writes[4].dstBinding       = 4;
    descr_writes[4].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descr_writes[4].dstArrayElement  = 0;
    descr_writes[4].descriptorCount  = normal_map_infos.size();
    descr_writes[4].pBufferInfo      = nullptr; 
    descr_writes[4].pImageInfo       = &normal_map_infos[0];
    descr_writes[4].pTexelBufferView = nullptr; 
    // color maps
    descr_writes[5].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descr_writes[5].pNext            = nullptr;    
    descr_writes[5].dstSet           = dg.descrsets[flight];
    descr_writes[5].dstBinding       = 5;
    descr_writes[5].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descr_writes[5].dstArrayElement  = 0;
    descr_writes[5].descriptorCount  = color_map_infos.size();
    descr_writes[5].pBufferInfo      = nullptr; 
    descr_writes[5].pImageInfo       = &color_map_infos[0];
    descr_writes[5].pTexelBufferView = nullptr; 

    
    //
    vkUpdateDescriptorSets (device.handle, descr_writes.size(), &descr_writes[0], 0, nullptr);
  }
  

  assert (false);
  return false;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupTerrainResources (mars::Glob& glob) { 
  // rokz::Buffer vb_x; 
  // rokz::CreateInfo_VB_stage (vb_x.ci, DarkrootMesh::VertexSize, glob.darkmesh.verts.size());
  // rokz::AllocCreateInfo_stage (vb_x.alloc_ci);
  // rokz::CreateBuffer (vb_x, glob.allocator);
  // if (rokz::MapMemory (&pmapped, vb_x.allocation, glob.allocator)) {
  //   memcpy (pmapped, &glob.darkmesh.verts[0], DarkrootMesh::VertexSize * glob.darkmesh.verts.size()); 
  //   rokz::UnmapMemory (vb_x.allocation, glob.allocator); 
  // }

  // rokz::CreateInfo_VB_device (glob.vma_vb_device.ci, DarkrootMesh::VertexSize, glob.darkmesh.verts.size());
  // rokz::AllocCreateInfo_device (glob.vma_vb_device.alloc_ci); 
  // rokz::CreateBuffer (glob.vma_vb_device, glob.allocator); 

  // //rokz::Transfer_2_Device;
  // rokz::MoveToBuffer_XB2DB (glob.vma_vb_device, vb_x, DarkrootMesh::VertexSize * glob.darkmesh.verts.size(), 
  //                           glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  // rokz::Destroy (vb_x, glob.allocator); 
  
  // // INDEX BUFF
  // rokz::Buffer ib_x;
  // rokz::CreateInfo_IB_16_stage (ib_x.ci, glob.darkmesh.indices.size()); 
  // rokz::AllocCreateInfo_stage (ib_x.alloc_ci);
  // rokz::CreateBuffer (ib_x, glob.allocator);

  // if (rokz::MapMemory (&pmapped, ib_x.allocation, glob.allocator)) {
  //   memcpy (pmapped, &glob.darkmesh.indices[0], DarkrootMesh::IndexSize * glob.darkmesh.indices.size()  ); 
  //   rokz::UnmapMemory (ib_x.allocation, glob.allocator); 
  // }
  
  // rokz::CreateInfo_IB_16_device (glob.vma_ib_device.ci, glob.darkmesh.indices.size()); 
  // rokz::AllocCreateInfo_device (glob.vma_ib_device.alloc_ci);
  // rokz::CreateBuffer (glob.vma_ib_device, glob.allocator);

  // rokz::MoveToBuffer_XB2DB  (glob.vma_ib_device, ib_x, DarkrootMesh::IndexSize * glob.darkmesh.indices.size (), 
  //                            glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 
  // rokz::Destroy (ib_x, glob.allocator); 
  // //DarkrootMesh& dark_mesh = glob.dark_mesh;
  return false;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupTerrainSamplers (Glob& glob) {
  printf ("%s \n", __FUNCTION__); 

  rokz::cx::CreateInfo (glob.sampler.ci, glob.physical_device.properties);
  
  rokz::cx::CreateSampler (glob.sampler, glob.device.handle);  

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupPatchGeometry (Glob& glob) {

  // wat even is this

  rokz::MVPTransform ub0;

}


#endif




