



#include "input_state.h"
#include "shared_types.h"
 #include "utility.h"
#include <glm/gtc/random.hpp>


using namespace rokz;

// 
void rokz::UpdateViewAttitude (glm::vec3& viewrot, glm::ivec2& mouse_prev, int& previnside, const InputState& input_state, float turnrate) {

  //const float turnrate = 0.02f;

  if (input_state.mouse.inside && input_state.mouse.left_butt) {

    int dx = input_state.mouse.x_pos - mouse_prev.x; 
    int dy = input_state.mouse.y_pos - mouse_prev.y; 

    printf ("<dx:%i, dy:%i>\n", dx, dy); 

    mouse_prev.x = input_state.mouse.x_pos; 
    mouse_prev.y = input_state.mouse.y_pos; 
    // glob.shared.view_rot.x += turnrate;
    // glob.shared.view_rot.y += turnrate;
    previnside = input_state.mouse.inside; 

    // printf (" [%i]--> S(%i, %i) | dS(%i, %i)\n", __LINE__,
    //         glob.input_state.mouse.x_pos,
    //         glob.input_state.mouse.y_pos,
    //         dx, dy); 
    const float half_pi = 0.5f * kPi; 

    viewrot.x += -dy * turnrate;
    viewrot.y += -dx * turnrate;

    
    if (viewrot.x > half_pi) 
      viewrot.x = half_pi;

    if (viewrot.x < -half_pi) 
      viewrot.x = -half_pi;
  }
  else {
    mouse_prev.x = input_state.mouse.x_pos;
    mouse_prev.y = input_state.mouse.y_pos;  
  }
  
}


glm::vec3 spherical_dir (float theta, float phi) {

  printf ("<theta:%f, phi:%f>\n", theta, phi); 
  return glm::vec3 ( std::cos (phi) * -std::sin (theta),
                     std::sin (phi),
                     std::cos (phi) * -std::cos (theta)); 
}
  


void rokz::UpdateViewPosition (glm::vec3& viewpos,
                               const glm::vec3& viewrot,
                               const rokz::InputState& input_state, float move_rate) {

  glm::vec3 lookdir = spherical_dir ( viewrot.y, viewrot.x); 
  
  //const float move_rate = 0.05f;
  
    if (input_state.keys.count (GLFW_KEY_F)) {
      // fwd
      printf ("spherical<%f, %f, %f>\n", lookdir.x, lookdir.y, lookdir.z); 
      viewpos += move_rate * lookdir;

      //viewpos.z -= move_rate;
      //printf ("--> [f] z:%f \n", viewpos.z);
    }    
    if (input_state.keys.count (GLFW_KEY_V)) {
      // ?? recedes
      printf ("spherical<%f, %f, %f>\n", lookdir.x, lookdir.y, lookdir.z); 
      viewpos -= move_rate * lookdir;

      //viewpos.z += move_rate;
      //printf ("--> [v] z:%f \n", glob.shared.view_pos.z);
    }    

    if (input_state.keys.count (GLFW_KEY_D)) {
      //viewpos.x -= move_rate;
      //printf ("--> [d] x:%f \n", glob.shared.view_pos.x);
    }    
    if (input_state.keys.count (GLFW_KEY_G)) {
      //viewpos.x += move_rate;
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


void rokz::UpdateViewPosition (glm::vec3& viewpos, const rokz::InputState& input_state, float move_rate) {
  //const float move_rate = 0.05f;
  
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


