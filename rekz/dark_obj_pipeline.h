//
#ifndef DARK_OBJ_PIPELINE
#define DARK_OBJ_PIPELINE

#include "rekz.h"
#include <vulkan/vulkan_core.h>

namespace rekz {

  constexpr size_t kMaxObjectCount = 64;

  typedef rekz::Vertex_pos_nrm_txc_col PolyObjVert;
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const Vec<VkDescriptorSetLayoutBinding>      kObjDescriptorBindings;
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const VkVertexInputBindingDescription        kVertexInputBindingDesc;
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const Vec<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;
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
