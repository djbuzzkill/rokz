
#ifndef ONSCREEN_PIPELINE_INLUCDE
#define ONSCREEN_PIPELINE_INCLUDE


#include "rekz.h"
#include "rokz/vert_defs.h"
#include <vulkan/vulkan_core.h>


namespace rekz { namespace onscreen {

    using namespace rokz;
    
    typedef rokz::PTx_Vert Vert;
    // ----------------------------------------------------------------------------------------------
    const size_t                                         kMaxCount = 128; // y?
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;   
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc; 
    extern const DescriptorSetLayoutBindings             kDescriptorBindings;

    // ----------------------------------------------------------------------------------------------
    struct PushConstant {

      uint32 resource_id;
      uint32 _unused_1;
      uint32 _unused_2;
      uint32 _unused_3;

      glm::vec4 color;
      glm::vec2 advance;
      glm::vec2 position;
    };

    // push const used in..
    const VkShaderStageFlags PCStages = VK_SHADER_STAGE_VERTEX_BIT  
                                      | VK_SHADER_STAGE_FRAGMENT_BIT;

    // 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234  
    struct UBText {
      uint32 text[64];
    };  // elem[kMaxCount] 

    // ---------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------
    bool InitPipeline (Pipeline&                         pipeline,
                       PipelineLayout&                   plo,
                       const Vec<VkDescriptorSetLayout>& dslos,
                       //0
                       const systempath&                 fspath,
                       const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                       VkSampleCountFlagBits             msaa_samples,
                       VkFormat                          color_format,
                       const Device&                     device); 
    // ----------------------------------------------------------------------------------------------
    // 
    // ----------------------------------------------------------------------------------------------
    bool BindObjectDescriptorResources (Vec<VkDescriptorSet>&       dss , 
                                        const Vec<rc::Buffer::Ref>& ubtext,
                                        const rc::ImageView::Ref    imageviews,  
                                        const rc::Sampler::Ref      sampler, 
                                        const DescriptorSetLayout&  dslayout, 
                                        const Device&               device) ;

  }} // rekz onscreen


#endif
