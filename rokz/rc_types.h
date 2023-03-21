
#ifndef ROKZ_RC_TYPES_INCLUDE
#define  ROKZ_RC_TYPES_INCLUDE


#include "common.h"
#include "rokz_types.h"
#include <vulkan/vulkan_core.h>

namespace rokz {
  // -----------------------------------------------------------------------------------------------


  namespace rc {

    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct Buffer : public deviceob <VkBuffer> {

      typedef std::shared_ptr<Buffer> Ref;
      
      Buffer (const Device& d) : deviceob (d), alloc_ci (), allocation (), alloc_info () {
      }

      VmaAllocationCreateInfo alloc_ci; 
      VmaAllocation           allocation;
      VmaAllocationInfo       alloc_info;
      virtual ~Buffer ();

    protected:

    };


    Buffer::Ref Create_VB_device  (const void* mem, size_t sz_mem, const rokz::Device& device);

    Buffer::Ref Create_uniform_mapped  (size_t size_e, size_t num_e, const Device& device); 

    inline void* MappedPointer (rc::Buffer::Ref buff) { 
      return  buff->alloc_info.pMappedData;
    }
    
    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct Image:  public deviceob<VkImage>  {

      Image (const Device& d) : deviceob (d), alloc_ci (), allocation (), alloc_info () {
      }

      typedef std::shared_ptr<Image> Ref;

      virtual ~Image (); 
      
      // vmaCreateBuffer(glob.allocator, &buffer_info, &allocInfo, &buffer, &allocation, nullptr);
      VmaAllocationCreateInfo alloc_ci; 
      VmaAllocation           allocation;
      VmaAllocationInfo       alloc_info;
  
    }; 

    Image::Ref CreateImage (const VkImageCreateInfo& ci, const Device& device);

    Image::Ref CreateImage_2D_color_sampling (uint32 , uint32, VkSampleCountFlagBits sampleflags, const Device& device); 

    // -----------------------------------------------------------------------------------------


    // -----------------------------------------------------------------------------------------

  }

  
}
#endif
