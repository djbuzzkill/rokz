
#include "dark_obj_pipeline.h"
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------


const size_t max_frames_in_flight  = darkroot::Glob::MaxFramesInFlight; 
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
namespace darkroot { 


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
}


// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool  darkroot::SetupObjectShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {

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
bool darkroot::SetupObjDescriptorPool (Glob& glob) {

  printf ("%s \n", __FUNCTION__); 
  //SetupDescriptorPool (glob.descr_pool, glob.device);
  rokz::DescriptorPool& dp = glob.descr_pool;
  
  dp.sizes.resize (3); 
  dp.sizes[0] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, max_frames_in_flight }; 
  dp.sizes[1] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  128 * max_frames_in_flight} ; //
  dp.sizes[2] = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, max_frames_in_flight } ; //

  rokz::cx::CreateInfo ( dp.ci, max_frames_in_flight, dp.sizes); 
  
  if (!rokz::cx::CreateDescriptorPool (dp, glob.device.handle)) {
    printf ("[FAILED] %s", __FUNCTION__);
    return false; 
  }

 return true; 
}


// --------------------------------------------------------------------
// SetupDarkDescriptorSetLayout (
// --------------------------------------------------------------------
bool darkroot::SetupObjectDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

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
bool darkroot::SetupObjectDescriptorSets (rekz::PipelineGroup& pipelinegroup,

                                const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                const rokz::ImageView& texture_imageview, 
                                const rokz::Sampler&   sampler, 

                                const rokz::DescriptorPool& descpool,
                                const rokz::Device&         device) {

  printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

  rokz::DescriptorGroup& dg = pipelinegroup.descrgroup;

  // use same layout for both allocations
  std::vector<VkDescriptorSetLayout> descrlos (max_frames_in_flight, dg.dslayout.handle);
  // could have also said: 
  //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
  // but that wouldnt work
  rokz::cx::AllocateInfo (dg.alloc_info , descrlos, descpool);
  
  if (!rokz::cx::AllocateDescriptorSets (dg.descrsets, max_frames_in_flight, dg.alloc_info, device.handle)) {
    printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
    return false;
  }
  //

  for (uint32_t i = 0; i < max_frames_in_flight; i++) {
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

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool darkroot::SetupObjectPipeline (rekz::PipelineGroup& pipelinegroup,
                          // const rokz::RenderPass& renderpass,  // using dynamic render pass
                          const std::filesystem::path& fspath,
                          const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          VkFormat color_format,
                          VkFormat depth_format,
                          const rokz::Device& device) {
  //
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
