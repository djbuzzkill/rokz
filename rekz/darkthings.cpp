

#include "darkrootgarden.h"


namespace darkroot {
  

  const DarkVert OctohedronVerts[] = {
    // top 
    {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5}}, 

    {{ 0.0, 0.0,  1.0 }, {-0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {0.0, 1.0}}, 
    {{ 1.0, 0.0,  0.0 }, { 0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {1.0, 1.0}}, 
    {{ 0.0, 0.0, -1.0 }, { 0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {1.0, 0.0}}, 
    {{-1.0, 0.0,  0.0 }, {-0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {0.0, 0.0}}, 

    // bottom
    {{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5 }}, 
    
  }; 

  const size_t kOctohedronVertexCount = sizeof(OctohedronVerts) / sizeof (DarkVert);
  static_assert (kOctohedronVertexCount == 6, "");

  // CW
  const uint16_t OctohedronIndices[] = {
    0, 2, 1,  0, 3, 2,  0, 4, 3,  0, 1, 4,
    5, 1, 2,  5, 2, 3,  5, 3, 4,  5, 4, 1
  }; 

  // CCW 
  // const uint16_t OctohedronIndices[] = {
  //   0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,
  //   5, 2, 1,  5, 3, 2,  5, 4, 3,  5, 1, 4
  // }; 

  const size_t kOctohedronIndicesCount = sizeof(OctohedronIndices) / sizeof (uint16_t); 

  static_assert (kOctohedronIndicesCount == 24, "");

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  const DarkMesh& DarkOctohedron () {

    static DarkMesh octo;
    if (octo.verts.size () == 0) { 
      octo.verts.assign  (OctohedronVerts, OctohedronVerts + kOctohedronVertexCount); 
      octo.indices.assign(OctohedronIndices, OctohedronIndices + kOctohedronIndicesCount);
    }
    return octo;
  }

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  const DarkVert dark_verts[] = {
    {{-0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                                      
    {{ 0.0f, -1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
  };

  const uint16_t dark_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------

  const DarkVert dark_cube[] = {
    {{-0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{ 0.0f, -1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
  };

  const uint16_t cube_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };

  
}





// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void darkroot::CleanupSwapchain (std::vector<rokz::ImageView>& sc_image_views,
                                 rokz::Image&                  msaa_color_image,
                                 rokz::ImageView&              msaa_color_imageview,

                                 rokz::Image&                  depth_image,
                                 rokz::ImageView&              depth_imageview,

                                 rokz::Swapchain&              swapchain,
                                 const rokz::Device&           device,
                                 const VmaAllocator&           allocator) {

  // for (auto fb : framebuffers) {
  //   vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  // }

  for (auto sc_imageview : sc_image_views) {
    vkDestroyImageView(device.handle, sc_imageview.handle, nullptr);
  }

  rokz::cx::Destroy (msaa_color_image, allocator);
  rokz::cx::Destroy (msaa_color_imageview, device.handle);

  rokz::cx::Destroy (depth_image, allocator);
  rokz::cx::Destroy (depth_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
}

void darkroot::Cleanup (VkPipeline&                 pipeline,
                        //std::vector<Framebuffer>&         framebuffers,
                        std::vector<rokz::ImageView>&           imageviews,

                        rokz::Swapchain&                  swapchain,
                        VkSurfaceKHR&                     surf,
                        VkCommandPool&                    command_pool,
                        std::vector<rokz::FrameSync>&     syncs, 
                        std::vector<rokz::ShaderModule>&  shader_modules,
                        VkPipelineLayout&                 pipeline_layout,
                        //rokz::RenderPass&                 render_pass,
                        rokz::Image&                      msaa_color_image,
                        rokz::ImageView&                  msaa_color_imageview,

                        rokz::Image&                      depth_image,
                        rokz::ImageView&                  depth_imageview,

                        GLFWwindow*                       w,
                        rokz::Device&                     device,
                        VmaAllocator&                     allocator, 
                        VkInstance&                       inst) {

  //    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  CleanupSwapchain (imageviews,
                    msaa_color_image, msaa_color_imageview,
                    depth_image, depth_imageview,
                    swapchain, device, allocator);
   
  vkDestroyPipeline (device.handle, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool (device.handle, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, syncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  //vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  vmaDestroyAllocator(allocator);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
  glfwDestroyWindow(w);
}



bool darkroot::RecreateSwapchain(rokz::Swapchain&  swapchain, const rokz::Window& win, 
                                 std::vector<rokz::Image>& swapchain_images, std::vector<rokz::ImageView>& imageviews,
                                 rokz::Image& depth_image, rokz::ImageView& depth_imageview,
                                 rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,
                                 const VmaAllocator& allocator, const rokz::Device& device) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(win.glfw_window, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(win.glfw_window, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (device.handle);

  CleanupSwapchain (imageviews,
                    depth_image, depth_imageview,
                    multisamp_color_image,
                    multisamp_color_imageview,
                    swapchain, device, allocator);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = rokz::cx::CreateSwapchain (swapchain, device);
  bool imageviews_res   = rokz::cx::CreateImageViews (imageviews, swapchain_images, device);

  return (swapchain_res && imageviews_res); 
}




// ---------------------------------------------------------------------
// RenderFrame_dynamic <-- RecordDynamicRenderPass 
// ---------------------------------------------------------------------
struct DarkResetSwapchain : public rokz::ResetSwapchainCB
  {
  public:
  
    DarkResetSwapchain (rokz::Swapchain& sc, 
                        std::vector<rokz::Image>& scis, std::vector<rokz::ImageView>& scivs,
                        rokz::Image& dp, rokz::ImageView& dpiv,  
                        rokz::Image& mscim, rokz::ImageView&  mscimv)
      : ResetSwapchainCB ()
      , swapchain (sc)
      , swapchain_images (scis)
      , swapchain_imageviews (scivs)
      , depth_image (dp)
      , depth_imageview(dpiv)
      , msaa_color_image(mscim)
      , msaa_color_imageview(mscimv) { 
    }
    
    //
    virtual bool ResetSwapchain (const rokz::Window& win, const rokz::Allocator& allocator,  const rokz::Device& device) {
      return darkroot::RecreateSwapchain (swapchain, win, 
                                          swapchain_images, swapchain_imageviews,
                                          depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                          msaa_color_image, msaa_color_imageview,
                                          allocator.handle, device);
    }
    
  protected:
    
    rokz::Swapchain&              swapchain;
    std::vector<rokz::Image>&     swapchain_images;
    std::vector<rokz::ImageView>& swapchain_imageviews;
    
    rokz::Image&                  depth_image;
    rokz::ImageView&              depth_imageview;  //
    rokz::Image&                  msaa_color_image;
    rokz::ImageView&              msaa_color_imageview; 
};


std::shared_ptr<rokz::ResetSwapchainCB> darkroot::CreateSwapchainResetter (rokz::Swapchain& sc, 
                                                           std::vector<rokz::Image>& scis, std::vector<rokz::ImageView>& scivs,
                                                           rokz::Image& dp, rokz::ImageView& div,
                                                           rokz::Image& mscim, rokz::ImageView& mscimv) {

  return std::make_shared<DarkResetSwapchain> (sc, scis, scivs, dp, div, mscim, mscimv); 

}




#ifdef DARkROOT_ENABLE_RENDERABLE

struct DarkRenderable : public Renderable {

  DarkRenderable () : num_verts (0), num_inds (0), vb_dev() , ib_dev () {
  }

  DarkRenderable (uint32_t v, uint32_t i) : num_verts (v), num_inds (i), vb_dev() , ib_dev () {
  }

  virtual auto SetupRS (VkCommandBuffer commandbuffer) -> int;
  virtual auto Draw (VkCommandBuffer commandbuffer) -> void;
  virtual auto AllocRes (VmaAllocator& allocator) -> int;
  virtual auto FreeRes (VmaAllocator& alloc) -> int;

  uint32_t num_verts;
  uint32_t num_inds;
  
  rokz::Buffer vb_dev;
  rokz::Buffer ib_dev;

  glm::vec3 pos;
  glm::quat qrot;
  
  friend DarkRenderable& Friendly ( DarkRenderable& dr); 

protected:
};

//
// --------------------------------------------------------------------
auto darkroot::DarkRenderable::SetupRS (VkCommandBuffer commandbuffer) -> int {
  return 0; 
}


// --------------------------------------------------------------------
auto darkroot::DarkRenderable::Draw (VkCommandBuffer commandbuffer) -> void  {
  // rokz::Pipeline pipeline;
  // rokz::DescriptorGroup desc_group;
  VkBuffer vertex_buffers[] = { vb_dev.handle};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertex_buffers, offsets);

  vkCmdBindIndexBuffer(commandbuffer, ib_dev.handle, 0, VK_INDEX_TYPE_UINT16);

  vkCmdDrawIndexed (commandbuffer, num_inds, 1, 0, 0, 0);
  //return 0; 
}


// --------------------------------------------------------------------
auto darkroot:: DarkRenderable::AllocRes (VmaAllocator& allocator) -> int {
    
  rokz::cx::CreateInfo_VB_device (vb_dev.ci, DarkrootMesh::VertexSize,  num_verts);
  rokz::cx::AllocCreateInfo_device (vb_dev.alloc_ci); 
  rokz::cx::CreateBuffer (vb_dev, allocator); 
    
  rokz::cx::CreateInfo_IB_16_device (ib_dev.ci, num_inds); 
  rokz::cx::AllocCreateInfo_device (ib_dev.alloc_ci);
  rokz::cx::CreateBuffer (ib_dev, allocator);

  return 0; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
auto darkroot:: DarkRenderable::FreeRes (VmaAllocator& alloc) -> int {

  rokz::cx::Destroy (vb_dev, alloc);
  rokz::cx::Destroy (ib_dev, alloc);

  return 0; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
constexpr size_t SizeOfDarkRenderable () {
  return sizeof ( darkroot::DarkRenderable); 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
 darkroot::DarkRenderable& Initialize ( darkroot::DarkRenderable& dr, uint32_t nv, uint32_t ni) {

  new (&dr) darkroot:: DarkRenderable (nv, ni);
  return dr;
}

 darkroot::DarkRenderable& Friendly (  darkroot::DarkRenderable& dr) {

  Initialize (dr, 0, 0);
  // dr.num_verts = 0;
  // dr.num_inds = 0;
  // dr.vb_dev;
  // dr.ib_dev;
  return dr;
}
#endif



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
#ifdef DARKROOT_ENABLE_RENDERABLE_TEST
void SetupDarkGeometry (Glob& glob) {

  printf ("%s\n", __FUNCTION__); 

  const DarkrootMesh& darkmesh = darkroot::DarkOctohedron (); 

  HalfEdge::BRep darkboundary;
  BuildBoundaryRep (darkboundary, glob.darkmesh);

  //DarkrootMesh& darkmesh = glob.darkmesh; 

  // create the renderable
  Initialize (glob.darkobj, darkmesh.verts.size(), darkmesh.indices.size()); 
  glob.renderables.push_back (&glob.darkobj);

  void* pmapped  = nullptr;
  // VERTEX BUFFER allocat and fill transfer buffers
  rokz::Buffer vb_x;
  rokz::CreateInfo_VB_stage (vb_x.ci, DarkrootMesh::VertexSize, darkmesh.verts.size());
  rokz::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::CreateBuffer (vb_x, glob.allocator);
  if (rokz::MapMemory (&pmapped, vb_x.allocation, glob.allocator)) {
    memcpy (pmapped,  &darkmesh.verts[0] , DarkrootMesh::VertexSize * darkmesh.verts.size()); 
    rokz::UnmapMemory (vb_x.allocation, glob.allocator); 
  }
  // INDEX BUFFER
  rokz::Buffer ib_x;
  rokz::CreateInfo_IB_16_stage (ib_x.ci, darkmesh.indices.size ()); 
  rokz::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::CreateBuffer (ib_x, glob.allocator);

  if (rokz::MapMemory (&pmapped, ib_x.allocation, glob.allocator)) {
    memcpy (pmapped, &darkmesh.indices[0], DarkrootMesh::IndexSize * darkmesh.indices.size ()); 
    rokz::UnmapMemory (ib_x.allocation, glob.allocator); 
  }

  // 
  for (auto r : glob.renderables) { 
    r->AllocRes (glob.allocator); 
  }
  
  //rokz::Transfer_2_Device;
  rokz::MoveToBuffer_XB2DB (glob.darkobj.vb_dev, vb_x, DarkrootMesh::VertexSize * glob.darkmesh.verts.size(), 
                            glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  rokz::MoveToBuffer_XB2DB  (glob.darkobj.ib_dev, ib_x, DarkrootMesh::IndexSize * glob.darkmesh.indices.size (),
                             glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  rokz::Destroy (vb_x, glob.allocator); 
  rokz::Destroy (ib_x, glob.allocator); 

}

#endif // DARKROOT_ENABLE_RENDERABLE_TEST



