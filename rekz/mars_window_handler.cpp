


#include "marscape.h"

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
void mars::window_handler::on_resize (GLFWwindow* window, int width, int height) {

  reinterpret_cast<mars::Glob*> (glfwGetWindowUserPointer(window))->fb_resize = true;


}

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
void mars::window_handler::on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
      printf ("%s [E] key press\n", __FUNCTION__); 
    }      
    else if (action == GLFW_PRESS) {
      printf ("%s other  key press\n", __FUNCTION__); 

    }      


}

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
void mars::window_handler::on_mouse_enter (GLFWwindow* window, int entered){
  if (entered) {
    // The cursor entered the content area of the window
  }
  else {
    // The cursor left the content area of the window
  }
}

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
void mars::window_handler::on_mouse_move (GLFWwindow* window, double xpos, double ypos) {

  printf ("%s [xpos:%f | ypos:%f]\n" ,__FUNCTION__,  xpos, ypos); 

}

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
void mars::window_handler::on_mouse_button (GLFWwindow* window, int button, int action, int mods) {

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    printf ("%s RIGHT MOUSE BUTTON\n", __FUNCTION__); 
    //     popup_menu();
  }

  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

    printf ("%s LEFT MOUSE BUTTON\n", __FUNCTION__); 
    //     popup_menu();
  }
  

}

