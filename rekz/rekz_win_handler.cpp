
#include "dark_types.h"
#include "darkrootgarden.h"
#include <GLFW/glfw3.h>






// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void darkroot::win_event::on_mouse_enter (GLFWwindow* window, int entered) {

  if (darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer (window))) {

    InputState& input_state = g->input_state;
    
    if (entered) {

      input_state.mouse.inside = 1;
      printf ("%s MOUSE ENTER\n", __FUNCTION__); 
      // The cursor entered the content area of the window
    }
    else {
      input_state.mouse.inside = 0;
      printf ("%s MOUSE EXIT\n", __FUNCTION__);

      input_state.mouse.x_pos = -1;
      input_state.mouse.y_pos = -1;
      // The cursor left the content area of the window
    }
  }
  // darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer (window)); 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void darkroot::win_event::on_resize (GLFWwindow* window, int width, int height) {
  // printf ("%s \n", __FUNCTION__); 
  if (darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer (window))) {

    //reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer(window))->fb_resize = true;
    g->fb_resize = true;

  }

}

// --------------------------------------------------------------------
// KEY PRESS
// --------------------------------------------------------------------
void darkroot::win_event::on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods) {

  if (darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer (window))) {

    InputState& input_state = g->input_state;
    switch (action) {

      // GLFW_PRESS
    case GLFW_PRESS:
      input_state.keys[key] = 1; 
      break;
      
      // GLFW_RELEASE
    case GLFW_RELEASE:
      input_state.keys.erase (key); 
      break;

      // GLFW_REPEAT
    case GLFW_REPEAT:
      if (input_state.keys.count (key)) 
        input_state.keys[key]++;
      else
        input_state.keys[key] = 1;
      break;
    }

  }
}

// --------------------------------------------------------------------
// MOUSE MOVEMENT
// --------------------------------------------------------------------
void darkroot::win_event::on_mouse_move (GLFWwindow* window, double xpos, double ypos) {
  //printf ("%s [xpos:%f | ypos:%f]\n" ,__FUNCTION__,  xpos, ypos); 
  if (darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer (window))) {

    InputState& input_state = g->input_state;

    if (input_state.mouse.inside) {

      int ipos_x = static_cast<int> (xpos);
      int ipos_y = static_cast<int> (ypos);
      
      if (input_state.mouse.x_pos == -1 || input_state.mouse.y_pos == -1) {
        input_state.mouse.x_pos = ipos_x ;
        input_state.mouse.y_pos = ipos_y ;

        input_state.mouse.dx = 0;
        input_state.mouse.dy = 0;
        
      }
      else {

        input_state.mouse.dx = ipos_x - input_state.mouse.x_pos;
        input_state.mouse.dy = ipos_y - input_state.mouse.y_pos;

        input_state.mouse.x_pos = ipos_x ;
        input_state.mouse.y_pos = ipos_y ;

      }
      // printf ("%s [dx:%i | dy:%i]\n" ,__FUNCTION__, input_state.mouse.dx, input_state.mouse.dy); 
    }
  }  
}

// --------------------------------------------------------------------
// MOUSE BUTTON
// --------------------------------------------------------------------
void darkroot::win_event::on_mouse_button (GLFWwindow* window, int button, int action, int mods)
{
  if (darkroot::Glob* g = reinterpret_cast<darkroot::Glob*> (glfwGetWindowUserPointer (window))) {

    InputState& input_state = g->input_state;
    switch (action) {

      // GLFW_PRESS
    case GLFW_PRESS:
      if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      }
      else if (button == GLFW_MOUSE_BUTTON_LEFT) {
      }
      else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
      }
          
    break;
      
      // GLFW_RELEASE
    case GLFW_RELEASE:
      if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      }
      else if (button == GLFW_MOUSE_BUTTON_LEFT) {
      }
      else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
      }
      break;

      // GLFW_REPEAT
    case GLFW_REPEAT:
      if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      }
      else if (button == GLFW_MOUSE_BUTTON_LEFT) {
      }
      else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
      }
      break;
    }

  }  
}







  

  


  
// std::pair<int, int> glfw_keys[] = {

