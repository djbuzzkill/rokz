
#ifndef ROKZ_RC_BUFFER_INCLUDE
#define ROKZ_RC_BUFFER_INCLUDE

#include "common.h"
#include "shared_types.h"

namespace rokz { namespace rc { 

    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct Buffer : public deviceob <VkBuffer, Buffer> {

      Buffer (const Device& d) : deviceob (d), alloc_ci (), allocation (), alloc_info () {
      }

      VmaAllocationCreateInfo alloc_ci; 
      VmaAllocation           allocation;
      VmaAllocationInfo       alloc_info;
      virtual ~Buffer ();

    protected:
      //
    };

    Buffer::Ref Create_uniform_mapped  (size_t size_e, size_t num_e, const Device& device); 
    Buffer::Ref CreateDeviceBuffer     (size_t reqsize, VkBufferUsageFlags usage, const Device& device);

    inline void* MappedPointer (rc::Buffer::Ref buff) { 
      return  buff->alloc_info.pMappedData;
    }
  

  }}


#endif
