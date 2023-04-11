
#ifndef ROKZ_RC_TYPES_INCLUDE
#define ROKZ_RC_TYPES_INCLUDE


#include "common.hpp"
#include "rokz_types.hpp"

namespace rokz { namespace rc {

    // -----------------------------------------------------------------------------------------
    struct Sampler : public deviceob<VkSampler, Sampler> {

      Sampler (const Device& d): deviceob (d) { }

      virtual ~Sampler (); 
    };
    //
    Sampler::Ref CreateSampler_default (const Device& device); 

    Sampler::Ref CreateSampler  (const VkSamplerCreateInfo ci, const Device& device); 

    // -----------------------------------------------------------------------------------------
    struct Pipeline: public deviceob<VkPipeline, Pipeline> {

      Pipeline (const Device& dev) : deviceob (dev), state () {
      }
      
      PipelineState              state;
      Vec<rokz::ShaderModule>    shader_modules; 
      Vec<VkDescriptorSetLayout> dslos;
    
    };


    struct DynRenderPipe: public Pipeline {

      DynRenderPipe (const Device& dev) : Pipeline (dev) {
      }
      
      // EXTENSIONS
      struct { 
        struct {
          VkPipelineRenderingCreateInfoKHR ci; 
          Vec<VkFormat>         color_formats;
          VkFormat              depth_format;
        } pipeline_rendering;
      } ext;

    };

    
    

  }}
#endif
