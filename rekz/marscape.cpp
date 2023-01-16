
#include "rekz.h"
#include "marscape.h"
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
#include "rekz/mars_types.h"
#include "rokz/descriptor.h"
#include "rokz/shared_types.h"
#include "vk_mem_alloc.h"

#define ROKZ_USE_VMA_ALLOCATION 1

namespace mars {

  template<typename VTy> 
  using Result = std::tuple<bool, VTy>;
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  const std::string   data_root =  "/home/djbuzzkill/owenslake/rokz/data"; // 
  const VkExtent2D    kTestExtent  = { 800, 600 };


  const size_t        kPatchCount  = 100;

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  const VkVertexInputBindingDescription kVertexBindingDesc =  {
    0,                            // binding    
    sizeof (MarsVert),       // stride      
    VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   
  }; 

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  const std::vector<VkVertexInputAttributeDescription> kBindingAttributeDesc = {

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

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  glm::vec3& unit_angle_xz (glm::vec3& v, float theta) {
    v.x = cos (theta) ;
    v.y = 0.0f;
    v.z = -sinf (theta) ;
    return v; 
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

} // MARS MARS MARS MARS MARS MARS MARS MARS MARS MARS MARS MARS MARS MARS MARS 


const size_t kMaxFramesInFlight = mars::Glob::MaxFramesInFlight; 
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
using namespace mars;


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool SetupMarsWindow (Glob& glob) {

  glfwInit();
  rokz::CreateWindow (glob.window, kTestExtent.width , kTestExtent.height, "wut"); 

  glfwSetFramebufferSizeCallback (glob.window.glfw_window, window_handler::on_resize ); 
  glfwSetKeyCallback (glob.window.glfw_window, window_handler::on_keypress);
  glfwSetCursorPosCallback(glob.window.glfw_window, window_handler::on_mouse_move);
  glfwSetMouseButtonCallback(glob.window.glfw_window, window_handler::on_mouse_button);
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

  rokz::CreateInfo (glob.sampler.ci, glob.physical_device.properties);
  
  rokz::CreateSampler (glob.sampler, glob.device.handle);  

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupPatchGeometry (Glob& glob) {

  // wat even is this

  rokz::MVPTransform ub0;

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void SetupMarsDepthBuffer (Glob& glob) {
  printf ("%s\n", __FUNCTION__); 

  rokz::SwapchainGroup& frame_group = glob.frame_group;


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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void CleanupMars (Glob& glob) {

  
  printf ("%s \n", __FUNCTION__); 

  for (auto& ub : glob.vma_uniform_buffs) {
    rokz::Destroy (ub, glob.allocator); 
  }

  for (auto buf : glob.vma_objparam_buffs) {  
    rokz::Destroy (buf, glob.allocator);
  }

  rokz::Destroy (glob.sampler, glob.device.handle); 
  rokz::Destroy (glob.descriptor_pool, glob.device.handle); 
  rokz::Destroy (glob.terrain_pipeline.descrgroup, glob.device.handle); 
  rokz::Destroy (glob.grid_pipeline.descrgroup, glob.device.handle); 
  rokz::Destroy (glob.texture_imageview, glob.device.handle);

  rokz::Destroy (glob.texture_image, glob.allocator);
  rokz::Destroy (glob.vma_vb_device, glob.allocator);
  rokz::Destroy (glob.vma_ib_device, glob.allocator);
  
  Cleanup (glob.terrain_pipeline.pipeline.handle,
           glob.frame_group.framebuffers, glob.frame_group.imageviews,

           glob.frame_group.swapchain,
           glob.surface,
           glob.command_pool.handle,
           glob.frame_sequence.syncs, 
           glob.terrain_pipeline.pipeline.shader_modules,
           glob.terrain_pipeline.pipeline.layout.handle, 
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
bool SetupMarsDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

  printf ("%s", __FUNCTION__); 

  //  UniformBinding
  //  SamplerBinding
  descrgroup.dslayout.bindings.resize (6);
  //rokz::Init (glob.desc_set_layout_bindings[0],

  // MVPTransform
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[0],
                                    0,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    1,
                                    VK_SHADER_STAGE_VERTEX_BIT);
  // ViewParams dont need yet rly
  // rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[x],
  //                                   1,
  //                                   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
  //                                   1,
  //                                   VK_SHADER_STAGE_VERTEX_BIT);
  // PatchParams
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[1],
                                    2,
                                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    kPatchCount, 
                                    VK_SHADER_STAGE_VERTEX_BIT);

  // HEIGHT map
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[2],
                                    3,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, //VK_DESCRIPTOR_TYPE_SAMPLER
                                    kPatchCount, 
                                    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);
  // NORMAL map
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[3],
                                    4,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, //VK_DESCRIPTOR_TYPE_SAMPLER
                                    kPatchCount, 
                                    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);

  // COLOR image
  rokz::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[4],
                                    5,
                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, //VK_DESCRIPTOR_TYPE_SAMPLER
                                    kPatchCount, 
                                    VK_SHADER_STAGE_FRAGMENT_BIT);

  if (!rokz::CreateDescriptorSetLayout (descrgroup.dslayout.handle,
                                        descrgroup.dslayout.ci,
                                        descrgroup.dslayout.bindings,
                                        device.handle)) {
    printf (" --> [FAILED] \n"); 
    return false;
  }

  return true; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupTerrainDescriptorSets (PipelineGroup& pipelinegroup,

                                const std::vector<rokz::Buffer>& vma_uniform_buffs,
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
  rokz::AllocateInfo (dg.alloc_info , descrlos, descpool);
  
  if (!rokz::AllocateDescriptorSets (dg.descrsets, kMaxFramesInFlight, dg.alloc_info, device.handle)) {
    printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
    return false;
  }
  //

  struct notype {}; 
  
  for (uint32_t flight = 0; flight < kMaxFramesInFlight; flight++) {

    // MVPTransform
    VkDescriptorBufferInfo mvp_info{};
    mvp_info.buffer     = 0; // vma_uniform_buffs[i].handle;
    mvp_info.offset     = 0;
    mvp_info.range      = sizeof (rokz::MVPTransform); 

    // mar::ViewParams
    VkDescriptorBufferInfo view_info{};
    view_info.buffer     = 0; // vma_uniform_buffs[i].handle;
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
  
  // for (uint32_t i = 0; i < kMaxFramesInFlight; i++) {
  //   // wtf does this do
  //   VkDescriptorBufferInfo buffer_info{};
  //   buffer_info.buffer     = vma_uniform_buffs[i].handle;
  //   buffer_info.offset     = 0;
  //   buffer_info.range      = vma_uniform_buffs[i].ci.size ;
    
  //   std::vector<VkDescriptorBufferInfo>  objparams (kSceneObjCount, VkDescriptorBufferInfo {});
  //   for (size_t iobj = 0; iobj < objparams.size (); ++iobj) { 
  //     objparams[iobj].buffer   = vma_objparam_buffs[i].handle; //
  //     objparams[iobj].offset   = 0;         // min_uniform_buffer_offset_alignment ??
  //     objparams[iobj].range    = sizeof(SceneObjParam) ;            //glob.vma_objparam_buffs[i].ci.size;
  //   }
    
   //   VkDescriptorImageInfo image_info {};
  //   image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
  //   image_info.imageView   = texture_imageview.handle;
  //   image_info.sampler     = sampler.handle;
  // //
  //   std::array<VkWriteDescriptorSet, 3>  descriptor_writes {};
  
  //   descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  //   descriptor_writes[0].pNext            = nullptr;    
  //   descriptor_writes[0].dstSet           = dg.descrsets[i];
  //   descriptor_writes[0].dstBinding       = 0;
  //   descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

  //   descriptor_writes[0].dstArrayElement  = 0;
  //   descriptor_writes[0].descriptorCount  = 1;
  //   descriptor_writes[0].pBufferInfo      = &buffer_info;
  //   descriptor_writes[0].pImageInfo       = nullptr; 
  //   descriptor_writes[0].pTexelBufferView = nullptr; 

  //   descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  //   descriptor_writes[1].pNext            = nullptr;    
  //   descriptor_writes[1].dstSet           = dg.descrsets[i];
  //   descriptor_writes[1].dstBinding       = 1;
  //   descriptor_writes[1].dstArrayElement  = 0;
  //   descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  //   descriptor_writes[1].descriptorCount  = objparams.size(); // <
  //   descriptor_writes[1].pBufferInfo      = &objparams[0]; 
  //   descriptor_writes[1].pImageInfo       = nullptr; 
  //   descriptor_writes[1].pTexelBufferView = nullptr; 

  //   descriptor_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  //   descriptor_writes[2].pNext            = nullptr;    
  //   descriptor_writes[2].dstSet           = dg.descrsets[i];
  //   descriptor_writes[2].dstBinding       = 2;
  //   descriptor_writes[2].dstArrayElement  = 0;
  //   descriptor_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
  //   descriptor_writes[2].descriptorCount  = 1;
  //   descriptor_writes[2].pBufferInfo      = nullptr;
  //   descriptor_writes[2].pImageInfo       = &image_info; 
  //   descriptor_writes[2].pTexelBufferView = nullptr; 

  //   vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  // }

    VkWriteDescriptorSet wdescs {}; 
    VK_DESCRIPTOR_TYPE_SAMPLER; 
  // return true;


  return false;
}



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupGlobalDescriptorPool (Glob& glob) {

  return false;
}



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupTerrainPipeline (PipelineGroup& pipelinegroup,
                          const rokz::ViewportState& vps,
                          const rokz::RenderPass& renderpass,
                          const std::filesystem::path& fspath,
                          const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          const rokz::Device& device) {

  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);

  //rokz::FrameGroup& frame_group = glob.frame_group;
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
  rokz::CreateInfo (psci.vertexinputstate, kVertexBindingDesc, kBindingAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, vps);
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_PATCH_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, pipelinegroup.pipeline.state.colorblend_attachments); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 
  SetupMarsDescriptorLayout (  pipelinegroup.descrgroup, device); 


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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool SetupMarsUniforms (mars::Glob& glob) {
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

    rokz::CreateInfo_uniform (objparams[i].ci, mars::SizeOf_PatchUBO, 128);
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
void UpdateMarsUniforms (mars::Glob& glob, uint32_t current_frame, double dt) {
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
 
  if (mars::PatchUBO* obj = reinterpret_cast<mars::PatchUBO*> (rokz::MappedPointer (glob.vma_objparam_buffs[current_frame]))) {
  
    glm::vec3 va, vb;
    unit_angle_xz (va, 5.0 * sim_timef ); 
    unit_angle_xz (vb, 5.0 * sim_timef + kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    // obj[0].modelmat = glm::rotate(model0, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    // obj[1].modelmat = glm::rotate(model1, sim_timef * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
  }
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
//void SetupDarkroot () { printf ("%s\n", __FUNCTION__); }
//void ShutdownDarkroot () { printf ("%s\n", __FUNCTION__); }
//void UpdateInput (Glob& glob, double dt) { }
void UpdateDarkroot (Glob& glob, double dt) { }

// ---------------------------------------------------------------------
// RecordDarkCommandBuffer_indexed
// ---------------------------------------------------------------------
bool RecordMarsRenderPass_indexed (Glob& glob, 
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
bool RenderMarsFrame (mars::Glob&           glob,
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
int mars_run (const std::vector<std::string>& args) {
  printf ("%s\n", __FUNCTION__);

  Glob glob; // *globmem; // something representing the app state
  glob.dt = 0.0;
  glob.fb_resize = false; 

  
  SetupMarsWindow (glob); 

  rokz::cx::AppInfo_default (glob.instance.app_info);

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

  rokz::cx::CreateInfo (glob.device.ci,
                    glob.device.vals, glob.device.valstrs, 
                    glob.device.dxs, glob.device.dxstrs, 
                    glob.device.queue_ci, &glob.physical_device.features);

  rokz::cx::CreateLogicalDevice (&glob.device.handle, &glob.device.ci, glob.physical_device.handle); 

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

  rokz::SwapchainGroup& frame_group = glob.frame_group;

  frame_group.swapchain.family_indices.push_back (glob.physical_device.family_indices.graphics.value());
  frame_group.swapchain.family_indices.push_back (glob.physical_device.family_indices.present.value ());
  
  rokz::cx::CreateInfo_default (frame_group.swapchain.ci,  
                            frame_group.swapchain.family_indices,
                            glob.surface,
                            kTestExtent, 
                            glob.swapchain_support_info, 
                            glob.physical_device);

  rokz::cx::CreateSwapchain (frame_group.swapchain, glob.device); 
  
  rokz::cx::GetSwapChainImages (frame_group.images, frame_group.swapchain, glob.device.handle); 


  rokz::CreateImageViews (frame_group.imageviews, frame_group.images, glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);


  rokz::CreateRenderPass (glob.render_pass,
                          frame_group.swapchain.ci.imageFormat,
                          glob.msaa_samples,
                          glob.device.handle,
                          glob.physical_device.handle);


  
  SetupViewportState (glob.terrain_pipeline.pipeline.state.viewport, glob.frame_group.swapchain.ci.imageExtent); 

  // SetupGridscape ();
  SetupTerrainPipeline (glob.terrain_pipeline,
                        glob.terrain_pipeline.pipeline.state.viewport,
                        glob.render_pass,
                        mars::data_root,
                        glob.frame_group.swapchain,
                        glob.msaa_samples,
                        glob.device); 
  //SetupTerrainPipeline (glob.terrain_pipeline, glob.viewport_state, glob.render_pass, dark_path, glob.frame_group.swapchain);

printf ("[ HIDE_MARS_RUN | %i ]\n", __LINE__ + 1);
#ifdef HIDE_MARS_RUN 


 SetupMultisampleColorResource (glob);

  SetupMarsDepthBuffer (glob);
  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);


  
  rokz::CreateFramebuffers (frame_group.swapchain_framebuffers, frame_group.swapchain_imageviews, glob.render_pass,
                            frame_group.swapchain.ci.imageExtent, glob.multisamp_color_imageview.handle,
                            glob.depth_imageview.handle, glob.device); 

  rokz::CreateInfo (glob.command_pool.ci, glob.physical_device.family_indices.graphics.value());
  rokz::CreateCommandPool (glob.command_pool.handle, glob.command_pool.ci, glob.device.handle);


  //SetupPatchGeometry (glob); 


  "Setup Index+Vertex Buffers";

  SetupTerrainResources (glob); 
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

  SetupTerrainSamplers (glob);
  

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

  
  if (!SetupTerrainDescriptorSets (glob.obj_pipeline,
                                   glob.vma_uniform_buffs,
                                   glob.vma_objparam_buffs,
                                   glob.texture_imageview,
                                   glob.sampler,
                                   glob.descr_pool, 
                                   glob.device)) {
    printf ("[FAILED] --> SetupTerrainDescriptorSets \n"); 
    return false;
  }
  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);



  //frame_group.command_buffer_group.buffers.resize (kMaxFramesInFlight);
  frame_group.syncs.resize (kMaxFramesInFlight);
  frame_group.command_buffers.resize (kMaxFramesInFlight);
  
  rokz::AllocateInfo (frame_group.command_buffer_alloc_info, glob.command_pool.handle); 

  // 
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    // sep
    rokz::CreateCommandBuffer(frame_group.command_buffers[i], 
                              frame_group.command_buffer_alloc_info,
                              glob.device.handle);

    rokz::CreateRenderSync (glob.frame_group.syncs[i], frame_group.render_sync_create_info, glob.device.handle);
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
    if (RenderMarsFrame (glob, image_index, glob.fb_resize, glob.render_pass, glob.obj_pipeline.pipeline,
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
  printf ("[ %s | %i ]\n", __FUNCTION__, __LINE__);


  // CLEAN UP
  CleanupMars (glob); 

#endif

  printf ("%[LEAVING] --> %s\n", __FUNCTION__);
  return 0; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
template<typename ElTy> 
struct RGB_gen {
  ElTy r;
  ElTy g;
  ElTy b;
};

typedef RGB_gen<unsigned char> RGBu8; 

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


//
//
bool SetupOutlinePipeline () {

  "shaders"; 
  "descriptor sets";
  "pipeline layout";
 
  return false;
}
