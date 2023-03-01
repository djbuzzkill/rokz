
#include "uniform.h"
#include "allocation.h"









bool rokz::CreateUniformBuffer (rokz::Buffer& buf, size_t sizeOf_el, size_t numberOf_el, const rokz::Device& device) {

    rokz::cx::CreateInfo_uniform (buf.ci, sizeOf_el, numberOf_el); 
    rokz::cx::AllocCreateInfo_mapped (buf.alloc_ci); 

    if (!rokz::cx::CreateBuffer (buf, device.allocator.handle)) {
      printf (" --> [FAIL]  create MVPTransform buffer \n"); 
      return false; 
    }

    return true; 
}



