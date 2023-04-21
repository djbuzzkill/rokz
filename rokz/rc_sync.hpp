
#ifndef ROKZ_RC_SYNCHRONIZATION_INCLUDE
#define ROKZ_RC_SYNCHRONIZATION_INCLUDE

#include "shared_types.hpp"
#include <vulkan/vulkan_core.h>



namespace rokz { namespace rc {
    // -----------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------

    struct Semaphore : public deviceob<VkSemaphore, Semaphore> {

      Semaphore (const Device& dev): deviceob(dev) {

      }


      virtual ~Semaphore () { }

    }; 

    // -------------------------------------------------------------
    Semaphore::Ref CreateSemaphore (const Device& dev); 

    // -----------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------
    struct Fence : public deviceob<VkFence, Fence> {

      Fence (const Device& dev) : deviceob (dev) {
      }

      
    };
    
    Fence::Ref CreateFence (VkFenceCreateFlagBits flags, const Device& device);
    

    
  }}
#endif

