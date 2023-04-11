
#ifndef ROKZ_TRANSFER_BUFFER_INCLUDE
#define ROKZ_TRANSFER_BUFFER_INCLUDE



#include "common.h"
#include "rokz_types.h"


namespace rokz { namespace cx {

    // ----------------------------------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------------------------------
    struct mappedbuffer_cb {
      virtual int on_mapped  (void* mappedmemory, size_t maxsize) = 0;
      
      typedef std::shared_ptr<mappedbuffer_cb> Ref;
    };
    
    int TransferToDeviceBuffer (VkBuffer& dstb, size_t sizemem, mappedbuffer_cb*, const rokz::Device& device);
    
    inline int TransferToDeviceBuffer (VkBuffer& dstb, size_t sizemem, mappedbuffer_cb::Ref cb, const rokz::Device& device) {
      return TransferToDeviceBuffer (dstb, sizemem, cb.get (), device); 
    }


  }}
#endif
