
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

  glob.shared.dt = dt;
  glob.shared.sim_time += dt;

  glob.shared.viewport_ext = glob.swapchain_group.swapchain.ci.imageExtent;
  
  // update MVPTransofrm buffer
  // 
  //

  float& sim_timef = glob.shared.sim_time;
  //float dtF = static_cast <float> (dt);
  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
  // printf ("m0 * v0 = <%f, %f, %f, %f>  \n",  v0.x, v0.y, v0.z, v0.w); 
  // printf ("v1 * m0 = <%f, %f, %f, %f>  \n",  v1.x, v1.y, v1.z, v1.w); 
  // printf ("m[3][0]=%f | m[3][1]=%f | m[3][2]=%f  \n",  m0[3][0], m0[3][1], m0[3][2] ); 
  rokz::MVPTransform mats; 
  mats.model = glm::rotate(posmat, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

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
  
  float sim_timef = glob.shared.sim_time;
  //  float dtF = static_cast <float> (dt);
  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
  // printf ("m0 * v0 = <%f, %f, %f, %f>  \n",  v0.x, v0.y, v0.z, v0.w); 
  // printf ("v1 * m0 = <%f, %f, %f, %f>  \n",  v1.x, v1.y, v1.z, v1.w); 
  // printf ("m[3][0]=%f | m[3][1]=%f | m[3][2]=%f  \n",  m0[3][0], m0[3][1], m0[3][2] ); 
  rokz::MVPTransform mats; 
  mats.model = glm::rotate(posmat, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

#ifndef WATlolwut // >>>>>>>>

  if (glob.input_state.mouse.inside != glob.prev_inside) { 
    glob.prev_x = glob.input_state.mouse.x_pos;
    glob.prev_y = glob.input_state.mouse.y_pos;
    glob.prev_inside = glob.input_state.mouse.inside; 
  }

  int mouse_dx = glob.input_state.mouse.x_pos - glob.prev_x;
  int mouse_dy = glob.input_state.mouse.y_pos - glob.prev_y; 

  if (std::abs(mouse_dx) > 100 || std::abs(mouse_dy) > 100) {
    printf ("[ dx:%i, dy:%i ]\n" , mouse_dx, mouse_dy);
  }

  glob.prev_x = glob.input_state.mouse.x_pos;
  glob.prev_y = glob.input_state.mouse.y_pos;

  const float ratef     = k2Pi * 0.0003f;
  glob.polyd.view_orie.theta -= mouse_dx * ratef;
  glob.polyd.view_orie.phi   -= mouse_dy * ratef;

#endif // <<<<<
  
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
    unit_angle_xz (va, 5.0 * sim_timef ); 
    unit_angle_xz (vb, 5.0 * sim_timef + kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));

    glob.polyd.obj_theta[0] += mouse_dx * (float) dt * darkroot::k2Pi;
    
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    obj[0].modelmat = glm::rotate(model0, glob.polyd.obj_theta[0], glm::vec3(0.0f, -1.0f, 0.0f));
    //obj[0].modelmat = glm::rotate(model0, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    obj[1].modelmat = glm::rotate(model1, sim_timef * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
  }
}



// ---------------------------------------------------------------------
// RecordDarkCommandBuffer_indexed
// ---------------------------------------------------------------------
bool RecordDynamicRenderPass (Glob& glob, 
                              VkCommandBuffer&         command_buffer,
                              const pipeline_assembly& pa, 
                              //                              const VkDescriptorSet&  desc_set, // this stays b/c [current_frame]
                              const VkBuffer&          vertex_buffer, 
                              const VkBuffer&          index_buffer, 
                              const VkExtent2D&        ext2d,
                              const rokz::Device&      device) {

  const DarkMesh& darkmesh = DarkOctohedron ();
  
#ifdef DARKROOT_HIDE_BUFFER_BEGIN_IN_RECORD_DYNAMIC
  // bc FrameDrawBegin does this already
  printf ("===> %i <=== ]\n", __LINE__);
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.flags = 0;                  // 
  begin_info.pInheritanceInfo = nullptr; // 

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
     printf ("failed to begin recording command buffer!");
     return false; 
  }
#else
  
      printf ("===> %i <=== ]\n", __LINE__);
