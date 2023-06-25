//
#ifndef REKZ_OBJ_PIPELINE
#define REKZ_OBJ_PIPELINE

#include "rekz.h"

namespace rekz {

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  namespace obz {   

    const auto kParamsBindingI  = 18;
    const auto kTextureBindingI = 19;
    // ----------------------------------------------------------------------------------------------
    const size_t                                         kMaxCount = 128; // y?
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;   
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc; 
    extern const DescriptorSetLayoutBindings             kDescriptorBindings;
    // ----------------------------------------------------------------------------------------------
    struct PushConstant {
    
      uint32 resourceID;
      uint32 _unused_01;
      uint32 _unused_02;
      uint32 _unused_03;
    }; 

    const VkShaderStageFlags PCStages = VK_SHADER_STAGE_VERTEX_BIT
                                      | VK_SHADER_STAGE_FRAGMENT_BIT;
  } // obz

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct PolygonParam {
    glm::mat4 modelmat;
  };

  // ----------------------------------------------------------------------------------------------

  // -- only sets up the ob portion of descriptor resources
  bool SetupObjectUniforms (Vec<rc::Buffer::Ref>& uniform_buffs, uint32_t num_sets, const Device& device);
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool BindObjectDescriptorResources (Vec<VkDescriptorSet>&         dss ,
                                      const Vec<rc::Buffer::Ref>&   globalub,
                                      const Vec<rc::Buffer::Ref>&   objectub,
                                      const Vec<rc::ImageView::Ref> imageviews,  //const ImageView&           texture_imageview, 
                                      const rc::Sampler::Ref        sampler, 
                                      const DescriptorSetLayout&    dslayout, //const rokz::DescriptorPool& descpool,
                                      const Device&                 device); 


  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool InitObjPipeline     (Pipeline&                         pipeline,
                            PipelineLayout&                   plo,
                            VkRenderPass                      renderpass, 
                            const Vec<VkDescriptorSetLayout>& dslos,
                            const std::filesystem::path&      fspath,
                            const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits             msaa_samples,
                            VkFormat                          color_format,
                            VkFormat                          depth_format,
                            const Device&                     device); 
  

}



#endif
