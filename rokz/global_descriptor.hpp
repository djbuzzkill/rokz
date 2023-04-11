

#ifndef ROKZ_GLOBAL_DESCRIPTOR
#define ROKZ_GLOBAL_DESCRIPTOR

#include "common.hpp"
#include "shared_types.hpp"
#include "rokz_types.hpp"
#include "rc_buffer.hpp"
#include "draw_sequence.hpp"
#include "descriptor.hpp"



namespace rokz {

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const DescriptorSetLayoutBindings kGlobalDescriptorBindings;

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool SetupGlobalUniforms (Vec<rc::Buffer::Ref>& uniform_buffs, uint32_t num_sets, const Device& device); 

  //bool BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<Buffer>& buffs, const Device& device);


  // ----------------------------------------------------------------------------------------------
  //
  // ----------------------------------------------------------------------------------------------
  bool BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rc::Buffer::Ref>& buffs, const rokz::Device& device);

  void UpdateGlobals (rokz::DrawSequence::Globals& shared, const rc::Buffer::Ref buf, const VkExtent2D& viewext, double dt);


  void CleanupGlobalUniforms (Vec<Buffer>& uniform_buffs, const Device& device);

}


#endif
