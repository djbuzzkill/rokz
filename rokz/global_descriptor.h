

#ifndef REKZ_GLOBAL_DESCRIPTOR
#define REKZ_GLOBAL_DESCRIPTOR

#include "rokz/rokz.h"

namespace rokz {

  using namespace rokz;

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const DescriptorSetLayoutBindings kGlobalDescriptorBindings;
  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  struct MVPTransform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct GridState {
    glm::vec4 xcolor;
    glm::vec4 ycolor;
    glm::vec4 zcolor;
  };
  //sizeof (rokz::MVPTransform);


  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool SetupGlobalUniforms (Vec<Buffer>& uniform_buffs, uint32_t num_sets, const Device& device);
  bool SetupGlobalUniforms (Vec<rc::Buffer::Ref>& uniform_buffs, uint32_t num_sets, const Device& device); 

  bool BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<Buffer>& buffs, const Device& device);
  bool BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rc::Buffer::Ref>& buffs, const rokz::Device& device);


  void UpdateGlobals (DrawSequence::Globals& shared, const Buffer& buf, const VkExtent2D& viewext, double dt);
  void UpdateGlobals (rokz::DrawSequence::Globals& shared, const rc::Buffer::Ref buf, const VkExtent2D& viewext, double dt);


  void CleanupGlobalUniforms (Vec<Buffer>& uniform_buffs, const Device& device);

}


#endif
