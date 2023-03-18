//
#ifndef ROKZ_SHARED_TYPES
#define ROKZ_SHARED_TYPES

#include "common.h"


namespace rokz { 

  // --------------------------------------------------------------------------------------------
  //                                          
  // --------------------------------------------------------------------------------------------
  typedef std::uint8_t  uint8;
  typedef std::uint16_t uint16; 
  typedef std::uint32_t uint32; 
  typedef std::uint64_t uint64; 
  
  typedef std::int8_t   int8;
  typedef std::int16_t  int16; 
  typedef std::int32_t  int32; 
  typedef std::int64_t  int64;
  typedef std::uint8_t  byte;

  // --------------------------------------------------------------------------------------------
  //                                          
  // --------------------------------------------------------------------------------------------
  template<typename Ty> 
    using Vec = std::vector<Ty>;
    


  // --------------------------------------------------------------------------------------------
  //                                          
  // --------------------------------------------------------------------------------------------
  typedef Vec<byte> bytearray; 
  
  // --------------------------------------------------------------------------------------------
  //                                          
  // --------------------------------------------------------------------------------------------
  template<typename Ty> struct YawPitchRoll {

    Ty yaw;
    Ty pitch;
    Ty roll; 
  };

  typedef YawPitchRoll<float>  YPRf;
  typedef YawPitchRoll<double> YPRd;
  
  //-------------------------------------------------------------------------------------
  //                
  //-------------------------------------------------------------------------------------
  typedef std::optional<uint32_t> MaybeIndex;

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
