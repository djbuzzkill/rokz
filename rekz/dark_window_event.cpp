
#include "dark_types.h"
#include "darkrootgarden.h"






void darkroot::window_event_cursor_enter (GLFWwindow* window, int entered) {
    if (entered) {
      // The cursor entered the content area of the window
    }
    else {
      // The cursor left the content area of the window
    }
  }

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
void darkroot::window_event_on_resize (GLFWwindow* window, int width, int height) {

  reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer(window))->fb_resize = true;

  }

  // --------------------------------------------------------------------
  // KEY PRESS
  // --------------------------------------------------------------------
void darkroot::window_event_on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
      printf ("%s [E] key press\n", __FUNCTION__); 
    }      
    else if (action == GLFW_PRESS) {
      printf ("%s other  key press\n", __FUNCTION__); 

    }      
  
  }

  // --------------------------------------------------------------------
  // MOUSE MOVEMENT
  // --------------------------------------------------------------------
void darkroot::window_event_on_mouse_move (GLFWwindow* window, double xpos, double ypos) {
    printf ("%s [xpos:%f | ypos:%f]\n" ,__FUNCTION__,  xpos, ypos); 
  
  }

  // --------------------------------------------------------------------
  // MOUSE BUTTON
  // --------------------------------------------------------------------
void darkroot::window_event_on_mouse_button (GLFWwindow* window, int button, int action, int mods)
  {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
      printf ("%s RIGHT MOUSE BUTTON\n", __FUNCTION__); 
      //     popup_menu();
    }

    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

      printf ("%s LEFT MOUSE BUTTON\n", __FUNCTION__); 
      //     popup_menu();
    }
  
  }

