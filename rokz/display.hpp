
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
  bool SetupDisplay (Display& display, const std::string& title , InputState& input_state, const VkExtent2D& dim, const Instance& instance); 

}
#endif
