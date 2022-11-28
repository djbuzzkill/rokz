
#include "rockz.h"              // 
#include "rokz/rokz.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "rokz/defaults.h"
#include "rokz/rokz_funcs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int main (int argv, char** argc) {

  const std::vector<std::string> args (argc, argc + argv);

  rokz_test_create (args); 
  //texture_tool (args); 

  printf ("lolz bai %s\n", __FUNCTION__); 
  return 0; 
}
  