#endif
      
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(ext2d.width);
  viewport.height = static_cast<float>(ext2d.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = ext2d;

      printf ("===> %i <=== ]\n", __LINE__);
  
  // 
  // [glob.rendering_info_group.ri] is updated in the calling function
  //
  vkCmdBeginRendering (command_buffer, &glob.rendering_info_group.ri);
  // vkCmdBeginRenderPass (command_buffer, &pass_info, VK_SUBPASS_CONTENTS_INLINE);
  
  vkCmdBindPipeline (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pa.pipeline.handle);

  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  vkCmdSetScissor(command_buffer, 0, 1, &scissor);

  vkCmdBindDescriptorSets (command_buffer,
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pa.plo, //                         pipelinedef.layout.pipeline.handle, // pipeline.layout.handle,
                           0,
                           1,
                           &pa.descrset, //&descriptorSets[currentFrame],
                           0,
                           nullptr);

  
  VkBuffer vertex_buffers[] = {vertex_buffer};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
  vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT16);

  for (uint32_t i = 0; i < 2; ++i) {

    darkroot::PushConstants pcs {};
    pcs.drawIDs.x = i; 
    pcs.drawIDs.y = i; 
    pcs.drawIDs.z = i; 
    pcs.drawIDs.w = i; 

    const VkShaderStageFlags shader_stages =
      VK_SHADER_STAGE_VERTEX_BIT ; //| VK_SHADER_STAGE_FRAGMENT_BIT;

    vkCmdPushConstants (command_buffer,
                        pa.plo, //                        pipelinedef.layout.pipeline.handle, // pipeline.layout.handle,
                        shader_stages,
                        0,
                        sizeof(darkroot::PushConstants),
                        &pcs);

    vkCmdDrawIndexed (command_buffer, darkmesh.indices.size(), 1, 0, 0, 0);
  }


#ifdef DARKROOT_HIDE_BUFFER_END_IN_RECORD_DYNAMIC
  // bc FrameDrawEmd does this already
  printf ("===> %i <=== ]\n", __LINE__);
  vkCmdEndRendering (command_buffer);
  // vkCmdEndRenderPass
  //
  if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("[FAILED] record command buffer\n");
    return false; 
  }
      printf ("===> %i <=== ]\n", __LINE__);
#endif
      
  //printf ("BAI %s\n", __FUNCTION__); 
  return true;
}

     



// ---------------------------------------------------------------------------------------
// EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT EXPERIMENT 
// ---------------------------------------------------------------------------------------
struct DarkLoop {

  Glob& glob;

  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 600;

  std::chrono::system_clock::time_point then;

  std::chrono::duration<size_t, std::chrono::microseconds::period> time_per_frame; //(time_per_frame_us);

  //
  DarkLoop (Glob& g, std::chrono::microseconds  time_per_frame_us ) :  glob(g), time_per_frame (time_per_frame_us) { 

    run        = true;
    curr_frame = 0; 
    result     = false;
    countdown  = 600;
    then = std::chrono::high_resolution_clock::now(); 
 
    printf ( "\nBegin run for [%i] frames.. \n\n", countdown); 

  }
  

  bool cond () {
    return countdown && run && !glfwWindowShouldClose(glob.window.glfw_window); 
  }

  bool loop () {

    //while (run_cond()) {

    glfwPollEvents(); 
    //start = std::chrono::high_resolution_clock::now();
    std::chrono::system_clock::time_point now =
          std::chrono::high_resolution_clock::now();    
    //dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds>(then - now).count (); 

    assert (false); // why is there a fixed 'dt' and a variable 'dt'
    // glob.polyd.dt = std::chrono::duration<double, std::chrono::seconds::period>(now - then).count();
    
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q))
      run = false;

    rokz::SwapchainGroup& scg = glob.swapchain_group;
      
      printf ("===> %i <=== ]\n", __LINE__);
    //
    // get image index up here
    uint32_t image_index; 
    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 
    
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_reset_cb->ResetSwapchain (glob.window, glob.device.allocator, glob.device);
      return true;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {
      
      printf ("===> %i <=== ]\n", __LINE__);

      UpdateDarkUniforms (glob, curr_frame, glob.shared.dt); 
      // Draw seqs...
      // for (auto drawseq : draw_seqs) {
      // drawseq->Prerec  (glob.pa_objs,  glob.pa_objs.descrgroup.descrsets[curr_frame],  glob.device);
      // }

      UpdateDynamicRenderingInfo (glob, image_index);

      // Transitioning Layout and stuff in here
      rokz::cx::FrameDrawBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                                image_index, glob.rendering_info_group.ri, glob.device);


