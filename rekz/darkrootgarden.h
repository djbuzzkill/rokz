


#ifndef DARKROOT_GARDEN_INCLUDE
#define DARKROOT_GARDEN_INCLUDE

#include "rokz/common.h"


#include "dark_types.h"


namespace darkroot {

  const float kPi = glm::pi<float> ();  



  //
  struct SceneObjParam {
    glm::mat4 modelmat;
    //  glm::mat4 unused0;
  };

  //
  const size_t SizeOf_SceneObjParam = sizeof (SceneObjParam); 


  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  const DarkrootMesh& DarkOctohedron (); 




  void window_event_cursor_enter (GLFWwindow* window, int entered);
  void window_event_on_resize (GLFWwindow* window, int width, int height); 
  void window_event_on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods); 
  void window_event_on_mouse_move (GLFWwindow* window, double xpos, double ypos); 
  void window_event_on_mouse_button (GLFWwindow* window, int button, int action, int mods); 


  
}
#endif
