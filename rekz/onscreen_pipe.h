
#ifndef ONSCREEN_PIPELINE_INLUCDE
#define ONSCREEN_PIPELINE_INCLUDE


#include "rekz.h"
#include "rokz/vert_defs.h"


namespace rekz {

  namespace onscreen {

    using namespace rokz;
    
    typedef rokz::PTx_Vert Vert;
    // ----------------------------------------------------------------------------------------------
    const size_t                                         kMaxCount = 128; // y?
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;   
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc; 
    extern const DescriptorSetLayoutBindings             kDescriptorBindings;

    // ----------------------------------------------------------------------------------------------
    struct PushConstant {

      uint32 resource_id;
      uint32 _unused_1;
      uint32 _unused_2;
      uint32 _unused_3;

      glm::vec4 color;
      glm::vec2 advance;
      glm::vec2 position;
    };


    // 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234  
    struct UBText {
      uint32 text[64];
    };  // elem[kMaxCount] 
  
  }
  

  
}

#endif
