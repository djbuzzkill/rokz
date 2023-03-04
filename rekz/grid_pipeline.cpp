
#include "grid_pipeline.h"
#include "rokz/descriptor.h"
#include "rokz/pipeline.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>
// ----------------------------------------------------------------------------------------
// vertex input
// ----------------------------------------------------------------------------------------
const VkVertexInputBindingDescription rekz::kGridVertexBindingDesc =  {
  0,                            // binding    
  sizeof (rekz::GridVert),       // stride      
  VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   

};
                                                     
// ----------------------------------------------------------------------------------------
// vertex attributes...pos, norm, color
// ----------------------------------------------------------------------------------------
const std::vector<VkVertexInputAttributeDescription> rekz::kGridVertInputAttributeDesc = {

  VkVertexInputAttributeDescription { // pos
    0,                                // .location 
    0,                                // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,       // .format   
    offsetof(rekz::GridVert, pos),    // .offset   
  },

  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(rekz::GridVert, col), 
  },
};


// ----------------------------------------------------------------------------------------
// descriptors 
// ----------------------------------------------------------------------------------------
const std::vector<VkDescriptorSetLayoutBinding> rekz::kGridDescriptorBindings = {

  // typedef struct VkDescriptorSetLayoutBinding {
  //   uint32_t              binding;
  //   VkDescriptorType      descriptorType;
  //   uint32_t              descriptorCount;
  //   VkShaderStageFlags    stageFlags;
  //   const VkSampler*      pImmutableSamplers;
  // } VkDescriptorSetLayoutBinding;


  {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr},
  
}; 

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool setup_grid_shader_modules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device) {

  printf ("%s \n", __FUNCTION__); 

   
 
  std::vector<VkPipelineShaderStageCreateInfo>& pss_create_infos = pipeline.state.ci.shader_stages; 
  std::vector<rokz::ShaderModule>&              shader_modules   = pipeline.shader_modules;

  shader_modules.resize  (2);
  pss_create_infos.resize(2);
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"data/shader/gridscape_vert.spv" ;
  rokz::CreateInfo (shader_modules[0].ci, rokz::From_file (shader_modules[0].bin, vert_file_path.string())); 
  if (!rokz::CreateShaderModule (shader_modules[0], device.handle))
    return false; 
  
  // for pipeline state ci
  rokz::CreateInfo (pss_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].entry_point, shader_modules[0].handle); 
  

  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"data/shader/gridscape_frag.spv" ;
  rokz::CreateInfo (shader_modules[1].ci, rokz::From_file (shader_modules[1].bin, frag_file_path.string())); 
  if (!rokz::CreateShaderModule (shader_modules[1], device.handle))
    return false; 

  // for pipeline state ci
  rokz::CreateInfo (pss_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[1].entry_point, shader_modules[1].handle); 
  //
  return true; 

}



// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------

bool rekz::InitGridPipeline (rokz::Pipeline&              pipeline,
                             rokz::PipelineLayout&        plo,
                             rokz::DescriptorSetLayout&   dslo,
                             //0
                             const std::filesystem::path& fspath,
                             const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                             VkSampleCountFlagBits        msaa_samples,
                             VkFormat                     color_format,
                             VkFormat                     depth_format,
                             const rokz::Device&          device) {
  //
  // Descriptor Set
  rokz::DefineDescriptorSetLayout (dslo,  kGridDescriptorBindings, device);

  //
  // Pipeline Layout
  rokz::DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(rekz::GridPushConstant), dslo.handle, device.handle);

  //
  // Pipeline States
  rokz::PipelineState_default (pipeline.state, msaa_samples,  kGridVertInputAttributeDesc,
                               kGridVertexBindingDesc, viewport_extent); 
  // change defaults
  // pipeline.state.ci.rasterizer.polygonMode = VK_POLYGON_MODE_LINE

  //
  pipeline.state.ci.input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
  // pipeline.state.ci.input_assembly.primitiveRestartEnable = ????; 
  
  //
  // Shader Modules
  setup_grid_shader_modules (pipeline, fspath, device); 

  //
  // dynamic  extensions
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = color_format;
  rokz::CreateInfo (pipeline.ext.pipeline_rendering.ci,
                    pipeline.ext.pipeline_rendering.color_formats, depth_format); 

  // the main create info
  auto& psci = pipeline.state.ci;
  rokz::CreateInfo (pipeline.ci,
                    plo.handle,
                    &pipeline.ext.pipeline_rendering.ci,                    
                    psci.shader_stages,       // const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
                    &psci.input_assembly,     // const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                    &psci.vertexinputstate,   // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                    &psci.viewport_state,     // const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                    nullptr,                  // tesselation 
                    &psci.rasterizer,         // const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
                    &psci.multisampling,      // const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
                    &psci.depthstencilstate,  // const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
                    &psci.colorblendstate,    // const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
                    &psci.dynamicstate);      // const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 

  if (!rokz::CreateGraphicsPipeline (pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }
  
  return true;
}


// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
bool rekz::BindGridDescriptorResources (std::vector<VkDescriptorSet>& dss, const std::vector<rokz::Buffer>& global_uniforms, const rokz::Device& device) {
  printf (" %s", __FUNCTION__);
  // DescriptorSets should be allocated
  // global_buffers should be create + allocated
  if (dss.size () != global_uniforms.size ()) {
    printf (" ERROR @line %i -->  descrriptorsets[%zu] != uniformbuffers[%zu]\n", __LINE__, dss.size (), global_uniforms.size ());
    return false; 
  }

  // is this rly necessary? grid may only need push constants
  //
  //
  //
  //

  size_t num_frames_in_flight = global_uniforms.size (); 
  //rokz::DescriptorGroup& dg = pipelinegroup.descrgroup;
  for (uint32_t iframe = 0; iframe < num_frames_in_flight; ++iframe) {

    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer     = global_uniforms[iframe].handle;
    buffer_info.offset     = 0;
    buffer_info.range      = global_uniforms[iframe].ci.size ;
    
    // only MVP 
    std::array<VkWriteDescriptorSet, 1>  descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;    
    descriptor_writes[0].dstSet           = dss[iframe];
    descriptor_writes[0].dstBinding       = 0;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &buffer_info;
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
  }


  printf (" bai\n");
  return false; 
  
}

  

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------

// bool rekz::SetupGridDescriptorPool (rokz::DescriptorPool& dp, uint32_t num_pools, const rokz::Device& device) {
//   printf ("%s \n", __FUNCTION__); 
  
//   dp.sizes.resize (1); 
//   dp.sizes[0] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, num_pools} ;  // MVPTransform struc

//   rokz::cx::CreateInfo ( dp.ci, num_pools, dp.sizes); 
//   if (!rokz::cx::CreateDescriptorPool (dp, device.handle)) {
//     printf ("[FAILED] %s", __FUNCTION__);
//     return false; 
//   }

//   return true; 
// }

// ---------------------------------------------------------------------
// Setup 'DescriptorLayout' is how it looks
// ---------------------------------------------------------------------

// bool SetupGridDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device) {

//   printf ("%s", __FUNCTION__); 

// #ifdef REKZ_GRID_HIDE_SETUPGRIDDESCRIPTORLAYOUT
  
//   descrgroup.dslayout.bindings.resize (1);
//   // MVPTransform
//   rokz::cx::DescriptorSetLayoutBinding (descrgroup.dslayout.bindings[0],
//                                         0,
//                                         VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//                                         1,
//                                         VK_SHADER_STAGE_VERTEX_BIT);
//   // 
//   if (!rokz::cx::CreateDescriptorSetLayout (descrgroup.dslayout.handle,
//                                             descrgroup.dslayout.ci,
//                                             descrgroup.dslayout.bindings,
//                                             device.handle)) {
//     printf (" --> [FAILED] \n"); 
//     return false;
//   }
// #endif
  
//   return true; 
// }



// bool rekz::SetupGridPipeline (rokz::Pipeline& pipeline,
//                         const std::filesystem::path& fspath,
//                         const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
//                         VkSampleCountFlagBits msaa_samples,
//                         VkFormat color_format,
//                         VkFormat depth_format,
//                         uint32_t sizeof_push_constants, 
//                         const rokz::Device& device) {

//   SetupGridPipeline;
                             
//   //
//   SetupGridShaderModules (pipeline, fspath, device); 

//   //
//   //rokz::Pipeline& pipeline = pipelinegroup.pipeline;
//   rokz::SetupViewportState (pipeline.state.viewport, viewport_extent); 

//   pipeline.state.colorblend_attachments.resize (1);

//   rokz::ColorBlendState_default (pipeline.state.colorblend_attachments[0]); 
//   rokz::DynamicState_default (pipeline.state.dynamics); 
//   //
//   rokz::PipelineStateCreateInfo& psci = pipeline.state.ci;
//   rokz::CreateInfo (psci.tesselation, 69); 
//   rokz::CreateInfo (psci.dynamicstate, pipeline.state.dynamics); 
//   rokz::CreateInfo (psci.vertexinputstate, kGridVertexBindingDesc, kGridVertInputAttributeDesc); 
//   rokz::CreateInfo (psci.viewport_state, pipeline.state.viewport);
//   rokz::CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_LINE_LIST); 
//   rokz::CreateInfo (psci.rasterizer); 
//   rokz::CreateInfo (psci.colorblendstate, pipeline.state.colorblend_attachments); 
//   rokz::CreateInfo (psci.multisampling, msaa_samples); 
//   rokz::CreateInfo (psci.depthstencilstate); 

