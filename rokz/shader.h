
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
                                                        const std::filesystem::path&                  fspath,
                                                        const VkDevice&                               device);
  //  VkShaderModule&    CreateShaderModule (VkShaderModule& shader_module, const bytearray& code, const VkDevice& dev); 
  //  bool               CreateShaderModules(std::vector<VkShaderModule>& shader_modules, std::vector<VkPipelineShaderStageCreateInfo> &shader_stage_create_infos, const std::filesystem::path& fspath, const VkDevice& device); 
  VkPipelineShaderStageCreateInfo& CreateInfo          (VkPipelineShaderStageCreateInfo& ci, VkShaderStageFlagBits stage_flags, const std::string& entry_point, const VkShaderModule& module); 
  VkShaderModuleCreateInfo&        CreateInfo          (VkShaderModuleCreateInfo& ci, const rokz::bytearray& bin);
  VkShaderModuleCreateInfo&        CreateInfo          (VkShaderModuleCreateInfo& ci, const rokz::spvcode& spv);
  
  bool                             CreateShaderModule  (VkShaderModule& shmod, const VkShaderModuleCreateInfo& ci, const VkDevice& device); 
  bool                             CreateShaderModule  (ShaderModule& shm, const VkDevice& dev);
  bool                             CreateShaderModule_spv (ShaderModule& sm, const VkDevice& device);


  bool CompileThisShader_source (spvcode& out, VkShaderStageFlagBits shadertype, const std::string& src);

  bool CompileThisShader_file   (spvcode& out, VkShaderStageFlagBits shadertype, const std::string& fname);

  inline bool CompileThisShader_vertf (spvcode& out, const std::string& fname) {
    return CompileThisShader_file (out, VK_SHADER_STAGE_VERTEX_BIT, fname);
  }
  
  inline bool CompileThisShader_fragf (spvcode& out, const std::string& fname) {
    return CompileThisShader_file (out, VK_SHADER_STAGE_FRAGMENT_BIT, fname);
  }

  inline bool CompileThisShader_tesef (spvcode& out, const std::string& fname) {
    return CompileThisShader_file (out, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, fname);
  }

  inline bool CompileThisShader_tescf (spvcode& out, const std::string& fname) {
    return CompileThisShader_file (out, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, fname);
  }



}

#endif




