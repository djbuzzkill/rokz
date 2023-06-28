
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

      Pipeline (const Device& dev) : deviceob (dev)  {
      }

      virtual ~Pipeline () { if (handle) {
          vkDestroyPipeline (device.handle, handle, VK_NULL_HANDLE);
          handle = VK_NULL_HANDLE; 
        }}

        PipelineState              state;
        Vec<ShaderModule>          shader_modules; 
        Vec<VkDescriptorSetLayout> dslos;
    };


    // -- -- 
    // struct Pipeline : deviceob<VkPipeline, Pipeline> {
    //   Pipeline (const Device& dev) : deviceob(dev) {};
    //   virtual ~Pipeline () { if (handle) {
    //       vkDestroyPipeline (device.handle, handle, VK_NULL_HANDLE);
    //       handle = VK_NULL_HANDLE; 
    //     }
    //   }}; 


    // -- ??? -- 
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

    // -- -- 
    struct RenderPass : public deviceob<VkRenderPass, RenderPass> {

      RenderPass (const Device& dev): deviceob (dev) {
      } 
      
      virtual ~RenderPass () { if (handle) { 
          vkDestroyRenderPass (device.handle, handle, VK_NULL_HANDLE); 
          handle = VK_NULL_HANDLE;
        }}
      
      Vec<VkAttachmentDescription> attach_desc;
      Vec<VkAttachmentReference>   attach_ref;

      Vec<VkSubpassDescription>    subpass_descs;
      Vec<VkSubpassDependency>     dependencies;
    };

    // -- -- 
    struct Framebuffer : public deviceob<VkFramebuffer, Framebuffer> { 
      Framebuffer (const Device& dev) : deviceob (dev) { 
      }
    
      virtual ~Framebuffer  () { if (handle) { 
          vkDestroyFramebuffer (device.handle, handle, VK_NULL_HANDLE);
          handle = VK_NULL_HANDLE; 
        }
      }
      //VkFramebufferCreateInfo  ci;
      //std::vector<VkImageView> attachments; 
    };
    
  }}
#endif
