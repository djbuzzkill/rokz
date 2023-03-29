
#include "rekz.h"
#include "rokz/imagebuff.h"


using namespace rokz; 



int stonk (const Vec<std::string>& args) {

  printf ( "lol --> %s\n", __FUNCTION__); 

  std::fmod (6000.0441f, 31.0000000617);


  rokz::imagebuff<float> h39 (1024, 1024);
  rokz::imagebuff<float> h49 (1024, 1024);

  std::string fname39 = "/home/djbuzzkill/owenslake/tmp/testmarz/marz-RED-DEM_x3z9.bin";
  std::string fname49 = "/home/djbuzzkill/owenslake/tmp/testmarz/marz-RED-DEM_x4z9.bin";
  
  rokz::load_from_file ( h39 , fname39 );
  rokz::load_from_file ( h49 , fname49 );

  if (h39.numpix () && h49.numpix()) {

    for (int i = 0; i < 1024; ++i) {
      printf (" h39[1023,%i] = %f | h49[0,%i] = %f\n", i, h39.el(1023, i), i, h49.el(0, i)); 
    } 

  }
  
  return 0; 

}
