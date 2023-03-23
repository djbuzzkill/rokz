
#ifndef ROKZ_RC_TYPES_INCLUDE
#define ROKZ_RC_TYPES_INCLUDE


#include "common.h"
#include "shared_types.h"

namespace rokz { namespace rc {

    // -----------------------------------------------------------------------------------------
    struct Sampler : public deviceob<VkSampler, Sampler> {

      Sampler (const Device& d): deviceob (d) { }

      virtual ~Sampler (); 
    };
    //
    
    Sampler::Ref CreateSampler_default (const Device& device); 
  }
}
#endif
