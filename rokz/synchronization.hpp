
#ifndef ROKZ_SYNCHRONIZATION_INCLUDE
#define ROKZ_SYNCHRONIZATION_INCLUDE

#include "common.h"
#include "rokz_types.h"

namespace rokz { namespace cx { 

  bool CreateFrameSync (FrameSync& sync, FrameSyncCreateInfo& create_info, const VkDevice& device);
    // -----------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------
    void Destroy (rokz::FrameSync& sync, const rokz::Device& device); 
    
    // -----------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------
    void Destroy (Vec<rokz::FrameSync>& syncs, const rokz::Device& device); 

  }}

#endif
