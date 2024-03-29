
#include "marz.h"
#include "grid_pipeline.h"
#include "rekz/landscape_pipeline.h"
#include "rekz/rekz.h"
#include "rokz/attachment.hpp"
#include "rokz/context.hpp"
#include "drawmarz.h"
#include "rokz/descriptor.hpp"
#include "rokz/draw_sequence.hpp"
#include "rokz/global_descriptor.hpp"

#include "rokz/rokz_types.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

//

using namespace rokz;
using namespace marz; 
using namespace rekz; 
using std::size_t; 

namespace {
  const VkExtent2D kDefaultDimensions {1024, 768}; 
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void CleanupMars (Glob& glob) {

  assert (false); // Cleanup
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

  glfwTerminate();

}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
struct MarzLoop {

  Glob& glob;

  bool       run        = true;
  std::uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 6000000;

  const double Dt; 
  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);

  void update_run_state  () {
    // const float move_rate = 0.05f;
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q)) {
      printf ("--> [q] pressed... quitting \n");
      run = false;
    }
  } 
  
  //
  // -------------------------------------------------------------
  MarzLoop (Glob& g, double dt) : glob(g), Dt(dt) { 
    then = std::chrono::high_resolution_clock::now(); 
    time_per_frame = std::chrono::microseconds (static_cast<size_t>(Dt * 1000000.0));
    printf ( "\nBegin run for [%i] frames.. \n\n", countdown); 
  }

  void UpdateRunState  () {
    // const float move_rate = 0.05f;
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q)) {
      printf ("--> [q] pressed... quitting \n");
      run = false;
    }

  } 

  // while (cond()) loop()
  // -------------------------------------------------------------
  bool cond () {
    bool should_close_win =     glfwWindowShouldClose(glob.display.window.glfw_window); 
    
    if ( !run ) printf ("!RUN\n");
    if ( should_close_win ) printf ("should close window\n");
    if ( countdown == 0) printf ("countdown is 0\n"); 
    
    return countdown && run && !glfwWindowShouldClose(glob.display.window.glfw_window); 
        //return false;
  }
  //
  // -------------------------------------------------------------
  bool loop () {

    glfwPollEvents(); 

    auto now = std::chrono::high_resolution_clock::now();    
    
    UpdateRunState () ;
    
    rekz::UpdateViewAttitude (glob.shared.view_rot, glob.mouse_prev, glob.prev_inside, glob.input_state, 0.008f);
    rekz::UpdateViewPosition (glob.shared.view_pos,glob.shared.view_rot,  glob.input_state, 0.05);
    //
    rc::SwapchainGroup& scg = glob.swapchain_group; 
    // get image index up here
    std::uint32_t image_index; 

    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain->handle, image_index,
                                                  glob.framesyncgroup.syncs[curr_frame].in_flight_fence,
                                                  glob.framesyncgroup.syncs[curr_frame].image_available_sem,
                                                  glob.device); 
    //VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain->handle, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 
    
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_resetter->Reset (glob.device);
      printf ("===> %i <=== ]\n", __LINE__);
      return true;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {

      //UpdateDarkUniforms (glob, curr_frame, Dt); 
      rokz::UpdateGlobals (glob.shared, glob.global_bu[curr_frame], kDefaultDimensions, Dt);

      //void UpdateGlobals (rokz::DrawSequence::Globals& shared, const rokz::Buffer& buf, const VkExtent2D& viewext, double dt) {
      // update data needed to record drawlist
      rokz::UpdateDynamicRenderingInfo (glob.rendering_info_group, glob.msaa_color_imageview->handle,
                                        glob.swapchain_group.views[image_index]->handle);

      // Transitioning Layout and stuff in here, BeginCommandBuffer is called here
      cx::FrameDrawingBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                          image_index, glob.rendering_info_group.ri, glob.device);

      // EXECUTE DRAW LIST RECORDING 
      rokz::DrawSequence::RenderEnv scape_re {
        glob.scape.pipe, glob.scape.plo.handle, glob.shared 
      };
      
      rokz::DrawSequence::RenderEnv grid_re {
        glob.grid.pipe, glob.grid.plo.handle, glob.shared
      };
        
      glob.scape.draw->Prep (curr_frame, scape_re, glob.device); 
      glob.scape.draw->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, scape_re);

      glob.grid.draw->Prep (curr_frame, grid_re, glob.device); 
      glob.grid.draw->Exec (glob.framesyncgroup.command_buffers[curr_frame], curr_frame, grid_re);

      // we are done, submit
      cx::FrameDrawingEnd (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame], 
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


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
struct MarzDeviceFeatures : public VkPhysicalDeviceFeatures2 {

