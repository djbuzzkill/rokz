
#ifndef ONSCREEN_PIPELINE_INCLUDE
#define ONSCREEN_PIPELINE_INCLUDE


#include "rekz.h"
#include "rokz/vert_defs.h"
#include "rokz/shared_descriptor.h"
#include "rokz/ut_offset.h"

#include <vulkan/vulkan_core.h>


namespace rekz { namespace onscreen {

    using namespace rokz;
    
    typedef rokz::PTx_Vert Vert;
    // ----------------------------------------------------------------------------------------------
    const size_t                                         kMaxGlyphCount = 128; 

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
      glm::vec4 advance;
      glm::vec4 position;

      //uint32  str[16]; 
    };

    // textelem[resource_id][gl_InstanceIndex]  == [line][char]

    // -- push constant used in ...
    const VkShaderStageFlags PCStages = VK_SHADER_STAGE_VERTEX_BIT  
                                      | VK_SHADER_STAGE_FRAGMENT_BIT;

    // 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234  
    bool InitPipeline (Pipeline&                         pipeline,
                       PipelineLayout&                   plo,
                       const Vec<VkDescriptorSetLayout>& dslos,
                       //0
                       const systempath&                 fspath,
                       const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                       VkSampleCountFlagBits             msaa_samples,
                       VkFormat                          colorfor,
                       VkFormat                          depthforr,
                       const Device&                     device); 
    // -- binding for 
    bool BindDescriptorResources (Vec<VkDescriptorSet>&       dss,
                                  const Vec<rc::Buffer::Ref>& globalubs, 
                                  //const Vec<rc::Buffer::Ref>& textubs,
                                  const rc::ImageView::Ref    imageview,  
                                  const rc::Sampler::Ref      sampler, 
                                  const DescriptorSetLayout&  dslayout, 
                                  const Device&               device); 

    // ---------------------------------------------------------------------------------------
    void UpdateOSD (rc::Buffer::Ref& buf, 
                    const std::array<std::string, global_ub::kMaxTextElements>& strings, 
                    const VkExtent2D& overlaysize, double dt);

  }} // rekz onscreen


#endif
