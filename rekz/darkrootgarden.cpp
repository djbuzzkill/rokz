
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
#include "rokz/descriptor.h"
#include "rokz/shared_types.h"
#include "vk_mem_alloc.h"
 
// #include "rokz/rokz.h"
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan_core.h>


#define ROKZ_USE_VMA_ALLOCATION 1

const size_t        kMaxFramesInFlight = 2;
const std::string   data_root =  "/home/djbuzzkill/owenslake/rokz/data"; // 
const VkExtent2D    kTestExtent  = { 800, 600 };
const size_t        kSceneObjCount = 128;
//
//
using namespace darkroot; 

//
// brep is associated with a geom
HalfEdge::BRep& BuildBoundaryRep (HalfEdge::BRep& brep, const DarkrootMesh& geom) {

  auto nverts = geom.verts.size ();
  auto ninds = geom.indices.size ();

  if (ninds % 3) {
    printf ("[%s] --> TriMesh should be:0\n", __FUNCTION__); 
    return brep;
  }

  auto nfaces = ninds / 3;
  
  brep.edgei.resize (nverts, HalfEdge::NullIndex);
  brep.edges.resize (nfaces * 3);
  brep.faces.resize (nfaces);

  auto edge_count = 0; 
  auto index_count = 0;
  
  for (size_t iface = 0; iface < nfaces; ++iface) {
    //geom.verts[
    // geom.indices[index_count+0]
    // geom.indices[index_count+1]
    // geom.indices[index_count+2]
    brep.faces[iface].edgei = edge_count+0;
    brep.edges[edge_count+0].facei = iface;
    brep.edges[edge_count+0].verti = geom.indices[index_count+0];
    brep.edges[edge_count+0].nexti = edge_count + 1;
    brep.edges[edge_count+0].pairi = HalfEdge::NullIndex; //<-- we dont know yet
        
    brep.edges[edge_count+1].facei = iface;
    brep.edges[edge_count+1].verti = geom.indices[index_count+1];
    brep.edges[edge_count+1].nexti = edge_count + 2;
    brep.edges[edge_count+1].pairi = HalfEdge::NullIndex; // <-- we dont know yet
      
    brep.edges[edge_count+2].facei = iface;
    brep.edges[edge_count+2].verti = geom.indices[index_count+2];
    brep.edges[edge_count+2].nexti = edge_count + 0;
    brep.edges[edge_count+2].pairi = HalfEdge::NullIndex; // <-- we dont know yet
    // verti : vertex at the end of the half-edge 
    // pairi : oppositely oriented adjacent half-edge 
    // facei : face the half-edge borders
    // nexti : next half-edge around the face
    edge_count  += 3;
    index_count += 3; 
  }

  // find pair 
  for (size_t iedge = 0; iedge < brep.edges.size (); ++iedge) {
    // make sure this isnt paried
    if (brep.edges[iedge].pairi != HalfEdge::NullIndex) 
      continue;
      
    for (size_t iiedge = 0; iiedge < brep.edges.size (); ++iiedge) {
      // if not itself & not paired
      if (iedge == iiedge || brep.edges[iiedge].pairi != HalfEdge::NullIndex) 
        continue;
      
      if (brep.edges[iedge].verti == brep.edges[iiedge].verti) {
        // iedge.pairi = iiedge.nexti.nexti;
        brep.edges[iedge].pairi = brep.edges[ brep.edges[iiedge].nexti ].nexti;
          
        // iiedge.nexti.nexti.pairi = iedge; 
        brep.edges[ brep.edges[ brep.edges[iiedge].nexti ].nexti ].pairi = iedge; 
      }
    }
    
  }
  
  return brep;
}

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
  , multisamp_color_image()
  , multisamp_color_imageview()
  , msaa_samples ()
  , vma_ib_device()
  , vma_vb_device()
  , command_pool()
  , render_pass()
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

  glfwSetFramebufferSizeCallback (glob.window.glfw_window, darkroot::window_event_on_resize ); 
  glfwSetKeyCallback (glob.window.glfw_window, darkroot::window_event_on_keypress);
  glfwSetCursorPosCallback(glob.window.glfw_window, darkroot::window_event_on_mouse_move);
  glfwSetMouseButtonCallback(glob.window.glfw_window, darkroot::window_event_on_mouse_button);

  //glfwSetCursorEnterCallback(window, rokz);

  glfwSetWindowUserPointer (glob.window.glfw_window, &glob);



  return true;
}
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupDarkGeometry (Glob& glob) {

  printf ("%s\n", __FUNCTION__); 


  glob.darkmesh = darkroot::DarkOctohedron (); 

  HalfEdge::BRep darkboundary;
  BuildBoundaryRep (darkboundary, glob.darkmesh);

  DarkrootMesh& darkmesh = glob.darkmesh; 

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

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupDarkDepthBuffer (Glob& glob) {
  printf ("%s\n", __FUNCTION__); 

  rokz::FrameGroup& frame_group = glob.frame_group;


  //
  uint32_t wd = frame_group.swapchain.ci.imageExtent.width; 
  uint32_t ht = frame_group.swapchain.ci.imageExtent.height;   

  VkFormat depth_format;

  if (rokz::FindDepthFormat (depth_format, glob.physical_device.handle)) {

    rokz::CreateInfo_2D_depthstencil (glob.depth_image.ci,
                                      depth_format, 
                                      glob.msaa_samples,
                                      wd, ht);
    rokz::AllocCreateInfo_device (glob.depth_image.alloc_ci); 
    rokz::CreateImage (glob.depth_image, glob.allocator);

    rokz::CreateInfo (glob.depth_imageview.ci, VK_IMAGE_ASPECT_DEPTH_BIT, glob.depth_image); 
    rokz::CreateImageView (glob.depth_imageview, glob.depth_imageview.ci, glob.device.handle);


    rokz::TransitionImageLayout; 
    //(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  } 
  
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupDarkSampler (Glob& glob) {
  printf ("%s \n", __FUNCTION__); 

  rokz::CreateInfo (glob.sampler.ci, glob.physical_device.properties);
  
  rokz::CreateSampler (glob.sampler, glob.device.handle);  
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

  rokz::Swapchain& swapchain = glob.frame_group.swapchain; 
  
  VkExtent2D& swapchain_ext    = swapchain.ci.imageExtent;
  VkFormat    swapchain_format = swapchain.ci.imageFormat; 

  rokz::CreateInfo_2D_color_target (glob.multisamp_color_image.ci, swapchain_format,
                                    glob.msaa_samples,swapchain_ext.width, swapchain_ext.height);

  rokz::AllocCreateInfo_device (glob.multisamp_color_image.alloc_ci);
  rokz::CreateImage (glob.multisamp_color_image, glob.allocator);

  // imageview 
  rokz::CreateInfo (glob.multisamp_color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.multisamp_color_image);
  rokz::CreateImageView (glob.multisamp_color_imageview,
                         glob.multisamp_color_imageview.ci,
                         glob.device.handle);

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


  for (auto r : glob.renderables) { 
    r->FreeRes  (glob.allocator);
  }


  rokz::Destroy (glob.sampler, glob.device.handle); 

  rokz::Destroy (glob.descr_pool, glob.device.handle); 
  rokz::Destroy (glob.obj_pipeline.descrgroup, glob.device.handle); 
  rokz::Destroy (glob.texture_imageview, glob.device.handle);

  rokz::Destroy (glob.texture_image, glob.allocator);
  rokz::Destroy (glob.vma_vb_device, glob.allocator);
  rokz::Destroy (glob.vma_ib_device, glob.allocator);
  
  Cleanup (glob.obj_pipeline.pipeline.handle,
           glob.frame_group.swapchain_framebuffers, glob.frame_group.swapchain_imageviews,

           glob.frame_group.swapchain,
           glob.surface,
           glob.command_pool.handle,
           glob.frame_group.syncs, 
           glob.obj_pipeline.pipeline.shader_modules,
           glob.obj_pipeline.pipeline.layout.handle, 
           glob.render_pass,

           glob.multisamp_color_image, glob.multisamp_color_imageview,

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

     size_t image_size = image_width * image_height *  bytes_per_pixel; 

     rokz::CreateInfo_buffer_stage (stage_image.ci, image_size);
     rokz::AllocCreateInfo_stage (stage_image.alloc_ci);
     rokz::CreateBuffer   (stage_image, glob.allocator); 

     void* mapped = nullptr; 
     rokz::MapMemory (&mapped, stage_image.allocation, glob.allocator);

     printf ( "image dim [w:%i, h:%i, d:%i | bpp:%i, bytes:%i, type:%x, format:%x]\n",
              image_width, image_height, image_depth,
              image_bpp, bytes_per_pixel, image_type, image_format); 

     ILubyte* image_data = ilGetData ();

     std::copy (image_data, image_data + image_size, reinterpret_cast<unsigned char*> (mapped));

     struct rgba { ILubyte r, g, b, a; }; 
     const int numpixs = image_width  * image_height; 
     ILubyte _; 
     rgba* rgba_mapped = reinterpret_cast<rgba*> (mapped); 
     // swizzle
     for (int i = 0; i < numpixs; ++i) {
       _ = rgba_mapped[i].r;
       rgba_mapped[i].r = rgba_mapped[i].b;
       rgba_mapped[i].b = _;       
     }
       
     rokz::UnmapMemory (stage_image.allocation, glob.allocator);
     ilDeleteImage (ilGetInteger (IL_ACTIVE_IMAGE)); 
   }
   else {
     printf ("failed\n"); 
   }// LoadImage

   
   ilShutDown ();

   rokz::Image& image = glob.texture_image; 

   rokz::CreateInfo_2D_color_sampling  (image.ci, VK_SAMPLE_COUNT_1_BIT, image_width, image_height);
   rokz::AllocCreateInfo_device (image.alloc_ci);
   if (!rokz::CreateImage (image, glob.allocator)) {
     printf ("[FAILED] %s setup test texture", __FUNCTION__);
     return false;
   }

   rokz::TransitionImageLayout (glob.texture_image.handle,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                glob.queues.graphics,
                                glob.command_pool.handle,
                                glob.device.handle);

   rokz::CopyBufferToImage (glob.texture_image.handle, stage_image.handle, image_width, image_height,
                            glob.queues.graphics,
                            glob.command_pool.handle,
                            glob.device.handle);

   rokz::TransitionImageLayout (glob.texture_image.handle,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                glob.queues.graphics,
                                glob.command_pool.handle,
                                glob.device.handle);


   rokz::Destroy (stage_image, glob.allocator); 

   printf ("385\n"); 
   return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void SetupDarkTextureImageView (Glob& glob) {

  printf ("[%s]\n", __FUNCTION__); 

  rokz::CreateInfo (glob.texture_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.texture_image);  
  if (vkCreateImageView(glob.device.handle, &glob.texture_imageview.ci, nullptr, &glob.texture_imageview.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
  }
}


// ---------------------------------------------------------------------
// Setup 'DescriptorLayout' is how it looks
// ---------------------------------------------------------------------
bool SetupTerrainDescriptorLayout (darkroot::PipelineGroup& pipelinegroup, uint32_t num_patches, const rokz::Device& device) {

  printf ("%s", __FUNCTION__); 

  rokz::DescriptorGroup&  descrgroup = pipelinegroup.descrgroup; 

  VkShaderStageFlags stage_flag_bits = VK_SHADER_STAGE_VERTEX_BIT
                                     | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
                                     | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; 

  descrgroup.dslayout.bindings.resize (4);

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
                                    num_patches, 
                                    stage_flag_bits);
  // color images
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[2],
                                    2,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                    num_patches, 
                                    VK_SHADER_STAGE_FRAGMENT_BIT);
  // height map textures - use for height + normal 
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[2],
                                    3,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                    num_patches, 
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
bool SetupTerrainkDescriptorSets (darkroot::PipelineGroup&              pipelinegroup,

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
//Drob
// ---------------------------------------------------------------------
bool SetupGlobalDescriptorPool (Glob& glob) {

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
void SetupViewportState (rokz::ViewportState & vps, VkExtent2D& swapchain_extent) {

  const VkOffset2D offs0 {0, 0};

  vps.viewports.resize (1);
  vps.scissors.resize (1);
  
  vps.scissors[0] = { offs0, swapchain_extent };
  rokz::ViewportState_default (vps, vps.scissors[0], 1.0f); 
  //

}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupObjectPipeline (darkroot::PipelineGroup& pipelinegroup,
                          const rokz::ViewportState& vps,
                          const rokz::RenderPass& renderpass,
                          const std::filesystem::path& fspath,
                          const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          const rokz::Device& device) {

  //rokz::FrameGroup& frame_group = glob.frame_group;
  SetupDarkShaderModules (pipelinegroup.pipeline, fspath, device);
  
  rokz::ColorBlendState_default (pipelinegroup.pipeline.state.color_blend_attachment); 
  rokz::DynamicState_default    (pipelinegroup.pipeline.state.dynamics); 

  // glob.pipeline;
  // glob.descrgroup;
  

  rokz::PipelineStateCreateInfo& psci = pipelinegroup.pipeline.state.ci;

  rokz::CreateInfo (psci.tesselation, 69); 
  rokz::CreateInfo (psci.dynamicstate, pipelinegroup.pipeline.state.dynamics); 
  rokz::CreateInfo (psci.dynamicstate, pipelinegroup.pipeline.state.dynamics); 
  rokz::CreateInfo (psci.vertexinputstate, kDarkVertexBindingDesc, kDarkvertBindingAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, vps);
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, pipelinegroup.pipeline.state.color_blend_attachment); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 
  SetupObjectDescriptorLayout (  pipelinegroup.descrgroup, device); 


  //
  rokz::CreateGraphicsPipelineLayout (pipelinegroup.pipeline.layout.handle,
                                      pipelinegroup.pipeline.layout.ci,
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
bool SetupTerrainPipeline (darkroot::PipelineGroup& pipelinegroup,
                           const rokz::ViewportState& vps,
                           const rokz::RenderPass& renderpass,
                           const std::filesystem::path& fspath,
                           const rokz::Swapchain& swapchain,
                           VkSampleCountFlagBits msaa_samples,
                           const rokz::Device& device) {

  rokz::Pipeline& pipeline = pipelinegroup.pipeline;
  
  rokz::ColorBlendState_default (pipeline.state.color_blend_attachment); 
  rokz::DynamicState_default (pipeline.state.dynamics); 
  // 
  // const VkOffset2D offs0 {0, 0};
  // glob.scissor_rect = {offs0, frame_group.swapchain.ci.imageExtent};
  // rokz::Viewport (glob.viewport, glob.scissor_rect.offset, glob.scissor_rect.extent, 1.0f);  

  rokz::PipelineStateCreateInfo& psci = pipeline.state.ci;

  rokz::CreateInfo (psci.tesselation, 69); 
  rokz::CreateInfo (psci.dynamicstate, pipeline.state.dynamics); 
  rokz::CreateInfo (psci.dynamicstate, pipeline.state.dynamics); 
  rokz::CreateInfo (psci.vertexinputstate, kDarkVertexBindingDesc, kDarkvertBindingAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, vps.viewports, vps.scissors);
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, pipeline.state.color_blend_attachment); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 


  const uint32_t num_patches = 69; // <--- dont know yet
  
  SetupTerrainDescriptorLayout (pipelinegroup, num_patches , device); 
  //
  rokz::CreateGraphicsPipelineLayout (pipeline.layout.handle,
                                      pipeline.layout.ci,
                                      pipelinegroup.descrgroup.dslayout.handle,
                                      device.handle);

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

  float asp = (float)glob.frame_group.swapchain.ci.imageExtent.width / (float)glob.frame_group.swapchain.ci.imageExtent.height;
    
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
void SetupDarkroot () { printf ("%s\n", __FUNCTION__); }
void ShutdownDarkroot () { printf ("%s\n", __FUNCTION__); }
void UpdateInput (Glob& glob, double dt) { }
void UpdateDarkroot (Glob& glob, double dt) { }

// ---------------------------------------------------------------------
// RecordDarkCommandBuffer_indexed
// ---------------------------------------------------------------------
bool RecordDarkRenderPass_indexed (Glob& glob, 
                                   VkCommandBuffer        &command_buffer,
                                   const rokz::Pipeline&        pipeline,
                                   const VkDescriptorSet& desc_set, 
                                   const VkBuffer&        vertex_buffer, 
                                   const VkBuffer&        index_buffer, 
                                   const VkExtent2D&      ext2d,
                                   const VkFramebuffer&   framebuffer,
                                   const rokz::RenderPass&      render_pass,
                                   const VkDevice&        device) {

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

  //for (
  vkCmdDrawIndexed (command_buffer, glob.darkmesh.indices.size(), 1, 0, 0, 0);

  vkCmdDrawIndexed (command_buffer, glob.darkmesh.indices.size(), 1, 0, 0, 1);

  vkCmdEndRenderPass(command_buffer);

  // end 
  if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("failed to record command buffer!");
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
  rokz::FrameGroup&               frame_group = glob.frame_group;

  rokz::Swapchain&                swapchain              = frame_group.swapchain; 
  std::vector<rokz::Image>&       swapchain_images       = frame_group.swapchain_images; 
  std::vector<rokz::Framebuffer>& swapchain_framebuffers = frame_group.swapchain_framebuffers; 
  std::vector<rokz::ImageView>&   swapchain_imageviews   = frame_group.swapchain_imageviews; 

  
  rokz::Image&                    depth_image            = glob.depth_image;
  rokz::ImageView&                depth_imageview        = glob.depth_imageview; 
  
  rokz::Image&                    msaa_color_image       = glob.multisamp_color_image; 
  rokz::ImageView&                msaa_color_imageview   = glob.multisamp_color_imageview; 

  VmaAllocator&                   allocator              =  glob.allocator;
  rokz::Window&                   window                 = glob.window;

  rokz::RenderSync&               render_sync    = frame_group.syncs[curr_frame];
    // glob.syncs;

  VkResult acquire_res = rokz::AcquireFrame (glob.frame_group.swapchain, render_sync, image_index, device); 
  
  if (acquire_res == VK_ERROR_OUT_OF_DATE_KHR || acquire_res == VK_SUBOPTIMAL_KHR || resize) {
    resize = false; 
    return rokz::RecreateSwapchain (swapchain, 
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
  VkCommandBuffer&   command_buffer = frame_group.command_buffers[curr_frame]; 
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
  
  return rokz::PresentFrame (glob.queues.present, glob.frame_group.swapchain, image_index, glob.frame_group.syncs[curr_frame]); 
  
}



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int darkroot_basin (const std::vector<std::string>& args) {
  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 

  Glob  glob; // *globmem; // something representing the app state
  
  glob.dt = 0.0;
  //std::shared_ptr<Glob> globmem = std::make_shared<Glob> ();

  auto dark_path = std::filesystem::path ( "/home/djbuzzkill/owenslake/rokz");
  //Default (glob); 
  
  glfwInit();

  
  glob.fb_resize = false; 
  
  //rokz::CreateWindow_glfw (glob.glfwin);
  SetupDarkrootWindow (glob); 

  

  rokz::AppInfo_default (glob.instance.app_info);

  //std::vector<const char*> _vls;
  //std::vector<std::string> vstrs;

  rokz::CreateInfo (glob.instance.ci,
                    glob.instance.required_extensions,
                    glob.instance.extension_strings,
                    glob.instance.vals, glob.instance.validation_strings, 
                    glob.instance.app_info); 

  rokz::CreateInstance  (glob.instance.handle, glob.instance.ci);
  rokz::CreateSurface   (&glob.surface, glob.window.glfw_window, glob.instance.handle);
  rokz::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance.handle);

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
  rokz::CreateInfo (glob.device.queue_ci[0], glob.physical_device.family_indices.graphics.value () , &glob.queue_priority);
  rokz::CreateInfo (glob.device.queue_ci[1], glob.physical_device.family_indices.present.value  () , &glob.queue_priority);
  
  // device info
  //VkDeviceCreateInfo&       Default (VkDeviceCreateInfo& info, VkDeviceQueueCreateInfo* quecreateinfo, VkPhysicalDeviceFeatures* devfeats); 
  glob.physical_device.features.samplerAnisotropy = VK_TRUE;

  rokz::CreateInfo (glob.device.ci,
                    glob.device.vals, glob.device.valstrs, 
                    glob.device.dxs, glob.device.dxstrs, 
                    glob.device.queue_ci, &glob.physical_device.features);

  rokz::CreateLogicalDevice (&glob.device.handle, &glob.device.ci, glob.physical_device.handle); 

  // get queue handle
  rokz::GetDeviceQueue (&glob.queues.graphics, glob.physical_device.family_indices.graphics.value(), glob.device.handle);
  rokz::GetDeviceQueue (&glob.queues.present,  glob.physical_device.family_indices.present.value(), glob.device.handle);
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
  
  rokz::QuerySwapchainSupport (glob.swapchain_support_info,
                               glob.surface,
                               glob.physical_device.handle);

  rokz::FrameGroup& frame_group = glob.frame_group;

  frame_group.swapchain.family_indices.push_back (glob.physical_device.family_indices.graphics.value());
  frame_group.swapchain.family_indices.push_back (glob.physical_device.family_indices.present.value ());
  
  rokz::CreateInfo_default (frame_group.swapchain.ci,  
                            frame_group.swapchain.family_indices,
                            glob.surface,
                            kTestExtent, 
                            glob.swapchain_support_info, 
                            glob.physical_device);

  rokz::CreateSwapchain (frame_group.swapchain, glob.device); 
  
  rokz::GetSwapChainImages (frame_group.swapchain_images, frame_group.swapchain, glob.device.handle); 
  rokz::CreateImageViews (frame_group.swapchain_imageviews, frame_group.swapchain_images, glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);


  rokz::CreateRenderPass (glob.render_pass,
                          frame_group.swapchain.ci.imageFormat,
                          glob.msaa_samples,
                          glob.device.handle,
                          glob.physical_device.handle);


  
  // SetupShaderModules also sets up Pipeline Shader State CreateInfo's
  //  SetupDarkShaderModules (glob.pipeline, dark_path, glob.device);
  // bool SetupDarkShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {
  SetupViewportState (glob.viewport_state, glob.frame_group.swapchain.ci.imageExtent); 

  SetupObjectPipeline (glob.obj_pipeline,
                       glob.viewport_state,
                       glob.render_pass,
                       dark_path,
                       glob.frame_group.swapchain,
                       glob.msaa_samples,
                       glob.device); 

  
  //SetupTerrainPipeline (glob.terrain_pipeline, glob.viewport_state, glob.render_pass, dark_path, glob.frame_group.swapchain);

  SetupDarkMultisampleColorResource (glob);

  SetupDarkDepthBuffer (glob);


  
  rokz::CreateFramebuffers (frame_group.swapchain_framebuffers, frame_group.swapchain_imageviews, glob.render_pass,
                            frame_group.swapchain.ci.imageExtent, glob.multisamp_color_imageview.handle,
                            glob.depth_imageview.handle, glob.device); 

  rokz::CreateCommandPool (glob.command_pool.handle, glob.command_pool.ci,
                           glob.physical_device.family_indices, glob.device.handle);

  SetupDarkGeometry (glob); 

  void* pmapped  = nullptr;

  rokz::Buffer vb_x; 
  rokz::CreateInfo_VB_stage (vb_x.ci, DarkrootMesh::VertexSize, glob.darkmesh.verts.size());
  rokz::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::CreateBuffer (vb_x, glob.allocator);
  if (rokz::MapMemory (&pmapped, vb_x.allocation, glob.allocator)) {
    memcpy (pmapped, &glob.darkmesh.verts[0], DarkrootMesh::VertexSize * glob.darkmesh.verts.size()); 
    rokz::UnmapMemory (vb_x.allocation, glob.allocator); 
  }

  rokz::CreateInfo_VB_device (glob.vma_vb_device.ci, DarkrootMesh::VertexSize, glob.darkmesh.verts.size());
  rokz::AllocCreateInfo_device (glob.vma_vb_device.alloc_ci); 
  rokz::CreateBuffer (glob.vma_vb_device, glob.allocator); 

  //rokz::Transfer_2_Device;
  rokz::MoveToBuffer_XB2DB (glob.vma_vb_device, vb_x, DarkrootMesh::VertexSize * glob.darkmesh.verts.size(), 
                            glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  rokz::Destroy (vb_x, glob.allocator); 
  
  // INDEX BUFF
  rokz::Buffer ib_x;
  rokz::CreateInfo_IB_16_stage (ib_x.ci, glob.darkmesh.indices.size()); 
  rokz::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::CreateBuffer (ib_x, glob.allocator);

  if (rokz::MapMemory (&pmapped, ib_x.allocation, glob.allocator)) {
    memcpy (pmapped, &glob.darkmesh.indices[0], DarkrootMesh::IndexSize * glob.darkmesh.indices.size()  ); 
    rokz::UnmapMemory (ib_x.allocation, glob.allocator); 
  }
  
  rokz::CreateInfo_IB_16_device (glob.vma_ib_device.ci, glob.darkmesh.indices.size()); 
  rokz::AllocCreateInfo_device (glob.vma_ib_device.alloc_ci);
  rokz::CreateBuffer (glob.vma_ib_device, glob.allocator);

  rokz::MoveToBuffer_XB2DB  (glob.vma_ib_device, ib_x, DarkrootMesh::IndexSize * glob.darkmesh.indices.size (), 
                             glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 
  rokz::Destroy (ib_x, glob.allocator); 
  //DarkrootMesh& dark_mesh = glob.dark_mesh;
  //
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
  SetupDarkTexture (glob); 
  SetupDarkTextureImageView (glob); 
  

  if (!SetupGlobalDescriptorPool (glob))
    return false;

  if (!SetupObjectDescriptorSets (glob.obj_pipeline,
                                  glob.vma_uniform_buffs,
                                  glob.vma_objparam_buffs,
                                  glob.texture_imageview,
                                  glob.sampler,
                                  glob.descr_pool, 
                                  glob.device)) {
    return false;
  }

  
  // items per frames 
  //frame_group.command_buffer_group.buffers.resize (kMaxFramesInFlight);
  frame_group.syncs.resize (kMaxFramesInFlight);
  frame_group.command_buffers.resize (kMaxFramesInFlight);
  
  rokz::AllocateInfo (frame_group.command_buffer_alloc_info, glob.command_pool.handle); 
  //glob.command_buffer_gourp.resize (kMaxFramesInFlight);

  //frame_group.syncs.resize (kMaxFramesInFlight);
  //glob.create_info.syncs.resize (kMaxFramesInFlight);

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    // sep
    rokz::CreateCommandBuffer(frame_group.command_buffers[i], 
                              frame_group.command_buffer_alloc_info,
                              glob.device.handle);

    rokz::CreateRenderSync (glob.frame_group.syncs[i], frame_group.render_sync_create_info, glob.device.handle);
  }

  //
  SetupDarkroot ();


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
  int        countdown  = 60;

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
    
    UpdateInput(glob, glob.dt);
    
    UpdateDarkroot(glob, glob.dt);

    //    result = RenderFrame (glob, curr_frame, fb_resize, glob.dt);
    uint32_t image_index; 
    if (RenderDarkFrame (glob, image_index, glob.fb_resize, glob.render_pass, glob.obj_pipeline.pipeline,
                     glob.obj_pipeline.descrgroup.descrsets[curr_frame], curr_frame, glob.dt)) {

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
  ShutdownDarkroot ();

  // CLEAN UP
  CleanupDarkroot (glob); 

  //  globmem.reset ();
  printf ( " Mv = v is the correct mul \n"); 
  return 0;
}


//#endif // HIDE_TEST_ROKZ



template<typename ElTy> 
struct RGB {
  ElTy r;
  ElTy g;
  ElTy b;
};

typedef RGB<unsigned char> RGBu8; 


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

  uint32_t mars_terrain_num_X_tiles         = 10;
  uint32_t mars_terrain_num_Y_tiles         = 10;

  float mars_terrain_easting_Tile_step   = 256.0;
  float mars_terrain_northing_Tile_step  = 256.0;
  float mars_tearrain_height_offset       = 0.0;
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
