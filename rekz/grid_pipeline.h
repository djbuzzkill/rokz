//
#ifndef REKZ_GRID_PIPELINE
#define REKZ_GRID_PIPELINE

#include "rekz.h"
#include "rokz/vert_defs.h"
//#include "rc_buffer.h"

namespace rekz { 


  //
  //
  namespace grid {

    typedef rekz::PC_Vert Vert; 

    extern const VkVertexInputBindingDescription        kVertexBindingDesc;
    extern const Vec<VkVertexInputAttributeDescription> kVertInputAttributeDesc;
    // const Vec<VkDescriptorSetLayoutBinding>          kGridDescriptorBindings = dont need
    // ----------------------------------------------------------------------------------------------
    struct PushConstant {

      glm::vec4 x_color;  // 10 is reasonable
      glm::vec4 z_color;  // 10 is reasonable
      glm::vec4 origin_color; 

      float     xstep;    // size every 'column'
      float     zstep;    // size every 'row' 
      float     xoffset;
      float     zoffset;
    };

    const VkShaderStageFlags kPCStages =
      VK_SHADER_STAGE_VERTEX_BIT;

  }

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  bool InitGridPipeline  (Pipeline&              pipeline,
                          PipelineLayout&        plo,
                          //0
                          const Vec<VkDescriptorSetLayout>& dslos,
                          const std::filesystem::path&      fspath,
                          const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits             msaa_samples,
                          VkFormat                          color_format,
                          VkFormat                          depth_format,
                          const Device&          device) ; 

  // ----------------------------------------------------------------------------------------------
  // connect descriptors to buffers images samplers etc
  // ----------------------------------------------------------------------------------------------
  
  bool BindGridDescriptorResources (std::vector<VkDescriptorSet>& dss, const std::vector<rc::Buffer::Ref>& globalbuffs, const rokz::Device& device);
  
}


#endif
