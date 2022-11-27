
#ifndef ROKZ_UNIFORM_H
#define ROKZ_UNIFORM_H

#include "common.h"
#include "utility.h"
#include "shared_types.h"

#include "buffer.h"


namespace rokz{

  bool CreateUniformBuffer (BufferStruc& buffstruc, 
                            size_t sizeof_elem,
                            size_t num_elem, 
                            const VkDevice& device,
                            const VkPhysicalDevice& physdev); 

}

#endif
