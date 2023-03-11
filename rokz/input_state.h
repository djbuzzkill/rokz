
#ifndef ROKZ_INPUT_CONTROL_INCLUDE
#define ROKZ_INPUT_CONTROL_INCLUDE


#include "common.h"

namespace rokz { 

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct MouseState {

    MouseState () : inside (0), left_butt(0), right_butt(0), middle_butt(0), x_pos (0), y_pos(0) {
    }

    int  inside; 
    int  left_butt;   
    int  right_butt;  
    int  middle_butt; 
    int  x_pos;
    int  y_pos;
  }; 

  // --------------------------------------------------------------------
  // UserPointer data
  // --------------------------------------------------------------------
  struct InputState { 

    InputState () : keys (), mouse (), fb_resize (false) {
    }
    
    std::map<int, int> keys;
    MouseState         mouse;
    bool               fb_resize; 
  };


  void UpdateViewPosition (glm::vec3& viewpos, const InputState& input_state) ; 
  void UpdateViewAttitude (glm::vec3& viewrot, glm::ivec2& mouse_prev, int& previnside, const InputState& input_state) ;
}
#endif
