
#include "shader.h"



#include "utility.h"
#include "defaults.h"
#include "pipeline.h"
#include <X11/X.h>
#include <memory>
#include <shaderc/env.h>
#include <shaderc/shaderc.h>
#include <shaderc/status.h>
#include <vulkan/vulkan_core.h>

#include <shaderc/shaderc.hpp>

using namespace rokz;

// -------------------------------------------------------------------------------------------
//                            
// -------------------------------------------------------------------------------------------
const std::map<VkShaderStageFlagBits, shaderc_shader_kind> VK_SHADER_2_shaderc_map = {
  { VK_SHADER_STAGE_VERTEX_BIT,  shaderc_vertex_shader},          
  { VK_SHADER_STAGE_FRAGMENT_BIT, shaderc_fragment_shader},        
  { VK_SHADER_STAGE_COMPUTE_BIT, shaderc_compute_shader},         
  { VK_SHADER_STAGE_GEOMETRY_BIT, shaderc_geometry_shader},          
  { VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,  shaderc_tess_control_shader},    
  { VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, shaderc_tess_evaluation_shader}, 
};

const std::map<VkShaderStageFlagBits, std::string> VK_shader_name_map = {
  { VK_SHADER_STAGE_VERTEX_BIT                 , "vertex_shader" },          
  { VK_SHADER_STAGE_FRAGMENT_BIT               , "fragment_shader" },        
  { VK_SHADER_STAGE_COMPUTE_BIT                , "compute_shader" },         
  { VK_SHADER_STAGE_GEOMETRY_BIT               , "geometry_shader" },          
  { VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT   , "tess_control_shader" },    
  { VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, "tess_evaluation_shader" }, 
};


struct H_INCLUDER : public shaderc::CompileOptions::IncluderInterface {
  //
public:

  H_INCLUDER () {
  } 

  std::shared_ptr<shaderc_include_result> include_result;
  std::string include_source;
  
  // Handles shaderc_include_resolver_fn callbacks.
  virtual shaderc_include_result* GetInclude(const char*          requested_source,
                                             shaderc_include_type type,
                                             const char*          requesting_source,
                                             size_t               include_depth) {
    filepath including_path = requesting_source;
    filepath parentpath     = including_path.parent_path ();
    filepath fqinclude      = parentpath/requested_source; 
    //printf("fqrequested %s\n", fqinclude.c_str());
    From_file (include_source, fqinclude); 
    include_result = std::make_shared<shaderc_include_result> ();
    include_result->content = include_source.c_str();
    include_result->content_length = include_source.size ();
    include_result->source_name = requested_source;
    include_result->source_name_length = strlen (requested_source);
    include_result->user_data = nullptr;
    // shaderc_include_type {
    //   shaderc_include_type_relative,  // E.g. #include "source"
    //   shaderc_include_type_standard   // E.g. #include <source>
    return include_result.get ();
  }

  // Handles shaderc_include_result_release_fn callbacks.
  virtual void ReleaseInclude(shaderc_include_result* data) {
    include_result.reset ();
  }

  virtual ~H_INCLUDER()  {
  }
};