#ifdef  DARKROOT_HIDE_DRAW_CALL
      // Draw seqs...
      RecordDynamicRenderPass (glob,
                               glob.framesyncgroup.command_buffers[curr_frame],
                               glob.obj_pipeline.pipeline,
                               glob.obj_pipeline.descrgroup.descrsets[curr_frame], 
                               glob.vma_vb_device.handle, 
                               glob.vma_ib_device.handle,
                               glob.swapchain_group.swapchain.ci.imageExtent,
                               glob.device );
      // + the progressively updated version
      // | 
      // v                                 
      pipeline_assembly pa {
        glob.polys_pl, glob.polys_plo.handle, glob.polyd.descrgroup.descrsets[curr_frame] }; 

      RecordDynamicRenderPass (glob, glob.framesyncgroup.command_buffers[curr_frame], pa, 
                               glob.polyd.vb_device.handle, glob.polyd.ib_device.handle, 
                               glob.swapchain_group.swapchain.ci.imageExtent, glob.device );

      printf ("===> %i <=== ]\n", __LINE__);

#else

      printf ("===> %i <=== ]\n", __LINE__);
      pipeline_assembly pa {
        glob.polys_pl, glob.polys_plo.handle, glob.polyd.descrgroup.descrsets[curr_frame] }; 

      RecordDynamicRenderPass (glob, glob.framesyncgroup.command_buffers[curr_frame], pa, 
                               glob.polyd.vb_device.handle, glob.polyd.ib_device.handle, 
                               glob.swapchain_group.swapchain.ci.imageExtent, glob.device );

      // Draw seqs...
      // for (auto drawseq : draw_seqs) {
      // drawseq->Prep (pa, glob.device)
      // drawseq->Record (glob.framesyncgroup.command_buffers[curr_frame], glob.pa_objs, 
      //                  glob.pa_objs.descrgroup.descrsets[curr_frame],  glob.device);
      // }
      
      // PolygonDraw::RecordDrawList (pipeline, frame_sync, device);
      // GridDraw::RecordDrawList    (pipeline, frame_sync, device);
      
#endif
      
      rokz::cx::FrameDrawEnd (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame], 
                              image_index, glob.framesyncgroup.syncs[curr_frame], glob.device);
    }
    
    printf ("===> %i <=== ]\n", __LINE__);
    
    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - now;
    if (time_to_make_frame < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for(sleep_time);
    }

    curr_frame = (curr_frame + 1) % kMaxFramesInFlight;
    then = now; // std::chrono::high_resolution_clock::now(); 
    countdown--; 

    // } while
    return true;
  }
    
};


// ------------------------------------------------------------------------------------------
//                                              
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
  // this sets up bindings + create dslo
  // called from inside  SetupObjectPipeline


  const char* a_terrain_pipeline_would_look_like_this[] = {
    "kDescriptorSetBindings", 
    "kVertexInputBindingDesc",
    "kVertexInputBindingAttributeDesc",
    "BindTerrainDescriptorSets",
    "InitTerrainPipeline"
  } ;

  
  if (!InitObjPipeline (glob.polys_pl, glob.polys_plo, glob.polys_dslo, dark_path,
                        glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
                        scg.swapchain.ci.imageFormat, glob.depth_format, glob.device)) {
    // SetupObjectPipeline (glob.polys_pl, glob.polys_plo, glob.polys_dslo, dark_path,
    //                      glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
    //                      scg.swapchain.ci.imageFormat, glob.depth_format, glob.device); 

    // failed InitObjPipeline
    return false;
  }

  // this is how sop would look if layouts  were already done
