

#include "onscreen_pipe.h"
#include <vulkan/vulkan_core.h>



using namespace rokz;


const VkVertexInputBindingDescription&        rekz::onscreen::kVertexInputBindingDesc   = rokz::kPTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::onscreen::kVertexInputAttributeDesc = rokz::kPTx_InputAttributeDesc; 

const DescriptorSetLayoutBindings rekz::onscreen::kDescriptorBindings = {
  { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::onscreen::kMaxCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::onscreen::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
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
  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(obz::PushConstant),
                                obz::PCStages, dslos, device.handle);
  
  PipelineState_default (pipeline.state, msaa_samples, obz::kVertexInputAttributeDesc,
                         obz::kVertexInputBindingDesc, viewport_extent); 
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
bool rekz::onscreen::BindObjectDescriptorResources (Vec<VkDescriptorSet>&       dss ,
                                                    const Vec<rc::Buffer::Ref>& objbuffs,
                                                    const rc::ImageView::Ref    imageviews,  
                                                    const rc::Sampler::Ref      sampler, 
                                                    const DescriptorSetLayout&  dslayout, 
                                                    const Device&               device)  {
  //printf ("[%i]  %s\n", __LINE__, __FUNCTION__);
   assert (dss.size () == objbuffs.size ());
  //
  for (uint32_t i = 0; i < dss.size (); i++) {
    
    Vec<VkDescriptorBufferInfo> obparams (obz::kMaxCount, VkDescriptorBufferInfo {});

    for (size_t iobj = 0; iobj < obparams.size (); ++iobj) { 
      obparams[iobj].buffer   = objbuffs[i]->handle;    //
      obparams[iobj].offset   = iobj * sizeof(PolygonParam); // min_uniform_buffer_offset_alignment ??
      obparams[iobj].range    = sizeof(PolygonParam) ;       //glob.vma_objparam_buffs[i].ci.size;
    }
    //VkDescriptorImageInfo image_info {};
    Vec<VkDescriptorImageInfo> imageinfos (rekz::obz::kMaxCount);
    for (size_t iview = 0; iview < rekz::obz::kMaxCount; ++iview) { 

      if (iview < imageviews.size ()) { 
        imageinfos[iview] = {};
        imageinfos[iview].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
        imageinfos[iview].imageView   = imageviews[iview]->handle;
        imageinfos[iview].sampler     = sampler->handle;
      }
      else { // err'thing must b written
        imageinfos[iview] = {};
        imageinfos[iview].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;
        imageinfos[iview].imageView   = imageviews[0]->handle;
        imageinfos[iview].sampler     = sampler->handle;
        }
    }
    //
    std::array<VkWriteDescriptorSet, 2> descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;
    descriptor_writes[0].dstSet           = dss[i];
    descriptor_writes[0].dstBinding       = 0;       // does it match in shader? 
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = obparams.size(); // <
    descriptor_writes[0].pBufferInfo      = &obparams[0]; 
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 
                      
    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = dss[i];
    descriptor_writes[1].dstBinding       = 1;      // <-- change shader too
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_SAMPLER;
    descriptor_writes[1].descriptorCount  = imageinfos.size(); 
    descriptor_writes[1].pBufferInfo      = nullptr;
    descriptor_writes[1].pImageInfo       = &imageinfos[0]; 
    descriptor_writes[1].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;
}

