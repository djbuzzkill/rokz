
#ifndef MARS_FILES_INCLUDE
#define MARS_FILES_INCLUDE

#include "rekz.h"
#include "rokz/shared_types.h"

//
namespace ESP_018065_1975_RED_ESP_019133_1975_RED {

  using namespace rokz;

  const std::string kDEM_name = "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.6900x17177.hgt"; 
  const std::string kDRG_name = "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat";
  const std::string kIGM_name = "ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat";
  
  const uint32 kWIDTH  = 6900;          
  const uint32 kHEIGHT = 17177;          

  void print_attributes () ; 
} // 

#endif
