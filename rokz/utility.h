

#ifndef ROKZ_UTILITY_INCLUDE
#define ROKZ_UTILITY_INCLUDE

#include "common.h"
#include "shared_types.h"
#include "binary_IO.h"

namespace rokz {

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

  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  inline size_t SizeOf_file (const std::string& fn) {
    //printf  ( "%s[%s]\n", __FUNCTION__, fn.c_str());    
    if (std::FILE* f = std::fopen (fn.c_str(), "r")) {
      //printf  ( "[f is good]\n" );    
      std::shared_ptr<std::FILE> sp (f, std::fclose); 
      if (0 == std::fseek (sp.get(), 0, SEEK_END)) 
	return  std::ftell (sp.get()); 
    }
    return 0; 
  }

  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------
  template<typename Seq>
  inline Seq& From_file (Seq& out, const std::string& fname) {

    if (auto sizeOf_file = SizeOf_file (fname)) {

      out.resize (sizeOf_file); 

      ReadStreamRef rs = CreateReadFileStream (fname);

      if (rs) {
	rs->Read (&out[0], sizeOf_file); 
      }
    }
    return out; 
  } 


  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------
  struct SwapchainSupportInfo {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;    
  };
  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------

  const std::vector<const char*>& GetDeviceExtensionNames (); 

  bool                  CheckValidationSupport (const std::vector<const char*>& val_layers); 
  bool                  SelectPhysicalDevice (VkPhysicalDevice& physdev, QueueFamilyIndices& queueind, const VkSurfaceKHR& surf, const VkInstance& inst);
  QueueFamilyIndices&   FindQueueFamilies (QueueFamilyIndices& queue_fams, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev);
  SwapchainSupportInfo& QuerySwapchainSupport (SwapchainSupportInfo& deets, const VkSurfaceKHR& surf, const VkPhysicalDevice& dev); 
  bool                  CheckValidationSupport (const std::vector<const char*>& validation_layers);

  bool                      CheckDeviceExtensionSupport (const VkPhysicalDevice& device); 
  bool                      FindMemoryType              (uint32_t& type_filter, VkMemoryPropertyFlags prop_flags, const VkPhysicalDevice& physdev ); 
  std::vector<const char*>& GetRequiredExtensionNames   (std::vector<const char*>&  exts); 

}


#endif