//   { GLFW_KEY_UNKNOWN      , ROKZ_KEY_UNKNOWN      },   
//   { GLFW_KEY_SPACE        , ROKZ_KEY_SPACE        },   
//   { GLFW_KEY_APOSTROPHE   , ROKZ_KEY_APOSTROPHE   },   /* ' */
//   { GLFW_KEY_COMMA        , ROKZ_KEY_COMMA        },   /* , */
//   { GLFW_KEY_MINUS        , ROKZ_KEY_MINUS        },   /* - */
//   { GLFW_KEY_PERIOD       , ROKZ_KEY_PERIOD       },   /* . */
//   { GLFW_KEY_SLASH        , ROKZ_KEY_SLASH        },   /* / */
//   { GLFW_KEY_0            , ROKZ_KEY_0            },  
//   { GLFW_KEY_1            , ROKZ_KEY_1            },  
//   { GLFW_KEY_2            , ROKZ_KEY_2            },  
//   { GLFW_KEY_3            , ROKZ_KEY_3            },  
//   { GLFW_KEY_4            , ROKZ_KEY_4            },  
//   { GLFW_KEY_5            , ROKZ_KEY_5            },  
//   { GLFW_KEY_6            , ROKZ_KEY_6            },  
//   { GLFW_KEY_7            , ROKZ_KEY_7            },  
//   { GLFW_KEY_8            , ROKZ_KEY_8            },  
//   { GLFW_KEY_9            , ROKZ_KEY_9            },  
//   { GLFW_KEY_SEMICOLON    , ROKZ_KEY_SEMICOLON    },   /* ; */
//   { GLFW_KEY_EQUAL        , ROKZ_KEY_EQUAL        },   /* = */
//   { GLFW_KEY_A            , ROKZ_KEY_A            },  
//   { GLFW_KEY_B            , ROKZ_KEY_B            },  
//   { GLFW_KEY_C            , ROKZ_KEY_C            },  
//   { GLFW_KEY_D            , ROKZ_KEY_D            },  
//   { GLFW_KEY_E            , ROKZ_KEY_E            },  
//   { GLFW_KEY_F            , ROKZ_KEY_F            },  
//   { GLFW_KEY_G            , ROKZ_KEY_G            },  
//   { GLFW_KEY_H            , ROKZ_KEY_H            },  
//   { GLFW_KEY_I            , ROKZ_KEY_I            },  
//   { GLFW_KEY_J            , ROKZ_KEY_J            },  
//   { GLFW_KEY_K            , ROKZ_KEY_K            },  
//   { GLFW_KEY_L            , ROKZ_KEY_L            },  
//   { GLFW_KEY_M            , ROKZ_KEY_M            },  
//   { GLFW_KEY_N            , ROKZ_KEY_N            },  
//   { GLFW_KEY_O            , ROKZ_KEY_O            },  
//   { GLFW_KEY_P            , ROKZ_KEY_P            },  
//   { GLFW_KEY_Q            , ROKZ_KEY_Q            },  
//   { GLFW_KEY_R            , ROKZ_KEY_R            },  
//   { GLFW_KEY_S            , ROKZ_KEY_S            },  
//   { GLFW_KEY_T            , ROKZ_KEY_T            },  
//   { GLFW_KEY_U            , ROKZ_KEY_U            },  
//   { GLFW_KEY_V            , ROKZ_KEY_V            },  
//   { GLFW_KEY_W            , ROKZ_KEY_W            },  
//   { GLFW_KEY_X            , ROKZ_KEY_X            },  
//   { GLFW_KEY_Y            , ROKZ_KEY_Y            },  
//   { GLFW_KEY_Z            , ROKZ_KEY_Z            },  
//   { GLFW_KEY_LEFT_BRACKET , ROKZ_KEY_LEFT_BRACKET },   /* [ */
//   { GLFW_KEY_BACKSLASH    , ROKZ_KEY_BACKSLASH    },   /* \ */
//   { GLFW_KEY_RIGHT_BRACKET, ROKZ_KEY_RIGHT_BRACKET},   /* ] */
//   { GLFW_KEY_GRAVE_ACCENT , ROKZ_KEY_GRAVE_ACCENT },   /* ` */
//   { GLFW_KEY_WORLD_1      , ROKZ_KEY_WORLD_1      },   /* non-US #1 */
//   { GLFW_KEY_WORLD_2      , ROKZ_KEY_WORLD_2      },   /* non-US #2 */
//   { GLFW_KEY_ESCAPE       , ROKZ_KEY_ESCAPE       },  
//   { GLFW_KEY_ENTER        , ROKZ_KEY_ENTER        },  
//   { GLFW_KEY_TAB          , ROKZ_KEY_TAB          },  
//   { GLFW_KEY_BACKSPACE    , ROKZ_KEY_BACKSPACE    },  
//   { GLFW_KEY_INSERT       , ROKZ_KEY_INSERT       },  
//   { GLFW_KEY_DELETE       , ROKZ_KEY_DELETE       },  
//   { GLFW_KEY_RIGHT        , ROKZ_KEY_RIGHT        },  
//   { GLFW_KEY_LEFT         , ROKZ_KEY_LEFT         },  
//   { GLFW_KEY_DOWN         , ROKZ_KEY_DOWN         },  
//   { GLFW_KEY_UP           , ROKZ_KEY_UP           },  
//   { GLFW_KEY_PAGE_UP      , ROKZ_KEY_PAGE_UP      },  
//   { GLFW_KEY_PAGE_DOWN    , ROKZ_KEY_PAGE_DOWN    },  
//   { GLFW_KEY_HOME         , ROKZ_KEY_HOME         },  
//   { GLFW_KEY_END          , ROKZ_KEY_END          },  
//   { GLFW_KEY_CAPS_LOCK    , ROKZ_KEY_CAPS_LOCK    },  
//   { GLFW_KEY_SCROLL_LOCK  , ROKZ_KEY_SCROLL_LOCK  },  
//   { GLFW_KEY_NUM_LOCK     , ROKZ_KEY_NUM_LOCK     },  
//   { GLFW_KEY_PRINT_SCREEN , ROKZ_KEY_PRINT_SCREEN },  
//   { GLFW_KEY_PAUSE        , ROKZ_KEY_PAUSE        },  
//   { GLFW_KEY_F1           , ROKZ_KEY_F1           },  
//   { GLFW_KEY_F2           , ROKZ_KEY_F2           },  
//   { GLFW_KEY_F3           , ROKZ_KEY_F3           },  
//   { GLFW_KEY_F4           , ROKZ_KEY_F4           },  
//   { GLFW_KEY_F5           , ROKZ_KEY_F5           },  
//   { GLFW_KEY_F6           , ROKZ_KEY_F6           },  
//   { GLFW_KEY_F7           , ROKZ_KEY_F7           },  
//   { GLFW_KEY_F8           , ROKZ_KEY_F8           },  
//   { GLFW_KEY_F9           , ROKZ_KEY_F9           },  
//   { GLFW_KEY_F10          , ROKZ_KEY_F10          },  
//   { GLFW_KEY_F11          , ROKZ_KEY_F11          },  
//   { GLFW_KEY_F12          , ROKZ_KEY_F12          },  
//   { GLFW_KEY_F13          , ROKZ_KEY_F13          },  
//   { GLFW_KEY_F14          , ROKZ_KEY_F14          },  
//   { GLFW_KEY_F15          , ROKZ_KEY_F15          },  
//   { GLFW_KEY_F16          , ROKZ_KEY_F16          },  
//   { GLFW_KEY_F17          , ROKZ_KEY_F17          },  
//   { GLFW_KEY_F18          , ROKZ_KEY_F18          },  
//   { GLFW_KEY_F19          , ROKZ_KEY_F19          },  
//   { GLFW_KEY_F20          , ROKZ_KEY_F20          },  
//   { GLFW_KEY_F21          , ROKZ_KEY_F21          },  
//   { GLFW_KEY_F22          , ROKZ_KEY_F22          },  
//   { GLFW_KEY_F23          , ROKZ_KEY_F23          },  
//   { GLFW_KEY_F24          , ROKZ_KEY_F24          },  
//   { GLFW_KEY_F25          , ROKZ_KEY_F25          },  
//   { GLFW_KEY_KP_0         , ROKZ_KEY_KP_0         },  
//   { GLFW_KEY_KP_1         , ROKZ_KEY_KP_1         },  
//   { GLFW_KEY_KP_2         , ROKZ_KEY_KP_2         },  
//   { GLFW_KEY_KP_3         , ROKZ_KEY_KP_3         },  
//   { GLFW_KEY_KP_4         , ROKZ_KEY_KP_4         },  
//   { GLFW_KEY_KP_5         , ROKZ_KEY_KP_5         },  
//   { GLFW_KEY_KP_6         , ROKZ_KEY_KP_6         },  
//   { GLFW_KEY_KP_7         , ROKZ_KEY_KP_7         },  
//   { GLFW_KEY_KP_8         , ROKZ_KEY_KP_8         },  
//   { GLFW_KEY_KP_9         , ROKZ_KEY_KP_9         },  
//   { GLFW_KEY_KP_DECIMAL   , ROKZ_KEY_KP_DECIMAL   },  
//   { GLFW_KEY_KP_DIVIDE    , ROKZ_KEY_KP_DIVIDE    },  
//   { GLFW_KEY_KP_MULTIPLY  , ROKZ_KEY_KP_MULTIPLY  },  
//   { GLFW_KEY_KP_SUBTRACT  , ROKZ_KEY_KP_SUBTRACT  },  
//   { GLFW_KEY_KP_ADD       , ROKZ_KEY_KP_ADD       },  
//   { GLFW_KEY_KP_ENTER     , ROKZ_KEY_KP_ENTER     },  
//   { GLFW_KEY_KP_EQUAL     , ROKZ_KEY_KP_EQUAL     },  
//   { GLFW_KEY_LEFT_SHIFT   , ROKZ_KEY_LEFT_SHIFT   },  
//   { GLFW_KEY_LEFT_CONTROL , ROKZ_KEY_LEFT_CONTROL },  
//   { GLFW_KEY_LEFT_ALT     , ROKZ_KEY_LEFT_ALT     },  
//   { GLFW_KEY_LEFT_SUPER   , ROKZ_KEY_LEFT_SUPER   },  
//   { GLFW_KEY_RIGHT_SHIFT  , ROKZ_KEY_RIGHT_SHIFT  },  
//   { GLFW_KEY_RIGHT_CONTROL, ROKZ_KEY_RIGHT_CONTROL},  
//   { GLFW_KEY_RIGHT_ALT    , ROKZ_KEY_RIGHT_ALT    },  
//   { GLFW_KEY_RIGHT_SUPER  , ROKZ_KEY_RIGHT_SUPER  },  
//   { GLFW_KEY_MENU         , ROKZ_KEY_MENU         },  
// };
