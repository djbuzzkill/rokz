
#ifndef ROKZ_SHADER_H
#define ROKZ_SHADER_H

#include "common.h"
#include "rokz_types.h"
#include "file.h"
#include <vulkan/vulkan_core.h>

namespace rokz {


  // -------------------------------------------------------------------------------------------
  //                       
  // -------------------------------------------------------------------------------------------

  bool                             CreateShaderModules (std::vector<ShaderModule>&                    shader_modules,
                                                        std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos, 
                                                        const systempath&                             fspath,
                                                        const VkDevice&                               device);
  //  VkShaderModule&    CreateShaderModule (VkShaderModule& shader_module, const bytearray& code, const VkDevice& dev); 
  //  bool               CreateShaderModules(std::vector<VkShaderModule>& shader_modules, std::vector<VkPipelineShaderStageCreateInfo> &shader_stage_create_infos, const std::filesystem::path& fspath, const VkDevice& device); 
  VkPipelineShaderStageCreateInfo& CreateInfo          (VkPipelineShaderStageCreateInfo& ci, VkShaderStageFlagBits stage_flags, const std::string& entry_point, const VkShaderModule& module); 
  //VkShaderModuleCreateInfo&        CreateInfo          (VkShaderModuleCreateInfo& ci, const rokz::bytearray& bin);
  VkShaderModuleCreateInfo&        CreateInfo          (VkShaderModuleCreateInfo& ci, const rokz::spvcode& spv);
  
  bool                             CreateShaderModule  (VkShaderModule& shmod, const VkShaderModuleCreateInfo& ci, const VkDevice& device); 
  bool                             CreateShaderModule  (ShaderModule& shm, const VkDevice& dev);
  bool                             CreateShaderModule_spv (ShaderModule& sm, const VkDevice& device);

  bool CompileThisShader_source (spvcode& out, VkShaderStageFlagBits shadertype, const std::string& srctex);
  bool CompileThisShader_file   (spvcode& out, VkShaderStageFlagBits shadertype, const systempath& fname);

}

#endif