// SetupObjectPipeline (glob.pipeline_objs, glob.pipeline_def_obj.layout.pipeline, 
//                      dark_path, glob.swapchain_group.swapchain.ci.imageExtent, glob.msaa_samples,
//                      scg.swapchain.ci.imageFormat, glob.depth_format, glob.device); 
 
  SetupRenderingAttachments (glob); // <-- this does all the additional  attachmentes

  //SetupDarkMultisampleColorResource (glob);
  // rekz::CreateMSAAColorImage  (glob.msaa_color_image, glob.msaa_color_imageview, glob.msaa_samples,
  //                        scg.swapchain.ci.imageFormat, glob.device.allocator.handle, glob.device.command_pool, 
  //                        glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent, glob.device);

  // //SetupDarkDepthBuffer (glob);
  // rekz::CreateDepthBufferImage (glob.depth_image, glob.depth_imageview, glob.msaa_samples, glob.depth_format, 
  //                         glob.device.command_pool, glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent,
  //                         glob.device.allocator.handle, glob.device);
  glob.swapchain_reset_cb = CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
                                                     glob.depth_image, glob.depth_imageview,
                                                     glob.msaa_color_image, glob.msaa_color_imageview); 

  // for BeginRendering ()
  SetupDynamicRenderingInfo (glob) ; 

  // **
  // SetupObjResources (glob.polygons, glob.device);
  // SetupObjectUniforms (glob.vma_uniform_buffs, glob.vma_objparam_buffs, glob.device);
  // SetupObjectTextureAndSampler
  //SetupObjResources (glob); <--- replaced by SetupPolygonData
  SetupPolygonData (glob.polyd, kMaxFramesInFlight, data_root, glob.device); 

  
  // 
  // 
  SetupObjectUniforms (glob.vma_shared_uniforms, glob.polyd.vma_poly_uniforms, kMaxFramesInFlight, glob.device);


  // SetupObjDescriptorPool
  if (!rokz::MakeDescriptorPool(glob.polyd.descrgroup.pool, kMaxFramesInFlight, kObjDescriptorBindings, glob.device)) {
    // if (!SetupObjDescriptorPool (glob.polys_descrg.pool, glob.device)) {
    //   printf ("[FAILED] --> SetupGlobalDescriptorPool \n"); 
    //   return false;
    // }
    printf ("[FAILED] --> MakeDescriptorPool \n"); 
    return false;
  }

  //
  if (!rokz::MakeDescriptorSets (glob.polyd.descrgroup.descrsets, glob.polyd.descrgroup.alloc_info, kMaxFramesInFlight,
                                 glob.polys_dslo.handle, glob.polyd.descrgroup.pool, glob.device)) {
    printf ("[FAILED] --> MakeDescriptorSets \n"); 
    return false;
  }
  // * DescriptorLayout is created in SetupObjectPipeline this is done here
  // * b/c SetupGlobalDescriptorPool is just barely above us

  //    --------------------- *
  // allocate descriptorsets is done in SODS is that correct ??
  //
  // ?? create DescriptorPool + DescriptorSets together
  //    --------------------- *  

  // Bind*DescriptorSets is part of a pipeline definition
  if (!BindObjectDescriptorSets (glob.polyd.descrgroup.descrsets, glob.vma_shared_uniforms,
                                 glob.polyd.vma_poly_uniforms, glob.polyd.imageview, glob.polyd.sampler,
                                 glob.polys_dslo, glob.device)) {
    // this is where all the VkWriteDescriptorSet's r
    // if (!SetupObjectDescriptorSets (glob.obj_pipeline,            glob.vma_uniform_buffs,
    //                                 glob.vma_objparam_buffs,      glob.texture_imageview,
    //                                 glob.sampler,glob.descr_pool, glob.device)) {
    //   printf ("[FAILED] --> SetupObjectDescriptorSets \n"); 
    //   return false;
    // }
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
  
  std::chrono::microseconds time_per_frame_us(static_cast<size_t>(time_per_frame_sec * 1000000.0));
  
  std::chrono::duration<size_t, std::chrono::microseconds::period>
    time_per_frame(time_per_frame_us);
  //size_t    frame_counter = 0;
  bool       run        = true;
  uint32_t   curr_frame = 0; 
  bool       result     = false;
  int        countdown  = 600;
  printf ( "\nBegin run for [%i] frames.. \n\n", countdown); 
  //
  auto t0 = std::chrono::high_resolution_clock::now(); 
  std::chrono::system_clock::time_point then = t0; 

  // DarkLoop darkloop (glob, time_per_frame_us ); 
  // RunApplicationLoop (darkloop);

  glob.polydraw = CreatePolygonDraw (glob.polyd);

  while (countdown && run && !glfwWindowShouldClose(glob.window.glfw_window)) {

    printf ("===> %i <=== ]\n", __LINE__);
    glfwPollEvents(); 
    printf ("===> %i <=== ]\n", __LINE__);

    //start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();    
    //dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds>(then - now).count (); 

    // ???
    //glob.polyd.dt = std::chrono::duration<double, std::chrono::seconds::period>(now - then).count();
    
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q)) {
      printf ("--> [q] pressed... quitting \n");
      run = false;
    }
    //
    // get image index up here
    uint32_t image_index; 
    VkResult acquireres = rokz::cx::AcquireFrame (scg.swapchain, glob.framesyncgroup.syncs[curr_frame], image_index, glob.device); 
    
    if (acquireres == VK_ERROR_OUT_OF_DATE_KHR || acquireres == VK_SUBOPTIMAL_KHR || glob.input_state.fb_resize) {
      glob.input_state.fb_resize = false; 
      glob.swapchain_reset_cb->ResetSwapchain (glob.window, glob.device.allocator, glob.device);
      printf ("===> %i <=== ]\n", __LINE__);
      continue;
    }
    else if (acquireres != VK_SUCCESS) {
      printf("failed to acquire swap chain image!");
      run = false;
    }
    else {
      
      // Draw seqs...
      // for (auto drawseq : draw_seqs) {
      // drawseq->Prerec  (glob.pa_objs,  glob.pa_objs.descrgroup.descrsets[curr_frame],  glob.device);
      // }

      UpdateDarkUniforms (glob, curr_frame, Dt); 

      // UpdateGlobals (glob, curr_frame, Dt);  

      // for drawseq's
      // glob.polydraw->Exec (glob.framesyncgroup.command_buffers[curr_frame], pa, glob.polyd.descrgroup.descrsets[curr_frame]);

      // make sure the correct swapchain image is used
      UpdateDynamicRenderingInfo (glob, image_index);

      // Transitioning Layout and stuff in here
      // BeginCommandBuffer is called here
      rokz::cx::FrameDrawBegin (glob.swapchain_group, glob.framesyncgroup.command_buffers[curr_frame],
                                image_index, glob.rendering_info_group.ri, glob.device);


#ifdef  DARKROOT_HIDE_DRAW_CALL
      // Draw seqs...
      // BeginCommandBuffer is called here
      RecordDynamicRenderPass (glob,
                               glob.framesyncgroup.command_buffers[curr_frame],
                               glob.obj_pipeline.pipeline,
                               glob.obj_pipeline.descrgroup.descrsets[curr_frame], 
                               glob.vma_vb_device.handle, 
                               glob.vma_ib_device.handle,
                               glob.swapchain_group.swapchain.ci.imageExtent,
                               glob.device );
      // + the progressively updated version
      // | 
      // v                                 
      pipeline_assembly pa {
        glob.polys_pl, glob.polys_plo.handle, glob.polyd.descrgroup.descrsets[curr_frame] }; 

      RecordDynamicRenderPass (glob, glob.framesyncgroup.command_buffers[curr_frame], pa, 
                               glob.polyd.vb_device.handle, glob.polyd.ib_device.handle, 
                               glob.swapchain_group.swapchain.ci.imageExtent, glob.device );


#else

      pipeline_assembly pa {
        glob.polys_pl, glob.polys_plo.handle, glob.polyd.descrgroup.descrsets[curr_frame] }; 

      // replace with polydraw
      RecordDynamicRenderPass (glob, glob.framesyncgroup.command_buffers[curr_frame], pa, 
                               glob.polyd.vb_device.handle, glob.polyd.ib_device.handle, 
                               glob.swapchain_group.swapchain.ci.imageExtent, glob.device );
      // 
      //glob.polydraw->Exec (glob.framesyncgroup.command_buffers[curr_frame], pa, glob.polyd.descrgroup.descrsets[curr_frame]);
      
      // Draw seqs...
      // for (auto drawseq : draw_seqs) {
      // drawseq->Prep (pa, glob.device)
      // drawseq->Record (glob.framesyncgroup.command_buffers[curr_frame], glob.pa_objs, 
      //                  glob.pa_objs.descrgroup.descrsets[curr_frame],  glob.device);
      // }
      
      // PolygonDraw::RecordDrawList (pipeline, frame_sync, device);
      // GridDraw::RecordDrawList    (pipeline, frame_sync, device);
      
#endif
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
  }

  
  vkDeviceWaitIdle(glob.device.handle);

  // end loop
  //ShutdownDarkroot ();

  // CLEAN UP
  CleanupDarkroot (glob); 
  printf ("===> %i <=== ]\n", __LINE__);
  
  //  globmem.reset ();
  return 0;
}




