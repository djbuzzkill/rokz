//
#ifndef DARK_OBJ_PIPELINE
#define DARK_OBJ_PIPELINE

#include "darkrootgarden.h"

namespace rekz {


  constexpr size_t kMaxObjectCount = 128;
  typedef rekz::Vertex_pos_nrm_txc_col PolyObjVert;
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkDescriptorSetLayoutBinding>      kObjDescriptorBindings;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const VkVertexInputBindingDescription                kVertexInputBindingDesc;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct PolygonParam {
    glm::mat4 modelmat;
  };

  // ----------------------------------------------------------------------------------------------
  // this function is sus, it is setting up a global buffer
  // ----------------------------------------------------------------------------------------------
  bool SetupObjectUniforms (std::vector<rokz::Buffer>& uniform_buffs, std::vector<rokz::Buffer>& objparams,
                            uint32_t num_sets, const rokz::Device& device);

  // ----------------------------------------------------------------------------------------------
  // sorta does same thing as  SetupObjectPipeline
  // ----------------------------------------------------------------------------------------------
  bool InitObjPipeline     (rokz::Pipeline&              pipeline,
                            rokz::PipelineLayout&        plo,
                            rokz::DescriptorSetLayout&   dslo,
                            //0
                            const std::filesystem::path& fspath,
                            const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits        msaa_samples,
                            VkFormat                     color_format,
                            VkFormat                     depth_format,
                            const rokz::Device&          device); 

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  bool BindObjectDescriptorSets (std::vector<VkDescriptorSet>&    dss ,
                                 const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                 const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                 const rokz::ImageView&           texture_imageview, 
                                 const rokz::Sampler&             sampler, 
                                 const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                 const rokz::Device&              device);
  // 
}



#endif
