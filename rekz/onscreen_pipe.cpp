

#include "onscreen_pipe.h"
#include "rokz/pipeline.h"
#include "rokz/shared_descriptor.h"
#include "rokz/ut_offset.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <vulkan/vulkan_core.h>



using namespace rokz;

using UBText = global_ub::TextItem;

const VkVertexInputBindingDescription&        rekz::onscreen::kVertexInputBindingDesc   = rokz::kPTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::onscreen::kVertexInputAttributeDesc = rokz::kPTx_InputAttributeDesc; 


const DescriptorSetLayoutBindings rekz::onscreen::kDescriptorBindings = {
  // MVP OVERLAY
  { global_ub::MVP_OVERLAY_BINDINGI, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER      , 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  // TEXT ELEMENTS
  { global_ub::TEXTITEMS_BINDINGI, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , 128, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT , nullptr }, // array of structs per obj
  // FONT FACE
  { global_ub::FONT_FACE_BINDINGI, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};


bool setup_onscreen_shaders (Pipeline& pipeline, const systempath& fspath, const Device& device) {

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
                                   VkFormat                          colorformat,
                                   VkFormat                          depthformat, 
                                   const Device&                     device)
{
  //printf ("[%s] --> %i \n", __FUNCTION__, __LINE__); 
  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(onscreen::PushConstant),
                                onscreen::PCStages, dslos, device.handle);

  PipelineState_alpha_test (pipeline.state, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, 
                            msaa_samples, onscreen::kVertexInputAttributeDesc,
                            onscreen::kVertexInputBindingDesc, viewport_extent); 
  // ^ !! shader modules is part of pipelinestate 
  if (!setup_onscreen_shaders  (pipeline, fspath, device)) {
    return false;
  }

  // proto more orthogonal version
  pipeline.ext.pipeline_rendering.color_formats.resize (1);
  pipeline.ext.pipeline_rendering.color_formats[0] = colorformat;

  CreateInfo  (pipeline.ext.pipeline_rendering.ci,
               pipeline.ext.pipeline_rendering.color_formats, depthformat); 

  auto& psci = pipeline.state.ci;
  //
  CreateInfo (pipeline.ci,
              plo.handle,
              &pipeline.ext.pipeline_rendering.ci,                    
              psci.shader_stages,      // const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
              &psci.input_assembly,    // const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
              &psci.vertexinputstate,  // const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
              &psci.viewport_state,    // const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
              nullptr,                 // tesselation 
              &psci.rasterizer,        // const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
              &psci.multisampling,     // const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
              &psci.depthstencilstate, // const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
              &psci.colorblendstate,   // const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
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
                                              //const Vec<rc::Buffer::Ref>& textubs,
                                              const rc::ImageView::Ref    imageview,  
                                              const rc::Sampler::Ref      sampler, 
                                              const DescriptorSetLayout&  dslayout, 
                                              const Device&               device) {
  assert (globalubs.size () != 0); 
  //assert (textubs.size () != 0); 
  // separate mvp+ostext buffers or combine
  for (uint32_t i = 0; i < dss.size (); i++) {
    // setup uniform
    VkDescriptorBufferInfo mvpinfo {};
    mvpinfo.buffer = globalubs[i]->handle;
    mvpinfo.offset = ut::offset_at (global_ub::UB_sizes, global_ub::MVP_OVERLAY_BINDINGI); 
    mvpinfo.range  = sizeof(global_ub::MVPTransform);
      
    Vec<VkDescriptorBufferInfo> textlines (global_ub::kMaxTextElements, VkDescriptorBufferInfo {});
    const size_t text_base = ut::offset_at (global_ub::UB_sizes, global_ub::TEXTITEMS_BINDINGI); 
    for (size_t iobj = 0; iobj < global_ub::kMaxTextElements; ++iobj) { 
      textlines[iobj].buffer = globalubs[i]->handle;    //
      textlines[iobj].offset = text_base + iobj * sizeof(global_ub::TextItem); 
      textlines[iobj].range = sizeof(global_ub::TextItem); 
    }
    // setup texture+sampler
    VkDescriptorImageInfo  imageinfo; //  (onscreen::kMaxCount);
    imageinfo = {};
    imageinfo.imageLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
    imageinfo.imageView      = imageview->handle;
    imageinfo.sampler        = sampler->handle;
    //

    const size_t num_to_write = 3; 
    std::array<VkWriteDescriptorSet, num_to_write> dws {};
    dws[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[0].pNext            = nullptr;
    dws[0].dstSet           = dss[i];
    dws[0].dstBinding       = global_ub::MVP_OVERLAY_BINDINGI;       // does it match in shader? 
    dws[0].dstArrayElement  = 0;
    dws[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dws[0].descriptorCount  = 1; // <
    dws[0].pBufferInfo      = &mvpinfo; 
    dws[0].pImageInfo       = nullptr; 
    dws[0].pTexelBufferView = nullptr; 

    dws[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[1].pNext            = nullptr;
    dws[1].dstSet           = dss[i];
    dws[1].dstBinding       = global_ub::TEXTITEMS_BINDINGI; 
    dws[1].dstArrayElement  = 0;
    dws[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dws[1].descriptorCount  = global_ub::kMaxTextElements;
    dws[1].pBufferInfo      = &textlines[0]; 
    dws[1].pImageInfo       = nullptr; 
    dws[1].pTexelBufferView = nullptr; 
    
    dws[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[2].pNext            = nullptr;    
    dws[2].dstSet           = dss[i];
    dws[2].dstBinding       = global_ub::FONT_FACE_BINDINGI;
    dws[2].dstArrayElement  = 0;
    dws[2].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    dws[2].descriptorCount  = 1; 
    dws[2].pBufferInfo      = nullptr;
    dws[2].pImageInfo       = &imageinfo; 
    dws[2].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, num_to_write, &dws[0], 0, nullptr);
  }

  return true;
}


// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
void rekz::onscreen::UpdateOSD (rc::Buffer::Ref& buf,
                                const std::array<std::string, global_ub::kMaxTextElements>& strings,
                                const VkExtent2D& viewext, double dt) {
  
  uint8_t* uc = (uint8_t*) rokz::rc::MappedPointer (buf);

  UBText* text = // textelem[resource_id][gl_InstanceIndex] == [line][char]
    reinterpret_cast<UBText*>(uc + ut::offset_at (global_ub::UB_sizes, global_ub::TEXTITEMS_BINDINGI));

  for (uint32 iline = 0; iline < global_ub::kMaxTextElements; ++iline) { 
    auto& s = strings[iline];
    std::copy  (s.begin (), s.end (), text[iline].begin()); 

  }

}