// --------------------------------------------------------------------
//
// --------------------------------------------------------------------


const std::string sbcs[] = {
  "k2",
  // passport
  // excavator
  "ride",
  // warpig
  // superpig
  // peaceseeker
  "capita",
  // indoor survival
  "jones",
  "yes", 
  "korua",
  // transition finder
  "rome",
  // national
}; 










#ifdef  DARKROOT_HIDE_darkroot_image_handler 
// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
int darkroot_image_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up) {

  darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (up); 
  
  if (rekz::LoadTexture_color_sampling (g->texture_image, VK_FORMAT_R8G8B8A8_SRGB ,
                                  VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                  dat, g->device.allocator.handle, g->device.queues.graphics, 
                                  g->device.command_pool, g->device)) {
    return 0;  
  }
  return __LINE__;
} 
#endif


#ifdef DARKROOT_HIDE_SETUPOBJECTUNIFORMS
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupObjectUniforms (Glob& glob) {
  printf ("%s", __FUNCTION__);

  VkDevice const&          device  = glob.device.handle;
  VkPhysicalDevice const&  physdev = glob.device.physical.handle;

  std::vector<rokz::Buffer>& uniform_buffs = glob.polyd.vma_uniform_buffs;
  std::vector<rokz::Buffer>& objparams   = glob.polyd.vma_objparam_buffs;
  
  uniform_buffs.resize (kMaxFramesInFlight);
  //mapped_ptrs.resize (kMaxFramesInFlight); 
  objparams.resize (kMaxFramesInFlight);
  //objparam_ptrs.resize (kMaxFramesInFlight);
  
  for (size_t i = 0; i <  kMaxFramesInFlight; i++) {

    rokz::cx::CreateInfo_uniform (uniform_buffs[i].ci, rokz::kSizeOf_MVPTransform, 1); 
    rokz::cx::AllocCreateInfo_mapped (uniform_buffs[i].alloc_ci); 
    if (!rokz::cx::CreateBuffer (uniform_buffs[i], glob.device.allocator.handle)) {
      printf (" --> [FAIL]  create MVPTransform \n"); 
      return false; 
    }

    rokz::cx::CreateInfo_uniform (objparams[i].ci, SizeOf_SceneObjParam, 128);
    rokz::cx::AllocCreateInfo_mapped (objparams[i].alloc_ci);
    if (!rokz::cx::CreateBuffer (objparams[i], glob.device.allocator.handle)) {
      printf (" --> [FAIL]  create SceneObjParam \n"); 
      return false; 
    }

  }

  printf (" --> [true] \n"); 
  return true; 
}
#endif

