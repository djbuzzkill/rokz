#include "pepper.h"

#include "grid_pipeline.h"
#include "rokz/framebuffer.hpp"
#include "rokz/renderpass.hpp"
#include <vulkan/vulkan_core.h>


using namespace rokz;

void darkroot_cleanup (Vec<VkPipeline>&         pipelines,
                       VkSurfaceKHR&            surf,
                       VkCommandPool&           command_pool,
                       Vec<rokz::FrameSync>&    syncs, 
                       Vec<rokz::ShaderModule>& shader_modules,
                       VkPipelineLayout&        pipeline_layout,
                       rokz::Display&           display,
                       rokz::Device&            device,
                       VkInstance&              inst) {
  
  for (auto& p : pipelines) {
    vkDestroyPipeline (device.handle, p, nullptr);
  }
  
  vkDestroySurfaceKHR (inst, surf, nullptr);
  surf = VK_NULL_HANDLE;
  
  vkDestroyCommandPool (device.handle, command_pool, nullptr);
  command_pool = VK_NULL_HANDLE;
  
  rokz::cx::Destroy (syncs, device); 

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  //vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  glfwDestroyWindow(display.window.glfw_window);

  vmaDestroyAllocator(device.allocator.handle);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
}

// -----------------------------------------------------------------------------------------
//                
// -----------------------------------------------------------------------------------------
bool pepper::setup_renderpasses (Glob& g) {

  enum att_type {

    COLOR_ATTACH = 0,
    DEPTH_ATTACH,
    RESOLVE_ATTACH,

    NUM_ATTACH
  };

  att_type  res = RESOLVE_ATTACH;
  att_type  col = COLOR_ATTACH;
  att_type  dep = DEPTH_ATTACH;

  // VkAttachmentDescription
  Vec<VkAttachmentDescription>  attde (NUM_ATTACH);
  attde[col] = {};
  attde[col].format         = g.msaacolor.format; 
  attde[col].samples        = g.device.msaa_samples; // g.msaa_samples;
  attde[col].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attde[col].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  attde[col].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attde[col].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attde[col].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED; 
  attde[col].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 

  attde[dep] = {};
  //ut::FindDepthFormat (render_pass.attach_desc[DP_n].format, g.device.physical);
  attde[dep].format         = g.depth.format;  
  attde[dep].samples        = g.device.msaa_samples; // g.msaa_samples;
  attde[dep].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;;
  attde[dep].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attde[dep].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attde[dep].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; 
  attde[dep].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED; 
  attde[dep].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 

  attde[res] = {};
  attde[res].format         = g.swapchain_group.format;  
  attde[res].samples        = VK_SAMPLE_COUNT_1_BIT; // resolve always has 1
  attde[res].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attde[res].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  attde[res].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attde[res].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attde[res].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED; 
  attde[res].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
  
  // VkAttachmentReference
  Vec<VkAttachmentReference> attref (NUM_ATTACH);
  attref[col].attachment = col;
  attref[col].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  attref[dep].attachment = dep; 
  attref[dep].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 

  attref[res].attachment = res; 
  attref[res].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 
  
  // VkSubpassDescription
  Vec<VkSubpassDescription> subpde (1);
  subpde[0] = {};
  subpde[0].pipelineBindPoint      = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpde[0].colorAttachmentCount   = 1;
  subpde[0].pColorAttachments      = &attref[col]; 
  subpde[0].inputAttachmentCount = 0;
  subpde[0].pInputAttachments    = nullptr;
  subpde[0].pDepthStencilAttachment= &attref[dep];
  subpde[0].preserveAttachmentCount= 0;
  subpde[0].pPreserveAttachments   = nullptr;
  subpde[0].pResolveAttachments    = &attref[res];
  subpde[0].flags                  = 0; 
  //
  Vec<VkSubpassDependency> subpdep (1);
  subpdep[0].srcSubpass    = VK_SUBPASS_EXTERNAL;
  subpdep[0].dstSubpass    = 0;
  subpdep[0].srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdep[0].srcAccessMask = VK_ACCESS_NONE;
  subpdep[0].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdep[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  g.renderpass = rc::CreateRenderPass (attde, subpde, subpdep, g.device); 

  return (g.renderpass->handle != VK_NULL_HANDLE);
}

// -----------------------------------------------------------------------------------------
//                
// -----------------------------------------------------------------------------------------
bool pepper::setup_framebuffers (Glob& g) {

  auto num_fbs = g.swapchain_group.images.size(); 
  
  g.framebuffers.resize (num_fbs); 
  
  for (size_t i = 0; i < num_fbs; i++) {

    Vec<VkImageView> attachments {
      g.msaacolor.view->handle, 
      g.depth.view->handle, 
      g.swapchain_group.views[i]->handle, 
    }; 

    g.framebuffers[i] = rc::CreateFramebuffer (g.renderpass->handle, attachments,
                                               g.swapchain_group.extent, g.device); 


    if ( !g.framebuffers[i] )
      return false;
  }  
  
  return true;
} 

// -----------------------------------------------------------------------------------------
//                
// -----------------------------------------------------------------------------------------
void pepper::cleanup ( pepper::Glob& glob) {

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

  rokz::cx::Destroy (glob.osd_de.pool, glob.device); 
  // ?  
  // polygons will make use of object descriptors

  // for (auto& buf : glob.poly_objects_bu) { 
  //   buf.reset ();
  //   //rokz::cx::Destroy  (buf, glob.device.allocator); 
  // }

  //
  rekz::CleanupSwapchain (glob.swapchain_group.views,
                          glob.msaacolor.image, glob.msaacolor.view,
                          glob.depth.image, glob.depth.view,
                          glob.swapchain_group.swapchain, 
                          glob.device);

  // the the  poly stuff is done in Cleanup, grids is done here
  for (auto shmod : glob.grid_pl.shader_modules) {
    vkDestroyShaderModule (glob.device.handle, shmod.handle, nullptr); 
  }

  vkDestroyPipelineLayout (glob.device.handle, glob.grid_plo.handle, nullptr);

  Vec<VkPipeline> pipes = { 
    glob.polys_pl.handle, glob.grid_pl.handle, glob.osd_pl.handle }; 

  darkroot_cleanup (pipes,
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

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
struct pepper::PepperLoop_rp {

  pepper::Glob& glob;

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
  
  // -------------------------------------------------------------
  PepperLoop_rp (pepper::Glob& g, float dt) : glob(g), Dt(dt) { 

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

      rokz::UpdateGlobals (glob.shared, glob.global_rc_uniform_bu [curr_frame], kDefaultDimensions, Dt);

      // did not work as planned
      //onscreen::UpdateOSD (glob.global_rc_uniform_bu[curr_frame], glob.osdata.strings, kTestExtent, Dt);  
      
      // make sure the correct swapchain image is used

      rokz::DrawSequence::RenderEnv poly_re {
        glob.polys_pl, glob.polys_plo.handle, glob.shared, 
      };

      rokz::DrawSequence::RenderEnv grid_re {
        glob.grid_pl, glob.grid_plo.handle, glob.shared, 
      };

      rokz::DrawSequence::RenderEnv osd_re  {
        glob.osd_pl, glob.osd_plo.handle, glob.shared, 
      };

      Vec<VkClearValue> clear_values (3); 
      clear_values[0].color        = {{0.1f, 0.1f, 0.1f, 1.0f}};
      clear_values[1].depthStencil = {1.0f, 0};
      clear_values[2].color        = {{0.1f, 0.1f, 0.1f, 1.0f}};


      if (VK_SUCCESS != vkResetCommandBuffer (glob.framesyncgroup.command_buffers[curr_frame], 0)) {  //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);
        return __LINE__; 
      }

      VkCommandBufferBeginInfo begin_info {};
      begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      begin_info.pNext = nullptr;
      begin_info.flags = 0;                  // 
      begin_info.pInheritanceInfo = nullptr; // 

      if (vkBeginCommandBuffer(glob.framesyncgroup.command_buffers[curr_frame], &begin_info) != VK_SUCCESS) {
        printf ("failed to begin recording command buffer!");
        return __LINE__; 
      }

      
      VkRenderPassBeginInfo rp {};
      rp.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      rp.pNext           = nullptr;
      rp.renderPass      = glob.renderpass->handle;
      rp.framebuffer     = glob.framebuffers[image_index]->handle; 
      rp.renderArea      = VkRect2D {VkOffset2D{0, 0}, glob.swapchain_group.extent}; 
      rp.clearValueCount = (uint32_t)clear_values.size ();
      rp.pClearValues    = &clear_values[0]; 

      vkCmdBeginRenderPass (glob.framesyncgroup.command_buffers[curr_frame],
                            &rp, VK_SUBPASS_CONTENTS_INLINE); 



      vkResetCommandBuffer; 
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

      // image_index, 
      //   glob.framesyncgroup.syncs[curr_frame].image_available_sem,
      //   glob.framesyncgroup.syncs[curr_frame].render_finished_sem,
      //   glob.framesyncgroup.syncs[curr_frame].in_flight_fence, 
      //   glob.device);
      //
      vkCmdEndRenderPass (glob.framesyncgroup.command_buffers[curr_frame]); 

      //
      if (VK_SUCCESS != vkEndCommandBuffer (glob.framesyncgroup.command_buffers[curr_frame]) != VK_SUCCESS) {
        printf ("[FAILED] record command buffer\n");
        return __LINE__; 
      }

      // -- we are done, submit
      //rokz::Swapchain& swapchain = scg.swapchain;
      {
        VkSubmitInfo submit_info {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;

        VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        Vec<VkSemaphore> waits    = { glob.framesyncgroup.syncs[curr_frame].image_available_sem };
        Vec<VkSemaphore> signals  = { glob.framesyncgroup.syncs[curr_frame].render_finished_sem }; 

        submit_info.pWaitDstStageMask    = wait_stages;

        submit_info.waitSemaphoreCount   = waits.size(); // sem_image_available waits here 
        submit_info.pWaitSemaphores      = &waits[0];

        submit_info.signalSemaphoreCount = signals.size();  // sem_render_finished signals here 
        submit_info.pSignalSemaphores    = &signals[0]; 

        submit_info.commandBufferCount   = 1;
        submit_info.pCommandBuffers      = &glob.framesyncgroup.command_buffers[curr_frame]; // &glob.command_buffer_group.buffers[curr_frame];

        //
        if (vkQueueSubmit (glob.device.queues.graphics, 1, &submit_info,
                           glob.framesyncgroup.syncs[curr_frame].in_flight_fence) != VK_SUCCESS) {
          printf("failed to submit draw command buffer!");
          return false; 
        }
      }

      {
        Vec<VkSemaphore> wait_render_finished = {
          glob.framesyncgroup.syncs[curr_frame].render_finished_sem  // sem_render_finished waits here 
        }; 

        if (!cx::PresentFrame ( glob.device.queues.present, image_index, scg.swapchain, wait_render_finished)) {
          return __LINE__;
        }
      }
      // cx::FrameDrawingEnd (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame], 
      //                   image_index, 
      //                   glob.framesyncgroup.syncs[curr_frame].image_available_sem,
      //                   glob.framesyncgroup.syncs[curr_frame].render_finished_sem,
      //                   glob.framesyncgroup.syncs[curr_frame].in_flight_fence, 
      //                   glob.device);
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
// ---------------------------------------------------------------------------------------
//  encapsulate rendering loop
// ---------------------------------------------------------------------------------------
struct pepper::PepperLoop_dyn {

  
  pepper::Glob& glob;

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
  
  // -------------------------------------------------------------
  PepperLoop_dyn (pepper::Glob& g, float dt) : glob(g), Dt(dt) { 

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
      rokz::UpdateGlobals (glob.shared, glob.global_rc_uniform_bu [curr_frame], kDefaultDimensions, Dt);

      // did not work as planned
      //onscreen::UpdateOSD (glob.global_rc_uniform_bu[curr_frame], glob.osdata.strings, kTestExtent, Dt);  
      
      // make sure the correct swapchain image is used

      rokz::UpdateDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolor.view->handle,
                                        glob.swapchain_group.views[image_index]->handle);

      // ------------------------------- render pass start -------------------------------
      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here
      cx::FrameDrawingBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
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

// -- FEATURES ----------------------------------------------------------------------
struct PepperFeatures : public   VkPhysicalDeviceFeatures2 {

  PepperFeatures () = default; 

  PepperFeatures (const rokz::PhysicalDevice& physdev) {

    dynamic_rendering_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    dynamic_rendering_feature.pNext = nullptr;
    dynamic_rendering_feature.dynamicRendering = VK_TRUE;


    sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2; 
    pNext  = &dynamic_rendering_feature; 
    features = physdev.features2.features; 
  }

  // extension structs
  VkPhysicalDeviceDynamicRenderingFeaturesKHR  dynamic_rendering_feature {};
  
}; 
  

// -- ------------------------------------------------------
int pepper::run (const Vec<std::string>& args) {
 
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

  rokz::SetupDisplay (glob.display, glob.input_state, "pepper", kDefaultDimensions, glob.instance); 
  //SetupDisplay (glob); 

  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_info, glob.display.surface, glob.device.physical.handle);

  //VkPhysicalDeviceFeatures2 features2 {};  
  PepperFeatures darkfeats (glob.device.physical);
  //rokz::ConfigureDevice (glob.device.physical, VK_TRUE);
  //rokz::ConfigureFeatures (features2, glob.device.physical);

  // this does a lot of shit
  rokz::InitializeDevice (glob.device, darkfeats, glob.device.physical, glob.instance);
  
  // put these somwehere else
  //glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  rokz::ut::FindDepthFormat (glob.depth.format, glob.device.physical.handle);

  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_info, glob.display.surface,
                           kDefaultDimensions, glob.device.physical, glob.device);


  //
  // 
  rc::CreateBasicMSAAAttachments (glob.msaacolor, glob.depth,       
                                  glob.device.msaa_samples,  scg.format,
                                  glob.depth.format,kDefaultDimensions,
                                  glob.device); // <-- this does all the additional  attachmentes

  // 
  //
  if (!setup_renderpasses (glob)) {
    printf ("[FAILED] --> setup_renderpasses \n"); 
    return false;
  }
  // 
  if (!setup_framebuffers (glob)) {
    printf ("[FAILED] --> setup_framebuffers \n"); 
    return false;
    
  }

  
  // ---------------- INIT POLYGON PIPELINE ---------------------
  rokz::DefineDescriptorSetLayout (glob.object_dslo, obz::kDescriptorBindings, glob.device); 

  glob.polys_pl.dslos.push_back (glob.object_dslo.handle);
  if (!rekz::InitObjPipeline (glob.polys_pl, glob.polys_plo, glob.renderpass->handle,
                              glob.polys_pl.dslos, dark_path, kDefaultDimensions,
                              glob.device.msaa_samples, scg.format,
                              glob.depth.format, glob.device)) {
    printf ("[FAILED] --> InitObjPipeline \n"); 
    return false;
  }

  
  // ---------------- INIT GRID PIPELINE  ---------------------
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, grid::kDescriptorBindings, glob.device); 

  glob.grid_pl.dslos.push_back (glob.grid_dslo.handle);
  if (!rekz::grid::InitPipeline (glob.grid_pl,  glob.grid_plo, glob.renderpass->handle,
                                 glob.grid_pl.dslos , dark_path, kDefaultDimensions,
                                 glob.device.msaa_samples, scg.format, glob.depth.format,
                                 glob.device)) { 
    printf ("[FAILED] --> InitGridPipeline \n"); 
    return false; 
  }

  // ---------------- INIT OSD PIPELINE  ---------------------
  rokz::DefineDescriptorSetLayout (glob.osd_dslo, onscreen::kDescriptorBindings, glob.device); 

  glob.osd_pl.dslos.push_back (glob.osd_dslo.handle); 
  if (!onscreen::InitPipeline (glob.osd_pl, glob.osd_plo, glob.renderpass->handle,
                               glob.osd_pl.dslos, dark_path, kDefaultDimensions,
                               glob.device.msaa_samples, scg.format, glob.depth.format, glob.device)) {
    printf ("[FAILED] --> OSD pipeline \n"); 
    return false; 
  }

  // DISPLAY CHANGED RESETTER
  glob.swapchain_resetter = rekz::CreateSwapchainResetter (scg.swapchain, glob.display, scg.images,
                                                           scg.views, glob.depth.image, glob.depth.view,
                                                           glob.msaacolor.image, glob.msaacolor.view); 

  // for BeginRendering ()
  // rokz::SetupDynamicRenderingInfo (glob.rendering_info_group, glob.msaacolor.view->handle,
  //                                  glob.depth.view->handle, scg.extent); 


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
  HERE (" -- > 831 "); 

  rokz::FrameSyncGroup& fsg = glob.framesyncgroup;
  // !! 
  fsg.command_buffers.resize (kMaxFramesInFlight);
  fsg.syncs.resize           (kMaxFramesInFlight);
  rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.device.command_pool.handle); 

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
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

  PepperLoop_rp rootloop (glob, Dt); 
  rokz::FrameLoop  (rootloop);

  vkDeviceWaitIdle(glob.device.handle);
  // CLEAN UP
  cleanup (glob); 
  HERE("bai");
  return 0;
}
