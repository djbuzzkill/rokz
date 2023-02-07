
#include "grid_pipeline.h"
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
const VkVertexInputBindingDescription kGridVertexBindingDesc =  {
  0,                            // binding    
  sizeof (rekz::GridVert),       // stride      
  VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   

};
                                                     
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
const std::vector<VkVertexInputAttributeDescription> kGridVertInputAttributeDesc = {
  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(rekz::GridVert, pos),  // .offset   
  },

  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(rekz::GridVert, col), 
  },
};

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool SetupGridShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device)  {

  printf ("%s \n", __FUNCTION__); 

  
  std::vector<VkPipelineShaderStageCreateInfo>& pss_create_infos = pipeline.state.ci.shader_stages; 
  std::vector<rokz::ShaderModule>&              shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (2);
  pss_create_infos.resize(2);
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"data/shader/gridscape_vert.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[0], vert_file_path.string(), device.handle))
    return false; 
  
  // for pipeline state ci
  rokz::CreateInfo (pss_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].handle); 
  
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"data/shader/gridscape_frag.spv" ;
  if (!rokz::CreateShaderModule (shader_modules[1], frag_file_path.string(), device.handle))
    return false; 

  // for pipeline state ci
  rokz::CreateInfo (pss_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[1].handle); 
  //
  return true; 

}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool rekz::SetupGridDescriptorPool (rokz::DescriptorPool& dp, uint32_t num_pools, const rokz::Device& device) {
  printf ("%s \n", __FUNCTION__); 
  
  dp.sizes.resize (1); 
  dp.sizes[0] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, num_pools} ;  // MVPTransform struc

  rokz::cx::CreateInfo ( dp.ci, num_pools, dp.sizes); 
  if (!rokz::cx::CreateDescriptorPool (dp, device.handle)) {
    printf ("[FAILED] %s", __FUNCTION__);
    return false; 
  }

  return true; 
}

// ---------------------------------------------------------------------
// Setup 'DescriptorLayout' is how it looks
// ---------------------------------------------------------------------
bool SetupGridDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

  printf ("%s", __FUNCTION__); 


  descrgroup.dslayout.bindings.resize (1);
  // MVPTransform
  rokz::cx::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[0],
                                        0,
                                        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                        1,
                                        VK_SHADER_STAGE_VERTEX_BIT);
  // 
  if (!rokz::cx::CreateDescriptorSetLayout (descrgroup.dslayout.handle,
                                            descrgroup.dslayout.ci,
                                            descrgroup.dslayout.bindings,
                                            device.handle)) {
    printf (" --> [FAILED] \n"); 
    return false;
  }

  return true; 
}


// ---------------------------------------------------------------------
// Setup 'DescriptorSets' is how it works
// ---------------------------------------------------------------------
bool rekz::SetupGridDescriptorSets (rekz::PipelineGroup&              pipelinegroup,
                                    const std::vector<rokz::Buffer>&  vma_uniform_buffs, // MVPTransform
                                    const rokz::DescriptorPool&       descpool,
                                    const rokz::Device&               device) {
  assert (false); 

  size_t num_frames_in_flight = vma_uniform_buffs.size (); 

  printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

  rokz::DescriptorGroup& dg = pipelinegroup.descrgroup;
 
  // use same layout for both allocations
  std::vector<VkDescriptorSetLayout> descrlos (num_frames_in_flight, dg.dslayout.handle);
  // could have also said: 
  //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
  // but that wouldnt work
  rokz::cx::AllocateInfo (dg.alloc_info , descrlos, descpool);
  
  if (!rokz::cx::AllocateDescriptorSets (dg.descrsets, num_frames_in_flight, dg.alloc_info, device.handle)) {
    printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
    return false;
  }
  //

  for (uint32_t iframe = 0; iframe < num_frames_in_flight; ++iframe) {

    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer     = vma_uniform_buffs[iframe].handle;
    buffer_info.offset     = 0;
    buffer_info.range      = vma_uniform_buffs[iframe].ci.size ;
    
    // only MVP 
    std::array<VkWriteDescriptorSet, 1>  descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;    
    descriptor_writes[0].dstSet           = dg.descrsets[iframe];
    descriptor_writes[0].dstBinding       = 0;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &buffer_info;
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
  }

  return false;
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------

bool rekz::SetupGridPipeline (rekz::PipelineGroup& pipelinegroup,
                        const std::filesystem::path& fspath,
                        const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
                        VkSampleCountFlagBits msaa_samples,
                        VkFormat color_format,
                        VkFormat depth_format,
                        uint32_t sizeof_push_constants, 
                        const rokz::Device& device) {

  SetupGridPipeline;
                             
  //
  SetupGridShaderModules (pipelinegroup.pipeline, fspath, device); 

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
  rokz::CreateInfo (psci.vertexinputstate, kGridVertexBindingDesc, kGridVertInputAttributeDesc); 
  rokz::CreateInfo (psci.viewport_state, pipeline.state.viewport);
  rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_LINE_LIST); 
  rokz::CreateInfo (psci.rasterizer); 
  rokz::CreateInfo (psci.colorblendstate, pipeline.state.colorblend_attachments); 
  rokz::CreateInfo (psci.multisampling, msaa_samples); 
  rokz::CreateInfo (psci.depthstencilstate); 

  if (!SetupGridDescriptorLayout (pipelinegroup.descrgroup, device)) {
    printf ("[FAILED] ->  SetupGridDescriptorLayout in <%s | l:%i>", __FUNCTION__, __LINE__); 
    return false; 
  }

  //
  rokz::CreateGraphicsPipelineLayout (pipelinegroup.pipeline.layout.handle,
                                      pipelinegroup.pipeline.layout.ci,
                                      sizeof_push_constants, 
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

