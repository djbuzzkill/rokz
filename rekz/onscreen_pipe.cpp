

#include "onscreen_pipe.h"
#include "rokz/shared_descriptor.h"
#include "rokz/ut_offset.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <vulkan/vulkan_core.h>



using namespace rokz;

using UBText = global_ub::TextItem;

const VkVertexInputBindingDescription&        rekz::onscreen::kVertexInputBindingDesc   = rokz::kPTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::onscreen::kVertexInputAttributeDesc = rokz::kPTx_InputAttributeDesc; 



const uint32 TEXT_ITEMS_BINDINGI = 16;
const uint32 FONT_FACE_BINDINGI  = 17;

const DescriptorSetLayoutBindings rekz::onscreen::kDescriptorBindings = {
  // TEXT ELEMENTS
  { TEXT_ITEMS_BINDINGI   , VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  // FONT FACE
  { FONT_FACE_BINDINGI    , VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::onscreen::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};


bool setup_onscreen_shaders (Pipeline& pipeline, const systempath& fspath, const Device& device) {

  HERE(" hai  ");
  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_cis = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules   = pipeline.shader_modules;

  pipeline.shader_stage_defs  = {
    { "main", fspath/"onscreen/onscreen_vert.vert", VK_SHADER_STAGE_VERTEX_BIT   },
    { "main", fspath/"onscreen/onscreen_frag.frag", VK_SHADER_STAGE_FRAGMENT_BIT },
  };

  //printf ( "LINE [%i] --> %s \n", __LINE__, vert_file_path.string().c_str()); 
  return rokz::SetupPipelinShaderStages (shader_stage_cis, shader_modules, pipeline.shader_stage_defs, device);
}
  
// ----------------------------------------------------------------------------------------------
// this function is sus, it is setting up a global buffer
// ----------------------------------------------------------------------------------------------
bool rekz::onscreen::InitPipeline (Pipeline&                         pipeline,
                                   PipelineLayout&                   plo,
                                   const Vec<VkDescriptorSetLayout>& dslos,
                                   //0
                                   const systempath&                 fspath,
                                   const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                                   VkSampleCountFlagBits             msaa_samples,
                                   VkFormat                          color_format,
                                   const Device&                     device)
{
  printf ("[%s] --> %i \n", __FUNCTION__, __LINE__); 
  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(onscreen::PushConstant),
                                onscreen::PCStages, dslos, device.handle);
  
  PipelineState_default (pipeline.state, msaa_samples, onscreen::kVertexInputAttributeDesc,
                         onscreen::kVertexInputBindingDesc, viewport_extent); 
  // ^ !! shader modules is part of pipelinestate 

  if (!setup_onscreen_shaders  (pipeline, fspath, device)) {
    return false;
  }

  // proto more orthogonal version
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = color_format;

  CreateInfo  (pipeline.ext.pipeline_rendering.ci,
                     pipeline.ext.pipeline_rendering.color_formats, VK_FORMAT_UNDEFINED); 

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
              nullptr, //&psci.depthstencilstate,       //const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
              &psci.colorblendstate,         //const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
              &psci.dynamicstate);     // const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 

  if (!CreateGraphicsPipeline (pipeline, device.handle)) {     //const VkDevice                           b          device)
    printf ("[FAILED] --> CreateGraphicsPipeline \n"); 
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------------------------
//  this needs to incorporate global uniforms
// 
// ----------------------------------------------------------------------------------------------
bool rekz::onscreen::BindDescriptorResources (Vec<VkDescriptorSet>&       dss,
                                              const Vec<rc::Buffer::Ref>& globalubs,
                                              const Vec<rc::Buffer::Ref>& textubs,
                                              const rc::ImageView::Ref    imageview,  
                                              const rc::Sampler::Ref      sampler, 
                                              const DescriptorSetLayout&  dslayout, 
                                              const Device&               device) {
  assert (globalubs.size () != 0); 
  assert (textubs.size () != 0); 
  // separate mvp+ostext buffers or combine

  //
  for (uint32_t i = 0; i < dss.size (); i++) {
    // setup uniform
    VkDescriptorBufferInfo mvpinfo {};
    mvpinfo.buffer = globalubs[i]->handle;
    mvpinfo.offset = ut::offset_at (global_ub::UB_sizes, global_ub::MVP_OVERLAY_BINDINGI); 
    mvpinfo.range  = sizeof(global_ub::MVPTransform);
      
    Vec<VkDescriptorBufferInfo> textlines (onscreen::kMaxCount, VkDescriptorBufferInfo {});
    for (size_t iobj = 0; iobj < textlines.size (); ++iobj) { 
      textlines[iobj].buffer = textubs[i]->handle;    //
      textlines[iobj].offset = iobj * sizeof(UBText); // min_uniform_buffer_offset_alignment ??
      textlines[iobj].range  = sizeof(UBText);       //glob.vma_objparam_buffs[i].ci.size;
    }
    // setup texture+sampler
    VkDescriptorImageInfo  imageinfo; //  (onscreen::kMaxCount);
    imageinfo = {};
    imageinfo.imageLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    imageinfo.imageView      = imageview->handle;
    imageinfo.sampler        = sampler->handle;
    //
    std::array<VkWriteDescriptorSet, 2> descrwrites {};
    descrwrites[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descrwrites[0].pNext            = nullptr;
    descrwrites[0].dstSet           = dss[i];
    descrwrites[0].dstBinding       = global_ub::MVP_OVERLAY_BINDINGI;       // does it match in shader? 
    descrwrites[0].dstArrayElement  = 0;
    descrwrites[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descrwrites[0].descriptorCount  = 1; // <
    descrwrites[0].pBufferInfo      = &mvpinfo; 
    descrwrites[0].pImageInfo       = nullptr; 
    descrwrites[0].pTexelBufferView = nullptr; 

    descrwrites[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descrwrites[1].pNext            = nullptr;
    descrwrites[1].dstSet           = dss[i];
    descrwrites[1].dstBinding       = TEXT_ITEMS_BINDINGI; 
    descrwrites[1].dstArrayElement  = 0;
    descrwrites[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descrwrites[1].descriptorCount  = textlines.size(); // <
    descrwrites[1].pBufferInfo      = &textlines[0]; 
    descrwrites[1].pImageInfo       = nullptr; 
    descrwrites[1].pTexelBufferView = nullptr; 
    
    descrwrites[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descrwrites[2].pNext            = nullptr;    
    descrwrites[2].dstSet           = dss[i];
    descrwrites[2].dstBinding       = FONT_FACE_BINDINGI;
    descrwrites[2].dstArrayElement  = 0;
    descrwrites[2].descriptorType   = VK_DESCRIPTOR_TYPE_SAMPLER;
    descrwrites[2].descriptorCount  = 1; 
    descrwrites[2].pBufferInfo      = nullptr;
    descrwrites[2].pImageInfo       = &imageinfo; 
    descrwrites[2].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descrwrites.size(), &descrwrites[0], 0, nullptr);
  }

  return true;
}

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
void rekz::onscreen::UpdateOverlayDescriptors (rc::Buffer::Ref& buf,
                                               const std::array<std::string, kMaxCount>& strings, 
                                               const VkExtent2D& viewext, double dt) {
  assert (false);
  HERE ("is this called?");
  
  uint8_t* uc = (uint8_t*) rokz::rc::MappedPointer (buf);

  using rokz::global_ub::MVPTransform; 
  
  //  
  // the MVP is set in UpdateGlobalUniforms
  // 
  // MVPTransform buffer
  // MVPTransform* mvp  =
  //   reinterpret_cast<MVPTransform*>(uc + ut::offset_at (UB_sizes, global_ub::MVP_Overlay));

  // mvp->model = glm::mat4(1); 
  // mvp->view  = glm::mat4(1); 

  // float lt = 0.0f;
  // float rt = viewext.width;
  // float bt = 0.0;
  // float tp = viewext.height; 

  // mvp->proj = glm::ortho ( lt, rt, bt, tp); 
  // mvp->proj[1][1] *= -1;
    
  //  updat text 
  UBText* text =
    reinterpret_cast<UBText*>(uc + ut::offset_at (UB_sizes, global_ub::TEXTITEMS_BINDINGI));

  for (uint32 iline = 0; iline < kMaxCount; ++iline) { 
    auto& s = strings[iline];
    std::copy  (s.begin (), s.end (), text[iline].ch); 
    // for (uint32  charind = 0; charind < string_record[iline].size (); ++charind) {
    //   text[iline].text[charind] = string_record[iline][charind]; 

  }

  
}




