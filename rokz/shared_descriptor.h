
#ifndef ROKZ_SHARED_DESCRIPTOR_INCLUDE
#define ROKZ_SHARED_DESCRIPTOR_INCLUDE

#include "common.h"
#include "shared_types.h"
#include "rokz_types.h"


namespace rokz { namespace global_ub {

    enum BindingEnum {
      MVP_Scene   = 0,
      MVP_Overlay = 1, 
      GridState   = 2,
      TextItems   = 3,
      BindingEnumMax = 16, 
    };       
    


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

    // ----------------------------------------------------------------------------------------------
    //                                    
    // ----------------------------------------------------------------------------------------------
    struct TextItem {
      enum { max_length = 64 }; 
      uint32 ch[max_length];
    }; 

    
    // for use with ut::offset_at
    const Vec<size_t> UB_sizes = {
      sizeof (MVPTransform), // MVP_Scene  
      sizeof (MVPTransform), // MVP_Overlay
      sizeof (GridState),    // GridState  
      sizeof (std::array<TextItem, TextItem::max_length>),
    }; 

    
  }}


#endif
