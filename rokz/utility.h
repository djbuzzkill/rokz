

#ifndef ROKZ_UTILITY_INCLUDE
#define ROKZ_UTILITY_INCLUDE

#include "common.h"
#include "shared_types.h"
//#include "binary_IO.h"

namespace rokz {

  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  typedef std::array<unsigned char, 4> rgba8;
  typedef std::array<unsigned char, 3> rgb8; 


  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  struct destructor {
    
    virtual ~destructor () = default;

  protected:
    destructor () = default;
  }; 

  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  template<typename Ty> 
  inline bool in_bounds_incl (Ty val , Ty minval , Ty maxval) {
    return (val >= minval && val <= maxval);
  }

  //
  template<typename Ty> 
  inline bool inb_in (Ty val , Ty minval , Ty maxval) {
    return (val >= minval && val <= maxval);
  }
  // 
  template<typename Ty> 
  inline bool in_bounds_excl (Ty val, Ty minval, Ty maxval) { 
    return (val > minval && val < maxval);
  }    
     
  //
  template<typename Ty> 
  inline bool inb_ex (Ty val, Ty minval, Ty maxval) { 
    return (val > minval && val < maxval);
  }    

  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------
  //bool CheckValidationSupport (const std::vector<const char*>& validation_layers);
  bool CheckValidationSupport (const std::vector<std::string>& val_layers); 
  bool FindMemoryType         (uint32_t& type_index, uint32_t type_filter, VkMemoryPropertyFlags prop_flags,  const VkPhysicalDevice& physdev ); 
  std::vector<std::string>& GetRequiredExtensionNames (std::vector<std::string>& extstrs); 

  bool FindSupportedFormat  (VkFormat& out, const std::vector<VkFormat>& candidates,
                             VkImageTiling tiling,
                             VkFormatFeatureFlags features, 
                             const VkPhysicalDevice& physdev);

  bool                  FindDepthFormat      (VkFormat& outfmt, const VkPhysicalDevice& physdev);
  bool                  HasStencilComponent  (VkFormat format);
  VkSampleCountFlagBits MaxUsableSampleCount (VkPhysicalDevice physdev); 
}


#endif


