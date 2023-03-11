#ifndef REKZ_WINDOW_HANDLER_INCLUDE
#define REKZ_WINDOW_HANDLER_INCLUDE

#include "common.h"
#include "input_control.h"

namespace rokz {

  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  namespace win_event { 
    void on_resize (GLFWwindow* window, int width, int height); 
    void on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods); 
    void on_mouse_enter (GLFWwindow* window, int entered);
    void on_mouse_move (GLFWwindow* window, double xpos, double ypos); 
    void on_mouse_button (GLFWwindow* window, int button, int action, int mods); 
  }


}
#endif
