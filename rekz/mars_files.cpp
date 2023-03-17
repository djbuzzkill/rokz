

#include "mars_files.h"

// #include "rekz.h"
// #include "image_loader.h"
// #include "imagebuff.h"
// #include "rokz/binary_IO.h"
// #include "rokz/file.h"

// #include <IL/il.h>
// #include <IL/ilu.h>

// #include <glm/fwd.hpp>
// #include <limits>


// using namespace rokz;


// ------------------------------------------------------------------------------------------
void ESP_018065_1975_RED_ESP_019133_1975_RED::print_attributes () 
{

  rekz::imagebuff<float> DRG ( kWIDTH, kHEIGHT ) ;

  const uint32 total_num_pixels = kWIDTH * kHEIGHT;

  std::string base_path = "/home/djbuzzkill/owenslake/data/Mars/"; 
  
  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM_f32x6900x17177.bin";
  std::string heightname = "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.6900x17177.hgt"; 

  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG_f32x6900x17177.bin";
  std::string colorname  = "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat";

  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM_dvec2x6900x17177.bin";
  std::string coordname  = "ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat";


  rekz::imagebuff<float> heightimage ( kWIDTH, kHEIGHT );
  
  std::string heightfile = base_path+heightname;
  if (size_t sizeOf_file = rokz::SizeOf_file (heightfile)) { 

    printf (" HeIgHt !!! ----> %s\n ", __FUNCTION__); 

    assert (sizeOf_file == total_num_pixels * sizeof(float)); 
    rokz::From_file (heightimage.dat, heightfile); 

    
    Vec<float> above253 (total_num_pixels, 0.0f);
    above253.clear ();

    for (auto val : heightimage.dat ) {
      if (val > -2550.0f) {
        above253.push_back (val); 
      }}  
    
    float minel = *std::min_element ( above253.begin(), above253.end ());
    float maxel =  *std::max_element ( above253.begin(), above253.end ());
    printf ( " --> height<min:%f | max:%f | diff : %f>\n", minel, maxel, maxel - minel);
  }
  
  std::string colorfile = base_path+colorname; 
  if (size_t sizeOf_file = rokz::SizeOf_file (colorfile)) { 

    printf (" CoLoR !!!  %s\n ", __FUNCTION__); 

    rekz::imagebuff<float> colorimage (kWIDTH, kHEIGHT); 
    assert (sizeOf_file == total_num_pixels * sizeof(float)); 
    rokz::From_file (colorimage.dat, colorfile); 

    Vec<float> above253 (total_num_pixels, 0.0f);
    above253.clear ();

    for (auto val : colorimage.dat) {
      if (val > -2550.0f) {
        above253.push_back (val); 
      }}  
    
    printf ( " --> color <min:%f | max:%f>\n", 
             *std::min_element ( above253.begin(), above253.end ()),
             *std::max_element ( above253.begin(), above253.end ()));
  }
  
  std::string coordfile = base_path+coordname; 
  if (size_t sizeOf_file = rokz::SizeOf_file (coordfile)) { 
    printf (" CoORds !!! ----> %s\n ", __FUNCTION__); 

    rekz::imagebuff<glm::dvec2> coordimage (kWIDTH, kHEIGHT);

    assert (sizeOf_file == total_num_pixels * sizeof(glm::dvec2)); 
    rokz::From_file (coordimage.dat, coordfile); 

    Vec<glm::dvec2> above253 (total_num_pixels, glm::dvec2{}); 
    above253.clear ();

    const float max_val = std::numeric_limits<float>::max();
    const float min_val = std::numeric_limits<float>::min();
    
    glm::vec2 min_v { max_val, max_val } ;
    glm::vec2 max_v { -max_val, -max_val}; 

    for (auto& crd : coordimage.dat ) {

      min_v.x =  (crd.x < min_v.x ? crd.x : min_v.x);
      min_v.y =  (crd.y < min_v.y ? crd.y : min_v.y);

      max_v.x =  (crd.x > max_v.x ? crd.x : max_v.x);
      max_v.y =  (crd.y > max_v.y ? crd.y : max_v.y);
      
    }

    printf ( " --> min crd <x:%f | y:%f>\n", min_v.x, min_v.y);
    printf ( " --> max crd <x:%f | y:%f>\n", max_v.x, max_v.y);

    glm::vec2 diffv =    max_v - min_v;
      
    printf ( " --> diffv <x:%f | y:%f>\n", diffv.x, diffv.y);
    
  }
}
