
#include "rekz.h"              // 
// #include "rokz/defaults.h"
// //#include "rokz/rokz_funcs.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <IL/il.h>
#include <IL/ilu.h>

//#define VMA_IMPLEMENTATION
#include "rokz/shared_types.h"
#include "vk_mem_alloc.h"
 

const size_t kMaxFramesInFlight = 2; 


const std::string data_root =  "/home/djbuzzkill/owenslake/rokz/data"; // 
// #include "rokz/rokz.h"
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan_core.h>

const VkExtent2D  kTestExtent  = { 800, 600 };

#define ROKZ_USE_VMA_ALLOCATION 1



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
struct Vertex_simple {
  glm::vec3 pos; 
  glm::vec3 col; 
  glm::vec2 txc0; 
}; 

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const VkVertexInputBindingDescription kSimpleVertexBindingDesc =  {
  0,                            // binding    
  sizeof (Vertex_simple),       // stride      
  VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   
}; 

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const std::vector<VkVertexInputAttributeDescription> kSimpleBindingAttributeDesc =  {

  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(Vertex_simple, pos),  // .offset   
  },
  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(Vertex_simple, col), 
  },
  VkVertexInputAttributeDescription { // tex coord
    2,                             
    0, 
    VK_FORMAT_R32G32_SFLOAT,
    offsetof(Vertex_simple, txc0), 
  }

}; 



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
const Vertex_simple simple_verts[] = {
    {{-0.7f, -0.7f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{ 0.0f, -1.0f, 0.2f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, 0.2f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, 0.2f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, 0.2f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};


const uint16_t simple_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
struct CreateInfo {

  //VkCommandPoolCreateInfo                  command_pool; 
  //  std::vector<rokz::SyncCreateInfo>        syncs; 
};

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
struct Glob {

  //#ifdef GLOB_COMMENT_OUT   
  Glob ();

  //VkApplicationInfo            app_info; // {};

  rokz::Instance                   instance;
  rokz::PhysicalDevice             physical_device;
  rokz::Device                     device;
  
  struct { VkQueue graphics; VkQueue present; } queues;

  VmaAllocator                     allocator;

  rokz::FrameGroup                       frame_group;
  rokz::SwapchainSupportInfo       swapchain_support_info;

  //std::vector<VkShaderModule>  shader_modules; 
  rokz::CommandPool            command_pool;

  rokz::DescriptorPool         descr_pool;
  rokz::DescriptorGroup          descr_group; 

  rokz::Image                  depth_image;
  rokz::ImageView              depth_imageview; 

  rokz::Image                  multisamp_color_image;
  rokz::ImageView              multisamp_color_imageview; 
  
  VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

  rokz::Buffer                 vma_ib_device;
  rokz::Buffer                 vma_vb_device;


  rokz::RenderPass             render_pass; 

  // image/texture
  rokz::Image                 texture_image; 
  rokz::ImageView             texture_imageview; 
  rokz::Sampler               sampler;

  std::vector<rokz::Buffer>   vma_uniform_buffs;
  std::vector<void*>          uniform_mapped_pointers; 


  rokz::Pipeline              pipeline; 
  rokz::Window                window;
  VkSurfaceKHR                surface; // 

  VkViewport                  viewport;
  VkRect2D                    scissor_rect; 

  float                       queue_priority;
  double                      sim_time; 
  float                       dt;
};



Glob::Glob () :
  instance(),
  device (),
  dt (),
  allocator(),

  // swapchain(),
  // swapchain_images(),
  // swapchain_imageviews(),
  // swapchain_framebuffers(),

  depth_image(),
  depth_imageview(),
  multisamp_color_image(),
  multisamp_color_imageview(),
  msaa_samples (),
  vma_ib_device(),
  vma_vb_device(),
  command_pool(),
  render_pass(),
  texture_image(),
  texture_imageview(),
  sampler(),
  vma_uniform_buffs(),
  uniform_mapped_pointers(),
  descr_pool(),
  descr_group(),
  pipeline(),
  window(),
  physical_device(),
  queue_priority(),
  swapchain_support_info(),
  surface(nullptr),     
  viewport(),
  sim_time(0.0)
{
  queues.graphics = {};
  queues.present = {}; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

//
void SetupDepthBuffer (Glob& glob) {
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
void SetupSampler (Glob& glob) {
  printf ("%s \n", __FUNCTION__); 

  rokz::CreateInfo (glob.sampler.ci, glob.physical_device.properties);
  
  rokz::CreateSampler (glob.sampler, glob.device.handle);  
}


 // ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupShaderModules (Glob& glob, const std::filesystem::path& fspath) {

  printf ("%s \n", __FUNCTION__); 
  // SetupCreateShaderModules (glob .shader_modules,
  //                            glob.pipeline.state_ci.shader_stages,
  //                            rokz_path,
  //                            glob.device);

  // std::vector<ShaderModule>&                    shader_modules,
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = glob.pipeline.state.ci.shader_stages; 
  // const VkDevice&                               device)

  glob.pipeline.shader_modules.resize (2);
  shader_stage_create_infos.resize    (2);
  std::vector<rokz::ShaderModule>& shader_modules = glob.pipeline.shader_modules;
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"data/shader/basic3D_tx_vert.spv" ;

  if (!rokz::CreateShaderModule (shader_modules[0], vert_file_path.string(), glob.device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].handle); 
  
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"data/shader/basic_tx_frag.spv" ;

  if (!rokz::CreateShaderModule (shader_modules[1], frag_file_path.string(), glob.device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[1].handle); 
  //
  return true; 
}

// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------
void SetupDescriptorSetLayout (Glob& glob)  {
  glob.descr_group.dslayout.bindings.resize (2); 
  //rokz::Init (glob.desc_set_layout_bindings[0],
  rokz::DescriptorSetLayoutBinding (glob.descr_group.dslayout.bindings[0],
                                    0,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    1, 
                                    VK_SHADER_STAGE_VERTEX_BIT);

  rokz::DescriptorSetLayoutBinding (glob.descr_group.dslayout.bindings[1],
                                    1,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                    1, 
                                    VK_SHADER_STAGE_FRAGMENT_BIT);

  rokz::CreateDescriptorSetLayout (glob.descr_group.dslayout.handle,
                                   glob.descr_group.dslayout.ci,
                                   glob.descr_group.dslayout.bindings,
                                   glob.device.handle); 

}
// --------------------------------------------------------------------
// VMA
// --------------------------------------------------------------------
void SetupMultisampleColorResource (Glob& glob) {

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
void TestCleanup (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 

  for (auto& ub : glob.vma_uniform_buffs) {
    rokz::Destroy (ub, glob.allocator); 
  }
  
  printf ( "[TODO]:DESTROY DESCRIPTOR LAYOUT\n"); 
  //vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

  rokz::Destroy (glob.sampler, glob.device.handle); 

  rokz::Destroy (glob.descr_pool, glob.device.handle); 
  rokz::Destroy (glob.descr_group, glob.device.handle); 
  rokz::Destroy (glob.texture_imageview, glob.device.handle);

  rokz::Destroy (glob.texture_image, glob.allocator);
  rokz::Destroy (glob.vma_vb_device, glob.allocator);
  rokz::Destroy (glob.vma_ib_device, glob.allocator);

  
  Cleanup (glob.pipeline.handle,
           glob.frame_group.swapchain_framebuffers, glob.frame_group.swapchain_imageviews,

           glob.frame_group.swapchain,
           glob.surface,
           glob.command_pool.handle,
           glob.frame_group.syncs, 
           glob.pipeline.shader_modules,
           glob.pipeline.layout.handle, 
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

bool SetupTexture (Glob& glob) {
  
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
void SetupTextureImageView (Glob& glob) {

  printf ("[%s]\n", __FUNCTION__); 

  rokz::CreateInfo (glob.texture_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, glob.texture_image);  
  if (vkCreateImageView(glob.device.handle, &glob.texture_imageview.ci, nullptr, &glob.texture_imageview.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
  }
}

  
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupDescriptorSets (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 
  // SetupDescriptorSets (glob.descr_group,
  //                      glob.uniform_buffers,
  //                      glob.descr_pool.handle, 
  //                      glob.device);  
  //  const size_t num_sets = kMaxFramesInFlight; // glob.vma_uniform_buffs.size (); 

  rokz::DescriptorGroup& dg = glob.descr_group;

  // use same layout for both allocations
  std::vector<VkDescriptorSetLayout> desc_layouts (kMaxFramesInFlight, dg.dslayout.handle);
  // could have also said: 
  //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
  // but that wouldnt work
  dg.alloc_info = {}; 
  dg.alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  dg.alloc_info.descriptorPool     = glob.descr_pool.handle; 
  dg.alloc_info.descriptorSetCount = kMaxFramesInFlight; // num_sets;
  dg.alloc_info.pSetLayouts        = &desc_layouts[0];
  
  //rokz::AllocateDescriptorSets; // (DescriptorPool& desc_pool, VkDescriptorType type, uint32_t desc_count, const VkDevice &device)
  if (!rokz::AllocateDescriptorSets (dg.descrsets,  kMaxFramesInFlight, dg.alloc_info, glob.device.handle)) {
    printf ("[FAILED] alloc desc sets %s", __FUNCTION__);
    return false;
  }
  
  for (uint32_t i = 0; i < kMaxFramesInFlight; i++) {
    // wtf does this do
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer     = glob.vma_uniform_buffs[i].handle;
    buffer_info.offset     = 0;
    buffer_info.range      = glob.vma_uniform_buffs[i].ci.size ;
    //buffer_info.range      = glob.uniform_buffers[i].create_info.size ;
    //printf ( "%s [%u] buffer_info.range = %lu\n", __FUNCTION__, i, buffer_strucs[i].create_info.size); 
    VkDescriptorImageInfo image_info {};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    image_info.imageView   = glob.texture_imageview.handle;
    image_info.sampler     = glob.sampler.handle;
    //
    std::array<VkWriteDescriptorSet, 2>  descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].dstSet           = dg.descrsets[i];
    descriptor_writes[0].dstBinding       = 0;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &buffer_info;
    descriptor_writes[0].pImageInfo       = nullptr; // Optional
    descriptor_writes[0].pTexelBufferView = nullptr; // Optional}

    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].dstSet           = dg.descrsets[i];
    descriptor_writes[1].dstBinding       = 1;
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[1].descriptorCount  = 1;
    descriptor_writes[1].pBufferInfo      = nullptr;
    descriptor_writes[1].pImageInfo       = &image_info; // Optional
    descriptor_writes[1].pTexelBufferView = nullptr; // Optional}

    vkUpdateDescriptorSets (glob.device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
  }

  return false;
  
}  

//VkDescriptorSetAllocateInfo
bool SetupDescriptorPool (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 
  //SetupDescriptorPool (glob.descr_pool, glob.device);
  rokz::DescriptorPool& dp = glob.descr_pool;
  
  dp.sizes.resize (2); 
  dp.sizes[0] = {} ; //
  dp.sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  dp.sizes[0].descriptorCount = static_cast<uint32_t>(kMaxFramesInFlight);

  dp.sizes[1] = {} ; //
  dp.sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  dp.sizes[1].descriptorCount = static_cast<uint32_t>(kMaxFramesInFlight);

  dp.ci = {};
  dp.ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  dp.ci.poolSizeCount = dp.sizes.size();
  dp.ci.pPoolSizes    = &dp.sizes[0];
  dp.ci.maxSets       = static_cast<uint32_t>(kMaxFramesInFlight);
  dp.ci.flags         = 0;
  
  if (!rokz::CreateDescriptorPool (dp, glob.device.handle)) {
    printf ("[FAILED] %s", __FUNCTION__);
    return false; 
  }

 return true; 
}

// ---------------------------------------------------------------------
// CreateGraphicsPipelineLayout 

// ---------------------------------------------------------------------
// bool CreateGraphicsPipelineLayout_defaults (
//     rokz::PipelineLayout&                 pipeline_layout, 
//     // VkPipelineLayout&                  pipeline_layout,
//     // VkPipelineLayoutCreateInfo&        create_info,
//     VkViewport&                        viewport,
//     VkRect2D&                          scissor,
//     rokz::PipelineStateCreateInfo&     sci,
//     const VkExtent2D&                  swapchain_extent,
//     const VkDevice&                    device)
// {
//   rokz::Viewport (viewport, 0, 0, swapchain_extent.width, swapchain_extent.height, 1.0f);
//   rokz::Rect2D (scissor,  VkOffset2D {0, 0}, swapchain_extent);

//   rokz::CreateInfo (sci.viewport_state, viewport, scissor);
//   rokz::CreateInfo (sci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
//   rokz::CreateInfo (sci.rasterizer); 
//   rokz::CreateInfo (sci.multisampling); 
//   rokz::CreateInfo (sci.depthstencilstate); 

//   if (vkCreatePipelineLayout (device, &pipeline_layout.ci, nullptr, &pipeline_layout.handle) != VK_SUCCESS) {
//     printf("failed to create pipeline layout!\n");
//     return false;
//   }

//   return true;
// }



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void look_at_this_shhhhi () {

  // auto tup = std::tuple{ 420, "wtf", 3.15f}; 
  // auto& [a1, b1, c1] = tup; 

}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupUniformBuffers (std::vector<rokz::BufferStruc>& uniform_buffers,
                           std::vector<void*>& mapped_uniform_pointers, 
                           const VkDevice& device,
                           const VkPhysicalDevice& physdev) {

  uniform_buffers.resize         (kMaxFramesInFlight);
  mapped_uniform_pointers.resize (kMaxFramesInFlight); 

  for (size_t i = 0; i <  kMaxFramesInFlight; i++) {

    if (!rokz::CreateUniformBuffer (uniform_buffers[i], rokz::kSizeOf_MVPTransform, 1, device, physdev)) {
      printf (" [FAIL] CreateUniformbuffer in  CreateUniformbuffers\n"); 
      return false; 
    }

    rokz::MapBuffer (&mapped_uniform_pointers[i], uniform_buffers[i], device); 
  }

  return true; 
}


bool SetupUniformBuffers (Glob& glob) {


  std::vector<void*>             uniform_mapped_pointers; 

  std::vector<rokz::Buffer>& uniform_buffs = glob.vma_uniform_buffs;
  std::vector<void*>&        mapped_ptrs = glob.uniform_mapped_pointers; 
  VkDevice const&          device = glob.device.handle;
  VkPhysicalDevice const&  physdev = glob.physical_device.handle;

  
  uniform_buffs.resize (kMaxFramesInFlight);
  mapped_ptrs.resize (kMaxFramesInFlight); 

  for (size_t i = 0; i <  kMaxFramesInFlight; i++) {

    rokz::CreateInfo_uniform (uniform_buffs[i].ci, rokz::kSizeOf_MVPTransform, 1); 
    rokz::AllocCreateInfo_mapped (uniform_buffs[i].alloc_ci); 

    if (!rokz::CreateBuffer  (uniform_buffs[i], glob.allocator)) {
      printf (" [FAIL] CreateUniformbuffer in  CreateUniformbuffers\n"); 
      return false; 
    }
    
    // if (!rokz::CreateUniformBuffer (uniform_buffs[i], kSizeOf_StandardTransform3D, 1, device, physdev)) {
    //   printf (" [FAIL] CreateUniformbuffer in  CreateUniformbuffers\n"); 
    //   return false; 
    // }
    mapped_ptrs[i] = uniform_buffs[i].alloc_info.pMappedData;
    
    //rokz::MapBuffer (&mapped_uniform_pointers[i], uniform_buffers[i], device); 
  }

  return true; 
}

  

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateUniformBuffer (Glob& glob, uint32_t current_frame, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.sim_time += dt;
  //  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);

  float sim_timef = glob.sim_time;
  
  float asp = (float)glob.frame_group.swapchain.ci.imageExtent.width / (float)glob.frame_group.swapchain.ci.imageExtent.height;
    
  rokz::MVPTransform  mats; 
  mats.model = glm::rotate(glm::mat4(1.0f), sim_timef * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mats.view  = glm::lookAt(glm::vec3(0.0f, 1.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 1.0f, 20.0f);
  mats.proj[1][1] *= -1;
 
  memcpy (glob.uniform_mapped_pointers[current_frame], &mats, rokz::kSizeOf_MVPTransform); 

}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void ResizeCB (GLFWwindow* window, int width, int height) {
  *reinterpret_cast<bool*> (glfwGetWindowUserPointer(window)) = true;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupScene () {
  printf ("%s\n", __FUNCTION__); 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void ShutdownScene () {
  printf ("%s\n", __FUNCTION__); 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateInput (Glob& glob, double dt) {

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void UpdateScene (Glob& glob, double dt) {

  // for (int i = 0; i < 1000000; ++i ) {
      
  //   std::cos ( 3.1459265 * i * 0.000001); 
  // }
}




// --------------------------------------------------------------------
// nue
// --------------------------------------------------------------------
bool RenderFrame (Glob&                   glob,
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

  //
  UpdateUniformBuffer (glob, curr_frame, dt); 

  rokz::Framebuffer& framebuffer    = swapchain_framebuffers[image_index]; 
  VkCommandBuffer&   command_buffer = frame_group.command_buffers[curr_frame]; 
  rokz::Buffer&      vma_vb_device  = glob.vma_vb_device;
  rokz::Buffer&      vma_ib_device  = glob.vma_ib_device; 

  vkResetCommandBuffer (command_buffer, 0); //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);

  rokz::RecordCommandBuffer_indexed (command_buffer,
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

int test_rokz (const std::vector<std::string>& args) {
  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 

  Glob glob; // *globmem; // something representing the app state
  
  glob.dt = 0.0;
  //std::shared_ptr<Glob> globmem = std::make_shared<Glob> ();

  auto rokz_path = std::filesystem::path ( "/home/djbuzzkill/owenslake");
  //Default (glob); 
  
  glfwInit();

  
  bool fb_resize = false; 
  
  //rokz::CreateWindow_glfw (glob.glfwin);
  rokz::CreateWindow (glob.window, kTestExtent.width , kTestExtent.height, "wut"); 

  glfwSetFramebufferSizeCallback (glob.window.glfw_window, ResizeCB); 
  glfwSetWindowUserPointer (glob.window.glfw_window, &fb_resize); 

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

  glob.msaa_samples = rokz::MaxUsableSampleCount (glob.physical_device.handle); 
  
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
 
  vmaCreateAllocator(&allocatorCreateInfo, &glob.allocator);


  // ------------------------------------------ VMA 

  // VkSwapchainCreateInfoKHR& CreateInfo_default (VkSwapchainCreateInfoKHR&   ci, 
  //                                               VkSurfaceKHR                surface,
  //                                               const VkExtent2D&           extent, 
  //                                               const SwapchainSupportInfo& swapchain_support_info, 
  //                                               const PhysicalDevice&       physdev); 
  
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
  //bool CreateSwapchain (VkSwapchainKHR& swapchain, const VkSwapchainCreateInfoKHR& ci, const Device& device);
  // rokz::CreateSwapchain (glob.swapchain, glob.create_info.swapchain,
  //                        glob.surface, glob.physical_device.handle,
  //                        glob.device.handle,
  //                        glob.window.glfw_window);

  // printf (" B4  GLOB RESET\n"); 
  // globmem.reset ();
  // printf (" AFTER GLOB RESET\n"); 
  
  rokz::GetSwapChainImages (frame_group.swapchain_images, frame_group.swapchain, glob.device.handle); 
  rokz::CreateImageViews (frame_group.swapchain_imageviews, frame_group.swapchain_images, glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);
  rokz::CreateRenderPass (glob.render_pass,
                          frame_group.swapchain.ci.imageFormat,
                          glob.msaa_samples,
                          glob.device.handle,
                          glob.physical_device.handle);
  
  // SetupShaderModules also sets up Pipeline Shader State CreateInfo's
  SetupShaderModules (glob, rokz_path);

  rokz::ColorBlendState_default (glob.pipeline.state.color_blend_attachment); 
  rokz::DynamicState_default (glob.pipeline.state.dynamics); 
  // 
  const VkOffset2D offs0 {0, 0}; 
  rokz::Viewport (glob.viewport, offs0.x, offs0.y,
                  frame_group.swapchain.ci.imageExtent.width, frame_group.swapchain.ci.imageExtent.height,
                  1.0f);

  rokz::PipelineStateCreateInfo& psci = glob.pipeline.state.ci;

  rokz::CreateInfo (psci.dynamicstate, glob.pipeline.state.dynamics);
  rokz::CreateInfo (psci.vertexinputstate, kSimpleVertexBindingDesc, kSimpleBindingAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, glob.viewport, rokz::Rect2D (glob.scissor_rect, offs0, frame_group.swapchain.ci.imageExtent));
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, glob.pipeline.state.color_blend_attachment); 
  rokz::CreateInfo (psci.multisampling, glob.msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 

  SetupDescriptorSetLayout (glob); 
  //
  rokz::CreateGraphicsPipelineLayout (glob.pipeline.layout.handle,
                                      glob.pipeline.layout.ci,
                                      glob.descr_group.dslayout.handle,
                                      glob.device.handle);

  rokz::CreateInfo (glob.pipeline.ci,
                    glob.pipeline.layout.handle,
                    glob.render_pass.handle,                    
                    psci.shader_stages,       //const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
                    &psci.input_assembly,     //const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                    &psci.vertexinputstate, // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                    &psci.viewport_state,     //const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                    nullptr, 
                    &psci.rasterizer,         //const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
                    &psci.multisampling,      //const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
                    &psci.depthstencilstate,       //const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
                    &psci.colorblendstate,         //const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
                    &psci.dynamicstate);     // const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 


  if (rokz::CreateGraphicsPipeline (glob.pipeline, glob.device.handle)) {     //const VkDevice                           b          device)
    // printf ("[SUCCESS] -> Create Graphics Pipeline\n"); 
  }

  SetupMultisampleColorResource (glob);

  SetupDepthBuffer (glob);
  
  rokz::CreateFramebuffers (frame_group.swapchain_framebuffers, frame_group.swapchain_imageviews, glob.render_pass,
                            frame_group.swapchain.ci.imageExtent, glob.multisamp_color_imageview.handle,
                            glob.depth_imageview.handle, glob.device); 

  rokz::CreateCommandPool (glob.command_pool.handle, glob.command_pool.ci,
                           glob.physical_device.family_indices, glob.device.handle);

  void* pmapped  = nullptr;

  rokz::Buffer vb_x; 
  rokz::CreateInfo_VB_stage (vb_x.ci, sizeof(Vertex_simple), 8);
  rokz::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::CreateBuffer (vb_x, glob.allocator);
  if (rokz::MapMemory (&pmapped, vb_x.allocation, glob.allocator)) {
    memcpy (pmapped, simple_verts,  sizeof(simple_verts) ); 
    rokz::UnmapMemory (vb_x.allocation, glob.allocator); 
  }

  rokz::CreateInfo_VB_device (glob.vma_vb_device.ci, sizeof(Vertex_simple), 8);
  rokz::AllocCreateInfo_device (glob.vma_vb_device.alloc_ci); 
  rokz::CreateBuffer (glob.vma_vb_device, glob.allocator); 

  //rokz::Transfer_2_Device;
  rokz::MoveToBuffer_XB2DB  (glob.vma_vb_device, // device buffer
                             vb_x, // user buffer, 
                             sizeof(simple_verts),
                             glob.command_pool.handle, 
                             glob.queues.graphics,
                             glob.device.handle); 

  rokz::Destroy (vb_x, glob.allocator); 
  
  // INDEX BUFF
  rokz::Buffer ib_x;
  rokz::CreateInfo_IB_16_stage (ib_x.ci, 12); 
  rokz::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::CreateBuffer (ib_x, glob.allocator);

  if (rokz::MapMemory (&pmapped, ib_x.allocation, glob.allocator)) {
    memcpy (pmapped, simple_indices, sizeof(simple_indices) ); 
    rokz::UnmapMemory (ib_x.allocation, glob.allocator); 
  }
  
  rokz::CreateInfo_IB_16_device (glob.vma_ib_device.ci, 12); 
  rokz::AllocCreateInfo_device (glob.vma_ib_device.alloc_ci);
  rokz::CreateBuffer (glob.vma_ib_device, glob.allocator);

  rokz::MoveToBuffer_XB2DB  (glob.vma_ib_device, // device buffer
                             ib_x, // user buffer, 
                             sizeof(simple_indices),
                             glob.command_pool.handle, 
                             glob.queues.graphics,
                             glob.device.handle); 
  rokz::Destroy  (ib_x, glob.allocator); 
  //
  SetupSampler (glob); 
  // SetupUniformBuffers (glob.uniform_buffers,
  //                      glob.uniform_mapped_pointers, 
  //                      glob.device,
  //                      glob.physical_device); 
  SetupUniformBuffers (glob);
  
  // rokz::DescriptorPool           uniform_descriptor_pool;
  // rokz::DescriptorGroup          uniform_group; 
  SetupTexture (glob); 
  SetupTextureImageView (glob); 
  
  SetupDescriptorPool (glob);

  SetupDescriptorSets (glob);  
  
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
    //rokz::CreateRenderSync (glob.frame_group.frames[i].sync, frame_group.syncs[i].ci, glob.device.handle);
  }

  //
  SetupScene ();
  //
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

  printf ( "Begin run  for [%i] frames.. \n", countdown); 
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
    
    UpdateScene(glob, glob.dt);

    //    result = RenderFrame (glob, curr_frame, fb_resize, glob.dt);
    uint32_t image_index; 
    if (RenderFrame (glob, image_index, fb_resize, glob.render_pass, glob.pipeline,
                     glob.descr_group.descrsets[curr_frame], curr_frame, glob.dt)) {

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
  ShutdownScene();

  // CLEAN UP
  TestCleanup (glob); 




  //#endif

  //intf ("%s [Glob:%zu]\n", __FUNCTION__ ,   sizeof(Glob)); 
  
  //  globmem.reset ();
  return 0;
}


//#endif // HIDE_TEST_ROKZ
