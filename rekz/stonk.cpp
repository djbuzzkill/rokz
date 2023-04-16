
#include "rekz.h"
#include "font_tool.h"
#include "rokz/imagebuff.hpp"
#include <tinygltf/tiny_gltf.h>


using namespace rokz; 


auto wtf_weird (int i) -> std::tuple<bool, float> {

  if ( i % 2 ) 
    return std::make_tuple (true, 69.0f);
  else
    return std::make_tuple (false, 0.0f); 
  
}
  

// ---------  -----------
int stonk (const Vec<std::string>& args) {

  tinygltf::Mesh mesh; 

  
   auto [a, b] = wtf_weird (69);

   if (a) 
     printf ( "lol --> %f\n", b); 

  std::fmod (6000.0441f, 31.0000000617);
  std::string glyphfilename = rekz::fonttool::font_glyph_filename (66);
  rokz::systempath basepath = "/home/djbuzzkill/owenslake/tmp/textbin/"; 

  if (std::filesystem::exists (basepath/glyphfilename)) {
    printf ( "%s exists \n", glyphfilename.c_str()); 

    rokz::imagebuff<float> testfile (64,64);

    std::string fqname = basepath/glyphfilename; 
    
    rekz::load_from_file (testfile, fqname); 

    printf ("num bytes %zu \n ", testfile.numbytes ()); 

    size_t    colcounter =0; 
    
    for (size_t ipix = 0; ipix < testfile.numpix (); ipix++) {
      printf ("ipix[%zu]: %f \n", ipix, testfile.dat[ipix] ); 
    }

    printf ("bytes over 0.9 %zu \n ", colcounter); 
  
  }
  
  printf ("ROKZ_OWENS_LAKE:%s\n", OWENS_LAKE); 

  // rokz::imagebuff<float> h39 (1024, 1024);
  // rokz::imagebuff<float> h49 (1024, 1024);

  // std::string fname39 = "/home/djbuzzkill/owenslake/tmp/testmarz/marz-RED-DEM_x3z9.bin";
  // std::string fname49 = "/home/djbuzzkill/owenslake/tmp/testmarz/marz-RED-DEM_x4z9.bin";
  
  // rokz::load_from_file ( h39 , fname39 );
  // rokz::load_from_file ( h49 , fname49 );

  // if (h39.numpix () && h49.numpix()) {

  //   for (int i = 0; i < 1024; ++i) {
  //     printf (" h39[1023,%i] = %f | h49[0,%i] = %f\n", i, h39.el(1023, i), i, h49.el(0, i)); 
  //   } 

  // }
  
  return 0; 

}
