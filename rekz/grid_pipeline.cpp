
#include "grid_pipeline.h"
#include "rokz/descriptor.h"
#include "rokz/pipeline.h"
#include "rokz/rokz_types.h"
#include "rokz/shader.h"
#include "rokz/shared_descriptor.h"
#include <vulkan/vulkan_core.h>

using namespace rokz;
// ----------------------------------------------------------------------------------------
// vertex input
// ----------------------------------------------------------------------------------------
const VkVertexInputBindingDescription rekz::grid::kVertexBindingDesc =  {
  0,                            // binding    
  sizeof (rekz::grid::Vert),       // stride      
  VK_VERTEX_INPUT_RATE_VERTEX   // inputRate   

};
                                                     
// ----------------------------------------------------------------------------------------
// vertex attributes...pos, norm, color
// ----------------------------------------------------------------------------------------
const Vec<VkVertexInputAttributeDescription> rekz::grid::kVertInputAttributeDesc = {

  VkVertexInputAttributeDescription { // pos
    0,                                // .location 
    0,                                // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,       // .format   
    offsetof(rekz::grid::Vert, pos),    // .offset   
  },

  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(rekz::grid::Vert, col), 
  },
};


// ----------------------------------------------------------------------------------------
// descriptors 
// ----------------------------------------------------------------------------------------
const Vec<VkDescriptorSetLayoutBinding> rekz::grid::kDescriptorBindings = {
  {global_ub::MVP_SCENE_BINDINGI, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr},
  {global_ub::GRIDSTATE_BINDINGI, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr},
}; 

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool setup_grid_shader_modules (rokz::Pipeline& pipeline, const systempath& fspath, const rokz::Device& device) {

  printf ("%s \n", __FUNCTION__); 

  pipeline.shader_stage_defs  = {
    { "main", fspath/"gridz/gridz_vert.vert", VK_SHADER_STAGE_VERTEX_BIT } , 
    { "main", fspath/"gridz/gridz_frag.frag", VK_SHADER_STAGE_FRAGMENT_BIT } , 
  };

  // //
  return SetupPipelinShaderStages (pipeline.state.ci.shader_stages,
                                   pipeline.shader_modules, pipeline.shader_stage_defs, device); 
}

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------

bool rekz::grid::InitPipeline (rokz::Pipeline&              pipeline,
                             rokz::PipelineLayout&        plo,
                             //0
                             const Vec<VkDescriptorSetLayout>&   dslos,
                             const systempath&            fspath,
                             const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                             VkSampleCountFlagBits        msaa_samples,
                             VkFormat                     color_format,
                             VkFormat                     depth_format,
                             const rokz::Device&          device) {
  //
  // Descriptor Set
  //rokz::DefineDescriptorSetLayout (dslo,  kGridDescriptorBindings, device);
  //
  // Pipeline Layout
    rokz::DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(rekz::grid::PushConstant),
                                        grid::kPCStages, dslos, device.handle);

  //
  // Pipeline States
  rokz::PipelineState_default (pipeline.state, msaa_samples,  grid::kVertInputAttributeDesc,
                               grid::kVertexBindingDesc, viewport_extent); 
  // change defaults
  // pipeline.state.ci.rasterizer.polygonMode = VK_POLYGON_MODE_LINE

  //
  pipeline.state.ci.input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
  // pipeline.state.ci.input_assembly.primitiveRestartEnable = ????; 
  
  //
  // Shader Modules
  if (!setup_grid_shader_modules (pipeline, fspath, device)) {
    HERE("FAILED");
    return false; 
  }

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

bool rekz::grid::BindDescriptorResources (Vec<VkDescriptorSet>&               dss,
                                          const std::vector<rc::Buffer::Ref>& globalubs,
                                          const rokz::Device&                 device) {
  assert (dss.size () == globalubs.size ());

  for (uint32_t i = 0; i < dss.size (); i++) {
    assert (dss[i]); 
    assert (globalubs[i]); 

    // MVP Scene
    VkDescriptorBufferInfo globalinfo = {};
    globalinfo.buffer = globalubs[i]->handle;
    globalinfo.offset = ut::offset_at (global_ub::UB_sizes, global_ub::MVP_SCENE_BINDINGI); 
    globalinfo.range  = sizeof (global_ub::MVPTransform); 

    VkDescriptorBufferInfo gridinfo = {};
    gridinfo.buffer = globalubs[i]->handle;
    gridinfo.offset = ut::offset_at (global_ub::UB_sizes, global_ub::GRIDSTATE_BINDINGI); 
    gridinfo.range  = sizeof (global_ub::GridState); 

    std::array<VkWriteDescriptorSet, 2> descrwrites {};
    descrwrites[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descrwrites[0].pNext            = nullptr;
    descrwrites[0].dstSet           = dss[i];
    descrwrites[0].dstBinding       = global_ub::MVP_SCENE_BINDINGI;       // does it match in shader? 
    descrwrites[0].dstArrayElement  = 0;
    descrwrites[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descrwrites[0].descriptorCount  = 1; 
    descrwrites[0].pBufferInfo      = &globalinfo; 
    descrwrites[0].pImageInfo       = nullptr; 
    descrwrites[0].pTexelBufferView = nullptr; 

    descrwrites[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descrwrites[1].pNext            = nullptr;
    descrwrites[1].dstSet           = dss[i];
    descrwrites[1].dstBinding       = global_ub::GRIDSTATE_BINDINGI;       // does it match in shader?
    descrwrites[1].dstArrayElement  = 0;
    descrwrites[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descrwrites[1].descriptorCount  = 1; // <
    descrwrites[1].pBufferInfo      = &gridinfo; 
    descrwrites[1].pImageInfo       = nullptr; 
    descrwrites[1].pTexelBufferView = nullptr; 
                      
    vkUpdateDescriptorSets (device.handle, descrwrites.size(), &descrwrites[0], 0, nullptr);
  }
  // grid uses data defined as part of global 
  return true; 
}


