

#ifndef REKZ_GLOBAL_DESCRIPTOR
#define REKZ_GLOBAL_DESCRIPTOR

#include "rokz/rokz.h"

namespace rekz {

  using namespace rokz;


  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  struct MVPTransform {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  //sizeof (rokz::MVPTransform);

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const Vec<VkDescriptorSetLayoutBinding> kGlobalDescriptorBindings;

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool SetupGlobalUniforms (Vec<Buffer>& uniform_buffs, uint32_t num_sets, const Device& device);
  bool BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<Buffer>& buffs, const Device& device);
  void UpdateGlobals (DrawSequence::Globals& shared, const Buffer& buf, const VkExtent2D& viewext, double dt);

}


#endif
