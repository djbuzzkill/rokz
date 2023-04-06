
//
#ifndef REKZ_LANSCAPE_PIPELINE_INCLUDE
#define REKZ_LANSCAPE_PIPELINE_INCLUDE

#include "rekz.h"
#include "rokz/vert_defs.h"
#include <vulkan/vulkan_core.h>

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  namespace lscape { 

    using namespace rokz; 
    
    typedef rokz::PNTx_Vert    Vert;
    const uint32               kControlPoints = 4; 

    extern const Vec<VkDescriptorSetLayoutBinding>       kDescriptorBindings;
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc;


    const VkShaderStageFlags kPCStages = VK_SHADER_STAGE_VERTEX_BIT
      | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
      | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT
      | VK_SHADER_STAGE_FRAGMENT_BIT;

    
    namespace tile {

      const uint32 kMaxCount = 128;
      
      struct PushConstant {
        glm::vec4 position;
        glm::vec4 scale;
        uint32 resource_id;
        uint32 _unused3_id;
        uint32 _unused4_id;
        uint32 _unused5_id;
      };

      static_assert (sizeof(PushConstant) <= 128, "exceeding max push constant"); 
    }    

    const uint32 HEIGHT_IMAGE_BINDINGI = 32;
    const uint32 NORMAL_IMAGE_BINDINGI = 33;
    const uint32 COLOR_IMAGE_BINDINGI  = 34;
    const uint32 PATCH_PARAMS_BINDINGI = 35;
    // ----------------------------------------------------------------------------------------
    //                     
    // ----------------------------------------------------------------------------------------
    bool InitPipeline (Pipeline&                    pipeline,
                       PipelineLayout&              plo,
                       Vec<VkDescriptorSetLayout>&  dslos,
                       VkSampleCountFlagBits        msaa_samples,
                       VkFormat                     color_format,
                       VkFormat                     depth_format,
                       const std::filesystem::path& pipe_path,
                       const VkExtent2D&            displayextent,
                       const Device&                device);
  
    // ----------------------------------------------------------------------------------------
    // set 0= Global  descriptors ,  set 1= landscape descriptors
    // ----------------------------------------------------------------------------------------
    bool BindDescriptorResources (Vec<VkDescriptorSet>&           dss,
                                  const Vec<rc::Buffer::Ref>&     globalubs, 

                                  const rc::Sampler::Ref&         colorsamp,
                                  const Vec<rc::ImageView::Ref>&  colorviews,

                                  const rc::Sampler::Ref&         heightsamp, 
                                  const Vec<rc::ImageView::Ref>&  heightviews,

                                  const rc::Sampler::Ref&         normalsamp, 
                                  const Vec<rc::ImageView::Ref>&  normalviews,

                                  const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                  const Device&              device);
  

  }




#endif
