
#include "dark_obj_pipeline.h"
#include "rokz/shared_descriptor.h"
#include <vulkan/vulkan_core.h>

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

using namespace rokz;
//const size_t max_frames_in_flight  = rekz::Glob::MaxFramesInFlight; 
const VkVertexInputBindingDescription&        rekz::obz::kVertexInputBindingDesc   = rokz::kPNCTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::obz::kVertexInputAttributeDesc = rokz::kPNCTx_InputAttributeDesc; 


// const DescriptorSetLayoutBindings rekz::obz::kDescriptorBindings = {
//   { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::obz::kMaxCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
//   { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::obz::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
// };

const auto kObParamsBindingI  = 18;
const auto kObTextureBindingI = 19;

const DescriptorSetLayoutBindings rekz::obz::kDescriptorBindings = {
  { global_ub::MVP_SCENE_BINDINGI, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , 1                   , VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { kObParamsBindingI            , VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::obz::kMaxCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { kObTextureBindingI           , VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::obz::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};



// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool setup_obz_shader_modules (Pipeline& pipeline, const systempath& fspath, const Device& device) {
  
  printf ("%s \n", __FUNCTION__); 

  "mack_rowes.h"; 

  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_cis = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;

  pipeline.shader_stage_defs  = {
    { "main", fspath/"objz/polyobj_vert.vert", VK_SHADER_STAGE_VERTEX_BIT   },
    { "main", fspath/"objz/polyobj_frag.frag", VK_SHADER_STAGE_FRAGMENT_BIT },
  };
  //printf ( "LINE [%i] --> %s \n", __LINE__, vert_file_path.string().c_str()); 
  return rokz::SetupPipelinShaderStages (shader_stage_cis, shader_modules, pipeline.shader_stage_defs, device);
}


// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::SetupObjectUniforms (Vec<rc::Buffer::Ref>& objparams, uint32_t num_sets, const Device& device) {
  //printf ("%s", __FUNCTION__);
 //
 objparams.resize (num_sets);
 for (size_t i = 0; i < num_sets; i++) {
   objparams[i] = rc::Create_uniform_mapped ( sizeof(PolygonParam), obz::kMaxCount, device);
   //CreateUniformBuffer (objparams[i], sizeof(PolygonParam), obz::kMaxCount, device);
 }
 //printf (" --> [true] \n"); 
 return true; 
}

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::BindObjectDescriptorResources (Vec<VkDescriptorSet>&         dss ,
                                          const Vec<rc::Buffer::Ref>&   globalub,
                                          const Vec<rc::Buffer::Ref>&   objectub,
                                          const Vec<rc::ImageView::Ref> imageviews,  //const ImageView&           texture_imageview, 
                                          const rc::Sampler::Ref        sampler, 
                                          const DescriptorSetLayout&    dslayout, //const rokz::DescriptorPool& descpool,
                                          const Device&                 device) {
  
  assert (dss.size () == objectub.size ());
  assert (dss.size () == globalub.size ());
  //
  for (uint32_t i = 0; i < dss.size (); i++) {
    // MVP Scene
    VkDescriptorBufferInfo globalinfo = {};
    globalinfo.buffer = globalub[i]->handle;
    globalinfo.offset = ut::offset_at (global_ub::UB_sizes, global_ub::MVP_SCENE_BINDINGI); 
    globalinfo.range  = sizeof (global_ub::MVPTransform); 
    // POLYGON PARAMS
    Vec<VkDescriptorBufferInfo> obparams (obz::kMaxCount, VkDescriptorBufferInfo {});
    for (size_t iobj = 0; iobj < rekz::obz::kMaxCount; ++iobj) { 
      obparams[iobj].buffer   = objectub[i]->handle;    //
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
    std::array<VkWriteDescriptorSet, 3> descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;
    descriptor_writes[0].dstSet           = dss[i];
    descriptor_writes[0].dstBinding       = global_ub::MVP_SCENE_BINDINGI;       // does it match in shader? 
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1; 
    descriptor_writes[0].pBufferInfo      = &globalinfo; 
    descriptor_writes[0].pImageInfo       = nullptr; 
    descriptor_writes[0].pTexelBufferView = nullptr; 

    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;
    descriptor_writes[1].dstSet           = dss[i];
    descriptor_writes[1].dstBinding       = kObParamsBindingI;       // does it match in shader?
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[1].descriptorCount  = obparams.size(); // <
    descriptor_writes[1].pBufferInfo      = &obparams[0]; 
    descriptor_writes[1].pImageInfo       = nullptr; 
    descriptor_writes[1].pTexelBufferView = nullptr; 
                      
    descriptor_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[2].pNext            = nullptr;    
    descriptor_writes[2].dstSet           = dss[i];
    descriptor_writes[2].dstBinding       = kObTextureBindingI;      // <-- change shader too
    descriptor_writes[2].dstArrayElement  = 0;
    descriptor_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[2].descriptorCount  = imageinfos.size(); 
    descriptor_writes[2].pBufferInfo      = nullptr;
    descriptor_writes[2].pImageInfo       = &imageinfos[0]; 
    descriptor_writes[2].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;

}

// ----------------------------------------------------------------------------------------
// init proto more orthogonal version (new SetupObjectPipeline)
// ----------------------------------------------------------------------------------------
bool rekz::InitObjPipeline (Pipeline&                   pipeline,
                            PipelineLayout&             plo,
                            const Vec<VkDescriptorSetLayout>& dslos,
                            //0
                            const std::filesystem::path&      fspath,
                            const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits             msaa_samples,
                            VkFormat                          color_format,
                            VkFormat                          depth_format,
                            const Device&               device) {

  printf ("[%s] --> %i \n", __FUNCTION__, __LINE__); 
  DefineGraphicsPipelineLayout (plo.handle, plo.ci, sizeof(obz::PushConstant),
                                obz::PCStages, dslos, device.handle);
  
  PipelineState_default (pipeline.state, msaa_samples, obz::kVertexInputAttributeDesc,
                         obz::kVertexInputBindingDesc, viewport_extent); 
  // ^ !! shader modules is part of pipelinestate 

  if (!setup_obz_shader_modules (pipeline, fspath, device)) {
    return false;
  }

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



// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz_BindObjectDescriptorResources (Vec<VkDescriptorSet>&         dss ,
                                          const Vec<rc::Buffer::Ref>&   objbuffs,
                                          const Vec<rc::ImageView::Ref> imageviews,  //const ImageView&           texture_imageview, 
                                          const rc::Sampler::Ref        sampler, 
                                          const DescriptorSetLayout&    dslayout, //const rokz::DescriptorPool& descpool,
                                          const Device&                 device) {
  
  //printf ("[%i]  %s\n", __LINE__, __FUNCTION__);
   assert (dss.size () == objbuffs.size ());
  //
  for (uint32_t i = 0; i < dss.size (); i++) {
    
    Vec<VkDescriptorBufferInfo> obparams (rekz::obz::kMaxCount, VkDescriptorBufferInfo {});

    for (size_t iobj = 0; iobj < obparams.size (); ++iobj) { 
      obparams[iobj].buffer   = objbuffs[i]->handle;    //
      obparams[iobj].offset   = iobj * sizeof(rekz::PolygonParam); // min_uniform_buffer_offset_alignment ??
      obparams[iobj].range    = sizeof(rekz::PolygonParam) ;       //glob.vma_objparam_buffs[i].ci.size;
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
    descriptor_writes[0].dstBinding       = 0;
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
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    descriptor_writes[1].descriptorCount  = imageinfos.size(); 
    descriptor_writes[1].pBufferInfo      = nullptr;
    descriptor_writes[1].pImageInfo       = &imageinfos[0]; 
    descriptor_writes[1].pTexelBufferView = nullptr; 

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);

  }

  return true;

}
