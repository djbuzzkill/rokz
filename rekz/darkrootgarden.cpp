
#include "darkrootgarden.h"
#include "rokz/buffer.h"
#include "rokz/context.h"
// 

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const size_t kMaxFramesInFlight = darkroot::Glob::MaxFramesInFlight; 

const std::string   data_root =  "/home/djbuzzkill/owenslake/rokz/data"; // 
const VkExtent2D    kTestExtent  = { 800, 600 };
const size_t        kSceneObjCount = 128;
//
//
using namespace darkroot; 

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const VkVertexInputBindingDescription kDarkVertexBindingDesc =  {
  0,                            // binding    
  sizeof (DarkVert),       // stride      
  VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   
}; 

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const std::vector<VkVertexInputAttributeDescription> kDarkvertBindingAttributeDesc = {

  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(DarkVert, pos),  // .offset   
  },

  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(DarkVert, nrm), 
  },

  VkVertexInputAttributeDescription { // color
    2,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(DarkVert, col), 
  },
  VkVertexInputAttributeDescription { // tex coord
    3,                             
    0, 
    VK_FORMAT_R32G32_SFLOAT,
    offsetof(DarkVert, txc0), 
  }

}; 

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
Glob::Glob()
  : instance()
  , device ()
  , dt ()
    //  , allocator()
  , depth_image()
  , depth_imageview()
  , msaa_samples ()
  , vma_ib_device()
  , vma_vb_device()
  , texture_image()
  , texture_imageview()
  , sampler()
  , vma_uniform_buffs()
  , descr_pool()
  , window()
  , physical_device()
  , swapchain_support_info()
  , surface(nullptr)
  , sim_time(0.0)
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
bool SetupDarkWindow (Glob& glob) {


  rokz::CreateWindow (glob.window, kTestExtent.width , kTestExtent.height, "wut"); 

  glfwSetFramebufferSizeCallback (glob.window.glfw_window, rekz::win_event::on_resize ); 
  glfwSetKeyCallback (glob.window.glfw_window, rekz::win_event::on_keypress);
  glfwSetCursorPosCallback(glob.window.glfw_window, rekz::win_event::on_mouse_move);
  glfwSetMouseButtonCallback(glob.window.glfw_window, rekz::win_event::on_mouse_button);
  glfwSetCursorEnterCallback (glob.window.glfw_window, rekz::win_event::on_mouse_enter); 
                              
  glfwSetWindowUserPointer (glob.window.glfw_window, &glob.input_state);


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

  glfwSetWindowUserPointer (glob.window.glfw_window, &glob);
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
//
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupObjectUniforms (Glob& glob) {
  printf ("%s", __FUNCTION__);

  VkDevice const&          device = glob.device.handle;
  VkPhysicalDevice const&  physdev = glob.physical_device.handle;

  std::vector<rokz::Buffer>& uniform_buffs = glob.vma_uniform_buffs;
  std::vector<rokz::Buffer>& objparams   = glob.vma_objparam_buffs;
  
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


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
int darkroot_file_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up) {

  darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (up); 
  
  if (rekz::LoadTexture_color_sampling (g->texture_image, VK_FORMAT_R8G8B8A8_SRGB ,
                                  VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                  dat, g->device.allocator.handle, g->device.queues.graphics, 
                                  g->device.command_pool, g->device)) {
    return 0;  
  }

  return __LINE__;
} 

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
bool SetupObjectTextureAndSampler (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 
  //rokz::ReadStreamRef rs = rokz::CreateReadFileStream (data_root + "/texture/blue_0_texture.png"); 
  const char*  test_image_files[] = { 
    "out_0_blue-texture-image-hd_rgba.png",
    "out_1_abstract-texture-3_rgba.png",
  };

   rokz::Buffer stage_image; 
  //   const std::string fq_test_file = data_root + "/texture/out_1_abstract-texture-3_rgba.png";  
   const std::string fq_test_file = data_root + "/texture/out_0_blue-texture-image-hd_rgba.png";  

   int res =  rekz::OpenImageFile (fq_test_file, darkroot_file_handler, &glob); 
  
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



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupObjResources (darkroot::Glob& glob) { 

  // used in here:
  SetupObjectUniforms;
  SetupObjectTextureAndSampler;
  

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

  return false;
}
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool CreateDepthBufferImage (rokz::Image&          depth_image,
                             rokz::ImageView&      depth_imageview,
                             //rokz::SwapchainGroup& scg,
                             VkSampleCountFlagBits msaa_samples, 
                             VkFormat              depth_format,
                             const rokz::CommandPool& command_pool,
                             const VkQueue&        queue, 
                             const VkExtent2D&     ext,
                             const VmaAllocator&   allocator,
                             const rokz::Device&   device)
{
  printf ("%s\n", __FUNCTION__); 

  //rokz::SwapchainGroup& scg = glob.swapchain_group;
  
  //
  // uint32_t wd = scg.swapchain.ci.imageExtent.width; 
  // uint32_t ht = scg.swapchain.ci.imageExtent.height;   

  rokz::cx::CreateInfo_2D_depthstencil (depth_image.ci,
                                        depth_format, 
                                        msaa_samples,
                                        ext.width, ext.height);

  rokz::cx::AllocCreateInfo_device (depth_image.alloc_ci); 
  rokz::cx::CreateImage (depth_image, allocator);

  rokz::cx::CreateInfo (depth_imageview.ci, VK_IMAGE_ASPECT_DEPTH_BIT, depth_image); 
  rokz::cx::CreateImageView (depth_imageview, depth_imageview.ci, device.handle);

  // manual transition b/c dynamic_rendering
  rokz::cx::TransitionImageLayout (depth_image.handle, depth_format,
                                   VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                   queue, device.command_pool.handle, device.handle);

  return true;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupDarkDepthBuffer (Glob& glob) {

  printf ("%s\n", __FUNCTION__); 

  rokz::SwapchainGroup& scg = glob.swapchain_group;

  //
  uint32_t wd = scg.swapchain.ci.imageExtent.width; 
  uint32_t ht = scg.swapchain.ci.imageExtent.height;   

  if (rokz::ut::FindDepthFormat (glob.depth_format, glob.physical_device.handle)) {

    rokz::cx::CreateInfo_2D_depthstencil (glob.depth_image.ci,
                                      glob.depth_format, 
                                      glob.msaa_samples,
                                      wd, ht);

    rokz::cx::AllocCreateInfo_device (glob.depth_image.alloc_ci); 
    rokz::cx::CreateImage (glob.depth_image, glob.device.allocator.handle);

    rokz::cx::CreateInfo (glob.depth_imageview.ci, VK_IMAGE_ASPECT_DEPTH_BIT, glob.depth_image); 
    rokz::cx::CreateImageView (glob.depth_imageview, glob.depth_imageview.ci, glob.device.handle);

    // manual transition b/c dynamic_rendering
    rokz::cx::TransitionImageLayout (glob.depth_image.handle, glob.depth_format,
                                 VK_IMAGE_LAYOUT_UNDEFINED,
                                 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                 glob.device.queues.graphics, glob.device.command_pool.handle, glob.device.handle);

  } 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool CreateMSAAColorImage  (rokz::Image&          color_image, 
                            rokz::ImageView&      color_imageview, 
                            VkSampleCountFlagBits msaa_samples,
                            VkFormat              image_format,
                            const VmaAllocator&   allocator, 
                            const rokz::CommandPool& command_pool, 
                            const VkQueue&        queue, 
                            const VkExtent2D&     ext,
                            const rokz::Device&   device) {

  printf ("%s\n", __FUNCTION__); 
  rokz::cx::CreateInfo_2D_color_target (color_image.ci, image_format, msaa_samples, ext.width, ext.height);

  rokz::cx::AllocCreateInfo_device (color_image.alloc_ci);
  rokz::cx::CreateImage (color_image, allocator);

  // imageview 
  rokz::cx::CreateInfo (color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, color_image);
  rokz::cx::CreateImageView (color_imageview, color_imageview.ci, device.handle);
  // dynamic_rendering
  rokz::cx::TransitionImageLayout (color_image.handle, image_format,
                               VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                               queue, device.command_pool.handle, device.handle);
  return true;

}

// --------------------------------------------------------------------
// VMA
// --------------------------------------------------------------------
void SetupDarkMultisampleColorResource (Glob& glob) {

  printf ("%s\n", __FUNCTION__); 

  rokz::Swapchain& swapchain = glob.swapchain_group.swapchain; 
  
  VkExtent2D& swapchain_ext    = swapchain.ci.imageExtent;
  VkFormat    swapchain_format = swapchain.ci.imageFormat; 

  rokz::cx::CreateInfo_2D_color_target (glob.msaa_color_image.ci, swapchain_format,
                                    glob.msaa_samples,swapchain_ext.width, swapchain_ext.height);

  rokz::cx::AllocCreateInfo_device (glob.msaa_color_image.alloc_ci);
  rokz::cx::CreateImage (glob.msaa_color_image, glob.device.allocator.handle);

  // imageview 
  rokz::cx::CreateInfo (glob.msaa_color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.msaa_color_image);
  rokz::cx::CreateImageView (glob.msaa_color_imageview, glob.msaa_color_imageview.ci, glob.device.handle);
  // dynamic_rendering
  rokz::cx::TransitionImageLayout (glob.msaa_color_image.handle, swapchain_format,
                               VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                               glob.device.queues.graphics, glob.device.command_pool.handle, glob.device.handle);

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void CleanupDarkroot (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 

  for (auto& ub : glob.vma_uniform_buffs) {
    rokz::cx::Destroy (ub, glob.device.allocator.handle); 
  }

  for (auto buf : glob.vma_objparam_buffs) {  
    rokz::cx::Destroy (buf, glob.device.allocator.handle);
  }

  rokz::cx::Destroy (glob.sampler, glob.device.handle); 
  rokz::cx::Destroy (glob.descr_pool, glob.device.handle); 
  rokz::cx::Destroy (glob.obj_pipeline.descrgroup, glob.device.handle); 
  rokz::cx::Destroy (glob.texture_imageview, glob.device.handle);

  rokz::cx::Destroy (glob.texture_image, glob.device.allocator.handle);
  rokz::cx::Destroy (glob.vma_vb_device, glob.device.allocator.handle);
  rokz::cx::Destroy (glob.vma_ib_device, glob.device.allocator.handle);
  
  Cleanup (glob.obj_pipeline.pipeline.handle,
           glob.swapchain_group.imageviews,

           glob.swapchain_group.swapchain,
           glob.surface,
           glob.device.command_pool.handle,
           glob.frame_sequence.syncs, 
           glob.obj_pipeline.pipeline.shader_modules,
           glob.obj_pipeline.pipeline.layout.handle, 
           glob.msaa_color_image, glob.msaa_color_imageview,

           glob.depth_image, glob.depth_imageview,

           glob.window.glfw_window,
           glob.device,
           glob.device.allocator.handle, 
           glob.instance.handle);

  glfwTerminate();
}

// --------------------------------------------------------------------
// SetupDarkDescriptorSetLayout (
// --------------------------------------------------------------------
bool SetupObjectDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

  printf ("%s", __FUNCTION__); 

  //  UniformBinding
  //  SamplerBinding
  descrgroup.dslayout.bindings.resize (3);
  //rokz::Init (glob.desc_set_layout_bindings[0],

  // MVPTransform
  rokz::cx::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[0],
                                    0,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    1,
                                    VK_SHADER_STAGE_VERTEX_BIT);

  // SceneObjParams
  rokz::cx::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[1],
                                    1,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    128, 
                                    VK_SHADER_STAGE_VERTEX_BIT);

  // sammpler+image
  rokz::cx::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[2],
                                    2,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                    1, 
                                    VK_SHADER_STAGE_FRAGMENT_BIT);

  if (!rokz::cx::CreateDescriptorSetLayout (descrgroup.dslayout.handle,
                                        descrgroup.dslayout.ci,
                                        descrgroup.dslayout.bindings,
                                        device.handle)) {
    printf (" --> [FAILED] \n"); 
    return false;
  }

  printf (" --> true\n"); 
  return true; 
}



  

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool SetupObjectDescriptorSets (rekz::PipelineGroup& pipelinegroup,

                                const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                const rokz::ImageView& texture_imageview, 
                                const rokz::Sampler&   sampler, 

                                const rokz::DescriptorPool& descpool,
                                const rokz::Device&         device) {

  printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

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

  for (uint32_t i = 0; i < kMaxFramesInFlight; i++) {
    // wtf does this do
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer     = vma_uniform_buffs[i].handle;
    buffer_info.offset     = 0;
    buffer_info.range      = vma_uniform_buffs[i].ci.size ;
    
    std::vector<VkDescriptorBufferInfo>  objparams (kSceneObjCount, VkDescriptorBufferInfo {});
    for (size_t iobj = 0; iobj < objparams.size (); ++iobj) { 
      objparams[iobj].buffer   = vma_objparam_buffs[i].handle; //
      objparams[iobj].offset   = iobj * sizeof(SceneObjParam);         // min_uniform_buffer_offset_alignment ??
      objparams[iobj].range    = sizeof(SceneObjParam) ;            //glob.vma_objparam_buffs[i].ci.size;
    }
    
    //buffer_info.range      = glob.uniform_buffers[i].create_info.size ;
    VkDescriptorImageInfo image_info {};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    image_info.imageView   = texture_imageview.handle;
    image_info.sampler     = sampler.handle;
  //
    std::array<VkWriteDescriptorSet, 3>  descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;    
    descriptor_writes[0].dstSet           = dg.descrsets[i];
    descriptor_writes[0].dstBinding       = 0;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &buffer_info;
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 

    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = dg.descrsets[i];
    descriptor_writes[1].dstBinding       = 1;
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[1].descriptorCount  = objparams.size(); // <
    descriptor_writes[1].pBufferInfo      = &objparams[0]; 
    descriptor_writes[1].pImageInfo       = nullptr; 
    descriptor_writes[1].pTexelBufferView = nullptr; 

    descriptor_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[2].pNext            = nullptr;    
    descriptor_writes[2].dstSet           = dg.descrsets[i];
    descriptor_writes[2].dstBinding       = 2;
    descriptor_writes[2].dstArrayElement  = 0;
    descriptor_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[2].descriptorCount  = 1;
    descriptor_writes[2].pBufferInfo      = nullptr;
    descriptor_writes[2].pImageInfo       = &image_info; 
    descriptor_writes[2].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;
  
}  
// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool SetupObjDescriptorPool (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 
  //SetupDescriptorPool (glob.descr_pool, glob.device);
  rokz::DescriptorPool& dp = glob.descr_pool;
  
  dp.sizes.resize (3); 
  dp.sizes[0] = {} ; //
  dp.sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  dp.sizes[0].descriptorCount = static_cast<uint32_t>(kMaxFramesInFlight);

  dp.sizes[1] = {} ; //
  dp.sizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  dp.sizes[1].descriptorCount = static_cast<uint32_t>( 128 * kMaxFramesInFlight);

  dp.sizes[2] = {} ; //
  dp.sizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  dp.sizes[2].descriptorCount = static_cast<uint32_t>(kMaxFramesInFlight);

  rokz::cx::CreateInfo ( dp.ci, kMaxFramesInFlight, dp.sizes); 
  
  if (!rokz::cx::CreateDescriptorPool (dp, glob.device.handle)) {
    printf ("[FAILED] %s", __FUNCTION__);
    return false; 
  }

 return true; 
}

 // ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupObjectShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {

  printf ("%s \n", __FUNCTION__); 

  
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  std::vector<rokz::ShaderModule>&              shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (2);
  shader_stage_create_infos.resize(2);
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"data/shader/darkroot_vertex.spv" ;

  if (!rokz::CreateShaderModule (shader_modules[0], vert_file_path.string(), device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].handle); 
  
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"data/shader/darkroot_fragment.spv" ;

  if (!rokz::CreateShaderModule (shader_modules[1], frag_file_path.string(), device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[1].handle); 
  //
  return true; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupObjectPipeline (rekz::PipelineGroup& pipelinegroup,
                          // const rokz::RenderPass& renderpass,  // using dynamic render pass
                          const std::filesystem::path& fspath,
                          const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          VkFormat color_format,
                          VkFormat depth_format,
                          const rokz::Device& device) {

  SetupObjectDescriptorLayout;
  SetupObjectShaderModules;
  
  //
  SetupObjectShaderModules (pipelinegroup.pipeline, fspath, device);

  //
  rokz::Pipeline& pipeline = pipelinegroup.pipeline;
  rekz::SetupViewportState (pipeline.state.viewport, viewport_extent); 

  pipeline.state.colorblend_attachments.resize (1);

  rokz::ColorBlendState_default (pipeline.state.colorblend_attachments[0]); 
  rokz::DynamicState_default (pipeline.state.dynamics); 
  //
  rokz::PipelineStateCreateInfo& psci = pipelinegroup.pipeline.state.ci;
  rokz::CreateInfo (psci.tesselation, 69); 
  rokz::CreateInfo (psci.dynamicstate, pipeline.state.dynamics); 
  rokz::CreateInfo (psci.vertexinputstate, kDarkVertexBindingDesc, kDarkvertBindingAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, pipeline.state.viewport);
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, pipeline.state.colorblend_attachments); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 
  SetupObjectDescriptorLayout (pipelinegroup.descrgroup, device); 

  //
  rokz::CreateGraphicsPipelineLayout (pipelinegroup.pipeline.layout.handle,
                                      pipelinegroup.pipeline.layout.ci,
                                      sizeof(darkroot::PushConstants), 
                                      pipelinegroup.descrgroup.dslayout.handle,
                                      device.handle);
  
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = color_format;

  rokz::CreateInfo  (pipeline.ext.pipeline_rendering.ci,
                     pipeline.ext.pipeline_rendering.color_formats,
                     depth_format); 

  
  //
  rokz::CreateInfo (pipelinegroup.pipeline.ci,
                    pipelinegroup.pipeline.layout.handle,
                    &pipeline.ext.pipeline_rendering.ci,                    
                    psci.shader_stages,       //const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
                    &psci.input_assembly,     //const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                    &psci.vertexinputstate, // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                    &psci.viewport_state,     //const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                    nullptr,                 // tesselation 
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
glm::vec3& unit_angle_xz (glm::vec3& v, float theta) {
  v.x = cos (theta) ;
  v.y = 0.0f;
  v.z = -sinf (theta) ;
  return v; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateDarkUniforms (Glob& glob, uint32_t current_frame, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.sim_time += dt;
  //  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);

  float sim_timef = glob.sim_time;
  float dtF = static_cast <float> (dt);
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

  memcpy (rokz::cx::MappedPointer (glob.vma_uniform_buffs[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 
 
  if (SceneObjParam* obj = reinterpret_cast<SceneObjParam*> (rokz::cx::MappedPointer (glob.vma_objparam_buffs[current_frame]))) {
  
    glm::vec3 va, vb;
    unit_angle_xz (va, 5.0 * sim_timef ); 
    unit_angle_xz (vb, 5.0 * sim_timef + kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));

    
    glob.obj_theta[0] += glob.input_state.mouse.dx * dtF * darkroot::k2Pi;
    
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    obj[0].modelmat = glm::rotate(model0, glob.obj_theta[0], glm::vec3(0.0f, -1.0f, 0.0f));
//obj[0].modelmat = glm::rotate(model0, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    obj[1].modelmat = glm::rotate(model1, sim_timef * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
  }
}



// ---------------------------------------------------------------------
// RecordDarkCommandBuffer_indexed
// ---------------------------------------------------------------------
bool RecordDynamicRenderPass (Glob& glob, 
                              VkCommandBuffer        &command_buffer,
                              const rokz::Pipeline&  pipeline,
                              const VkDescriptorSet& desc_set, 
                              const VkBuffer&        vertex_buffer, 
                              const VkBuffer&        index_buffer, 
                              const VkExtent2D&      ext2d,
                              const rokz::Device&    device) {

  const DarkMesh& darkmesh = DarkOctohedron ();
  
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.flags = 0;                  // 
  begin_info.pInheritanceInfo = nullptr; // 

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
     printf ("failed to begin recording command buffer!");
     return false; 
  }

    
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


  // 
  // [glob.rendering_info_group.ri] is updated in the calling function
  //
  vkCmdBeginRendering (command_buffer, &glob.rendering_info_group.ri);
  // vkCmdBeginRenderPass (command_buffer, &pass_info, VK_SUBPASS_CONTENTS_INLINE);
  
  vkCmdBindPipeline (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle);

  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  vkCmdSetScissor(command_buffer, 0, 1, &scissor);

  vkCmdBindDescriptorSets (command_buffer,
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipeline.layout.handle,
                           0,
                           1,
                           &desc_set, //&descriptorSets[currentFrame],
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
                        pipeline.layout.handle,
                        shader_stages,
                        0,
                        sizeof(darkroot::PushConstants),
                        &pcs);

    vkCmdDrawIndexed (command_buffer, darkmesh.indices.size(), 1, 0, 0, 0);
  }


  vkCmdEndRendering (command_buffer);
  // vkCmdEndRenderPass

  //
  if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("[FAILED] record command buffer\n");
    return false; 
  }
  
  //printf ("BAI %s\n", __FUNCTION__); 
  return true;
}

// ---------------------------------------------------------------------
// RenderFrame_dynamic <-- RecordDynamicRenderPass 
// ---------------------------------------------------------------------
bool RenderFrame_dynamic (Glob&                   glob,
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

  rokz::RenderSync&               render_sync            = glob.frame_sequence.syncs[curr_frame];
    // glob.syncs;

  VkResult acquire_res = rokz::cx::AcquireFrame (glob.swapchain_group.swapchain, render_sync, image_index, device); 
  
  if (acquire_res == VK_ERROR_OUT_OF_DATE_KHR || acquire_res == VK_SUBOPTIMAL_KHR || resize) {
    resize = false; 
    return darkroot::RecreateSwapchain (swapchain, 
                                        swapchain_images, swapchain_imageviews,
                                        depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                        msaa_color_image, msaa_color_imageview,
                                        allocator, window.glfw_window, device);
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
  
  VkCommandBuffer&   command_buffer = glob.frame_sequence.command_buffers[curr_frame]; 
  rokz::Buffer&      vma_vb_device  = glob.vma_vb_device;
  rokz::Buffer&      vma_ib_device  = glob.vma_ib_device; 

  vkResetCommandBuffer (command_buffer, 0); //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);

  //RecordDarkRenderPass (command_buffer,
  // RecordDarkRenderPaass
  UpdateDynamicRenderingInfo (glob, image_index); 

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
  VkSemaphore wait_semaphores[]      = {render_sync.image_available_sem};
  VkSemaphore signal_semaphores[]    = {render_sync.render_finished_sem }; 
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submit_info.pWaitDstStageMask    = wait_stages;

  submit_info.waitSemaphoreCount   = 1;
  submit_info.pWaitSemaphores      = wait_semaphores;
  submit_info.signalSemaphoreCount = 1; 
  submit_info.pSignalSemaphores    = signal_semaphores; 
  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &command_buffer; // &glob.command_buffer_group.buffers[curr_frame];

  if (vkQueueSubmit (glob.device.queues.graphics, 1, &submit_info, render_sync.in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }

  //  dynamic_rendering now, we have to manually transition
  rokz::cx::TransitionImageLayout (glob.swapchain_group.images[image_index].handle,
                               swapchain.ci.imageFormat,
                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                               VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                               glob.device.queues.graphics, glob.device.command_pool.handle, device.handle);

  
  return rokz::cx::PresentFrame (glob.device.queues.present, glob.swapchain_group.swapchain, image_index, glob.frame_sequence.syncs[curr_frame]); 
  
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool InitializeInstance ();
bool InitializeSurface  ();
//bool SelectPhysicalDevice ();
bool IniitializeDevice  ();



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool InitializeSwapchain (darkroot::Glob& glob) {

  // rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info,
  //                              glob.surface, glob.physical_device.handle);

  // rokz::SwapchainGroup& scg = glob.swapchain_group;

  // scg.swapchain.family_indices.push_back (glob.physical_device.family_indices.graphics.value());
  // scg.swapchain.family_indices.push_back (glob.physical_device.family_indices.present.value ());
  
  // rokz::cx::CreateInfo_default (scg.swapchain.ci,  
  //                               scg.swapchain.family_indices,
  //                               glob.surface,
  //                               kTestExtent, 
  //                               glob.swapchain_support_info, 
  //                               glob.physical_device);

  // glob.surface_format.format =  scg.swapchain.ci.imageFormat;
  
  // rokz::cx::CreateSwapchain (scg.swapchain, glob.device); 
  
  // rokz::cx::GetSwapChainImages (scg.images, scg.swapchain, glob.device.handle); 
  // rokz::cx::CreateImageViews       (scg.imageviews, scg.images, glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);
  // printf ("transitions require commands buffers [%i]\n", __LINE__);

  return false;
}

bool CreateAttachementSet () {

  // the canvas
  return false; 
}
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int darkroot_basin (const std::vector<std::string>& args) {

  printf ( " Mv = v is the correct order \n"); 


  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 

  Glob  glob; // *globmem; // something representing the app state
  
  glob.dt = 0.0;
  glob.obj_theta[0] =   0.0;
  glob.obj_theta[1] =   0.0;
  //std::shared_ptr<Glob> globmem = std::make_shared<Glob> ();

  auto dark_path = std::filesystem::path ( "/home/djbuzzkill/owenslake/rokz/");
  //Default (glob); 
  
  glfwInit();
  glob.input_state.fb_resize = false; 
  
  //rokz::CreateWindow_glfw (glob.glfwin);
  SetupDarkWindow (glob); 

  // CREATE INSTANCE AND DEVICE
  //bool rekz::InitializeInstance (rokz::Instance& instance) {

  rokz::InitializeInstance (glob.instance); 

  rokz::cx::CreateSurface  (&glob.surface, glob.window.glfw_window, glob.instance.handle);
  rokz::cx::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance.handle);
  //
  // InitializeSwapchain ();
  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info,
                                   glob.surface,
                                   glob.physical_device.handle);
  // VkSurfaceFormatKHR swap_surface_format = rokz::cx::ChooseSwapSurfaceFormat (glob.swapchain_support_info.formats);
  // VkPresentModeKHR   present_mode        = rokz::cx::ChooseSwapPresentMode   (glob.swapchain_support_info.present_modes);
  rokz::ConfigureDevice    (glob.physical_device , VK_TRUE);
  rokz::InitializeDevice (glob.device, glob.physical_device, glob.instance);
  
  // put these somwehere
  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.physical_device); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.physical_device.handle);

  rokz::SwapchainGroup& scg = glob.swapchain_group;

  // InitializeSwapchain ()
  rokz::InitializeSwapchain (scg, glob.swapchain_support_info, glob.surface,
                             kTestExtent, glob.physical_device, glob.device);

#ifdef DARKROOT_ENABLE_GRID

  SetupGridscapePipeline (glob.grid_pipeline, glob.grid_pipeline.pipeline.state.viewport, 
                          glob.render_pass, dark_path, glob.swapchain_group.swapchain,
                          glob.msaa_samples, glob.device); 

#endif

  SetupObjectPipeline (glob.obj_pipeline, dark_path, glob.swapchain_group.swapchain.ci.imageExtent,
                       glob.msaa_samples, scg.swapchain.ci.imageFormat, glob.depth_format, glob.device); 

  //SetupDarkMultisampleColorResource (glob);
  CreateMSAAColorImage  (glob.msaa_color_image, glob.msaa_color_imageview, glob.msaa_samples,
                         scg.swapchain.ci.imageFormat, glob.device.allocator.handle, glob.device.command_pool, 
                         glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent, glob.device);

  // VkExtent2D& swapchain_ext    = swapchain.ci.imageExtent;
  // VkFormat    swapchain_format = swapchain.ci.imageFormat; 
  //SetupDarkDepthBuffer (glob);
  CreateDepthBufferImage (glob.depth_image, glob.depth_imageview, glob.msaa_samples, glob.depth_format, 
                          glob.device.command_pool, glob.device.queues.graphics, glob.swapchain_group.swapchain.ci.imageExtent,
                          glob.device.allocator.handle, glob.device);

  // for BeginRendering ()
  SetupDynamicRenderingInfo (glob) ; 

  SetupObjResources (glob);
  if (!SetupObjDescriptorPool (glob)) {
    printf ("[FAILED] --> SetupGlobalDescriptorPool \n"); 
    return false;
  }

  // * DescriptorLayout is created in SetupObjectPipeline this is done here
  // * b/c SetupGrlobalDescriptorPool is just barely above us

  // this is where all the VkWriteDescriptorSet's r
  if (!SetupObjectDescriptorSets (glob.obj_pipeline,            glob.vma_uniform_buffs,
                                  glob.vma_objparam_buffs,      glob.texture_imageview,
                                  glob.sampler,glob.descr_pool, glob.device)) {
    printf ("[FAILED] --> SetupObjectDescriptorSets \n"); 
    return false;
  }
  
  // items per frames 
  //scg.command_buffer_group.buffers.resize (kMaxFramesInFlight);
  rokz::FrameSequencing& frameseq = glob.frame_sequence;
  // !! 
  frameseq.command_buffers.resize (kMaxFramesInFlight);
  frameseq.syncs.resize           (kMaxFramesInFlight);
  rokz::cx::AllocateInfo (frameseq.command_buffer_alloc_info, glob.device.command_pool.handle); 

  for (size_t i = 0; i < Glob::MaxFramesInFlight; ++i) {
    // ^^ 'CreateCommandBuffers' should be called, we call it 
    rokz::cx::CreateCommandBuffer(frameseq.command_buffers[i], frameseq.command_buffer_alloc_info,glob.device.handle);
    rokz::cx::CreateRenderSync (frameseq.syncs[i], frameseq.syncs[i].ci, glob.device.handle);
  }

  // RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION 
  // RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION 
  // RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION RENDER LOOP SECTION 
  const double time_per_frame_sec = 1.0 / 60.0;
  glob.dt = time_per_frame_sec; // just do this for now
  
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

  auto then = t0; 
  while (countdown && run && !glfwWindowShouldClose(glob.window.glfw_window)) {

    glfwPollEvents(); 
    //start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();    
    //dt = -0.000001 * std::chrono::duration_cast<std::chrono::microseconds>(then - now).count (); 
    glob.dt = std::chrono::duration<double, std::chrono::seconds::period>(now - then).count();
    
    //UpdateInput(glob, glob.dt);
    if (glob.input_state.keys.count (GLFW_KEY_Q))
      run = false;

    //    UpdateDarkroot(glob, glob.dt);
    //    result = RenderFrame (glob, curr_frame, fb_resize, glob.dt);
    uint32_t image_index; 
    if (RenderFrame_dynamic (glob, image_index, glob.input_state.fb_resize, glob.obj_pipeline.pipeline,
                             glob.obj_pipeline.descrgroup.descrsets[curr_frame], curr_frame, glob.dt)) {
      // no render pass of framebuffer
    }
    else {
      run = false;
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
  
  //  globmem.reset ();
  return 0;
}

