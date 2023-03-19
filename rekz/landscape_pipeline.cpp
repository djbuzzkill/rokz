

#include "landscape_pipeline.h"
#include "rokz/vert_defs.h"
#include <vulkan/vulkan_core.h>

using namespace rokz;
// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
const Vec<VkDescriptorSetLayoutBinding> rekz::landscape::kDescriptorBindings = {

  // height
  { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, landscape::kMaxPatchCount, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, nullptr }, // height
  // normal 
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, landscape::kMaxPatchCount, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, nullptr }, // normnal
  // color
  { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, landscape::kMaxPatchCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr                }, // color

};

//
const VkVertexInputBindingDescription&
rekz::landscape::kVertexInputBindingDesc = rokz::kPNTx_InputBindingDesc;

//
const Vec<VkVertexInputAttributeDescription>&
rekz::landscape::kVertexInputAttributeDesc = rokz::kPNTx_InputAttributeDesc;

// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool setup_landscape_shader_modules (Pipeline& pipeline,
                                  const std::filesystem::path& fspath,
                                  const Device& device) {
  //
  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (2);
  shader_stage_create_infos.resize(2);
  //
  // VERT SHADER
  std::string vert_name = "landscape/landscape_vert.spv";
  std::filesystem::path vert_file_path  = fspath/vert_name;
  CreateInfo (shader_modules[0].ci, From_file (shader_modules[0].bin, vert_file_path.string())); 
  if (!CreateShaderModule (shader_modules[0], device.handle)) {
    printf (" this didnt work out -> %s\n", vert_name.c_str ()); 
    return false;
  }

  CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT,
              shader_modules[0].entry_point, shader_modules[0].handle); //   

  // TESS CTRL SHADER
  std::string tesc_name = "landscape/landscape_tesc.spv" ;
  std::filesystem::path tesc_file_path  = fspath/tesc_name;
  CreateInfo (shader_modules[1].ci, From_file (shader_modules[1].bin, tesc_file_path.string())); 
  if (!CreateShaderModule (shader_modules[1], device.handle)) {
    printf (" this didnt work out -> %s\n", tesc_name.c_str ()); 
    return false;
  }
  CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
              shader_modules[1].entry_point, shader_modules[1].handle); //   

  // TESS CTRL SHADER
  std::string tese_name = "landscape/landscape_tese.spv" ;
  std::filesystem::path tese_file_path  = fspath/tese_name;
  CreateInfo (shader_modules[2].ci, From_file (shader_modules[2].bin, tese_file_path.string())); 
  if (!CreateShaderModule (shader_modules[2], device.handle)) {
    printf (" this didnt work out -> %s\n", tese_name.c_str ()); 
    return false;
  }

  CreateInfo (shader_stage_create_infos[2], VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
              shader_modules[2].entry_point, shader_modules[2].handle); //   

  // FRAG SHADER
  std::string frag_name = "landscape/landscape_frag.spv"; 
  std::filesystem::path frag_file_path = fspath/frag_name;
  //printf ( "LINE [%i] --> %s \n", __LINE__, frag_file_path.string().c_str());
  CreateInfo (shader_modules[3].ci, From_file (shader_modules[3].bin, frag_file_path.string())); 
  if (!CreateShaderModule (shader_modules[3], device.handle)) {
    printf (" this didnt work out -> %s\n", frag_name.c_str ()); 
    return false;
  }
  
  CreateInfo (shader_stage_create_infos[3], VK_SHADER_STAGE_FRAGMENT_BIT,
              shader_modules[3].entry_point,  shader_modules[3].handle); 
  //
  return true; 
}


// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool rekz::InitLandscapePipeline (Pipeline&                    pipeline,
                                  PipelineLayout&              plo,
                                  Vec<VkDescriptorSetLayout>&  dslos,
                                  VkSampleCountFlagBits        msaa_samples,
                                  VkFormat                     color_format,
                                  VkFormat                     depth_format,
                                  const std::filesystem::path& pipe_path,
                                  const VkExtent2D&            displayextent,
                                  const Device&                device)
{
  HERE("hai");

  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(landscape::PatchPushConstants), dslos, device.handle);

  
  PipelineState_default (pipeline.state, msaa_samples, landscape::kVertexInputAttributeDesc,
                         landscape::kVertexInputBindingDesc, displayextent); 

  // ^ !! shader modules is part of pipelinestate 
  bool shmodres = setup_landscape_shader_modules (pipeline, pipe_path, device);
  if (!shmodres) {
    HERE("FAILED setup_landscape_shader_modules"); 
    return false;
  }
  
  // DYNAMIC RENDERING IS AN EXTENSION
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = color_format;
  CreateInfo  (pipeline.ext.pipeline_rendering.ci,
               pipeline.ext.pipeline_rendering.color_formats, depth_format); 

  //
  auto& psci = pipeline.state.ci;
  CreateInfo (pipeline.ci,
              plo.handle,
              &pipeline.ext.pipeline_rendering.ci,                    
              psci.shader_stages,       // const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
              &psci.input_assembly,     // const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
              &psci.vertexinputstate,   // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
              &psci.viewport_state,     // const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
              &psci.tesselation,        // tesselation 
              &psci.rasterizer,         // const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
              &psci.multisampling,      // const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
              &psci.depthstencilstate,  // const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
              &psci.colorblendstate,    // const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
              &psci.dynamicstate);      // const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 

  if (!CreateGraphicsPipeline (pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }

  HERE("bai");
  return true;
}

// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool rekz::SetupLandscapeResources (Buffer& patches_vb, Buffer& patches_ib,
                                    uint32_t num_sets, const Device& device) {


  
  // wat happens n here, iz this the same as 'SetupLandscapeData'

  
  return false;
}

// ----------------------------------------------------------------------------------------
// set 0= Global  descriptors ,  set 1= landscape descriptors
// ----------------------------------------------------------------------------------------
bool rekz::BindLandscapeResources (VkDescriptorSet            ds,
                                  const Sampler&             colorsamp,
                                  const Vec<VkImageView>&    colorviews,
                                  const Sampler&             heightsamp, 
                                  const Vec<VkImageView>&    heightviews,
                                  const Sampler&             normalsamp, 
                                  const Vec<VkImageView>&    normalviews,
                                  const DescriptorSetLayout& dslayout, 
                                  const Device&              device) {


  
  return false;
}
