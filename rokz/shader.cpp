
#include "shader.h"



#include "utility.h"
#include "defaults.h"
#include "pipeline.h"
#include <vulkan/vulkan_core.h>

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineShaderStageCreateInfo& rokz::CreateInfo (VkPipelineShaderStageCreateInfo& ci, VkShaderStageFlagBits stage_flags,
                                                   const std::string& entry_point, const VkShaderModule& module)
{
  ci.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.stage  = stage_flags; // VK_SHADER_STAGE_VERTEX_BIT;
  ci.module = module;
  ci.pSpecializationInfo = nullptr; 
  ci.pName = entry_point.c_str();

  return ci; 
}

//-------------------------------------------------------------------------------------
//                
//-------------------------------------------------------------------------------------
VkShaderModuleCreateInfo& rokz::CreateInfo (VkShaderModuleCreateInfo& ci, const rokz::bytearray& bin) {
  printf ( "%s", __FUNCTION__); 
  //printf ("\n     --> size[%zu] | %s   \n", sm.bin.size(), fsrc.c_str() ); 
  ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  ci.pNext    = nullptr;
  ci.flags    = 0; // NOT VkShaderStageFlagBits
  ci.codeSize = bin.size();
  ci.pCode    = reinterpret_cast<const uint32_t*>(&bin[0]);
  //  printf (" %s --> bin.size : [%zu] \n", __FUNCTION__, bin.size ()); 

  return ci; 
}

//-------------------------------------------------------------------------------------
//                
//-------------------------------------------------------------------------------------
bool rokz::CreateShaderModule (VkShaderModule& shmod, const VkShaderModuleCreateInfo& ci, const VkDevice& device) {

  printf ( "%s", __FUNCTION__); 

  if (vkCreateShaderModule(device, &ci, nullptr, &shmod) != VK_SUCCESS) {
    printf ("[%s] FAILED create shader module\n", __FUNCTION__);
    return false;
  }
  
  return true;
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool rokz::CreateShaderModule (ShaderModule& sm, const VkDevice& device) {

  printf ( "%s", __FUNCTION__); 

  if (vkCreateShaderModule(device, &sm.ci, nullptr, &sm.handle) != VK_SUCCESS) {
    printf ("[%s]FAILED create shader module\n", __FUNCTION__);
    return false;
  }

  return true;
}
//-------------------------------------------------------------------------------------
//                
//-------------------------------------------------------------------------------------
// bool rokz::CreateShaderModules (
//     std::vector<ShaderModule>&                    shader_modules,
//     std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos, 
//     const std::filesystem::path&                  fspath,
//     const VkDevice&                               device)
// {

//   printf ("%s\n", __FUNCTION__); 
//   shader_modules.resize            (2);
//   shader_stage_create_infos.resize (2);
  
//   // VERT SHADER 
//   printf ("[2] B4 VERT %s:\n", __FUNCTION__, __LINE__); 
//   std::filesystem::path vert_file_path  =  fspath / "data/shader/basic3D_vert.spv" ;

//   if (!rokz::CreateShaderModule (shader_modules[0], vert_file_path.string(), device))
//     return false; 
  
//   rokz::CreateInfo (shader_stage_create_infos[0], VK_SHADER_STAGE_VERTEX_BIT, shader_modules[0].handle); 

  
//   // FRAG SHADER
//   printf ("[3] B4 FRAG %s\n", __FUNCTION__); 
//   std::filesystem::path frag_file_path = fspath /  "data/shader/basic_frag.spv" ;

//   if (!rokz::CreateShaderModule (shader_modules[1], frag_file_path.string(), device))
//     return false; 
  
//   rokz::CreateInfo (shader_stage_create_infos[1], VK_SHADER_STAGE_FRAGMENT_BIT, shader_modules[1].handle); 
//   //

//   return true; 
// }


 

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

// VkShaderModule& rokz::CreateShaderModule (VkShaderModule& shader_module,
//                                           const rokz::bytearray& code,
//                                           const VkDevice& dev) {
  
//   VkShaderModuleCreateInfo create_info {};
//   create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//   create_info.pNext = nullptr;
//   create_info.codeSize = code.size();
//   create_info.pCode = reinterpret_cast<const uint32_t *>(&code[0]);

//   if (vkCreateShaderModule(dev, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
//     printf ("failed to create shader module!\n");
//     //throw std::runtime_error("failed to create shader module!");
//   }

//   return shader_module; 
// }


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// bool rokz::CreateShaderModules (
//     std::vector<VkShaderModule> &shader_modules,
//     std::vector<VkPipelineShaderStageCreateInfo> &shader_stage_create_infos, 
//     const std::filesystem::path& fspath,
//     const VkDevice& device) {
  
//   printf ("%s\n", __FUNCTION__); 
//   shader_stage_create_infos.resize (2);
  

//   // VERT SHADER 
//   std::filesystem::path vert_file_path  =  fspath / "data/shader/basic3D_vert.spv" ;

//   printf ("[1] %s\n",   vert_file_path.string().c_str() );
//   bytearray             vertbin;
//   VkShaderModule        vertmod; 
//   From_file(vertbin, vert_file_path.string());
//   //if (!rokz::CreateShaderModule (shader_modules[0], vertbin, device))
//   if (!rokz::CreateShaderModule (vertmod, vertbin, device))
//     return false; 
  
//   printf ("[2] %s\n", __FUNCTION__); 
//   shader_modules.push_back (vertmod); // vertmod); 
//   VkPipelineShaderStageCreateInfo& vert_shader_stage_info = shader_stage_create_infos[0];
//   vert_shader_stage_info = {};   
//   vert_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//   vert_shader_stage_info.pNext = nullptr;
//   vert_shader_stage_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
//   vert_shader_stage_info.module = vertmod;
//   vert_shader_stage_info.pSpecializationInfo = nullptr; 
//   vert_shader_stage_info.pName = "main";

//   printf ("[3] %s\n", __FUNCTION__); 
//   // FRAG SHADER
//   std::filesystem::path frag_file_path = fspath /  "data/shader/basic_frag.spv" ;
//   bytearray      fragbin;
//   VkShaderModule fragmod; 
//   From_file(fragbin, frag_file_path.string());
//   //if (!rokz::CreateShaderModule (shader_modules[1],  fragbin, device))
//   if (!rokz::CreateShaderModule (fragmod, fragbin, device))
//     return false; 
  
//   shader_modules.push_back (fragmod); // fragmod); 
//   VkPipelineShaderStageCreateInfo& frag_shader_stage_info  = shader_stage_create_infos[1];
//   frag_shader_stage_info = {}; 
//   frag_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//   frag_shader_stage_info.pNext = nullptr;
//   frag_shader_stage_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
//   frag_shader_stage_info.module = fragmod;
//   frag_shader_stage_info.pSpecializationInfo = nullptr; 
//   frag_shader_stage_info.pName = "main";

//   //
//   VkPipelineShaderStageCreateInfo shader_stages[] = {
//     vert_shader_stage_info,
//     frag_shader_stage_info
//   };


//   printf ("BAI %s\n", __FUNCTION__); 
//   return true; 
// }


