

#include "landscape_pipeline.h"
#include "rokz/vert_defs.h"
#include <vulkan/vulkan_core.h>

using namespace rokz;




  // namespace landscape { 
  //   typedef rekz::Vertex_pos_nrm_txc                     PatchVert;
  //   // ----------------------------------------------------------------------------------------------
  //   extern const Vec<VkDescriptorSetLayoutBinding>&      kDescriptorBindings;
  //   // ----------------------------------------------------------------------------------------------
  //   extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;
  //   // ----------------------------------------------------------------------------------------------
  //   extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc;
  // }


const Vec<VkDescriptorSetLayoutBinding> rekz::landscape::kDescriptorBindings = {

  { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, kMaxPatchCount, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, nullptr }, // height
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, kMaxPatchCount, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, nullptr }, // normnal
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, kMaxPatchCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // color

};

const VkVertexInputBindingDescription&
rekz::landscape::kVertexInputBindingDesc   = rokz::kPNT_InputBindingDesc;

const Vec<VkVertexInputAttributeDescription>&
rekz::landscape::kVertexInputAttributeDesc = rokz::kPNT_InputAttributeDesc;


// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool setup_landscape_shader_modules (Pipeline& pipeline,
                                  const std::filesystem::path& fspath,
                                  const Device& device) {
  "mack_rowes.h"; 
  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (2);
  shader_stage_create_infos.resize(2);
  //
  // VERT SHADER 
  std::filesystem::path vert_file_path  = fspath/"landscape/landscape_vert.spv" ;
  CreateInfo (shader_modules[0].ci, From_file (shader_modules[0].bin, vert_file_path.string())); 
  if (!CreateShaderModule (shader_modules[0], device.handle))
    return false; 
  CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT,
              shader_modules[0].entry_point, shader_modules[0].handle); //   


  // TESS CTRL SHADER 
  std::filesystem::path tesc_file_path  = fspath/"landscape/landscape_tesc.spv" ;
  CreateInfo (shader_modules[1].ci, From_file (shader_modules[1].bin, tesc_file_path.string())); 
  if (!CreateShaderModule (shader_modules[1], device.handle))
    return false; 
  CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
              shader_modules[1].entry_point, shader_modules[1].handle); //   


  // TESS CTRL SHADER 
  std::filesystem::path tese_file_path  = fspath/"landscape/landscape_tese.spv" ;
  CreateInfo (shader_modules[2].ci, From_file (shader_modules[2].bin, tese_file_path.string())); 
  if (!CreateShaderModule (shader_modules[2], device.handle))
    return false; 
  CreateInfo (shader_stage_create_infos[2], VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
              shader_modules[2].entry_point, shader_modules[2].handle); //   

  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath/"landscape/landscape_frag.spv"; 
  //printf ( "LINE [%i] --> %s \n", __LINE__, frag_file_path.string().c_str());
  CreateInfo (shader_modules[3].ci, From_file (shader_modules[3].bin, frag_file_path.string())); 
  if (!CreateShaderModule (shader_modules[3], device.handle))
    return false; 
  
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


  // (kVertexInputBindingAttributeDesc, kVertexInputBindingDesc, viewport_extent); 


  
  printf ("[%s] --> %i \n", __FUNCTION__, __LINE__); 

  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(PushConstants), dslos, device.handle);

  PipelineState_default (pipeline.state, msaa_samples, landscape::kVertexInputAttributeDesc,
                         landscape::kVertexInputBindingDesc, displayextent); 
  // ^ !! shader modules is part of pipelinestate 
  setup_landscape_shader_modules (pipeline, pipe_path.string(), device);

  // proto more orthogonal version
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = color_format;

  CreateInfo  (pipeline.ext.pipeline_rendering.ci,
                     pipeline.ext.pipeline_rendering.color_formats, depth_format); 

  auto& psci = pipeline.state.ci;
  //
  CreateInfo (pipeline.ci,
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

  if (!CreateGraphicsPipeline (pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool rekz::SetupLandscapeResources (Buffer& patches_vb, Buffer& patches_ib,
                                    uint32_t num_sets, const Device& device) {


  

  
  return false;
}

// ----------------------------------------------------------------------------------------
// set 0= Global  descriptors ,  set 1= landscape descriptors
// ----------------------------------------------------------------------------------------
bool rekz::BindLanscapeDescriptors (VkDescriptorSet            ds,
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
