 
#ifndef ROKZ_TRANSFER_IMAGE_INCLUDE
#define ROKZ_TRANSFER_IMAGE_INCLUDE

#include "rokz_types.hpp"

namespace rokz { namespace cx { 
    // ----------------------------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------------------------
    struct mappedimage_cb {
      virtual int on_mapped  (void* mappedp, size_t maxsize, const VkExtent2D& ext2d) = 0;

      typedef std::shared_ptr<mappedimage_cb> Ref;
    };


    // ----------------------------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------------------------
    int TransferToDeviceImage (VkImage& dstb, size_t sizemem, VkFormat targetformat, 
                               const VkExtent2D& ext2d, mappedimage_cb*, const rokz::Device& device);

    inline int TransferToDeviceImage (VkImage& dstb, size_t sizemem, VkFormat targetformat,
                                      const VkExtent2D& ext2d, mappedimage_cb::Ref cb, const rokz::Device& device) {
      return TransferToDeviceImage (dstb, sizemem, targetformat, ext2d, cb.get (), device); 
    }

    // ----------------------------------------------------------------------------------------
    // mebe combine this w other Transfer fn
    // ----------------------------------------------------------------------------------------
    struct mappedlayer_cb {
      typedef std::shared_ptr<mappedlayer_cb> Ref;
      virtual int on_mapped (void* mappedp, size_t maxsize, uint32 layerindex, const VkExtent2D& ext) = 0; 
    };

    // ----------------------------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------------------------
    int TransferToImageLayer (VkImage& dsti, VkFormat format, size_t sizebytes,
                              uint32 numlayers, uint32 beginlayer, const VkExtent2D& ext2d,
                              mappedlayer_cb* cb, const rokz::Device& device); 

    inline int TransferToImageLayer (VkImage& dsti, VkFormat format, size_t sizebytes,
                                     uint32 numlayers, uint32 beginlayer, const VkExtent2D& ext2d,
                                     mappedlayer_cb::Ref cb, const rokz::Device& device) { 
      return TransferToImageLayer (dsti, format, sizebytes, numlayers, beginlayer,
                                   ext2d, cb.get (), device); 
    }

  }}

#endif
