

#include "landscape_pipeline.h"
#include "rokz/pipeline.h"
#include "rokz/vert_defs.h"
#include <vulkan/vulkan_core.h>

using namespace rokz;
// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
const VkShaderStageFlags landscape_shader_stages = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT
                                                 | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
                                                 | VK_SHADER_STAGE_VERTEX_BIT
                                                 | VK_SHADER_STAGE_FRAGMENT_BIT;



const Vec<VkDescriptorSetLayoutBinding> lscape::kDescriptorBindings = {

  // height
  { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, lscape::tile::kMaxCount, kPCStages, nullptr }, // height
  // normal 
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, lscape::tile::kMaxCount, kPCStages, nullptr }, // normnal
  // color
  { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, lscape::tile::kMaxCount, kPCStages, nullptr }, // color
  // PatchParams
  { 3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , lscape::tile::kMaxCount, kPCStages, nullptr }, // color
};

//
const VkVertexInputBindingDescription&
   lscape::kVertexInputBindingDesc = rokz::kPNTx_InputBindingDesc;

//
const Vec<VkVertexInputAttributeDescription>&
   lscape::kVertexInputAttributeDesc = rokz::kPNTx_InputAttributeDesc;


// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool setup_landscape_shader_modules (Pipeline&            pipeline, 
                                     const filepath&   fspath,
                                     const Device&        device) {
  //
  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;

  shader_modules.resize  (4);
  shader_stage_create_infos.resize(4);
  //
  // VERT SHADER
  std::string vert_name = "landscape/landscape_vert.spv";
  filepath vert_file_path  = fspath/vert_name;
  CreateInfo (shader_modules[0].ci, From_file (shader_modules[0].bin, vert_file_path.string())); 
  if (!CreateShaderModule (shader_modules[0], device.handle)) {
    printf (" this didnt work out -> %s\n", vert_name.c_str ()); 
    return false;
  }

  CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT,
              shader_modules[0].entry_point, shader_modules[0].handle); //   
  HERE("vert");
  // TESS CTRL SHADER
  std::string tesc_name = "landscape/landscape_tesc.spv" ;
  filepath tesc_file_path  = fspath/tesc_name;
  CreateInfo (shader_modules[1].ci, From_file (shader_modules[1].bin, tesc_file_path.string())); 
  if (!CreateShaderModule (shader_modules[1], device.handle)) {
    printf (" this didnt work out -> %s\n", tesc_name.c_str ()); 
    return false;
  }
  CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
              shader_modules[1].entry_point, shader_modules[1].handle); //   

  HERE("tesc");

  // TESS CTRL SHADER
  std::string tese_name = "landscape/landscape_tese.spv" ;
  filepath tese_file_path  = fspath/tese_name;
  CreateInfo (shader_modules[2].ci, From_file (shader_modules[2].bin, tese_file_path.string())); 
  if (!CreateShaderModule (shader_modules[2], device.handle)) {
    printf (" this didnt work out -> %s\n", tese_name.c_str ()); 
    return false;
  }

  CreateInfo (shader_stage_create_infos[2], VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
              shader_modules[2].entry_point, shader_modules[2].handle); //   

  // FRAG SHADER
  std::string frag_name = "landscape/landscape_frag.spv"; 
  filepath frag_file_path = fspath/frag_name;
  //printf ( "LINE [%i] --> %s \n", __LINE__, frag_file_path.string().c_str());
  CreateInfo (shader_modules[3].ci, From_file (shader_modules[3].bin, frag_file_path.string())); 
  if (!CreateShaderModule (shader_modules[3], device.handle)) {
    printf (" this didnt work out -> %s\n", frag_name.c_str ()); 
    return false;
  }
  
  CreateInfo (shader_stage_create_infos[3], VK_SHADER_STAGE_FRAGMENT_BIT,
              shader_modules[3].entry_point,  shader_modules[3].handle); 

  HERE("frag");
  //VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
  
  //
  return true; 
}


// ----------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------
bool lscape::InitPipeline (Pipeline&                    pipeline,
                           PipelineLayout&              plo,
                           Vec<VkDescriptorSetLayout>&  dslos,
                           VkSampleCountFlagBits        msaa_samples,
                           VkFormat                     color_format,
                           VkFormat                     depth_format,
                           const std::filesystem::path& pipe_path,
                           const VkExtent2D&            displayextent,
                           const Device&                device)
{

  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(lscape::tile::PushConstant),
                                lscape::kPCStages, dslos, device.handle);
  
  PipelineState_tessellation (pipeline.state, msaa_samples,
                              lscape::kControlPoints,
                              lscape::kVertexInputAttributeDesc,
                              lscape::kVertexInputBindingDesc, displayextent); 

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
bool lscape::SetupResources (rc::Buffer::Ref& geomb, uint32_t num_sets, const Device& device) {

  // ???  what would this do difrnt from marzdata 
  
  return false;
}

