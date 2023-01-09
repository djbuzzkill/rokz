
#include "rekz.h"              // 
//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"



int test_rokz (const std::vector<std::string>& args); 
int test_rokz_hpp (const std::vector<std::string>& args); 
int texture_tool (const std::vector<std::string>& args); 
int test_ouput (const std::vector<std::string>& args); 
int test_time (); 
int darkroot_basin (const std::vector<std::string>& args);
int mars_prelim (const std::vector<std::string>& args);

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int main (int argv, char** argc) {

  const std::vector<std::string> args (argc, argc + argv);

  //   test_time (); 
  //test_rokz (args); 
  //darkroot_basin  (args);
  mars_prelim  (args);
  // test_rokz_hpp (args); 
  // texture_tool (args); 
  //test_ouput (args);
  
  printf ("lolz bai %s\n", __FUNCTION__); 
  return 0; 
}
