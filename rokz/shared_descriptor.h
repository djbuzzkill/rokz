#ifndef ROKZ_SHARED_DESCRIPTOR_INCLUDE
#define ROKZ_SHARED_DESCRIPTOR_INCLUDE

#include "common.h"
#include "shared_types.h"


namespace rokz { namespace descriptor {
    // ----------------------------------------------------------------------------------------------
    //                                    
    // ----------------------------------------------------------------------------------------------
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
    
  }}


#endif
