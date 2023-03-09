

#ifndef ROKZ_UTILITY_INCLUDE
#define ROKZ_UTILITY_INCLUDE

#include "common.h"
#include "rokz_types.h"
//#include "binary_IO.h"



#define HERE(x) printf("%s --> [%s]:%i\n",(x), __FUNCTION__,__LINE__);

namespace rokz {

  namespace ut { 
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
  // --------------------------------------------------------------
  inline VkSampleCountFlagBits MaxUsableSampleCount  (const PhysicalDevice& physdev) {
    const VkPhysicalDeviceProperties& phys_dev_props =     physdev.properties;
    VkSampleCountFlags counts = phys_dev_props.limits.framebufferColorSampleCounts & phys_dev_props.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }
    return VK_SAMPLE_COUNT_1_BIT;
  }
  // --------------------------------------------------------------
  inline VkDeviceSize MinUniformBufferOffsetAlignment (const PhysicalDevice& phdev) {
   return phdev.properties.limits.minUniformBufferOffsetAlignment;
  }


  const char* VkResult_2_string (VkResult r); 
}
}


#endif


