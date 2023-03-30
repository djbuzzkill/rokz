
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

    
      
    struct PushConstant {};

    struct TextBuff {
      char text[128];
    }; 
    
    

  }
  

  
}

#endif