#ifdef   DARKROOT_HIDE_SETUPOBJECTTEXTUREANDSAMPLER
// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
bool SetupObjectTextureAndSampler (Glob& glob) {

  int res = ~0; 


    
  printf ("%s \n", __FUNCTION__); 
  //rokz::ReadStreamRef rs = rokz::CreateReadFileStream (data_root + "/texture/blue_0_texture.png"); 
  const char*  test_image_files[] = { 
    "out_0_blue-texture-image-hd_rgba.png",
    "out_1_abstract-texture-3_rgba.png",
  };

   rokz::Buffer stage_image; 
  //   const std::string fq_test_file = data_root + "/texture/out_1_abstract-texture-3_rgba.png";  
   const std::string fq_test_file = data_root + "/texture/out_0_blue-texture-image-hd_rgba.png";  

   res =  rekz::OpenImageFile (fq_test_file, darkroot_image_handler, &glob); 
  
   if (res == 0) {
     rokz::cx::CreateInfo (glob.texture_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.texture_image);  
     if (VK_SUCCESS == vkCreateImageView(glob.device.handle, &glob.texture_imageview.ci, nullptr, &glob.texture_imageview.handle)) {
       // make the sampler
       rokz::cx::CreateInfo (glob.sampler.ci, glob.physical_device.properties);
       rokz::cx::CreateSampler (glob.sampler, glob.device.handle);
       printf ("[SUCCESS] %s all things created\n", __FUNCTION__);

     }
     else {
       printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
       res = __LINE__;
     }
   }

   return (res == 0); 
}
#endif


