
#include "rekz.h"
#include "darkrootgarden.h"
// 
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vulkan/vulkan_core.h>

//#define VMA_IMPLEMENTATION
#include "rekz/dark_types.h"
#include "rokz/command.h"
#include "rokz/descriptor.h"
#include "rokz/shared_types.h"
#include "vk_mem_alloc.h"
 
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
#define ROKZ_USE_VMA_ALLOCATION 1


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
  sizeof (DarkrootVert),       // stride      
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
    offsetof(DarkrootVert, pos),  // .offset   
  },
  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(DarkrootVert, nrm), 
  },

  VkVertexInputAttributeDescription { // color
    2,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(DarkrootVert, col), 
  },
  VkVertexInputAttributeDescription { // tex coord
    3,                             
    0, 
    VK_FORMAT_R32G32_SFLOAT,
    offsetof(DarkrootVert, txc0), 
  }
}; 




// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
auto DarkRenderable::SetupRS (VkCommandBuffer commandbuffer) -> int {
  return 0; 
}


// --------------------------------------------------------------------
auto DarkRenderable::Draw (VkCommandBuffer commandbuffer) -> void  {
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
auto DarkRenderable::AllocRes (VmaAllocator& allocator) -> int {
    
  rokz::CreateInfo_VB_device (vb_dev.ci, DarkrootMesh::VertexSize,  num_verts);
  rokz::AllocCreateInfo_device (vb_dev.alloc_ci); 
  rokz::CreateBuffer (vb_dev, allocator); 
    
  rokz::CreateInfo_IB_16_device (ib_dev.ci, num_inds); 
  rokz::AllocCreateInfo_device (ib_dev.alloc_ci);
  rokz::CreateBuffer (ib_dev, allocator);

  return 0; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
auto DarkRenderable::FreeRes (VmaAllocator& alloc) -> int {

  rokz::Destroy (vb_dev, alloc);
  rokz::Destroy (ib_dev, alloc);

  return 0; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
constexpr size_t SizeOfDarkRenderable () {
  return sizeof (DarkRenderable); 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
DarkRenderable& Initialize (DarkRenderable& dr, uint32_t nv, uint32_t ni) {

  new (&dr) DarkRenderable (nv, ni);
  return dr;
}

DarkRenderable& Friendly ( DarkRenderable& dr) {

  Initialize (dr, 0, 0);
  // dr.num_verts = 0;
  // dr.num_inds = 0;
  // dr.vb_dev;
  // dr.ib_dev;
  return dr;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
Glob::Glob()
  : instance()
  , device ()
  , dt ()
  , allocator()
  , depth_image()
  , depth_imageview()
  , msaa_samples ()
  , vma_ib_device()
  , vma_vb_device()
  , command_pool()
  , texture_image()
  , texture_imageview()
  , sampler()
  , vma_uniform_buffs()
  , descr_pool()
  , window()
  , physical_device()
  , queue_priority()
  , swapchain_support_info()
  , surface(nullptr)
  , sim_time(0.0)
{ 
  queues.graphics = {};
  queues.present = {}; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupDarkrootWindow (Glob& glob) {
  rokz::CreateWindow (glob.window, kTestExtent.width , kTestExtent.height, "wut"); 

  glfwSetFramebufferSizeCallback (glob.window.glfw_window, win_event::on_resize ); 
  glfwSetKeyCallback (glob.window.glfw_window, win_event::on_keypress);
  glfwSetCursorPosCallback(glob.window.glfw_window, win_event::on_mouse_move);
  glfwSetMouseButtonCallback(glob.window.glfw_window, win_event::on_mouse_button);
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


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool LoadTexture_color_sampling (rokz::Image&        image,
                           VkFormat            format,
                           const VkExtent2D&   ext2d,
                           const void*         srcimage,
                           const VmaAllocator& allocator, 
                           const VkQueue&      queue, 
                           const rokz::CommandPool& commandpool, 
                           const rokz::Device& device) {

  //size_t image_size = image_width * image_height *  bytes_per_pixel; 
  auto image_size = SizeOfComponents (format)
                  * NumberOfComponents (format)
                  * ext2d.width * ext2d.height;
  assert (image_size); 

  rokz::Buffer stage_buff; 
  
  rokz::CreateInfo_buffer_stage (stage_buff.ci, image_size);
  rokz::AllocCreateInfo_stage (stage_buff.alloc_ci);
  rokz::CreateBuffer   (stage_buff, allocator); 

  void* mapped = nullptr; 
  if (rokz::MapMemory (&mapped, stage_buff.allocation, allocator)) { 
  
    const uint8_t* image_data = reinterpret_cast<const unsigned char*> (srcimage); 
    std::copy (image_data, image_data + image_size, reinterpret_cast<uint8_t*> (mapped));
  }
  rokz::UnmapMemory (stage_buff.allocation, allocator);

  rokz::CreateInfo_2D_color_sampling  (image.ci, VK_SAMPLE_COUNT_1_BIT, ext2d.width, ext2d.height);
  rokz::AllocCreateInfo_device (image.alloc_ci);
  if (!rokz::CreateImage (image, allocator)) {
    printf ("[FAILED] %s setup test texture", __FUNCTION__);
    return false;
  }

  //VK_FORMAT_R8G8B8A8_SRGB
  rokz::TransitionImageLayout (image.handle, format, VK_IMAGE_LAYOUT_UNDEFINED,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               queue, commandpool.handle, device.handle);

  rokz::CopyBufferToImage (image.handle, stage_buff.handle, ext2d.width, ext2d.height,
                           queue, commandpool.handle, device.handle);

  rokz::TransitionImageLayout (image.handle, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                               queue, commandpool.handle, device.handle);

  rokz::Destroy (stage_buff, allocator); 
  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupDarkUniforms (Glob& glob) {
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


    rokz::CreateInfo_uniform (uniform_buffs[i].ci, rokz::kSizeOf_MVPTransform, 1); 
    rokz::AllocCreateInfo_mapped (uniform_buffs[i].alloc_ci); 
    if (!rokz::CreateBuffer (uniform_buffs[i], glob.allocator)) {
      printf (" --> [FAIL]  create MVPTransform \n"); 
      return false; 
    }

    rokz::CreateInfo_uniform (objparams[i].ci, SizeOf_SceneObjParam, 128);
    rokz::AllocCreateInfo_mapped (objparams[i].alloc_ci);
    if (!rokz::CreateBuffer (objparams[i], glob.allocator)) {
      printf (" --> [FAIL]  create SceneObjParam \n"); 
      return false; 
    }

  }

  printf (" --> [true] \n"); 
  return true; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupDarkSampler (Glob& glob) {
  printf ("%s \n", __FUNCTION__); 

  rokz::CreateInfo (glob.sampler.ci, glob.physical_device.properties);
  
  rokz::CreateSampler (glob.sampler, glob.device.handle);  
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupDarkTexture (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 
  //rokz::ReadStreamRef rs = rokz::CreateReadFileStream (data_root + "/texture/blue_0_texture.png"); 
  const char*  test_image_files[] = { 
    "out_0_blue-texture-image-hd_rgba.png",
    "out_1_abstract-texture-3_rgba.png",
  };

   rokz::Buffer stage_image; 
  //   const std::string fq_test_file = data_root + "/texture/out_1_abstract-texture-3_rgba.png";  
   const std::string fq_test_file = data_root + "/texture/out_0_blue-texture-image-hd_rgba.png";  

   ilInit ();
   ilBindImage (ilGenImage ());

   int image_width    = 0;
   int image_height   = 0;
   int image_depth    = 0;
   int bytes_per_pixel= 0;
   int image_bpp      = 0;
   int image_type     = 0;
   int image_format   = 0;
   
   bool result = false;
   printf ("loading.. %s ", fq_test_file.c_str()); 
   if (ilLoadImage(fq_test_file.c_str())) {

     printf ("succeeded\n"); 
     image_width    = ilGetInteger (IL_IMAGE_WIDTH); 
     image_height   = ilGetInteger (IL_IMAGE_HEIGHT);
     image_depth    = ilGetInteger (IL_IMAGE_DEPTH);
     bytes_per_pixel= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
     image_bpp      = ilGetInteger (IL_IMAGE_BPP);
     image_type     = ilGetInteger (IL_IMAGE_TYPE);
     image_format   = ilGetInteger (IL_IMAGE_FORMAT); 

     printf ( "image dim [w:%i, h:%i, d:%i | bpp:%i, bytes:%i, type:%x, format:%x]\n",
              image_width, image_height, image_depth,
              image_bpp, bytes_per_pixel, image_type, image_format); 

     if (LoadTexture_color_sampling (glob.texture_image, VK_FORMAT_R8G8B8A8_SRGB ,
                                     VkExtent2D{ (uint32_t)image_width, (uint32_t) image_height},
                                     ilGetData (), glob.allocator, glob.queues.graphics, 
                                     glob.command_pool, glob.device)) {

       ilDeleteImage (ilGetInteger (IL_ACTIVE_IMAGE)); 
       result = true; 
     }
     
   }

   ilShutDown ();

   if (result) {
     // void SetupDarkTextureImageView (Glob& glob) {
     result = false;
     rokz::CreateInfo (glob.texture_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.texture_image);  
     if (VK_SUCCESS == vkCreateImageView(glob.device.handle, &glob.texture_imageview.ci, nullptr, &glob.texture_imageview.handle)) {
       result = true; 
     }
     else {
       printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
     }
   }


   if (!result) {
     printf ("%s [FAILED] --> false\n", __FUNCTION__); 
   }
   
   return result; 
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#ifdef DARKROOT_DYNAMIC_RENDER_ENABLE


bool SetupDynamicRenderingInfo (darkroot::Glob& glob) {

  RenderingInfoGroup& rig = glob.rendering_info_group;
  
  rig.clear_colors.resize (1);
  rig.color_attachment_infos.resize (1);

  rig.clear_colors[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  rig.clear_depth.depthStencil = {1.0f, 0};

  rokz::AttachmentInfo (rig.color_attachment_infos[0],
                        glob.msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT,
                        glob.swapchain_group.imageviews[0].handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, rig.clear_colors[0]);

  // rokz::AttachmentInfo (rig.color_attachment_infos[1],
  //                       glob.swapchain_group.imageviews[0].handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  //                       VK_RESOLVE_MODE_NONE, 
  //                       nullptr, VK_IMAGE_LAYOUT_UNDEFINED,
  //                       VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, rig.clear_colors[1]);
  
  rokz::AttachmentInfo (rig.depth_attachment_info,
                        glob.depth_imageview.handle, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_NONE,
                        nullptr, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, rig.clear_depth);
  
  rig.render_area =  { 
    VkOffset2D {0, 0}, glob.swapchain_group.swapchain.ci.imageExtent
  };

  rokz::RenderingInfo (rig.ri, rig.render_area, 1, 0, rig.color_attachment_infos, &rig.depth_attachment_info, nullptr);

  
  return true;

}
#endif


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void UpdateDynamicRenderingInfo (darkroot::Glob& glob, uint32_t image_index) {
  //printf ("%s\n", __FUNCTION__); 
  RenderingInfoGroup& rig = glob.rendering_info_group;

  rokz::AttachmentInfo (rig.color_attachment_infos[0],
                        glob.msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT, glob.swapchain_group.imageviews[image_index].handle,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
                        VK_ATTACHMENT_STORE_OP_STORE, rig.clear_colors[0]);

  // rokz::AttachmentInfo (rig.color_attachment_infos[1],
  //                       glob.swapchain_group.imageviews[image_index].handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  //                       VK_RESOLVE_MODE_NONE, 
  //                       nullptr, VK_IMAGE_LAYOUT_UNDEFINED,
  //                       VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, rig.clear_colors[1]);


// VUID-vkCmdDrawIndexed-pDepthAttachment-06181(ERROR / SPEC): msgNum: -1016735096 - Validation Error:
// [ VUID-vkCmdDrawIndexed-pDepthAttachment-06181 ] Object 0: handle = 0x56169b0053e0, type = VK_OBJECT_TYPE_COMMAND_BUFFER;
// | MessageID = 0xc365da88 | vkCmdDrawIndexed:
  "Depth attachment imageView format (VK_FORMAT_D32_SFLOAT) must match corresponding format in pipeline\
  (VK_FORMAT_UNDEFINED) The Vulkan spec states: If the current render pass instance was begun with vkCmdBeginRendering\
  and VkRenderingInfo::pDepthAttachment->imageView was not VK_NULL_HANDLE, the value of\
  VkPipelineRenderingCreateInfo::depthAttachmentFormat used to create the currently bound\
  graphics pipeline must be equal to the VkFormat used to create VkRenderingInfo::pDepthAttachment->imageView\
  (https://www.khronos.org/registry/vulkan/specs/1.3-extensions/html/vkspec.html#VUID-vkCmdDrawIndexed-pDepthAttachment-06181)";
//     Objects: 1


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SetupObjResources (darkroot::Glob& glob) { 



  const DarkrootMesh& darkmesh = darkroot::DarkOctohedron (); 

  void* pmapped  = nullptr;
  rokz::Buffer vb_x; 
  rokz::CreateInfo_VB_stage (vb_x.ci, DarkrootMesh::VertexSize, darkmesh.verts.size());
  rokz::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::CreateBuffer (vb_x, glob.allocator);
  if (rokz::MapMemory (&pmapped, vb_x.allocation, glob.allocator)) {
    memcpy (pmapped, &darkmesh.verts[0], DarkrootMesh::VertexSize * darkmesh.verts.size()); 
    rokz::UnmapMemory (vb_x.allocation, glob.allocator); 
  }

  rokz::CreateInfo_VB_device (glob.vma_vb_device.ci, DarkrootMesh::VertexSize, darkmesh.verts.size());
  rokz::AllocCreateInfo_device (glob.vma_vb_device.alloc_ci); 
  rokz::CreateBuffer (glob.vma_vb_device, glob.allocator); 

  //rokz::Transfer_2_Device;
  rokz::MoveToBuffer_XB2DB (glob.vma_vb_device, vb_x, DarkrootMesh::VertexSize * darkmesh.verts.size(), 
                            glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  rokz::Destroy (vb_x, glob.allocator); 
  
  // INDEX BUFF
  rokz::Buffer ib_x;
  rokz::CreateInfo_IB_16_stage (ib_x.ci, darkmesh.indices.size()); 
  rokz::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::CreateBuffer (ib_x, glob.allocator);

  if (rokz::MapMemory (&pmapped, ib_x.allocation, glob.allocator)) {
    memcpy (pmapped, &darkmesh.indices[0], DarkrootMesh::IndexSize * darkmesh.indices.size()  ); 
    rokz::UnmapMemory (ib_x.allocation, glob.allocator); 
  }
  
  rokz::CreateInfo_IB_16_device (glob.vma_ib_device.ci, darkmesh.indices.size()); 
  rokz::AllocCreateInfo_device (glob.vma_ib_device.alloc_ci);
  rokz::CreateBuffer (glob.vma_ib_device, glob.allocator);

  rokz::MoveToBuffer_XB2DB  (glob.vma_ib_device, ib_x, DarkrootMesh::IndexSize * darkmesh.indices.size (), 
                             glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 
  rokz::Destroy (ib_x, glob.allocator); 
  //DarkrootMesh& dark_mesh = glob.dark_mesh;

  SetupDarkSampler (glob); 
  // SetupUniformBuffers (glob.uniform_buffers,
  //                      glob.uniform_mapped_pointers, 
  //                      glob.device,
  //                      glob.physical_device); 

  if (!SetupDarkUniforms (glob)) {
    printf ("[FAILED] --> SetupDarkUniforms \n"); 
    return false;
  }
  
  // rokz::DescriptorPool           uniform_descriptor_pool;
  // rokz::DescriptorGroup          uniform_group; 
  if (!SetupDarkTexture (glob)) {
    printf ("[FAILED] --> SetupDarkTexture \n"); 
    return false;
  }

return false;
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

  if (rokz::FindDepthFormat (glob.depth_format, glob.physical_device.handle)) {

    rokz::CreateInfo_2D_depthstencil (glob.depth_image.ci,
                                      glob.depth_format, 
                                      glob.msaa_samples,
                                      wd, ht);

    rokz::AllocCreateInfo_device (glob.depth_image.alloc_ci); 
    rokz::CreateImage (glob.depth_image, glob.allocator);

    rokz::CreateInfo (glob.depth_imageview.ci, VK_IMAGE_ASPECT_DEPTH_BIT, glob.depth_image); 
    rokz::CreateImageView (glob.depth_imageview, glob.depth_imageview.ci, glob.device.handle);

    rokz::TransitionImageLayout (glob.depth_image.handle, glob.depth_format,
                                 VK_IMAGE_LAYOUT_UNDEFINED,
                                 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                 glob.queues.graphics, glob.command_pool.handle, glob.device.handle);

  } 
  
}



bool SetupGridscapeShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device)  {

  printf ("%s \n", __FUNCTION__); 

  
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  std::vector<rokz::ShaderModule>&              shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (2);
  shader_stage_create_infos.resize(2);
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"data/shader/gridscape_vert.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[0], vert_file_path.string(), device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].handle); 
  
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"data/shader/gridscape_frag.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[1], frag_file_path.string(), device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[1].handle); 
  //
  return true; 

}

 // ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupDarkShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {

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




// --------------------------------------------------------------------
// VMA
// --------------------------------------------------------------------
void SetupDarkMultisampleColorResource (Glob& glob) {

  printf ("%s\n", __FUNCTION__); 

  rokz::Swapchain& swapchain = glob.swapchain_group.swapchain; 
  
  VkExtent2D& swapchain_ext    = swapchain.ci.imageExtent;
  VkFormat    swapchain_format = swapchain.ci.imageFormat; 

  rokz::CreateInfo_2D_color_target (glob.msaa_color_image.ci, swapchain_format,
                                    glob.msaa_samples,swapchain_ext.width, swapchain_ext.height);

  rokz::AllocCreateInfo_device (glob.msaa_color_image.alloc_ci);
  rokz::CreateImage (glob.msaa_color_image, glob.allocator);

  // imageview 
  rokz::CreateInfo (glob.msaa_color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.msaa_color_image);
  rokz::CreateImageView (glob.msaa_color_imageview, glob.msaa_color_imageview.ci, glob.device.handle);

  rokz::TransitionImageLayout (glob.msaa_color_image.handle, swapchain_format,
                               VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                               glob.queues.graphics, glob.command_pool.handle, glob.device.handle);

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void CleanupDarkroot (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 

  for (auto& ub : glob.vma_uniform_buffs) {
    rokz::Destroy (ub, glob.allocator); 
  }

  for (auto buf : glob.vma_objparam_buffs) {  
    rokz::Destroy (buf, glob.allocator);
  }

  // for (auto r : glob.renderables) { 
  //   r->FreeRes  (glob.allocator);
  // }

  rokz::Destroy (glob.sampler, glob.device.handle); 
  rokz::Destroy (glob.descr_pool, glob.device.handle); 
  rokz::Destroy (glob.obj_pipeline.descrgroup, glob.device.handle); 
  rokz::Destroy (glob.texture_imageview, glob.device.handle);

  rokz::Destroy (glob.texture_image, glob.allocator);
  rokz::Destroy (glob.vma_vb_device, glob.allocator);
  rokz::Destroy (glob.vma_ib_device, glob.allocator);
  
  Cleanup (glob.obj_pipeline.pipeline.handle,
           glob.swapchain_group.imageviews,

           glob.swapchain_group.swapchain,
           glob.surface,
           glob.command_pool.handle,
           glob.frame_sequence.syncs, 
           glob.obj_pipeline.pipeline.shader_modules,
           glob.obj_pipeline.pipeline.layout.handle, 
           // glob.render_pass,

           glob.msaa_color_image, glob.msaa_color_imageview,

           glob.depth_image, glob.depth_imageview,

           glob.window.glfw_window,
           glob.device,
           glob.allocator, 
           glob.instance.handle);

  glfwTerminate();
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

bool GenerateMipMaps (rokz::Image& image, 
                      bool        generate_mipmap,
                      uint32_t    num_mipmaps) {

  return false; 
}



bool LoadIndexBuffer_static ();
bool LoadVertexBuffer_static();



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// void SetupDarkTextureImageView (Glob& glob) {

//   printf ("[%s]\n", __FUNCTION__); 

//   rokz::CreateInfo (glob.texture_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.texture_image);  
//   if (vkCreateImageView(glob.device.handle, &glob.texture_imageview.ci, nullptr, &glob.texture_imageview.handle) != VK_SUCCESS) {
//     printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
//   }
// }


// ---------------------------------------------------------------------
// Setup 'DescriptorLayout' is how it looks
// ---------------------------------------------------------------------
bool SetupGridscapeDescriptorLayout (darkroot::PipelineGroup& pipelinegroup, const rokz::Device& device) {

  printf ("%s", __FUNCTION__); 

  rokz::DescriptorGroup&  descrgroup = pipelinegroup.descrgroup; 

  VkShaderStageFlags stage_flag_bits = VK_SHADER_STAGE_VERTEX_BIT;
                                     // | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
                                     // | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; 

  descrgroup.dslayout.bindings.resize (2);

  // MVPTransform
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[0],
                                    0,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    1,
                                    stage_flag_bits);
  // patch params
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[1],
                                    1,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    kSceneObjCount, 
                                    stage_flag_bits);

  
  if (!rokz::CreateDescriptorSetLayout (descrgroup.dslayout.handle,
                                        descrgroup.dslayout.ci,
                                        descrgroup.dslayout.bindings,
                                        device.handle)) {
    printf (" --> [FAILED] \n"); 
    return false;
  }

  printf (" --> true\n"); 
  return true; 
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
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[0],
                                    0,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    1,
                                    VK_SHADER_STAGE_VERTEX_BIT);

  // SceneObjParams
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[1],
                                    1,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    128, 
                                    VK_SHADER_STAGE_VERTEX_BIT);

  // sammpler+image
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[2],
                                    2,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                    1, 
                                    VK_SHADER_STAGE_FRAGMENT_BIT);

  if (!rokz::CreateDescriptorSetLayout (descrgroup.dslayout.handle,
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
// Setup 'DescriptorSets' is how it works
// ---------------------------------------------------------------------
bool SetupGridscapeDescriptorSets (darkroot::PipelineGroup&              pipelinegroup,

                                  const std::vector<rokz::Buffer>& vma_terr_uniform_buffs,
                                  const std::vector<rokz::Buffer>& vma_terr_patch_buffs,
                                  // color texture array per patch
                                  const std::vector<rokz::ImageView>& txr_imageview_color, 
                                  // normal texture array
                                  const std::vector<rokz::ImageView>& txr_imageview_normal, 

                                  const rokz::Sampler&   normal_sampler, 
                                  const rokz::Sampler&   color_sampler, 

                                  const rokz::DescriptorPool& descpool,
                                  const rokz::Device&         device) {

  return false;
}
  

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool SetupObjectDescriptorSets (darkroot::PipelineGroup& pipelinegroup,

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
  rokz::AllocateInfo (dg.alloc_info , descrlos, descpool);
  
  if (!rokz::AllocateDescriptorSets (dg.descrsets, kMaxFramesInFlight, dg.alloc_info, device.handle)) {
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

  rokz::CreateInfo ( dp.ci, kMaxFramesInFlight, dp.sizes); 
  
  if (!rokz::CreateDescriptorPool (dp, glob.device.handle)) {
    printf ("[FAILED] %s", __FUNCTION__);
    return false; 
  }

 return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent) {

  const VkOffset2D offs0 {0, 0};

  vps.viewports.resize (1);
  vps.scissors.resize (1);
  
  vps.scissors[0] = { offs0, swapchain_extent };
  rokz::ViewportState_default (vps, vps.scissors[0], 1.0f); 

}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupObjectPipeline (darkroot::PipelineGroup& pipelinegroup,
                          // const rokz::RenderPass& renderpass,  // using dynamic render pass
                          const std::filesystem::path& fspath,
                          const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          const rokz::Device& device) {

  
  //
  SetupDarkShaderModules (pipelinegroup.pipeline, fspath, device);

  //
  rokz::Pipeline& pipeline = pipelinegroup.pipeline;
  SetupViewportState (pipeline.state.viewport, swapchain.ci.imageExtent); 

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
  

  //
  rokz::CreateInfo (pipelinegroup.pipeline.ci,
                    pipelinegroup.pipeline.layout.handle,
                    &pipelinegroup.pr_ci,                    
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupGridscapePipeline (darkroot::PipelineGroup& pipelinegroup,
                           const rokz::RenderPass& renderpass,
                           const std::filesystem::path& fspath,
                           const rokz::Swapchain& swapchain,
                           VkSampleCountFlagBits msaa_samples,
                           const rokz::Device& device) {

  

  SetupGridscapeShaderModules (pipelinegroup.pipeline, fspath, device);
  

  rokz::Pipeline& pipeline = pipelinegroup.pipeline;

  SetupViewportState (pipeline.state.viewport, swapchain.ci.imageExtent); 

  pipeline.state.colorblend_attachments.resize (1);
  rokz::ColorBlendState_default (pipeline.state.colorblend_attachments[0]); 

  rokz::DynamicState_default (pipeline.state.dynamics); 

  rokz::PipelineStateCreateInfo& psci = pipeline.state.ci;
  // !! VK_PRIMITIVE_TOPOLOGY_LINE_LIST
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_LINE_LIST); 
  rokz::CreateInfo (psci.vertexinputstate, kDarkVertexBindingDesc, kDarkvertBindingAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, pipeline.state.viewport);
  rokz::CreateInfo (psci.tesselation, 69); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 
  rokz::CreateInfo (psci.colorblendstate, pipeline.state.colorblend_attachments); 
  rokz::CreateInfo (psci.dynamicstate, pipeline.state.dynamics); 
  //rokz::DescriptorGroup& descrgroup = pipelinegroup.descrgroup;
  // pipelinegroup.descrgroup.dslayout;
  // pipelinegroup.descrgroup.descrsets;
  // pipelinegroup.descrgroup.alloc_info;
  SetupGridscapeDescriptorLayout (pipelinegroup, device); 

  std::vector<VkDescriptorSetLayout> dslos =  {
    pipelinegroup.descrgroup.dslayout.handle
  };

  std::vector<VkPushConstantRange> pcs = {
    VkPushConstantRange { VK_SHADER_STAGE_VERTEX_BIT, 0,  sizeof(darkroot::PushConstants)  }
  };

  rokz::CreateInfo (pipeline.layout.ci, dslos, pcs); 
  rokz::CreateGraphicsPipelineLayout (pipeline.layout.handle, pipeline.layout.ci, device.handle); 
  // rokz::CreateGraphicsPipelineLayout (pipeline.layout.handle,
  //                                     pipeline.layout.ci,
  //                                     pipelinegroup.descrgroup.dslayout.handle,
  //                                     device.handle);
  rokz::CreateInfo (pipeline.ci,
                    pipeline.layout.handle,
                    renderpass.handle,                    
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

  if (!rokz::CreateGraphicsPipeline (pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }
  
  return false; 
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

  memcpy (rokz::MappedPointer (glob.vma_uniform_buffs[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 
 
  if (SceneObjParam* obj = reinterpret_cast<SceneObjParam*> (rokz::MappedPointer (glob.vma_objparam_buffs[current_frame]))) {
  
    glm::vec3 va, vb;
    unit_angle_xz (va, 5.0 * sim_timef ); 
    unit_angle_xz (vb, 5.0 * sim_timef + kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    obj[0].modelmat = glm::rotate(model0, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    obj[1].modelmat = glm::rotate(model1, sim_timef * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
  }
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
//void SetupDarkroot () { printf ("%s\n", __FUNCTION__); }
//void ShutdownDarkroot () { printf ("%s\n", __FUNCTION__); }
//void UpdateInput (Glob& glob, double dt) { }
//void UpdateDarkroot (Glob& glob, double dt) { }

// ---------------------------------------------------------------------
// RecordDarkCommandBuffer_indexed
// ---------------------------------------------------------------------
bool RecordDarkRenderPass_indexed (Glob& glob, 
                                   VkCommandBuffer        &command_buffer,
                                   const rokz::Pipeline&  pipeline,
                                   const VkDescriptorSet& desc_set, 
                                   const VkBuffer&        vertex_buffer, 
                                   const VkBuffer&        index_buffer, 
                                   const VkExtent2D&      ext2d,
                                   const VkFramebuffer&   framebuffer,
                                   const rokz::RenderPass&render_pass,
                                   const VkDevice&        device) {



  const DarkrootMesh& darkmesh = DarkOctohedron ();
  
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.flags = 0;                  // 
  begin_info.pInheritanceInfo = nullptr; // 

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
     printf ("failed to begin recording command buffer!");
     return false; 
  }


  VkClearValue clear_values[3] = {};
  clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clear_values[1].depthStencil = {1.0f, 0};
  clear_values[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};

  // begin command list
  VkRenderPassBeginInfo pass_info{};
  pass_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  pass_info.pNext             = nullptr;
  pass_info.renderPass        = render_pass.handle; 
  pass_info.framebuffer       = framebuffer; 
  pass_info.renderArea.offset = {0, 0};
  pass_info.renderArea.extent = ext2d;
  pass_info.clearValueCount   = 3; 
  pass_info.pClearValues      = clear_values; // ?does this match attachment order


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

  
  vkCmdBeginRenderPass (command_buffer, &pass_info, VK_SUBPASS_CONTENTS_INLINE);
  
  vkCmdBindPipeline    (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle);

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


  //
  vkCmdEndRenderPass(command_buffer);
  //
  if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("[FAILED] record command buffer\n");
    return false; 
  }
  
  //printf ("BAI %s\n", __FUNCTION__); 
  return true;
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
                              const VkDevice&        device) {

  const DarkrootMesh& darkmesh = DarkOctohedron ();
  
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


// --------------------------------------------------------------------
// nue
// --------------------------------------------------------------------
bool RenderDarkFrame (Glob&           glob,
                      uint32_t&               image_index,
                      bool&                   resize,
                      rokz::RenderPass&       renderpass, 
                      const rokz::Pipeline&   pipeline,
                      const VkDescriptorSet&  descr_set, 
                      uint32_t                curr_frame,
                      double dt) {

  const rokz::Device&             device      = glob.device; 
  rokz::SwapchainGroup&           scg = glob.swapchain_group;

  rokz::Swapchain&                swapchain              = scg.swapchain; 
  std::vector<rokz::Image>&       swapchain_images       = scg.images; 
  std::vector<rokz::Framebuffer>& swapchain_framebuffers = scg.framebuffers; 
  std::vector<rokz::ImageView>&   swapchain_imageviews   = scg.imageviews; 

  
  rokz::Image&                    depth_image            = glob.depth_image;
  rokz::ImageView&                depth_imageview        = glob.depth_imageview; 
  
  rokz::Image&                    msaa_color_image       = glob.msaa_color_image; 
  rokz::ImageView&                msaa_color_imageview   = glob.msaa_color_imageview; 

  VmaAllocator&                   allocator              =  glob.allocator;
  rokz::Window&                   window                 = glob.window;

  rokz::RenderSync&               render_sync            = glob.frame_sequence.syncs[curr_frame];
    // glob.syncs;

  VkResult acquire_res = rokz::AcquireFrame (glob.swapchain_group.swapchain, render_sync, image_index, device); 
  
  if (acquire_res == VK_ERROR_OUT_OF_DATE_KHR || acquire_res == VK_SUBOPTIMAL_KHR || resize) {
    resize = false; 
    return rokz::cx::RecreateSwapchain (swapchain, 
                                    swapchain_images,
                                    swapchain_framebuffers,
                                    swapchain_imageviews,
                                    renderpass, 
                                    depth_image, depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                    msaa_color_image, msaa_color_imageview,
                                    device,
                                    allocator,
                                    window.glfw_window);
  } 
  else if (acquire_res != VK_SUCCESS) {
    printf("failed to acquire swap chain image!");
    return false;
  }

  //printf ("[%s] line:%i\n", __FUNCTION__, __LINE__); 
  // this is wat works right now
  UpdateDarkUniforms (glob, curr_frame, dt); 

  rokz::Framebuffer& framebuffer    = swapchain_framebuffers[image_index]; 
  VkCommandBuffer&   command_buffer = glob.frame_sequence.command_buffers[curr_frame]; 
  rokz::Buffer&      vma_vb_device  = glob.vma_vb_device;
  rokz::Buffer&      vma_ib_device  = glob.vma_ib_device; 

  vkResetCommandBuffer (command_buffer, 0); //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);

  //RecordDarkRenderPass (command_buffer,
  // RecordDarkRenderPaass
  RecordDarkRenderPass_indexed (glob,
                                command_buffer,
                                pipeline,
                                descr_set, 
                                vma_vb_device.handle, 
                                vma_ib_device.handle,
                                swapchain.ci.imageExtent,
                                framebuffer.handle, // swapchain_framebuffers[image_index].handle,
                                renderpass,
                                device.handle);
  
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

  if (vkQueueSubmit (glob.queues.graphics, 1, &submit_info, render_sync.in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }
  
  return rokz::PresentFrame (glob.queues.present, glob.swapchain_group.swapchain, image_index, glob.frame_sequence.syncs[curr_frame]); 
  
}


bool RenderFrame_dynamic (Glob&                   glob,
                          uint32_t&               image_index,
                          bool&                   resize,
                          //rokz::RenderPass&       renderpass, 
                          const rokz::Pipeline&   pipeline,
                          const VkDescriptorSet&  descr_set, 
                          uint32_t                curr_frame,
                          double dt) {

  const rokz::Device&             device      = glob.device; 
  rokz::SwapchainGroup&           scg = glob.swapchain_group;

  rokz::Swapchain&                swapchain              = scg.swapchain; 
  std::vector<rokz::Image>&       swapchain_images       = scg.images; 
  std::vector<rokz::ImageView>&   swapchain_imageviews   = scg.imageviews; 

  rokz::Image&                    depth_image            = glob.depth_image;
  rokz::ImageView&                depth_imageview        = glob.depth_imageview; 
  
  rokz::Image&                    msaa_color_image       = glob.msaa_color_image; 
  rokz::ImageView&                msaa_color_imageview   = glob.msaa_color_imageview; 

  VmaAllocator&                   allocator              = glob.allocator;
  rokz::Window&                   window                 = glob.window;

  rokz::RenderSync&               render_sync            = glob.frame_sequence.syncs[curr_frame];
    // glob.syncs;

  VkResult acquire_res = rokz::AcquireFrame (glob.swapchain_group.swapchain, render_sync, image_index, device); 
  
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


  rokz::TransitionImageLayout (glob.swapchain_group.images[image_index].handle,
                               glob.surface_format.format, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                               glob.queues.graphics, glob.command_pool.handle, device.handle);
  
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
                           device.handle);
  
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

  if (vkQueueSubmit (glob.queues.graphics, 1, &submit_info, render_sync.in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }


  rokz::TransitionImageLayout (glob.swapchain_group.images[image_index].handle,
                               glob.surface_format.format,
                               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                               VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                               glob.queues.graphics, glob.command_pool.handle, device.handle);




// VUID-vkCmdDrawIndexed-colorAttachmentCount-06179(ERROR / SPEC): msgNum: -52447566 - Validation Error: [ VUID-vkCmdDrawIndexed-colorAttachmentCount-06179 ]
//  Object 0: handle = 0x55efab9566a0, type = VK_OBJECT_TYPE_COMMAND_BUFFER; | MessageID = 0xfcdfb6b2 | vkCmdDrawIndexed:
  "Currently bound pipeline VkPipeline 0xdcc8fd0000000012[] colorAttachmentCount ([2) must be equal to pBeginRendering->colorAttachmentCount\
  ([1) The Vulkan spec states: If the current render pass instance was begun with vkCmdBeginRendering, the currently bound graphics pipeline\
  must have been created with a VkPipelineRenderingCreateInfo::colorAttachmentCount equal to VkRenderingInfo::colorAttachmentCount\
  (https://www.khronos.org/registry/vulkan/specs/1.3-extensions/html/vkspec.html#VUID-vkCmdDrawIndexed-colorAttachmentCount-06179)"; 
//Objects: 1
//    [0] 0x55efab9566a0, type: 6, name: NULL

  
  return rokz::PresentFrame (glob.queues.present, glob.swapchain_group.swapchain, image_index, glob.frame_sequence.syncs[curr_frame]); 
  
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
  //std::shared_ptr<Glob> globmem = std::make_shared<Glob> ();

  auto dark_path = std::filesystem::path ( "/home/djbuzzkill/owenslake/rokz/");
  //Default (glob); 
  
  glfwInit();

  
  glob.fb_resize = false; 
  
  //rokz::CreateWindow_glfw (glob.glfwin);
  SetupDarkrootWindow (glob); 

  rokz::cx::AppInfo_default (glob.instance.app_info);

  //std::vector<const char*> _vls;
  //std::vector<std::string> vstrs;

  rokz::cx::CreateInfo (glob.instance.ci,
                    glob.instance.required_extensions,
                    glob.instance.extension_strings,
                    glob.instance.vals, glob.instance.validation_strings, 
                    glob.instance.app_info); 

  rokz::cx::CreateInstance  (glob.instance.handle, glob.instance.ci);
  rokz::cx::CreateSurface   (&glob.surface, glob.window.glfw_window, glob.instance.handle);
  rokz::cx::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance.handle);

  glob.msaa_samples = rokz::MaxUsableSampleCount (glob.physical_device); 

  VkDeviceSize min_uniform_buffer_offset_alignment =
    rokz::MinUniformBufferOffsetAlignment (glob.physical_device);

  printf ( "----------> min_uniform_buffer_offset_alignment [%zu]\n", min_uniform_buffer_offset_alignment); 
//VkPhysicalDeviceLimits::minUniformBufferOffsetAlignment
  
  glob.queue_priority = 1.0f;
  if (glob.physical_device.family_indices.graphics.has_value ()) {
    printf ("HAS_VALUE:TRUE\n"); 
    printf ("  graphics[%u]\n", glob.physical_device.family_indices.graphics.value ()); 
  }
  else  {
    printf ("HAS_VALUE:FALSE\n"); 
  }

  glob.device.queue_ci.resize  (2); 
  // VkQueueCreateInfo
  rokz::cx::CreateInfo (glob.device.queue_ci[0], glob.physical_device.family_indices.graphics.value () , &glob.queue_priority);
  rokz::cx::CreateInfo (glob.device.queue_ci[1], glob.physical_device.family_indices.present.value  () , &glob.queue_priority);
  
  // device info
  //VkDeviceCreateInfo&       Default (VkDeviceCreateInfo& info, VkDeviceQueueCreateInfo* quecreateinfo, VkPhysicalDeviceFeatures* devfeats); 
  glob.physical_device.features.samplerAnisotropy = VK_TRUE;


  // * 01/15/2023 - use dynamic rendering pass 
  VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature {};
  dynamic_rendering_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
  dynamic_rendering_feature.pNext = nullptr;
  dynamic_rendering_feature.dynamicRendering = VK_TRUE;

  rokz::cx::CreateInfo (glob.device.ci,
                        &dynamic_rendering_feature, 
                        glob.device.vals, glob.device.valstrs, 
                        glob.device.dxs, glob.device.dxstrs, 
                        glob.device.queue_ci, &glob.physical_device.features);

  rokz::cx::CreateLogicalDevice (&glob.device.handle, &glob.device.ci, glob.physical_device.handle); 
  // *

  // get queue handle
  rokz::cx::GetDeviceQueue (&glob.queues.graphics, glob.physical_device.family_indices.graphics.value(), glob.device.handle);
  rokz::cx::GetDeviceQueue (&glob.queues.present,  glob.physical_device.family_indices.present.value(), glob.device.handle);
  // VMA SECTION
  // VmaVulkanFunctions vulkanFunctions = {};
  // vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
  // vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

  VmaAllocatorCreateInfo allocatorCreateInfo = {};
  allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
  allocatorCreateInfo.physicalDevice   = glob.physical_device.handle;
  allocatorCreateInfo.device           = glob.device.handle;
  allocatorCreateInfo.instance         = glob.instance.handle;

  allocatorCreateInfo.flags = 0;
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  vmaCreateAllocator(&allocatorCreateInfo, &glob.allocator);
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  
  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info,
                               glob.surface,
                               glob.physical_device.handle);

  rokz::SwapchainGroup& scg = glob.swapchain_group;

  scg.swapchain.family_indices.push_back (glob.physical_device.family_indices.graphics.value());
  scg.swapchain.family_indices.push_back (glob.physical_device.family_indices.present.value ());
  
  rokz::cx::CreateInfo_default (scg.swapchain.ci,  
                                scg.swapchain.family_indices,
                                glob.surface,
                                kTestExtent, 
                                glob.swapchain_support_info, 
                                glob.physical_device);

  glob.surface_format.format =  scg.swapchain.ci.imageFormat;
  
  rokz::cx::CreateSwapchain (scg.swapchain, glob.device); 
  
  rokz::cx::GetSwapChainImages (scg.images, scg.swapchain, glob.device.handle); 
  rokz::CreateImageViews (scg.imageviews, scg.images, glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);

#ifdef DARKROOT_DYNAMIC_RENDER_ENABLE

  // this is passed on as a VkGraphicsPipelineCreateInfo::pNext
  //  VkPipelineRenderingCreateInfo
  std::vector<VkFormat> color_formats = {
        glob.surface_format.format };
  rokz::CreateInfo  (glob.obj_pipeline.pr_ci, color_formats, glob.depth_format); 

#else
  // this wont compile
  rokz::CreateRenderPass (glob.render_pass,
                          scg.swapchain.ci.imageFormat,
                          glob.msaa_samples,
                          glob.device.handle,
                          glob.physical_device.handle);
#endif
  
  // SetupShaderModules also sets up Pipeline Shader State CreateInfo's
  //  SetupDarkShaderModules (glob.pipeline, dark_path, glob.device);
  // bool SetupDarkShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {
  // Command Pool 
  rokz::CreateInfo (glob.command_pool.ci, glob.physical_device.family_indices.graphics.value());
  rokz::CreateCommandPool (glob.command_pool.handle, glob.command_pool.ci, glob.device.handle);


  printf ("transition after we have command pool [%i]\n", __LINE__);
  for (size_t iimg = 0; iimg < scg.images.size (); ++iimg) {
    
    rokz::TransitionImageLayout (scg.images[iimg].handle,
                                 glob.surface_format.format,
                                 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                 glob.queues.graphics, glob.command_pool.handle, glob.device.handle);
  }


  
#ifdef DARKROOT_ENABLE_GRID
  
  SetupGridscapePipeline (glob.grid_pipeline,
                          glob.grid_pipeline.pipeline.state.viewport, 
                          glob.render_pass,
                          dark_path,
                          glob.swapchain_group.swapchain,
                          glob.msaa_samples,
                          glob.device); 

#endif

  SetupObjectPipeline (glob.obj_pipeline,
                       //glob.render_pass,
                       dark_path,
                       glob.swapchain_group.swapchain,
                       glob.msaa_samples,
                       glob.device); 


  SetupDarkMultisampleColorResource (glob);

  SetupDarkDepthBuffer (glob);


#ifdef DARKROOT_DYNAMIC_RENDER_ENABLE

  SetupDynamicRenderingInfo (glob) ; 

#else
  // keep around for now
  rokz::CreateFramebuffers (scg.framebuffers, scg.imageviews, glob.render_pass,
                            scg.swapchain.ci.imageExtent, glob.multisamp_color_imageview.handle,
                            glob.depth_imageview.handle, glob.device); 

#endif

  
// #ifdef DARKROOT_ENABLE_RENDERABLE_TEST
//   SetupDarkGeometry (glob); 
// #endif

  SetupObjResources (glob);

  if (!SetupObjDescriptorPool (glob)) {
    printf ("[FAILED] --> SetupGlobalDescriptorPool \n"); 
    return false;
  }

  // DescriptorLayout is created in SetupDarkObjectPipeline
  // this is done here b/c SetupGrlobalDescriptorPool is
  // just barely above us

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
  rokz::AllocateInfo (frameseq.command_buffer_alloc_info, glob.command_pool.handle); 

  for (size_t i = 0; i < Glob::MaxFramesInFlight; ++i) {
    // ^^ 'CreateCommandBuffers' should be called, we call it 
    rokz::CreateCommandBuffer(frameseq.command_buffers[i], frameseq.command_buffer_alloc_info,glob.device.handle);
    rokz::CreateRenderSync (frameseq.syncs[i], frameseq.syncs[i].ci, glob.device.handle);
  }

  //
  //SetupDarkroot ();


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
  int        countdown  = 1;

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
    
    //    UpdateDarkroot(glob, glob.dt);

    //    result = RenderFrame (glob, curr_frame, fb_resize, glob.dt);

#ifdef DARKROOT_DYNAMIC_RENDER_ENABLE    
    uint32_t image_index; 
    if (RenderFrame_dynamic (glob, image_index, glob.fb_resize, glob.obj_pipeline.pipeline,
                             glob.obj_pipeline.descrgroup.descrsets[curr_frame], curr_frame, glob.dt)) {
      // no render pass of framebuffer
    }
    else {
      run = false;
    }
#else

    uint32_t image_index; 
    if (RenderDarkFrame (glob, image_index, glob.fb_resize, glob.render_pass, glob.obj_pipeline.pipeline,
                     glob.obj_pipeline.descrgroup.descrsets[curr_frame], curr_frame, glob.dt)) {
    }
    else {
      run = false;
    }
#endif
    
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


//#endif // HIDE_TEST_ROKZ

