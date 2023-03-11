



#include "input_state.h"




// 
void rokz::UpdateViewAttitude (glm::vec3& viewrot, glm::ivec2& mouse_prev, int& previnside, const InputState& input_state) {

  const float turnrate = 0.02f;
  
  if (input_state.mouse.inside) {
    if (!previnside) {
      mouse_prev.x = input_state.mouse.x_pos; 
      mouse_prev.y = input_state.mouse.y_pos; 
    }

    int dx = input_state.mouse.x_pos - mouse_prev.x; 
    int dy = input_state.mouse.y_pos - mouse_prev.y; 
        
    mouse_prev.x = input_state.mouse.x_pos; 
    mouse_prev.y = input_state.mouse.y_pos; 
    // glob.shared.view_rot.x += turnrate;
    // glob.shared.view_rot.y += turnrate;
    previnside = input_state.mouse.inside; 

    // printf (" [%i]--> S(%i, %i) | dS(%i, %i)\n", __LINE__,
    //         glob.input_state.mouse.x_pos,
    //         glob.input_state.mouse.y_pos,
    //         dx, dy); 

    viewrot.x += -dy * turnrate;
    viewrot.y += -dx * turnrate;
    viewrot.z = 0.0f;
  }
  
}

void rokz::UpdateViewPosition (glm::vec3& viewpos, const rokz::InputState& input_state) {

    const float move_rate = 0.05f;

    if (input_state.keys.count (GLFW_KEY_F)) {
      // fwd
      viewpos.z -= move_rate;
      //printf ("--> [f] z:%f \n", viewpos.z);
    }    
    if (input_state.keys.count (GLFW_KEY_V)) {
      // ?? recedes
      viewpos.z += move_rate;
      //printf ("--> [v] z:%f \n", glob.shared.view_pos.z);
    }    

    if (input_state.keys.count (GLFW_KEY_D)) {
      viewpos.x -= move_rate;
      //printf ("--> [d] x:%f \n", glob.shared.view_pos.x);
    }    
    if (input_state.keys.count (GLFW_KEY_G)) {
      viewpos.x += move_rate;
      //printf ("--> [g] x:%f \n", glob.shared.view_pos.x);
    }    

    if (input_state.keys.count (GLFW_KEY_J)) {
      // ??? climbs
      viewpos.y += move_rate;
      //printf ("--> [j] y:%f \n", glob.shared.view_pos.y);
    }    
    if (input_state.keys.count (GLFW_KEY_K)) {
      // appears to descend
      viewpos.y -= move_rate;
      //printf ("--> [k] y:%f \n", glob.shared.view_pos.y);
    }    

  } 

