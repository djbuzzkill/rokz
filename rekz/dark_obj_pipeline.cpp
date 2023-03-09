
#include "dark_obj_pipeline.h"
#include "rokz/descriptor.h"
#include "rokz/file.h"
#include "rokz/pipeline.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

//const size_t max_frames_in_flight  = rekz::Glob::MaxFramesInFlight; 
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
namespace rekz { 

  //   typedef struct VkDescriptorSetLayoutBinding {
  //     uint32_t              binding;
  //     VkDescriptorType      descriptorType;
  //     uint32_t              descriptorCount;
  //     VkShaderStageFlags    stageFlags;
  //     const VkSampler*      pImmutableSamplers;
  // } VkDescriptorSetLayoutBinding;
  const std::vector<VkDescriptorSetLayoutBinding> kObjDescriptorBindings =
    {
      { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , kMaxObjectCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
      { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1              , VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
    };

  // --------------------------------------------------------------------
  // vert input binding, diffrnt from input attriubutes
  // --------------------------------------------------------------------
  const VkVertexInputBindingDescription kVertexInputBindingDesc = {
    0,                          // binding    
    sizeof (PolyObjVert),       // stride      
    VK_VERTEX_INPUT_RATE_VERTEX // inputRate   
  }; 

  // --------------------------------------------------------------------
  // vert atribute desc
  // --------------------------------------------------------------------
  const std::vector<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc = {

    VkVertexInputAttributeDescription { // pos
      0,                             // .location 
      0,                             // .binding  
      VK_FORMAT_R32G32B32_SFLOAT,    // .format   
      offsetof(PolyObjVert, pos),  // .offset   
    },

    VkVertexInputAttributeDescription { // color
      1,                              
      0, 
      VK_FORMAT_R32G32B32_SFLOAT,
      offsetof(PolyObjVert, nrm), 
    },

    VkVertexInputAttributeDescription { // color
      2,                              
      0, 
      VK_FORMAT_R32G32B32_SFLOAT,
      offsetof(PolyObjVert, col), 
    },
    VkVertexInputAttributeDescription { // tex coord
      3,                             
      0, 
      VK_FORMAT_R32G32_SFLOAT,
      offsetof(PolyObjVert, txc0), 
    }

  }; 
}

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool setup_object_shader_modules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {

  printf ("%s \n", __FUNCTION__); 
  
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  std::vector<rokz::ShaderModule>&              shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (2);
  shader_stage_create_infos.resize(2);
  //
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"data/objz/polyobj_vert.spv" ;
  //printf ( "LINE [%i] --> %s \n", __LINE__, vert_file_path.string().c_str()); 
  
  rokz::CreateInfo (shader_modules[0].ci, rokz::From_file (shader_modules[0].bin, vert_file_path.string())); 
  if (!rokz::CreateShaderModule (shader_modules[0], device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].entry_point, shader_modules[0].handle); //   

  //
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"data/objz/polyobj_frag.spv" ;
  //printf ( "LINE [%i] --> %s \n", __LINE__, frag_file_path.string().c_str()); 
  
  rokz::CreateInfo (shader_modules[1].ci, rokz::From_file (shader_modules[1].bin, frag_file_path.string())); 
  if (!rokz::CreateShaderModule (shader_modules[1], device.handle))
    return false; 
  
  rokz::CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT,  shader_modules[1].entry_point,  shader_modules[1].handle); 
  //
  return true; 
}


// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::SetupObjectUniforms (std::vector<rokz::Buffer>& uniform_buffs, std::vector<rokz::Buffer>& objparams,
                                uint32_t num_sets, const rokz::Device& device) {
 printf ("%s", __FUNCTION__);

 // does this belong in pipeline?  
 uniform_buffs.resize (num_sets);

 
 //
 objparams.resize (num_sets);
 for (size_t i = 0; i < num_sets; i++) {
   // TODO: do this somewhere else, this isnt truly global, since poly_pipeline creates it
   CreateUniformBuffer (uniform_buffs[i], sizeof(rokz::MVPTransform), 1, device);

   CreateUniformBuffer (objparams[i], sizeof(PolygonParam), kMaxObjectCount, device);
 }

 printf (" --> [true] \n"); 
 return true; 
}

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::SetupObjectUniforms (std::vector<rokz::Buffer>& objparams, uint32_t num_sets, const rokz::Device& device) {
  //printf ("%s", __FUNCTION__);
 //
 objparams.resize (num_sets);
 for (size_t i = 0; i < num_sets; i++) {

   CreateUniformBuffer (objparams[i], sizeof(PolygonParam), kMaxObjectCount, device);
 }

 //printf (" --> [true] \n"); 
 return true; 
}


// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::BindObjectDescriptorResources (std::vector<VkDescriptorSet>&    dss ,
                                          const std::vector<rokz::Buffer>& objparam_buffs,
                                          const rokz::ImageView&           texture_imageview, 
                                          const rokz::Sampler&             sampler, 
                                          const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                          const rokz::Device&              device) {

  //printf ("[%i]  %s\n", __LINE__, __FUNCTION__);
   assert (dss.size () == objparam_buffs.size ());

  //
  for (uint32_t i = 0; i < dss.size (); i++) {

    std::vector<VkDescriptorBufferInfo> objparams (kMaxObjectCount, VkDescriptorBufferInfo {});

    for (size_t iobj = 0; iobj < objparams.size (); ++iobj) { 
      objparams[iobj].buffer   = objparam_buffs[i].handle;    //
      objparams[iobj].offset   = iobj * sizeof(PolygonParam); // min_uniform_buffer_offset_alignment ??
      objparams[iobj].range    = sizeof(PolygonParam) ;       //glob.vma_objparam_buffs[i].ci.size;
    }
    
    // vector<VkDescriptorImageInfo> imageinfos; 
    VkDescriptorImageInfo image_info {};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    image_info.imageView   = texture_imageview.handle;
    image_info.sampler     = sampler.handle;

    //
    std::array<VkWriteDescriptorSet, 2> descriptor_writes {};

    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;
    descriptor_writes[0].dstSet           = dss[i];
    descriptor_writes[0].dstBinding       = 0;       // does it match in shader? 
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = objparams.size(); // <
    descriptor_writes[0].pBufferInfo      = &objparams[0]; 
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 
                      
    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = dss[i];
    descriptor_writes[1].dstBinding       = 1;      // <-- change shader too
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[1].descriptorCount  = 1; // kMaxObjectCount <--- soon
    descriptor_writes[1].pBufferInfo      = nullptr;
    descriptor_writes[1].pImageInfo       = &image_info; 
    descriptor_writes[1].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;

}

// ----------------------------------------------------------------------------------------
// init proto more orthogonal version (new SetupObjectPipeline)
// ----------------------------------------------------------------------------------------
bool rekz::InitObjPipeline (rokz::Pipeline&                           pipeline,
                            rokz::PipelineLayout&                     plo,
                            const std::vector<VkDescriptorSetLayout>& dslos,
                            //0
                            const std::filesystem::path&              fspath,
                            const VkExtent2D&                         viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits                     msaa_samples,
                            VkFormat                                  color_format,
                            VkFormat                                  depth_format,
                            const rokz::Device&                       device) {

  printf ("[%s] --> %i \n", __FUNCTION__, __LINE__); 
  //std::vector<VkDescriptorSetLayout> dslos (1, dslo.handle); 
  //  rokz::CreateInfo (plo.ci, dslos); //, push_constants); 

  // darkroot::PushConstants; <-- darkroot shouldnt belong in here 
  
  rokz::DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(rekz::PushConstants), dslos, device.handle);

  rokz::PipelineState_default (pipeline.state, msaa_samples, kVertexInputBindingAttributeDesc,
                               kVertexInputBindingDesc, viewport_extent); 
  // ^ !! shader modules is part of pipelinestate 
  setup_object_shader_modules (pipeline, fspath, device);

  // proto more orthogonal version
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = color_format;

  rokz::CreateInfo  (pipeline.ext.pipeline_rendering.ci,
                     pipeline.ext.pipeline_rendering.color_formats, depth_format); 

  auto& psci = pipeline.state.ci;
  //
  rokz::CreateInfo (pipeline.ci,
                    plo.handle,
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

  if (!rokz::CreateGraphicsPipeline (pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }

  return true;
}

#ifdef REKZ_HIDE_PER_DESCRIPTORSET_BINDOBJECTDESCRIPTORRESOURCES
// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::BindObjectDescriptorResources (VkDescriptorSet                  ds,
                                          rokz::Buffer&                    objparam_buff,
                                          const rokz::ImageView&           texture_imageview, 
                                          const rokz::Sampler&             sampler, 
                                          const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                          const rokz::Device&              device) {

  printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

  std::vector<VkDescriptorBufferInfo> objparams (kMaxObjectCount, VkDescriptorBufferInfo {});

  for (size_t iobj = 0; iobj < objparams.size (); ++iobj) { 
    objparams[iobj].buffer   = objparam_buff.handle;    //
    objparams[iobj].offset   = iobj * sizeof(PolygonParam); // min_uniform_buffer_offset_alignment ??
    objparams[iobj].range    = sizeof(PolygonParam) ;       //glob.vma_objparam_buffs[i].ci.size;
  }
    
  // vector<VkDescriptorImageInfo> imageinfos; 
  VkDescriptorImageInfo image_info {};
  image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
  image_info.imageView   = texture_imageview.handle;
  image_info.sampler     = sampler.handle;

  //
  std::array<VkWriteDescriptorSet, 2> descriptor_writes {};

  descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor_writes[0].pNext            = nullptr;
  descriptor_writes[0].dstSet           = ds;
  descriptor_writes[0].dstBinding       = 0;       // does it match in shader? 
  descriptor_writes[0].dstArrayElement  = 0;
  descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptor_writes[0].descriptorCount  = objparams.size(); // <
  descriptor_writes[0].pBufferInfo      = &objparams[0]; 
  descriptor_writes[0].pImageInfo       = nullptr; 
  descriptor_writes[0].pTexelBufferView = nullptr; 
                      
  descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor_writes[1].pNext            = nullptr;    
  descriptor_writes[1].dstSet           = ds;
  descriptor_writes[1].dstBinding       = 1;      // <-- change shader too
  descriptor_writes[1].dstArrayElement  = 0;
  descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
  descriptor_writes[1].descriptorCount  = 1; // kMaxObjectCount <--- soon
  descriptor_writes[1].pBufferInfo      = nullptr;
  descriptor_writes[1].pImageInfo       = &image_info; 
  descriptor_writes[1].pTexelBufferView = nullptr; 

  vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  return true;

}
#endif


#ifdef REKZ_HIDE_OLD_BINDOBJECTDESCRIPTORRESOURCES
// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::BindObjectDescriptorSets (std::vector<VkDescriptorSet>&    dss ,
                                     const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                     const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                     const rokz::ImageView&           texture_imageview, 
                                     const rokz::Sampler&             sampler, 
                                     const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                     const rokz::Device&              device) {

  printf ("[%i]  %s\n", __LINE__, __FUNCTION__);
  //  SetupObjectDescriptorSets does too many things. it:
  //  - initializes DescriptorGroup::vector<vkDescriptorSets>
  //  - allocates from descriptor pool, but relies on externally initialized DescriptorPool
  //  - binds resources to the descriptors vkUpdaateDescriptorSets

   assert (dss.size () == vma_uniform_buffs.size ());
   assert (dss.size () == vma_objparam_buffs.size ());

  // rokz::DescriptorGroup& dg = descriptorgroup;

  // // use same layout for both allocations
  // std::vector<VkDescriptorSetLayout> descrlos (max_frames_in_flight, dslayout.handle);
  // // could have also said: 
  // //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
  // // but that wouldnt work
  // rokz::cx::AllocateInfo (dg.alloc_info , descrlos, descriptorgroup.pool);
  
  // if (!rokz::cx::AllocateDescriptorSets (dg.descrsets, max_frames_in_flight, dg.alloc_info, device.handle)) {
  //   printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
  //   return false;
  // }

  //
  for (uint32_t i = 0; i < dss.size (); i++) {
    // wtf does this do
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer     = vma_uniform_buffs[i].handle;
    buffer_info.offset     = 0;
    buffer_info.range      = vma_uniform_buffs[i].ci.size ;

    std::vector<VkDescriptorBufferInfo>
      objparams (kMaxObjectCount, VkDescriptorBufferInfo {});

    for (size_t iobj = 0; iobj < objparams.size (); ++iobj) { 
      objparams[iobj].buffer   = vma_objparam_buffs[i].handle; //
      objparams[iobj].offset   = iobj * sizeof(PolygonParam);  // min_uniform_buffer_offset_alignment ??
      objparams[iobj].range    = sizeof(PolygonParam) ;        //glob.vma_objparam_buffs[i].ci.size;
    }
    
    //buffer_info.range      = glob.uniform_buffers[i].create_info.size ;
    VkDescriptorImageInfo image_info {};
    image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    image_info.imageView   = texture_imageview.handle;
    image_info.sampler     = sampler.handle;
    //
    std::array<VkWriteDescriptorSet, 3> descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;    
    descriptor_writes[0].dstSet           = dss[i];
    descriptor_writes[0].dstBinding       = 0;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &buffer_info;
    descriptor_writes[0].pImageInfo       = nullptr;
    descriptor_writes[0].pTexelBufferView = nullptr;

    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;
    descriptor_writes[1].dstSet           = dss[i];
    descriptor_writes[1].dstBinding       = 1;
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[1].descriptorCount  = objparams.size(); // <
    descriptor_writes[1].pBufferInfo      = &objparams[0]; 
    descriptor_writes[1].pImageInfo       = nullptr; 
    descriptor_writes[1].pTexelBufferView = nullptr; 

    descriptor_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[2].pNext            = nullptr;    
    descriptor_writes[2].dstSet           = dss[i];
    descriptor_writes[2].dstBinding       = 2;
    descriptor_writes[2].dstArrayElement  = 0;
    descriptor_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[2].descriptorCount  = 1;
    descriptor_writes[2].pBufferInfo      = nullptr;
    descriptor_writes[2].pImageInfo       = &image_info; 
    descriptor_writes[2].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;

}
#endif
