

#include "onscreen_pipe.h"
#include "rokz/shared_descriptor.h"
#include "rokz/ut_offset.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <vulkan/vulkan_core.h>



using namespace rokz;


const VkVertexInputBindingDescription&        rekz::onscreen::kVertexInputBindingDesc   = rokz::kPTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::onscreen::kVertexInputAttributeDesc = rokz::kPTx_InputAttributeDesc; 

const DescriptorSetLayoutBindings rekz::onscreen::kDescriptorBindings = {
  { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , 1                        , VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::onscreen::kMaxCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::onscreen::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};


bool setup_onscreen_shaders (Pipeline& pipeline, const systempath& fspath, const Device& device) {

  HERE(" using  ");
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
// 
// ----------------------------------------------------------------------------------------------
bool rekz::onscreen::BindDescriptorResources (Vec<VkDescriptorSet>&       dss ,
                                                    const Vec<rc::Buffer::Ref>& ubtext,
                                                    const rc::ImageView::Ref    imageview,  
                                                    const rc::Sampler::Ref      sampler, 
                                                    const DescriptorSetLayout&  dslayout, 
                                                    const Device&               device)  {

  HERE("onscreen");
  for (uint32_t i = 0; i < dss.size (); i++) {
    // setup uniform
    Vec<VkDescriptorBufferInfo> textlines (onscreen::kMaxCount, VkDescriptorBufferInfo {});
    for (size_t iobj = 0; iobj < textlines.size (); ++iobj) { 
      textlines[iobj].buffer   = ubtext[i]->handle;    //
      textlines[iobj].offset   = iobj * sizeof(UBText); // min_uniform_buffer_offset_alignment ??
      textlines[iobj].range    = sizeof(UBText);       //glob.vma_objparam_buffs[i].ci.size;
    }  
    // setup texture+sampler
    VkDescriptorImageInfo  imageinfo; //  (onscreen::kMaxCount);
    imageinfo = {};
    imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    imageinfo.imageView   = imageview->handle;
    imageinfo.sampler     = sampler->handle;
    //
    std::array<VkWriteDescriptorSet, 2> descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;
    descriptor_writes[0].dstSet           = dss[i];
    descriptor_writes[0].dstBinding       = 0;       // does it match in shader? 
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = textlines.size(); // <
    descriptor_writes[0].pBufferInfo      = &textlines[0]; 
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 
                      
    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = dss[i];
    descriptor_writes[1].dstBinding       = 1;      // <-- change shader too
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_SAMPLER;
    descriptor_writes[1].descriptorCount  = 1; 
    descriptor_writes[1].pBufferInfo      = nullptr;
    descriptor_writes[1].pImageInfo       = &imageinfo; 
    descriptor_writes[1].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;
}

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
void rekz::onscreen::UpdateOverlayDescriptors (rc::Buffer::Ref& buf,
                                               const std::array<std::string, kMaxCount>& strings, 
                                               const VkExtent2D& viewext, double dt) {

  uint8_t* uc = (uint8_t*) rokz::rc::MappedPointer (buf);

  using rokz::descriptor::MVPTransform; 
  

  // MVPTransform buffer
  MVPTransform* mvp  = reinterpret_cast<MVPTransform*>( uc + ut::offset_at (UB_sizes, 0) );

  mvp->model = glm::mat4(1); 
  mvp->view  = glm::mat4(1); 

  float lt = 0.0f;
  float rt = viewext.width;
  float bt = 0.0;
  float tp = viewext.height; 

  mvp->proj = glm::ortho ( lt, rt, bt, tp); 
  mvp->proj[1][1] *= -1;
    
  //  updat text 
  UBText* text = reinterpret_cast<UBText*>( uc + ut::offset_at (UB_sizes, 1) );

  for (uint32 iline = 0; iline < kMaxCount; ++iline) { 
    auto& s = strings[iline];
    std::copy  (s.begin (), s.end (), text[iline].text); 
    // for (uint32  charind = 0; charind < string_record[iline].size (); ++charind) {
    //   text[iline].text[charind] = string_record[iline][charind]; 

  }

  
}




