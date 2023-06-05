

  

#include "rekz.h"              // 

using namespace rokz;
//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
#include "milkshake.h"
//#include "okui.h"

//
int texture_tool (const Vec<std::string>& args); 
int test_ouput (const Vec<std::string>& args); 
int test_time ();

int darkrootbasin (const Vec<std::string>& args);
int run_marz (const Vec<std::string>& args);
int tile_tool (const Vec<std::string>& args); 
int font_tool (const Vec<std::string>& args); 
int stonk (const Vec<std::string>& args); 

// -------------------------------------------------------------------------------------------
int main (int argv, char** argc) {

  const Vec<std::string> args (argc, argc + argv);

  typedef std::function<int(const Vec<std::string>& args)> exe_fn;

  std::map<std::string, exe_fn> exemap = {
    {"texture_tool" , texture_tool},
    {"test_ouput"   , test_ouput },
    {"darkrootbasin", darkrootbasin },
    {"marz"         , run_marz },
    {"tilez"        , tile_tool},
    {"fontz"        , font_tool },
    {"stonk"        , stonk },
    {"milkshake"    , milkshake::run } , 
    //  {"okui"         , okui::run } , 

  };

  int res = __LINE__;

  if (args.size () > 1 && exemap.count (args[1])) {
    res = exemap[ args[1] ](args);
  }
  else {
    res = darkrootbasin (args);
  }
  
  printf ("lolz bai %s\n", __FUNCTION__); 
  return res; 
}
