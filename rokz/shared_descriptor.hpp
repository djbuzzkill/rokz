
#ifndef ROKZ_SHARED_DESCRIPTOR_INCLUDE
#define ROKZ_SHARED_DESCRIPTOR_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"
#include "rokz_types.hpp"


namespace rokz { namespace global_ub {

    //const uint32 kMaxTextElements = 128;

    enum BindingEnum {
      MVP_SCENE_BINDINGI   = 0,
      MVP_OVERLAY_BINDINGI = 1, 
      GRIDSTATE_BINDINGI   = 2,
      TEXTITEMS_BINDINGI   = 3,
      FONT_FACE_BINDINGI   = 8, 
      GLOBAL_BINDING_ENUM_MAX = 16, 
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
    // struct TextItem {
    //   enum { max_length = 64 }; 
    //   uint32 ch[max_length];
    // }; 
    const size_t text_item_length = 64;
    struct TextElement {

      TextElement (): text (global_ub::text_item_length, ' '), position (0.0), color (1.0){
      }

      std::string text;
      glm::vec4   position; 
      glm::vec4   color; 
    };
      

    
    const size_t sizeof_FONT_FACE_BINDINGI = 0; 
    const size_t sizeof_TEXTITEMS_BINDINGI = 0;
    // for use with ut::offset_at
    const Vec<size_t> UB_sizes = {
      sizeof (MVPTransform), // MVP_Scene  
      sizeof (MVPTransform), // MVP_Overlay
      sizeof (GridState),    // GridState  
      sizeof_TEXTITEMS_BINDINGI,
      sizeof_FONT_FACE_BINDINGI, 
    }; 
  }}


#endif
