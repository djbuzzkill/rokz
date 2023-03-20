
#ifndef ROKZ_RC_TYPES_INCLUDE
#define  ROKZ_RC_TYPES_INCLUDE


#include "common.h"
#include "rokz_types.h"

namespace rokz {
  // -----------------------------------------------------------------------------------------------


  namespace rc {

    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct Buffer : public hresourc<VkBuffer> {

      typedef std::shared_ptr<Buffer> Ref;
      
      Buffer (const Device& d) : hresourc (d), alloc_ci (), allocation (), alloc_info () {
      }

      
      VmaAllocationCreateInfo alloc_ci; 
      VmaAllocation           allocation;
      VmaAllocationInfo       alloc_info;
      virtual ~Buffer ();

    protected:

    };


    Buffer::Ref Create_VB_device  (const void* mem, size_t sz_mem, const rokz::Device& device);

    Buffer::Ref Create_uniform_mapped  (size_t size_e, size_t num_e, const Device& device); 

    
    /* hresourc<VkBuffer>::Ref Create_IB_16_device  (const void* mem, size_t sz_mem, const rokz::Device& device); */
    /* hresourc<VkBuffer>::Ref Create_uniform  (const void* mem, size_t sz_mem, const rokz::Device& device); */

    inline void* MappedPointer (rc::Buffer::Ref buff) { 
      return  buff->alloc_info.pMappedData;
    }
    
    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct Image:  public hresourc<VkImage>  {

      Image (const Device& d) : hresourc (d), alloc_ci (), allocation (), alloc_info () {
      }

      virtual ~Image (); 
      
      //VkDeviceMemory       mem; 
      //    VkMemoryAllocateInfo alloc_info;
    
      // vmaCreateBuffer(glob.allocator, &buffer_info, &allocInfo, &buffer, &allocation, nullptr);
      VmaAllocationCreateInfo alloc_ci; 
      VmaAllocation           allocation;
      VmaAllocationInfo       alloc_info;
  
    }; 

    Image::Ref CreateImage (const VkImageCreateInfo& ci, const Device& device);

  }

  
}
#endif
