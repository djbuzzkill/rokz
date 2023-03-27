//
#ifndef ROKZ_SHARED_TYPES
#define ROKZ_SHARED_TYPES

#include "common.h"
#include <vulkan/vulkan_core.h>


 #define NO_PROB(x)(x)==0
 #define GOT_PROB(x)(x)!=0

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
  using MaybeIndex  = std::optional<uint32>;

  using filepath = std::filesystem::path; 
  // ----------------------------------------------------------------------------------------------
  //                           
  // ----------------------------------------------------------------------------------------------
  template<typename Ty> struct Spherical {

    Spherical () : theta (0.0), phi (0.0), radius(0.0) {
    }
    
    Spherical (Ty th, Ty ph, Ty r) : theta (th), phi (ph), radius (r) {
    }

    Ty theta; // "longitude" 0 -> 360
    Ty phi;   // "latitude"  -90 -> +90
    Ty radius;
  }; 

  typedef Spherical<float>  Sphericf;
  typedef Spherical<double> Sphericd;
    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    struct destructor {
    
      virtual ~destructor () { assert ("virtual destruct"); } 

    protected:
      destructor () {}
    }; 
  

  struct Device;
  // ----------------------------------------------------------------------------------------------
  //                                                
  // ----------------------------------------------------------------------------------------------
  template<typename VkTy,
           typename ObjTy> struct deviceob : public destructor {

    typedef std::shared_ptr<ObjTy> Ref;
    
    deviceob (Device const& d) : device (d), handle (VK_NULL_HANDLE) {
    }

    Device const& device;
    VkTy handle;

  };


}

#endif
