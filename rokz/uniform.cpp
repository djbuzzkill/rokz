
#include "uniform.h"


// create  buffer
bool rokz::CreateUniformBuffer (BufferStruc& buffstruc, 
                                size_t sizeof_elem,
                                size_t num_elem, 
                                const VkDevice& device,
                                const VkPhysicalDevice& physdev){

  buffstruc.create_info = {};
  buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffstruc.create_info.size  = sizeof_elem * num_elem; 
  buffstruc.create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  return rokz::CreateBuffer (buffstruc, device, physdev); 
}
