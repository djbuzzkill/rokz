
#include "marz.h"
#include "grid_pipeline.h"
//




using namespace marz; 

namespace { 

const VkExtent2D kDisplayDimensions {800, 600}; 

}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool SetupMarsWindow (rokz::Window& window, void* user_pointer) {

  return false;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool SetupGridUniforms (Glob& glob) {
  printf ("%s", __FUNCTION__);
  return true; 
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool SetupGridResources (Glob& glob) {
  return false;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool UpdateGridUniforms (Glob& glob, uint32_t current_frame, double dt) {

  return false;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool SetupDynamicRenderingInfo (Glob& glob) {
  return false;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool SetupRenderAttachments (Glob& glob) {

  Vec<int> ints;
  
  return false; 

}


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void CleanupMars (Glob& glob) {

  
  // printf ("%s \n", __FUNCTION__); 

  // for (auto& ub : glob.uniform_mvp) {
  //   rokz::Destroy (ub, glob.allocator); 
  // }

  // assert (false); // rokz::cx::Destroy (glob.terrain_pipeline.descrgroup
  // //rokz::cx::Destroy (glob.terrain_pipeline.descrgroup, glob.device.handle); 

  // rokz::cx::Destroy (glob.descriptor_pool, glob.device.handle); 

  // assert (false); // rokz::cx::Destroy (glob.grid_pipeline.descrgroup, 
  // //rokz::cx::Destroy (glob.grid_pipeline.descrgroup, glob.device.handle); 

  // rokz::Destroy (glob.vma_vb_device, glob.allocator);
  // rokz::Destroy (glob.vma_ib_device, glob.allocator);
  
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

  assert (false); // Cleanup
  glfwTerminate();

}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool SetupMarsTexturesAndImageViews (Glob& glob) {

  // SetupDarkTexture (glob); 
  // SetupDarkTextureImageView (glob); 

  "height textures";

  "normal textures";

  "color textures"; 
    
  return false;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
// bool SetupGlobalDescriptorPool (Glob& glob) {

//   assert (false);
//   return false;
// }


// bool SetupTerrainDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

//   assert (false);
//   return false;
// }


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
// bool SetupMarsUniforms (Glob& glob) {
//   return false;
// }


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
// void UpdateMarsUniforms (Glob& glob, uint32_t current_frame, double dt) {
 
// }

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
// bool RecordDynamicRender (Glob& glob, 
//                           VkCommandBuffer        &command_buffer,
//                           const rokz::Pipeline&        pipeline,
//                           const VkDescriptorSet& desc_set, 
//                           const VkBuffer&        vertex_buffer, 
//                           const VkBuffer&        index_buffer, 
//                           const VkExtent2D&      ext2d,
//                           const VkFramebuffer&   framebuffer,
//                           const rokz::RenderPass&      render_pass,
//                           const VkDevice&        device) {

//   return false;
// }
// // --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
// bool RenderFrame (mars::Glob&           glob,
//                       uint32_t&               image_index,
//                       bool&                   resize,
//                       rokz::RenderPass&       renderpass, 
//                       const rokz::Pipeline&   pipeline,
//                       const VkDescriptorSet&  descr_set, 
//                       uint32_t                curr_frame,
//                       double dt) {


//   return false;
// }



// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
int run_marz (const std::vector<std::string>& args) {
  printf ("%s\n", __FUNCTION__);

  Glob glob; //
  rokz::SwapchainGroup&  scg   = glob.swapchain_group;
  rokz::FrameSyncGroup&  fsg   = glob.framesyncgroup; 
  
  std::filesystem::path pipe_path = "/home/djbuzzkill/owenslake/rokz/pipeline";
  std::filesystem::path data_path = "/home/djbuzzkill/owenslake/rokz/data"; // 

  //Default (glob); 
  
  glfwInit();
  

  rokz::InitializeInstance (glob.instance); 

  rekz::SetupDisplay (glob.display, glob.input_state, kDisplayDimensions, glob.instance); 
  
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
                             kDisplayDimensions, glob.device.physical, glob.device);
  //assert (false);

  //
  rekz::SetupRenderingAttachments (glob.msaa_color_image,
                                   glob.msaa_color_imageview, 
                                                         
                                   glob.depth_image,       
                                   glob.depth_imageview,
                            
                                   glob.msaa_samples,
                                   scg.swapchain.ci.imageFormat,
                                   glob.depth_format,          
                                   scg.swapchain.ci.imageExtent,
                                   glob.device); // <-- this does all the additional  attachmentes

  //
  glob.swapchain_reset_cb = rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
                                                           glob.depth_image, glob.depth_imageview,
                                                           glob.msaa_color_image, glob.msaa_color_imageview); 
  //
  // for BeginRendering ()
  SetupDynamicRenderingInfo (glob); 
  // define first 
  rokz::DefineDescriptorSetLayout (glob.global_dslo, rokz::kGlobalDescriptorBindings, glob.device); 
  //rokz::DefineDescriptorSetLayout (glob.object_dslo, rekz::kObjDescriptorBindings, glob.device); 

  // SetupMarsWindow (glob.window, &glob.input_state); 


  // grid only uses globals

  glob.grid.pipe.dslos.push_back (glob.global_dslo.handle);
  if (!rekz::InitGridPipeline (glob.grid.pipe,  glob.grid.plo, glob.grid.pipe.dslos , pipe_path,
                               glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
                               scg.swapchain.ci.imageFormat, glob.depth_format, glob.device)) { 
    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }
  // rokz::InitializeInstance (glob.instance); 
  // rokz::cx::CreateSurface  (&glob.surface, glob.window.glfw_window, glob.instance.handle);
  // rokz::cx::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance.handle);

  // rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info, glob.surface, glob.physical_device.handle);

  // rokz::ConfigureDevice ( glob.physical_device , VK_TRUE); 
  // rokz::InitializeDevice (glob.device,glob.physical_device, glob.instance);

  // glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.physical_device); 
  // rokz::ut::FindDepthFormat (glob.depth_format, glob.physical_device.handle);
  
  // //
  // rokz::InitializeSwapchain (scg, glob.swapchain_support_info, glob.surface,
  //                            kWinExtent, glob.physical_device, glob.device);


// #ifdef MARSCAPE_CALL_SETUP_GRID_PIPELINE_
  
//   rekz::SetupGridPipeline (glob.grid_pipeline,
//                            mars_path,
//                            kWinExtent, //const rokz::Swapchain& swapchain,
//                            glob.msaa_samples, 
//                            glob.swapchain_group.swapchain.ci.imageFormat,
//                            glob.depth_format,
//                            sizeof(PushConstants), 
//                            glob.device);
// #endif

//   // SetupRenderAttachments
//   if (!SetupRenderAttachments (glob)) {
//     printf ("[FAILED|%i] -> SetupRenderAttachments \n", __LINE__);
//     return false; 
//   }
      
//   if (! SetupGridResources (glob) ) {
//     return false; 
//   }
  
// #ifdef MARS_ENABLE_TERRAIN_PIPELINE  


//   SetupTerrainPipeline (glob.terrain_pipeline,
//                         glob.terrain_pipeline.pipeline.state.viewport,
//                         glob.render_pass,
//                         mars::data_root,
//                         glob.swapchain_group.swapchain,
//                         glob.msaa_samples,
//                         glob.device); 
//   //SetupTerrainPipeline (glob.terrain_pipeline, glob.viewport_state, glob.render_pass, dark_path, glob.swapchain_group.swapchain);

//   SetupTerrainResources (glob); 
//   "Setup Index+Vertex Buffers";

//   SetupTerrainSamplers (glob);

  
// #endif //  MARS_ENABLE_TERRAIN_PIPELINE

  

//   // for BeginRendering ()
//   SetupDynamicRenderingInfo (glob) ; 
  

  

//   printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);
//   if (!SetupMarsUniforms (glob)) {
//     printf ("[FAILED] --> SetupDarkUniforms \n"); 
//     return false;
//   }


//   if (!SetupMarsTexturesAndImageViews (glob)) {
//     printf ("[FAILED] --> SetupMarsTexturesAndImageViews \n"); 
//     return false;
//   }
  
  
//   if (!SetupGlobalDescriptorPool (glob)) {
//     printf ("[FAILED] --> SetupGlobalDescriptorPool \n"); 
//     return false;
//   }


// #ifdef MARS_TERRAIN_ENABLE 
//   if (!SetupTerrainDescriptorSets (glob.obj_pipeline,
//                                    glob.uniform_mvp,
//                                    glob.vma_objparam_buffs,
//                                    glob.texture_imageview,
//                                    glob.sampler,
//                                    glob.descr_pool, 
//                                    glob.device)) {
//     printf ("[FAILED] --> SetupTerrainDescriptorSets \n"); 
//     return false;
//   }
// #endif
  
// #ifdef MARSCAPE_CALL_SETUP_GRID_DESCRIPTORSETS  
//   if (!rekz::SetupGridDescriptorSets (glob.descrgroup_grid, glob.uniform_mvp,
//                                       glob.pipeline_def_grid.layout.descriptor, glob.device)) {
//     printf ("[FAILED] --> SetupTerrainDescriptorSets \n"); 
//     return false;
//   }
// #endif
  
//   printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);

//   //swapchain_group.command_buffer_group.buffers.resize (kMaxFramesInFlight);

//   fsg.syncs.resize (kMaxFramesInFlight);
//   fsg.command_buffers.resize (kMaxFramesInFlight);
  
//   rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.command_pool.handle); 

//   // 
//   for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
//     // sep
//     rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], 
//                               fsg.command_buffer_alloc_info,
//                               glob.device.handle);

//     rokz::cx::CreateFrameSync (fsg.syncs[i], fsg.syncs[i].ci, glob.device.handle);
//   }

//   // SetupDarkroot ();

//   printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);
//   const double time_per_frame_sec = 1.0 / 60.0;
//   glob.dt = time_per_frame_sec; // just do this for now
  
//   std::chrono::microseconds time_per_frame_us(static_cast<size_t>(time_per_frame_sec * 1000000.0));
  
//   std::chrono::duration<size_t, std::chrono::microseconds::period>
//     time_per_frame(time_per_frame_us);

//   bool       run        = true;
//   uint32_t   curr_frame = 0; 
//   bool       result     = false;
//   int        countdown  = 60;
  
//   //printf ( "\nBegin run for [%i] frames.. \n\n", countdown); 
//   //
//   auto t0 = std::chrono::high_resolution_clock::now(); 
//   auto then = t0; 

//   printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);
//   while (countdown && run && !glfwWindowShouldClose(glob.window.glfw_window)) {

//     glfwPollEvents(); 
//     //start = std::chrono::high_resolution_clock::now();
//     auto now = std::chrono::high_resolution_clock::now();    
//     //dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds>(then - now).count (); 
//     glob.dt = std::chrono::duration<double, std::chrono::seconds::period>(now - then).count();
    
//     //    result = RenderFrame (glob, curr_frame, fb_resize, glob.dt);
//     uint32_t image_index; 

// assert (false); // RenderFrame
//     // if (RenderFrame (glob, image_index, glob.fb_resize, glob.render_pass, glob.grid_pipeline.pipeline,
//     //                  glob.grid_pipeline.descrgroup.descrsets[curr_frame], curr_frame, glob.dt)) {

//     // }
//     // else {
//     //   run = false;
//     // }
    
//     // how long did we take
//     auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
//     if (time_to_make_frame < time_per_frame) {
//       auto sleep_time = time_per_frame - time_to_make_frame;
//       std::this_thread::sleep_for(sleep_time);
//     }

//     curr_frame = (curr_frame + 1) % kMaxFramesInFlight;
//     then = now; // std::chrono::high_resolution_clock::now(); 
//     countdown--; 
//   }

  
//   vkDeviceWaitIdle(glob.device.handle);
//   printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);


//   // CLEAN UP
//   CleanupMars (glob); 

//   printf ("%[LEAVING] --> %s\n", __FUNCTION__);

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




