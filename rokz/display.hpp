
#ifndef ROKZ_DISPLAY_INCLUDE
#define ROKZ_DISPLAY_INCLUDE

#include  "common.hpp"
#include  "rokz_types.hpp"


namespace rokz {

  struct InputState;
  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  struct Display {

    Display () : window(), surface(nullptr) {
    }

    Window       window;
    VkSurfaceKHR surface;                 
  };

  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  bool SetupDisplay (Display& display, InputState& input_state, const std::string& title,
                     const VkExtent2D& dim, const Instance& instance); 

}
#endif