#ifdef DARKROOT_HIDE_SETUPOBJRESOURCES
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupObjResources (darkroot::Glob& glob) { 
  // used in here:
  SetupObjectUniforms;
  SetupObjectTextureAndSampler;

  // maybe this shouldnt be called in here 
  //SetupObjectDescriptorLayout (descrgroup, device); 
  

  const DarkMesh& darkmesh = darkroot::DarkOctohedron (); 

  void* pmapped  = nullptr;
  rokz::Buffer vb_x; 
  rokz::cx::CreateInfo_VB_stage (vb_x.ci, DarkMesh::VertexSize, darkmesh.verts.size());
  rokz::cx::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::cx::CreateBuffer (vb_x, glob.device.allocator.handle);
  if (rokz::cx::MapMemory (&pmapped, vb_x.allocation, glob.device.allocator.handle)) {
    memcpy (pmapped, &darkmesh.verts[0], DarkMesh::VertexSize * darkmesh.verts.size()); 
    rokz::cx::UnmapMemory (vb_x.allocation, glob.device.allocator.handle); 
  }

  rokz::cx::CreateInfo_VB_device (glob.vma_vb_device.ci, DarkMesh::VertexSize, darkmesh.verts.size());
  rokz::cx::AllocCreateInfo_device (glob.vma_vb_device.alloc_ci); 
  rokz::cx::CreateBuffer (glob.vma_vb_device, glob.device.allocator.handle); 

  //rokz::Transfer_2_Device;
  rokz::cx::MoveToBuffer_XB2DB (glob.vma_vb_device, vb_x, DarkMesh::VertexSize * darkmesh.verts.size(), 
                            glob.device.command_pool.handle, glob.device.queues.graphics, glob.device.handle); 

  rokz::cx::Destroy (vb_x, glob.device.allocator.handle); 
  
  // INDEX BUFF
  rokz::Buffer ib_x;
  rokz::cx::CreateInfo_IB_16_stage (ib_x.ci, darkmesh.indices.size()); 
  rokz::cx::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::cx::CreateBuffer (ib_x, glob.device.allocator.handle);

  if (rokz::cx::MapMemory (&pmapped, ib_x.allocation, glob.device.allocator.handle)) {
    memcpy (pmapped, &darkmesh.indices[0], DarkMesh::IndexSize * darkmesh.indices.size()  ); 
    rokz::cx::UnmapMemory (ib_x.allocation, glob.device.allocator.handle); 
  }
  
  rokz::cx::CreateInfo_IB_16_device (glob.vma_ib_device.ci, darkmesh.indices.size()); 
  rokz::cx::AllocCreateInfo_device (glob.vma_ib_device.alloc_ci);
  rokz::cx::CreateBuffer (glob.vma_ib_device, glob.device.allocator.handle);

  rokz::cx::MoveToBuffer_XB2DB  (glob.vma_ib_device, ib_x, DarkMesh::IndexSize * darkmesh.indices.size (), 
                             glob.device.command_pool.handle, glob.device.queues.graphics, glob.device.handle); 
  rokz::cx::Destroy (ib_x, glob.device.allocator.handle); 
  //DarkMesh& dark_mesh = glob.dark_mesh;

  if (!SetupObjectUniforms (glob)) {
    printf ("[FAILED] --> SetupObjectUniforms \n"); 
    return false;
  }
  
  // rokz::DescriptorPool           uniform_descriptor_pool;
  // rokz::DescriptorGroup          uniform_group; 
  if (!SetupObjectTextureAndSampler (glob)) {
    printf ("[FAILED] --> SetupObjectTexture \n"); 
    return false;
  }

  return true;
}
#endif



