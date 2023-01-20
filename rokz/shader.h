
#ifndef ROKZ_SHADER_H
#define ROKZ_SHADER_H

#include "common.h"
#include "rokz_types.h"
#include "file.h"

namespace rokz {

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkPipelineShaderStageCreateInfo& CreateInfo (VkPipelineShaderStageCreateInfo& ci, VkShaderStageFlagBits stage_flags, const VkShaderModule& module); 
  
  bool               CreateShaderModule (ShaderModule& shm, const std::string fsrc, const VkDevice& dev);
  bool               CreateShaderModules (std::vector<ShaderModule>&                    shader_modules,
                                          std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_create_infos, 
                                          const std::filesystem::path&                  fspath,
                                          const VkDevice&                               device);


  VkShaderModule&    CreateShaderModule (VkShaderModule& shader_module, const bytearray& code, const VkDevice& dev); 
  bool               CreateShaderModules(std::vector<VkShaderModule>& shader_modules, std::vector<VkPipelineShaderStageCreateInfo> &shader_stage_create_infos, const std::filesystem::path& fspath, const VkDevice& device); 



}

#endif




