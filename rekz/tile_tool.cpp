

// #ifndef REKZ_EXAMINE_TIFFS_INCLUDE
// #define REKZ_EXAMINE_TIFFS_INCLUDE
// //
#include "rekz.h"

#include "mars_files.h"
#include "image_loader.h"
#include "image_tool.h"
#include "rokz/binary_IO.h"
#include "rokz/file.h"

#include <IL/il.h>
#include <IL/ilu.h>

#include <algorithm>
#include <glm/fwd.hpp>
#include <limits>
//#include "rokz/shared_types.h"

using namespace rokz;

using namespace ESP_018065_1975_RED_ESP_019133_1975_RED; 
// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
const uint32 k_tile_dim = 1024;
const uint32 k_total_image_pixels = kWIDTH * kHEIGHT;
const uint32 k_total_tile_pixels = k_tile_dim * k_tile_dim;

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
struct fcolor_tile_handler : public rekz::TileCB<float> { 

  int Exec (const imagebuff<float>& tilei, uint32 xtile, uint32 ytile) {

  ilInit ();
  iluInit (); 

  const float kDRG_MIN = -0.016220f;
  
  ILuint testim = ilGenImage ();
  ilBindImage (testim);

  imagebuff<glm::u8vec3> otile (k_tile_dim, k_tile_dim); 
  for (uint32 i = 0; i < k_total_tile_pixels; ++i) {
    if (tilei.dat[i] > -2550.0f) {
      uint8 grad = uint8 ((tilei.dat[i] - kDRG_MIN) * 128.0f);
      otile.dat[i] = glm::u8vec3 (grad, grad, grad);
    }
    else {
      otile.dat[i] = glm::u8vec3 (0); 
      }
  }

  bool img_res = ilTexImage (k_tile_dim, k_tile_dim, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, &otile.dat[0]); 
  if (!img_res) { 
    printf ("img error : %s\n", iluErrorString (ilGetError()));
  }

  char namebuf[64];
  sprintf (namebuf, "DRG_tile_%u_%u.png", xtile, ytile); 
  std::string savename = "/home/djbuzzkill/owenslake/tmp/";
  savename += namebuf;

  printf ("saving out...\n    %s\n ", savename.c_str()); 
  ilSave (IL_PNG, savename.c_str());  
  ilDeleteImage (testim);
  ilShutDown ();

  return 0;  
  }};

// -------------------------------------------------------------------------------------------
int generate_DRG_tiles (const Vec<std::string>& args) {
  printf ("%s\n ", __FUNCTION__); 
  using namespace ESP_018065_1975_RED_ESP_019133_1975_RED; 
  std::string      base_path = "/home/djbuzzkill/owenslake/data/Mars/"; 
  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG_f32x6900x17177.bin";
  std::string      colorname = "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat";
  std::string      fqcolorfile = base_path + colorname; 
  imagebuff<float> colorimage (kWIDTH, kHEIGHT);
  load_from_file (colorimage, fqcolorfile); 

  rekz::iteratetileparams params {
    {k_tile_dim, k_tile_dim},
    {6, 10},
    true
  };

  
  rekz::iterate_over_tiles (colorimage, params, std::make_shared<fcolor_tile_handler>()) ; 

  printf ("bai %s\n ", __FUNCTION__); 
  return 0;

}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
struct fheight_tile_handler : public rekz::TileCB<float> { 

  int Exec (const imagebuff<float>& tilei, uint32 xtile, uint32 ytile) {
  
  float min_elem = *std::min_element (  tilei.dat.begin (), tilei.dat.end () );
  float max_elem = *std::max_element (  tilei.dat.begin (), tilei.dat.end () );
  printf (" tile_ext<min:%f, max:%f, diff:%f> ", min_elem, max_elem, max_elem - min_elem);

  const float kDEM_MIN = -2550.0;
  const float kDEM_MAX = -1400.0;
  const float diff = kDEM_MAX - kDEM_MIN;
  
  imagebuff<glm::u8vec3> otile (k_tile_dim, k_tile_dim); 

  for (uint32 i = 0; i < k_total_tile_pixels; ++i) {
    if (tilei.dat[i] > kDEM_MIN) {
      uint8 u8grad = uint8 (256.0 * (tilei.dat[i] - kDEM_MIN) / diff);
      otile.dat[i] = glm::u8vec3 (u8grad, u8grad, u8grad);
    }
    else {
      otile.dat[i] = glm::u8vec3 (0); 
    }}

  ilInit ();   // just save to png for now

  ILuint testim = ilGenImage ();
  ilBindImage (testim);

  bool imgres = ilTexImage (k_tile_dim, k_tile_dim, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, &otile.dat[0]); 
  if (!imgres) { 
    printf ("img error : %s\n", iluErrorString (ilGetError()));
  }

  char buf[64];
  sprintf (buf, "DEM_tile_%u_%u.png", xtile, ytile); 

  std::string savename = "/home/djbuzzkill/owenslake/tmp/";
  savename += buf;

  printf ("saving out to..\n    %s\n ", savename.c_str()); 
  ilSave (IL_PNG, savename.c_str());  

  ilDeleteImage (testim);
  ilShutDown ();

  return 0;  
  }};

// -------------------------------------------------------------------------------------------
int generate_DEM_tiles (const Vec<std::string>& args) {
  printf ("%s\n ", __FUNCTION__); 
  using namespace ESP_018065_1975_RED_ESP_019133_1975_RED; 
  std::string      base_path = "/home/djbuzzkill/owenslake/data/Mars/"; 
  std::string      heightname = "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.6900x17177.hgt"; 
  std::string      fqheightfile = base_path + heightname; 
  imagebuff<float> colorimage (kWIDTH, kHEIGHT);
  load_from_file (colorimage, fqheightfile); 

  rekz::iteratetileparams params {
    {k_tile_dim, k_tile_dim},
    {6, 10},
    true
  };
  
  rekz::iterate_over_tiles (colorimage, params, std::make_shared<fheight_tile_handler>()) ; 

  printf ("bai %s\n ", __FUNCTION__); 
 return 0;
}



// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
struct coord_tile_handler : rekz::TileCB<glm::vec2> {
  
  int Exec (const imagebuff<glm::vec2>& tilei, uint32 xtile, uint32 ytile) {

  return 0;
  }};

// ------------------------------------------------------------------------------------------
int generate_IGM_tiles (const Vec<std::string>& args) {
  printf ("%s\n ", __FUNCTION__); 

  std::string base_path   = "/home/djbuzzkill/owenslake/data/Mars/"; 
  std::string coordname   = "ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat";
  std::string fqcoordfile = base_path + coordname; 

  imagebuff<glm::vec2> coordimage (kWIDTH, kHEIGHT);
  load_from_file (coordimage, fqcoordfile); 

  rekz::iteratetileparams params {
    {k_tile_dim, k_tile_dim},
    {6, 10},
    true
  };
  
  rekz::iterate_over_tiles (coordimage, params, std::make_shared<coord_tile_handler>()) ; 

  printf ("bai %s\n ", __FUNCTION__); 
 return 0;
}

// ------------------------------------------------------------------------------------------
// main()
// ------------------------------------------------------------------------------------------
int tile_tool (const Vec<std::string>& args) {

  printf ("%s\n ", __FUNCTION__); 

  generate_DRG_tiles (args);
  generate_DEM_tiles (args);

  ESP_018065_1975_RED_ESP_019133_1975_RED::print_attributes ();

  return 0; 
  
}