#if REKZ_OLD_MONOLITHIC_RENDER_FUNC 
// ---------------------------------------------------------------------
// RenderFrame_dynamic <-- RecordDynamicRenderPass 
// ---------------------------------------------------------------------
bool RenderFrame_dynamic_pass (Glob&                   glob,
                               uint32_t&               image_index,
                               bool&                   resize,
                               //rokz::RenderPass&       renderpass, 
                               const rokz::Pipeline&   pipeline,
                               const VkDescriptorSet&  descr_set, 
                               uint32_t                curr_frame,
                               double                  dt) {

  const rokz::Device&             device      = glob.device; 
  rokz::SwapchainGroup&           scg = glob.swapchain_group;

  rokz::Swapchain&                swapchain              = scg.swapchain; 
  std::vector<rokz::Image>&       swapchain_images       = scg.images; 
  std::vector<rokz::ImageView>&   swapchain_imageviews   = scg.imageviews; 

  rokz::Image&                    depth_image            = glob.depth_image;
  rokz::ImageView&                depth_imageview        = glob.depth_imageview; 
  
  rokz::Image&                    msaa_color_image       = glob.msaa_color_image; 
  rokz::ImageView&                msaa_color_imageview   = glob.msaa_color_imageview; 

  VmaAllocator&                   allocator              = glob.device.allocator.handle;
  rokz::Window&                   window                 = glob.window;



  rokz::FrameSync&          framesync            = glob.framesyncgroup.syncs[curr_frame];
  VkResult acquire_res = rokz::cx::AcquireFrame (glob.swapchain_group.swapchain, framesync , image_index, device); 
  
  if (acquire_res == VK_ERROR_OUT_OF_DATE_KHR || acquire_res == VK_SUBOPTIMAL_KHR || resize) {
    resize = false; 
    return darkroot::RecreateSwapchain (swapchain, window,
                                        swapchain_images, swapchain_imageviews,
                                        depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                        msaa_color_image, msaa_color_imageview,
                                        allocator, device);
  } 
  else if (acquire_res != VK_SUCCESS) {
    printf("failed to acquire swap chain image!");
    return false;
  }

  //printf ("[%s] line:%i\n", __FUNCTION__, __LINE__); 
  // this is wat works right now
  UpdateDarkUniforms (glob, curr_frame, dt); 


  // dynamic_rendering now, we have to manually transition
  rokz::cx::TransitionImageLayout (glob.swapchain_group.images[image_index].handle,
                               swapchain.ci.imageFormat, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                               glob.device.queues.graphics, glob.device.command_pool.handle, device.handle);
  
  VkCommandBuffer&   command_buffer = glob.framesyncgroup.command_buffers[curr_frame]; 
  rokz::Buffer&      vma_vb_device  = glob.vma_vb_device;
  rokz::Buffer&      vma_ib_device  = glob.vma_ib_device; 

  vkResetCommandBuffer (command_buffer, 0); //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);

  //RecordDarkRenderPass (command_buffer,
  // RecordDarkRenderPaass
  UpdateDynamicRenderingInfo (glob, image_index); 

  // 
  // construct render sequence
  // generate render sequence
  RecordDynamicRenderPass (glob,
                           command_buffer,
                           pipeline,
                           descr_set, 
                           vma_vb_device.handle, 
                           vma_ib_device.handle,
                           swapchain.ci.imageExtent,
                           glob.device );
  
  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[]      = {framesync.image_available_sem};
  VkSemaphore signal_semaphores[]    = {framesync.render_finished_sem }; 
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submit_info.pWaitDstStageMask    = wait_stages;

  submit_info.waitSemaphoreCount   = 1;
  submit_info.pWaitSemaphores      = wait_semaphores;
  submit_info.signalSemaphoreCount = 1; 
  submit_info.pSignalSemaphores    = signal_semaphores; 
  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &command_buffer; // &glob.command_buffer_group.buffers[curr_frame];

  if (vkQueueSubmit (glob.device.queues.graphics, 1, &submit_info, framesync.in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }

  //  dynamic_rendering now, we have to manually transition
  rokz::cx::TransitionImageLayout (glob.swapchain_group.images[image_index].handle,
                               swapchain.ci.imageFormat,
                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                               VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                               glob.device.queues.graphics, glob.device.command_pool.handle, device.handle);

  
  return rokz::cx::PresentFrame (glob.device.queues.present, glob.swapchain_group.swapchain, image_index, glob.framesyncgroup.syncs[curr_frame]); 
  
}
#endif 