// -------------------------------------------------------------------------------------------
bool rokz::CompileThisShader_file (spvcode& out, VkShaderStageFlagBits shadertype, const std::string& fname) {

  assert (VK_shader_name_map.count (shadertype));
  assert (VK_SHADER_2_shaderc_map.count (shadertype)); 
  
  shaderc::Compiler compiler; 
  //shaderc_compiler_t shc =  shaderc_compiler_initialize (); 
  //fname.c_str()
  std::string srcstr;
  rokz::From_file (srcstr, fname, true); 
  
  const  char*        infname    = fname.c_str();
  //VK_shader_name_map.at (shadertype).c_str ();

  filepath fqpath =  fname;

  printf ("[%s] -> ", fqpath.filename ().c_str ());
  
  shaderc_shader_kind shaderkind = VK_SHADER_2_shaderc_map.at (shadertype); 
  
  shaderc::CompileOptions ppopts;
  ppopts.SetTargetSpirv (shaderc_spirv_version_1_4); 
  ppopts.AddMacroDefinition ("MAX_OBJ_COUNT", "128");
  ppopts.SetIncluder (std::make_unique<H_INCLUDER> ()); 
  
  shaderc::PreprocessedSourceCompilationResult ppres =
    compiler.PreprocessGlsl ( srcstr.c_str(), shaderkind, infname, ppopts); 

  switch (ppres.GetCompilationStatus()) { 
  case shaderc_compilation_status_success:  {
    printf ("PREPROC -> ");
    //const std::string&  errstr = ppres.GetErrorMessage (); 
    //std::string ppsourc (ppres.begin (), ppres.end ()) ;
    //srcstr = ppres.begin ();
    //printf (" sourcestr :%s \n", srcstr.c_str());
    //printf ( " : preprocessed source: \n%s\n", ppsourc.c_str ()); 
  } break;
    
  case shaderc_compilation_status_compilation_error: {
    const std::string &err = ppres.GetErrorMessage (); 
    printf ("PREPROC ERROR: %s\n", err.c_str());
    printf ("compilation status: %i\n", ppres.GetCompilationStatus()); 
    printf ("error(s): %zu, warning(s): %zu.................\n", ppres.GetNumErrors (), ppres.GetNumWarnings());
    return false;
  } break;
  default :
    printf ("default\n" );
    break;    
  }

  
  shaderc::CompileOptions opts;
  opts.SetTargetSpirv (shaderc_spirv_version_1_6); 

  printf ("COMPILE -> ");
  shaderc::SpvCompilationResult compres =
    compiler.CompileGlslToSpv (srcstr, shaderkind, infname, ppopts);

  switch (compres.GetCompilationStatus ()) {
  case shaderc_compilation_status_success: {
    out.assign (compres.begin (), compres.end ());
    printf ("SUCCESS\n");
    return true;
  }

  default: { 
    const std::string& errstr = compres.GetErrorMessage ();
    printf ( "COMPILE ERROR :%s\n", errstr.c_str());
  }}
  
  return false;
}

bool rokz::CompileThisShader_vertf (spvcode& out, const std::string& fname) {
  return CompileThisShader_file (out, VK_SHADER_STAGE_VERTEX_BIT, fname);
}
  
bool rokz:: CompileThisShader_fragf (spvcode& out, const std::string& fname) {
  return CompileThisShader_file (out, VK_SHADER_STAGE_FRAGMENT_BIT, fname);
}

bool rokz:: CompileThisShader_tesef (spvcode& out, const std::string& fname) {
  return CompileThisShader_file (out, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, fname);
}

bool rokz:: CompileThisShader_tescf (spvcode& out, const std::string& fname) {
  return CompileThisShader_file (out, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, fname);
}
// -------------------------------------------------------------------------------------------
//                            
// -------------------------------------------------------------------------------------------
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
  //printf ( "%s\n", __FUNCTION__); 
  //printf ("\n     --> size[%zu] | %s   \n", sm.bin.size(), fsrc.c_str() ); 
  ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  ci.pNext    = nullptr;
  ci.flags    = 0; // NOT VkShaderStageFlagBits
  ci.codeSize = bin.size();
  ci.pCode    = reinterpret_cast<const uint32_t*>(&bin[0]);
  //printf (" %s --> bin.size : [%zu] \n", __FUNCTION__, bin.size ()); 

  return ci; 
}

//-------------------------------------------------------------------------------------
//                
//-------------------------------------------------------------------------------------
VkShaderModuleCreateInfo& rokz::CreateInfo (VkShaderModuleCreateInfo& ci, const rokz::spvcode& spv) {
  //printf ( "%s\n", __FUNCTION__); 
  
  ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  ci.pNext    = nullptr;
  ci.flags    = 0; // NOT VkShaderStageFlagBits
  ci.codeSize = sizeof(uint32) * spv.size (); 
  ci.pCode    = &spv[0];
  //printf (" %s --> bin.size : [%zu] \n", __FUNCTION__, bin.size ()); 

  return ci; 
}
//-------------------------------------------------------------------------------------
//                
//-------------------------------------------------------------------------------------
bool rokz::CreateShaderModule (VkShaderModule& shmod, const VkShaderModuleCreateInfo& ci, const VkDevice& device) {

  //printf ( "%s", __FUNCTION__); 

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

  if (sm.bin.size () < 4) {
    HERE("ERROR BAD BINARY SIZE");
    return false;
  }
  //printf ( "%s", __FUNCTION__);
  

  if (vkCreateShaderModule(device, &sm.ci, nullptr, &sm.handle) != VK_SUCCESS) {
    printf ("[%s]FAILED create shader module\n", __FUNCTION__);
    return false;
  }

  return true;
}

bool rokz::CreateShaderModule_spv (ShaderModule& sm, const VkDevice& device) {

  if (sm.spv.size () < 4) {
    HERE("ERROR BAD SPV SIZE");
    return false;
  }
  //printf ( "%s", __FUNCTION__); 

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


