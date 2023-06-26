

#ifndef ROKZ_UTILITY_INCLUDE
#define ROKZ_UTILITY_INCLUDE

#include "common.hpp"
#include "rokz_types.hpp"


#define HERE(x) printf(" [%s|line:%i] --> %s \n",__FUNCTION__,__LINE__,(x));

#define CHECK_VK_HANDLE(x) ((x)!=VK_NULL_HANDLE)

namespace rokz { namespace ut { 

    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    typedef std::array<unsigned char, 4> rgba8;
    typedef std::array<unsigned char, 3> rgb8; 

    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    template<typename Ty> 
    inline bool in_bounds_incl (Ty val , Ty minval , Ty maxval) {
      return (val >= minval && val <= maxval);
    }

    //
    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    template<typename Ty> inline bool inb_in (Ty val , Ty minval , Ty maxval) {
      return (val >= minval && val <= maxval);
    }
    // 
    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    template<typename Ty> 
    inline bool in_bounds_excl (Ty val, Ty minval, Ty maxval) { 
      return (val > minval && val < maxval);
    }    
     
    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    template<typename Ty> 
    inline bool inb_ex (Ty val, Ty minval, Ty maxval) { 
      return (val > minval && val < maxval);
    }    

    // ----------------------------------------------------------------------------------------------
    //
    //-------- yaw/left/right arrow --------------
    inline glm::vec3 unit_dir_xz (float theta) { 
      return glm::vec3 (cos (theta), 0.0f, -sinf (theta));
    }
    // ------- pitch/up/down arrow ---------------
    inline glm::vec3 unit_dir_yz (float theta) {
      return glm::vec3 (0.0, cos (theta), -sinf (theta));
    }
    // ------- roll/cw/ccw ---------------
    inline glm::vec3 unit_dir_xy (float theta) {
      // this has not been tested
      return glm::vec3 (cos (theta), sinf (theta), 0.0);
    }


    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    glm::mat4 orthographic_projection ( float left_plane, float right_plane, float bottom_plane,
                                        float top_plane, float near_plane, float far_plane ); 
    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
    float ViewAspectRatio (uint32_t w, uint32_t h);

    float AspectRatio (const VkExtent2D& ext);
    // ----------------------------------------------------------------------------------------------
    //                                                
    // ----------------------------------------------------------------------------------------------
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
    void PrintPhysicalDeviceLimits (const VkPhysicalDeviceLimits& limits); 

    inline VkSampleCountFlagBits MaxUsableSampleCount  (const PhysicalDevice& physdev) {
      const VkPhysicalDeviceProperties& phys_dev_props = physdev.properties;
      VkSampleCountFlags counts = phys_dev_props.limits.framebufferColorSampleCounts & phys_dev_props.limits.framebufferDepthSampleCounts;
      if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
      if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
      if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
      if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
      if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
      if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }
      return VK_SAMPLE_COUNT_1_BIT;
    }
    // -------------------------------------------------------------------------------------
    inline VkDeviceSize MinUniformBufferOffsetAlignment (const PhysicalDevice& phdev) {
      return phdev.properties.limits.minUniformBufferOffsetAlignment;
    }

    // -------------------------------------------------------------------------------------
    const char* VkResult_2_string (VkResult r); 

    // -------------------------------------------------------------------------------------
    void printmat (glm::mat4& mat);

  }} // rokz ut


#endif