// ----------------------------------------------------------------------------------------
// set 0= Global  descriptors ,  set 1= landscape descriptors
// ----------------------------------------------------------------------------------------
bool lscape::BindDescriptorResources (Vec<VkDescriptorSet>&          dss,
                                      const rc::Sampler::Ref&         colorsamp,
                                      const Vec<rc::ImageView::Ref>&  colorviews,

                                      const rc::Sampler::Ref&         heightsamp, 
                                      const Vec<rc::ImageView::Ref>&  heightviews,

                                      const rc::Sampler::Ref&         normalsamp, 
                                      const Vec<rc::ImageView::Ref>&  normalviews,

                                      const DescriptorSetLayout& dslayout, 
                                      const Device&              device) {

  assert (colorviews.size () < lscape::tile:: kMaxCount);
  assert (heightviews.size () < lscape::tile::kMaxCount);
  assert (normalviews.size () < lscape::tile::kMaxCount);
  //
  for (uint32_t i = 0; i < dss.size (); i++) {
    //
    Vec<VkDescriptorImageInfo> colorinfos (lscape::tile::kMaxCount);
    for (size_t iview = 0; iview < lscape::tile::kMaxCount; ++iview) { 

      if (iview < colorviews.size ()) { 
        colorinfos[iview] = {};
        colorinfos[iview].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
        colorinfos[iview].imageView   = colorviews[iview]->handle;
        colorinfos[iview].sampler     = colorsamp->handle;
      }
      else { // err'thing must b written
        colorinfos[iview] = {};
        colorinfos[iview].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
        colorinfos[iview].imageView   = colorviews[0]->handle;
        colorinfos[iview].sampler     = colorsamp->handle;
        }
    }

    Vec<VkDescriptorImageInfo> heightinfos (lscape::tile::kMaxCount);
    for (size_t iview = 0; iview < lscape::tile::kMaxCount; ++iview) {
      if (iview < heightviews.size ()) { 
      heightinfos[iview] = {};
      heightinfos[iview].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      heightinfos[iview].imageView   = heightviews[iview]->handle; 
      heightinfos[iview].sampler     = heightsamp->handle; 
      }
      else {
        heightinfos[iview] = {};
        heightinfos[iview].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        heightinfos[iview].imageView   = heightviews[0]->handle; 
        heightinfos[iview].sampler     = heightsamp->handle; 
      }

      Vec<VkDescriptorImageInfo> normalinfos (lscape::tile::kMaxCount);
      // for (size_t iview = 0; iview < landscape::kMaxPatchCount; ++iview) {
      //   if (iview < normalviews.size ()) {
      //   }
      //   else {
      //   }
    }
    //

    const uint32 height_binding_index = 0; // <-- make sure shader matches
    const uint32 normal_binding_index = 1;
    const uint32 color_binding_index  = 2; // <-- make sure shader matches

    std::array<VkWriteDescriptorSet, 2> descriptor_writes {};

    // COLOR
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;
    descriptor_writes[0].dstSet           = dss[i];
    descriptor_writes[0].dstBinding       = color_binding_index; // does it match in shader? 
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptor_writes[0].descriptorCount  = colorinfos.size(); // <
    descriptor_writes[0].pBufferInfo      = nullptr;
    descriptor_writes[0].pImageInfo       = &colorinfos[0]; ; 
    descriptor_writes[0].pTexelBufferView = nullptr; 
    // height
    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = dss[i];
    descriptor_writes[1].dstBinding       = height_binding_index;      // <-- change shader too
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[1].descriptorCount  = heightinfos.size(); 
    descriptor_writes[1].pBufferInfo      = nullptr;
    descriptor_writes[1].pImageInfo       = &heightinfos[0]; 
    descriptor_writes[1].pTexelBufferView = nullptr; 
    // normal
    // descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    // descriptor_writes[1].pNext            = nullptr;    
    // descriptor_writes[1].dstSet           = dss[i];
    // descriptor_writes[1].dstBinding       = normal_binding_index;     
    // descriptor_writes[1].dstArrayElement  = 0;
    // descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    // descriptor_writes[1].descriptorCount  = normalinfos.size(); 
    // descriptor_writes[1].pBufferInfo      = nullptr;
    // descriptor_writes[1].pImageInfo       = &normalinfos[0]; 
    // descriptor_writes[1].pTexelBufferView = nullptr; 
    // // write 
    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return false;
}