//   assert (false); // SetupGridDescriptorLayout

// #ifdef REKZ_HIDE_SETUPGRIDPIPELINE
  
//     if (!SetupGridDescriptorLayout (pipelinegroup.descrgroup, device)) {
//     printf ("[FAILED] ->  SetupGridDescriptorLayout in <%s | l:%i>", __FUNCTION__, __LINE__); 
//     return false; 
//   }

//   //
//   rokz::CreateGraphicsPipelineLayout (pipelinegroup.pipeline.layout.handle,
//                                       pipelinegroup.pipeline.layout.ci,
//                                       sizeof_push_constants, 
//                                       pipelinegroup.descrgroup.dslayout.handle,
//                                       device.handle);
  
//   pipeline.ext.pipeline_rendering.color_formats.resize (1);
//   pipeline.ext.pipeline_rendering.color_formats[0] = color_format;

//   rokz::CreateInfo  (pipeline.ext.pipeline_rendering.ci,
//                      pipeline.ext.pipeline_rendering.color_formats,
//                      depth_format); 
//   //
//   rokz::CreateInfo (pipelinegroup.pipeline.ci,
//                     pipelinegroup.pipeline.layout.handle,
//                     &pipeline.ext.pipeline_rendering.ci,                    
//                     psci.shader_stages,       //const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
//                     &psci.input_assembly,     //const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
//                     &psci.vertexinputstate, // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
//                     &psci.viewport_state,     //const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
//                     nullptr,                 // tesselation 
//                     &psci.rasterizer,         //const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
//                     &psci.multisampling,      //const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
//                     &psci.depthstencilstate,       //const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
//                     &psci.colorblendstate,         //const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
//                     &psci.dynamicstate);     // const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 

//   if (!rokz::CreateGraphicsPipeline (pipelinegroup.pipeline, device.handle)) {     //const VkDevice                           b          device)
//     printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
//     return false;
//   }
// #endif
  
//   return true;

// }


// ---------------------------------------------------------------------
// this is "Bind Descriptors" 
// ---------------------------------------------------------------------

// bool rekz::SetupGridDescriptorSets (rokz::DescriptorGroup&            dg,
//                                     const std::vector<rokz::Buffer>&  vma_uniform_buffs, // MVPTransform
//                                     const rokz::DescriptorSetLayout&  dslo, //const rokz::DescriptorPool& descpool,
//                                     const rokz::Device&               device) {


//   assert (false); 

//   size_t num_frames_in_flight = vma_uniform_buffs.size (); 

//   printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

//   //rokz::DescriptorGroup& dg = pipelinegroup.descrgroup;
 
//   // use same layout for both allocations
//   std::vector<VkDescriptorSetLayout> descrlos (num_frames_in_flight, dslo.handle);
//   // could have also said: 
//   //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
//   // but that wouldnt work
//   rokz::cx::AllocateInfo (dg.alloc_info , descrlos, dg.pool);
  
//   if (!rokz::cx::AllocateDescriptorSets (dg.descrsets, num_frames_in_flight, dg.alloc_info, device.handle)) {
//     printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
//     return false;
//   }

//   //
//   for (uint32_t iframe = 0; iframe < num_frames_in_flight; ++iframe) {

//     VkDescriptorBufferInfo buffer_info{};
//     buffer_info.buffer     = vma_uniform_buffs[iframe].handle;
//     buffer_info.offset     = 0;
//     buffer_info.range      = vma_uniform_buffs[iframe].ci.size ;
    
//     // only MVP 
//     std::array<VkWriteDescriptorSet, 1>  descriptor_writes {};
//     descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     descriptor_writes[0].pNext            = nullptr;    
//     descriptor_writes[0].dstSet           = dg.descrsets[iframe];
//     descriptor_writes[0].dstBinding       = 0;
//     descriptor_writes[0].dstArrayElement  = 0;
//     descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     descriptor_writes[0].descriptorCount  = 1;
//     descriptor_writes[0].pBufferInfo      = &buffer_info;
//     descriptor_writes[0].pImageInfo       = nullptr; 
//     descriptor_writes[0].pTexelBufferView = nullptr; 

//     vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
//   }

//   return false;
// }
