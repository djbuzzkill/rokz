

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

// "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM_f32x6900x17177.bin";
std::string kDEM_heightname = "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.6900x17177.hgt"; 

// "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG_f32x6900x17177.bin";
std::string kDRG_colorname  = "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat";

// "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM_dvec2x6900x17177.bin";
std::string kIGM_coordname  = "ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat";



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
    load_from_file (heightimage, heightfile ); 

    
    Vec<float> below2530;
    Vec<float> below2500;
    Vec<float> below2000;
    Vec<float> below1500;
    Vec<float> below1400;
    Vec<float> below1300;
    Vec<float> depths;
    
    for (auto val : heightimage.dat ) {

      if (val > -1300.0f)
        below1300.push_back(val);          
      else if (val > -1400.0f)
        below1400.push_back(val);          
      else if (val > -1500.0f)
        below1500.push_back(val);          
      else if (val > -2000.0f)
        below2000.push_back(val);     
      else if (val > -2500.0f)
        below2500.push_back (val); 
      else if (val > -2550.0f)
        below2530.push_back (val);
      else
        depths.push_back(val); 
      
    }  
    
    printf (" --> below1300:%zu\n", below1300.size ());    
    printf (" --> below1400:%zu\n", below1400.size ());    
    printf (" --> below1500:%zu\n", below1500.size ());    
    printf (" --> below2000:%zu\n", below2000.size ());
    printf (" --> below2500:%zu\n", below2500.size ()); 
    printf (" --> below2530:%zu\n", below2530.size ());
    printf (" --> depths:%zu\n", depths.size ());

    float minel = *std::min_element ( heightimage.dat.begin(), heightimage.dat.end ());
    float maxel = *std::max_element ( heightimage.dat.begin(), heightimage.dat.end ());

    printf ( " --> height<min:%f | max:%f | diff : %f>\n", minel, maxel, maxel - minel);
  }
  
  std::string colorfile = base_path+colorname; 
  if (size_t sizeOf_file = rokz::SizeOf_file (colorfile)) { 

    printf (" CoLoR !!!  %s\n ", __FUNCTION__); 

    rekz::imagebuff<float> colorimage (kWIDTH, kHEIGHT); 
    assert (sizeOf_file == total_num_pixels * sizeof(float)); 
    load_from_file (colorimage, colorfile); 

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
    load_from_file (coordimage, coordfile); 

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
