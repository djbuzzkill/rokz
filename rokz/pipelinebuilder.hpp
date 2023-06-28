//
#ifndef ROKZ_PIPELINE_BUILDER_INCLUDE
#define ROKZ_PIPELINE_BUILDER_INCLUDE

#include "rokz_types.hpp"
#include "rc_types.hpp"
#include "common.hpp"



namespace rokz { 


  class PipelineBuilder {
    //  
  public:

    rc::Pipeline::Ref MakePipeline (const Device& device); 


    PipelineBuilder& SetPipelineLayout      (VkPipelineLayout plo);
    PipelineBuilder& SetDescriptorSetLayout (const Vec<DescriptorSetLayout>& dslos); 
    // SetDefault*****State ( ***State& 


    //    VkPipelineLayoutCreateInfo&

    
  protected:
    
    PipelineState                pls;
    VkGraphicsPipelineCreateInfo ci;
    Vec<rokz::ShaderModule>      shader_modules; 
    Vec<ShaderStageDef>          shader_stage_defs;
    PipelineState                state;
    VkPipelineLayout             plo;
    Vec<DescriptorSetLayout>     dslos;

    
  };


}
#endif
