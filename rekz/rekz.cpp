
#include "rekz.h"              // 
//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"



int test_rokz (const std::vector<std::string>& args); 
int texture_tool (const std::vector<std::string>& args); 
int test_time (); 
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int main (int argv, char** argc) {

  const std::vector<std::string> args (argc, argc + argv);

  //   test_time (); 
  test_rokz (args); 
  //texture_tool (args); 

  printf ("lolz bai %s\n", __FUNCTION__); 
  return 0; 
}
