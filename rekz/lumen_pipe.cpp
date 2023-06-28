
#include "lumen_pipe.h"


using namespace rokz;
// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
const VkShaderStageFlags k_descriptor_stages =
  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; 

// -------------------------------------------------------------------------------------------
namespace lumen::gbuff
{ 
  // ------------------------------------------------------------------------------------
  const VkVertexInputBindingDescription&        kVertexInputBindingDesc = kPNTx_InputBindingDesc;
  const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc = kPNTx_InputAttributeDesc;
  //
  const Vec<VkDescriptorSetLayoutBinding> kDescriptorBindings = {
    { UBO_BINDINGI,                 VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         0, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // height
    { POSITION_ATTACHMENT_BINDINGI, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // 
    { NORMAL_ATTACHMENT_BINDINGI,   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // 
    { ALBEDO_ATTACHMENT_BINDINGI,   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // 
  };
}

// -------------------------------------------------------------------------------------------
namespace lumen::lcomp
{ 
  const VkVertexInputBindingDescription&        kVertexInputBindingDesc   = kPTx_InputBindingDesc; 

  // const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc = kPTx_InputAttributeDesc;

}




// bool setup_shader_modules (Pipeline& pipeline,
//                            const std::filesystem::path& respath, const rokz::Device& device) {

//   Vec<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos = pipeline.state.ci.shader_stages; 
//   Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;
//   //ShaderStateDef { vert_name_src, VK_SHADER_STAGE_VERTEX_BIT }
//   std::string vert_name_src = respath/"lumen/lumen_vert.vert";
//   std::string frag_name_src = respath/"lumen/lumen_frag.frag"; 

//   Vec<ShaderStageDef> stagedefs = {
//     { "main", vert_name_src, VK_SHADER_STAGE_VERTEX_BIT },
//     { "main", frag_name_src, VK_SHADER_STAGE_FRAGMENT_BIT },
//   };


//   shader_modules.resize  (4);
//   shader_stage_create_infos.resize(4);

//   for (size_t istage = 0; istage < stagedefs.size (); ++istage) {
//     if (!CompileThisShader_file (shader_modules[istage].spv, stagedefs[istage].stage, stagedefs[istage].fqsource)) 
//        return false; 

//      CreateInfo (shader_modules[istage].ci, shader_modules[istage].spv);
//      if (!CreateShaderModule_spv (shader_modules[istage], device.handle)) 
//        return false;

//      CreateInfo (shader_stage_create_infos[istage], stagedefs[istage].stage,
//                  stagedefs[istage].entrypoint, shader_modules[istage].handle); //   

//    }

//   return true; 
// }

//
bool setup_gbuff_shader_modules (Pipeline& pipeline, const systempath& fspath, const Device& device) {
  HERE("");

  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_cis = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;
  // std::string vert_name_src = respath/"lumen/lumen_vert.vert";
  // std::string frag_name_src = respath/"lumen/lumen_frag.frag"; 
  const std::string gbuff_vs_file = fspath/"lumen/lumen_gbuff_vert.vert";
  const std::string gbuff_fs_file = fspath/"lumen/lumen_gbuff_frag.frag";

  pipeline.shader_stage_defs  = {
    { "main", gbuff_vs_file, VK_SHADER_STAGE_VERTEX_BIT   },
    { "main", gbuff_fs_file, VK_SHADER_STAGE_FRAGMENT_BIT },
  };



  //printf ( "LINE [%i] --> %s \n", __LINE__, vert_file_path.string().c_str()); 
  return rokz::SetupPipelinShaderStages (shader_stage_cis, shader_modules, pipeline.shader_stage_defs, device);
}

// -- 
bool setup_lcomp_shader_modules (Pipeline& pipeline, const systempath& fspath, const Device& device) {
  HERE("");

  Vec<VkPipelineShaderStageCreateInfo>& shader_stage_cis = pipeline.state.ci.shader_stages; 
  Vec<ShaderModule>&                    shader_modules            = pipeline.shader_modules;

  const std::string lcomp_vs_file = fspath/"lumen/lumen_lcomp_vert.vert";
  const std::string lcomp_fs_file = fspath/"lumen/lumen_lcomp_frag.frag";

  pipeline.shader_stage_defs  = {
    { "main", lcomp_vs_file, VK_SHADER_STAGE_VERTEX_BIT   },
    { "main", lcomp_fs_file, VK_SHADER_STAGE_FRAGMENT_BIT },
  };

  //printf ( "LINE [%i] --> %s \n", __LINE__, vert_file_path.string().c_str()); 
  return rokz::SetupPipelinShaderStages (shader_stage_cis, shader_modules, pipeline.shader_stage_defs, device);
}


// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
bool lumen::InitPipeline (Pipeline&                    pipeline,
                          PipelineLayout&              plo,
                          VkRenderPass                 renderpass, 
                          const Vec<VkDescriptorSetLayout>&  dslos,
                          // VkSampleCountFlagBits        msaa_samples,
                          VkFormat                     color_format,
                          VkFormat                     depth_format,
                          const std::filesystem::path& pipe_path,
                          const VkExtent2D&            displayextent,
                          const Device&                device)
{
  HERE("HAI");

  const std::string lcomp_vs_file = pipe_path/"lumen/lumen_comp_vert.vert";
  const std::string lcomp_fs_file = pipe_path/"lumen/lumen_comp_frag.frag";

  const std::string gbuff_vs_file = pipe_path/"lumen/lumen_gbuf_vert.vert";
  const std::string gbuff_fs_file = pipe_path/"lumen/lumen_gbuf_frag.frag";

  setup_lcomp_shader_modules;
  setup_gbuff_shader_modules;
  
  HERE("BAI");
  return true;
}
  
    // ----------------------------------------------------------------------------------------
    // set 0= Global  descriptors ,  set 1= landscape descriptors
    // ----------------------------------------------------------------------------------------
bool lumen::BindDescriptorResources (Vec<VkDescriptorSet>&      dss,
                                    const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                    const Device&              device) {
  HERE("HI");




  
  assert (false); 
  return true; 
}





