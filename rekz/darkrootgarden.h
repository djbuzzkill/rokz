


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

  const DarkrootMesh& DarkOctohedron (); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  namespace win_handler { 
    void on_resize (GLFWwindow* window, int width, int height); 
    void on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods); 
    void on_mouse_enter (GLFWwindow* window, int entered);
    void on_mouse_move (GLFWwindow* window, double xpos, double ypos); 
    void on_mouse_button (GLFWwindow* window, int button, int action, int mods); 
  }
  //
  HalfEdge::BRep& BuildBoundaryRep (HalfEdge::BRep& brep, const DarkrootMesh& geom); 
  
}
#endif
