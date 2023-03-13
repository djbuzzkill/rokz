//
#ifndef REKZ_OBJ_PIPELINE
#define REKZ_OBJ_PIPELINE

#include "rekz.h"
#include "rokz/vert_defs.h"
//#include <vulkan/vulkan_core.h>

namespace rekz {

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  namespace polyobj {   

    typedef rokz::PNCTx_Vert                              kVert;
    // ----------------------------------------------------------------------------------------------
    const size_t                                         kMaxCount = rekz::kMaxObjectCount;
    // ----------------------------------------------------------------------------------------------
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;   // rokz::kPNCT_InputBindingDesc;
    // ----------------------------------------------------------------------------------------------
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc; // rokz::kPNCT_InputBindingAttributeDesc; 
  }

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct PolygonParam {
    glm::mat4 modelmat;
  };

  // ----------------------------------------------------------------------------------------------
  // this function is sus, it is setting up a global buffer
  // ----------------------------------------------------------------------------------------------
  bool SetupObjectUniforms (Vec<Buffer>& uniform_buffs, Vec<Buffer>& objparams,
                            uint32_t num_sets, const Device& device);

  // ----------------------------------------------------------------------------------------------
  // sorta does same thing as  SetupObjectPipeline
  // ----------------------------------------------------------------------------------------------
  bool InitObjPipeline     (Pipeline&                         pipeline,
                            PipelineLayout&                   plo,
                            const Vec<VkDescriptorSetLayout>& dslos,
                            //0
                            const std::filesystem::path&      fspath,
                            const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits             msaa_samples,
                            VkFormat                          color_format,
                            VkFormat                          depth_format,
                            const Device&                     device); 

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  bool BindObjectDescriptorSets (Vec<VkDescriptorSet>&    dss ,
                                 const Vec<Buffer>&       vma_uniform_buffs,
                                 const Vec<Buffer>&       vma_objparam_buffs,

                                 const ImageView&           texture_imageview, 
                                 const Sampler&             sampler, 
                                 const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                 const Device&              device);



  // ----------------------------------------------------------------------------------------------
  // these r the new ones
  // ----------------------------------------------------------------------------------------------
  bool BindObjectDescriptorResources (Vec<VkDescriptorSet>& dss ,
                                      const Vec<Buffer>&    objparam_bu,
                                      const ImageView&             texture_imageview, 
                                      const Sampler&               sampler, 
                                      const DescriptorSetLayout&   dslayout, //const rokz::DescriptorPool& descpool,
                                      const Device&                device);

  
  bool BindObjectDescriptorResources (VkDescriptorSet                  ds,
                                      Buffer&                    objparam_buff,
                                      const ImageView&           texture_imageview, 
                                      const Sampler&             sampler, 
                                      const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                      const Device&              device) ; 

  //
  // 
  bool SetupObjectUniforms (Vec<Buffer>& objparams, uint32_t num_sets, const Device& device);
  // 

}



#endif
