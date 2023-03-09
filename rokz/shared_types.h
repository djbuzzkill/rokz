//
#ifndef ROKZ_SHARED_TYPES
#define ROKZ_SHARED_TYPES

#include "common.h"


namespace rokz { 

  template<typename Ty> struct YawPitchRoll {

    Ty yaw;
    Ty pitch;
    Ty roll; 
  };

  typedef YawPitchRoll<float>  YPRf;
  typedef YawPitchRoll<double> YPRd;
  
  // ----------------------------------------------------------------------------------------------
  //                           
  // ----------------------------------------------------------------------------------------------
  template<typename Ty> struct Spherical {

    Spherical () : theta (0.0f), phi (0.0f) {
    }
    
    Spherical (Ty th, Ty ph) : theta (th), phi (ph) {
    }

    Ty theta; // "longitude" 0 -> 360
    Ty phi;   // "latitude"  -90 -> +90
  }; 

  typedef Spherical<float>  Sphericf;
  typedef Spherical<double> Sphericd;

}

#endif