  MarzDeviceFeatures (const VkPhysicalDeviceFeatures& devfeats)  {
    
    dynamic_rendering_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    dynamic_rendering_feature.pNext = nullptr;
    dynamic_rendering_feature.dynamicRendering = VK_TRUE;

    sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2; 
    pNext = &dynamic_rendering_feature; 
    features = devfeats; 
  }

  // extension structs
  VkPhysicalDeviceDynamicRenderingFeaturesKHR  dynamic_rendering_feature {};
};

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
int run_marz (const std::vector<std::string>& args) {
  printf ("%s\n", __FUNCTION__);

  Glob glob; //
  rc::SwapchainGroup&  scg   = glob.swapchain_group;
  FrameSyncGroup&  fsg   = glob.framesyncgroup; 

  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath pipe_path = "/home/djbuzzkill/owenslake/rokz/pipeline";
  systempath data_path = "/home/djbuzzkill/owenslake/rokz/data"; // 
  //Default (glob); 
  
  glfwInit();
  
  rokz::InitializeInstance (glob.instance); 

  rekz::SetupDisplay (glob.display, glob.input_state, "marz", kDefaultDimensions , glob.instance); 
  
  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_info, glob.display.surface, glob.device.physical.handle);

  // VkPhysicalDeviceFeatures2 f2 {};
  //rokz::ConfigureFeatures; 
  MarzDeviceFeatures marzfeats (glob.device.physical.features2.features);
  // marzfeats.features.samplerAnisotropy  =  glob.device.physical.features2.features.samplerAnisotropy  ;
  // marzfeats.features.tessellationShader =  glob.device.physical.features2.features.tessellationShader ;
  // this does a lot of shit
  //rokz::InitializeDevice (glob.device, f2, glob.device.physical, glob.instance);
  rokz::InitializeDevice (glob.device, marzfeats, glob.device.physical, glob.instance);
  
  // put these somwehere
  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);
  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_info, glob.display.surface,
                            kDefaultDimensions, glob.device.physical, glob.device);
  //
  rc::SetupMSAARenderingAttachments (glob.msaa_color_image, glob.msaa_color_imageview, 
                                     glob.depth_image, glob.depth_imageview,
                                     glob.device.msaa_samples, scg.format,
                                     glob.depth_format, scg.extent,
                                     glob.device); // <-- this does all the additional  attachmentes
  //
  glob.swapchain_resetter = rekz::CreateSwapchainResetter (scg.swapchain, glob.display, scg.images,
                                                           scg.views, glob.depth_image, glob.depth_imageview,
                                                           glob.msaa_color_image, glob.msaa_color_imageview); 

  // for BeginRendering ()
  rokz::SetupDynamicRenderingInfo (glob.rendering_info_group, glob.msaa_color_imageview->handle,
                                   glob.depth_imageview->handle, scg.extent); 

  
  // define first 
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, rekz::grid::kDescriptorBindings, glob.device); 

  rokz::DefineDescriptorSetLayout (glob.landscape_dslo, lscape::kDescriptorBindings, glob.device); 

  //
  rokz::SetupGlobalUniforms (glob.global_bu, kMaxFramesInFlight, glob.device); 

  // grid only uses globals
  glob.grid.pipe.dslos.push_back (glob.grid_dslo.handle);

  if (!grid::InitPipeline (glob.grid.pipe,  glob.grid.plo, VK_NULL_HANDLE, glob.grid.pipe.dslos ,
                           pipe_path, scg.extent, glob.device.msaa_samples,
                               scg.format, glob.depth_format, glob.device)) { 
    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }
  //
  //glob.scape.pipe.dslos.push_back (glob.global_dslo.handle); 
  glob.scape.pipe.dslos.push_back (glob.landscape_dslo.handle); 
  if (!lscape::InitPipeline (glob.scape.pipe, glob.scape.plo, VK_NULL_HANDLE, glob.scape.pipe.dslos,
                             glob.device.msaa_samples, scg.format, glob.depth_format,
                             pipe_path, scg.extent, glob.device)) {
    printf ("[FAILED] --> InitLandscapeTiler \n"); 
    return false; 
  }      

  
  // 
  if (!marz::SetupData (glob.scape.data, glob.device)) {
    HERE("FAILED marz::SetupData");
    return false;
  }
  
  glob.scape.draw = marz::CreateDrawMarsLandscape (glob.scape.data, glob.landscape_de.descrsets); 

  // LANDSCAPE DESC 
  if (!rokz::MakeDescriptorPool (glob.landscape_de.pool, kMaxFramesInFlight,
                                 lscape::kDescriptorBindings, glob.device)) {
    HERE("");
    return false;
    
  }
                             
  if (!rokz::MakeDescriptorSets (glob.landscape_de.descrsets, glob.landscape_de.alloc_info,
                                 kMaxFramesInFlight, glob.landscape_dslo.handle,
                                 glob.landscape_de.pool, glob.device)) {
    HERE("");
    return false;
  }

  printf ("num descriptorsets:%zu\n", glob.landscape_de.descrsets.size ());
  lscape::BindDescriptorResources (glob.landscape_de.descrsets,
                                   glob.global_bu, 
                                   glob.scape.data.colorsampler, glob.scape.data.colorviews,
                                   glob.scape.data.heightsampler, glob.scape.data.heightviews,
                                   glob.scape.data.normalsampler, glob.scape.data.normalviews,
                                   glob.landscape_dslo, glob.device); 



  // GRID DESC
  if (!rokz::MakeDescriptorPool (glob.grid_de.pool, kMaxFramesInFlight,
                                 rekz::grid::kDescriptorBindings, glob.device)) {
    HERE("");
    return false;
  }
                             
  if (!rokz::MakeDescriptorSets (glob.grid_de.descrsets, glob.grid_de.alloc_info,
                                 kMaxFramesInFlight, glob.grid_dslo.handle,
                                 glob.grid_de.pool, glob.device)) {
    HERE("");
  }

  if (!rekz::grid::BindDescriptorResources (glob.grid_de.descrsets, glob.global_bu, glob.device)) {
    printf ("[FAILED] --> BindGridDescriptorResources \n"); 
  }

  size_t gridvertoffs;
  size_t gridindoffs;

  glob.grid.buff = rekz::SetupGridData (gridvertoffs, gridindoffs, 11, 11, 20.0f, 20.0f, glob.device); 
  glob.grid.draw = rekz::CreateDrawGrid (glob.grid.buff, glob.grid_de,  gridvertoffs, gridindoffs);  
  
  // create frame syncs
  fsg.command_buffers.resize (kMaxFramesInFlight);
  fsg.syncs.resize           (kMaxFramesInFlight);
  rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.device.command_pool.handle); 

  for (std::size_t i = 0; i < kMaxFramesInFlight; ++i) {
    // ^^ 'CreateCommandBuffers' should be called, we call it 
    rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], fsg.command_buffer_alloc_info, glob.device.handle);
    rokz::cx::CreateFrameSync (fsg.syncs[i], fsg.syncs[i].ci, glob.device.handle);
  } 

  
  const double time_per_frame_sec = 1.0 / 60.0;
  double Dt = time_per_frame_sec; // just do this for now
  // auto t0 = std::chrono::high_resolution_clock::now(); 
  // std::chrono::system_clock::time_point then = t0; 
  MarzLoop marzloop (glob, Dt); 
  rokz::FrameLoop  (marzloop);


  HERE(" loope exited "); 
  vkDeviceWaitIdle(glob.device.handle);

//   // CLEAN UP
   CleanupMars (glob); 


  return 0; 
}

// ------------------------------------------------------------------------------------------
//                            
// ------------------------------------------------------------------------------------------

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




