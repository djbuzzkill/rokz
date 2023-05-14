

  //
  inline VkAttachmentDescription&
  AttachmentDescription_default (VkAttachmentDescription& desc,          
                                 VkFormat format, VkSampleCountFlagBits sample_count_bits, 
                                 VkImageLayout initial_layout, VkImageLayout final_layout)
  {

    return AttachmentDescription (desc, format, sample_count_bits, 
                                  VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
                                  VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, 
                                  initial_layout, final_layout); 
  }

  //
  inline VkAttachmentDescription&
  ColorAttachment_default  (VkAttachmentDescription& desc, VkFormat format, VkSampleCountFlagBits sample_count_bits)
  {
    return AttachmentDescription_default (desc, format, sample_count_bits, VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ); 
  
   }

  inline VkAttachmentDescription&
  DepthAttachment_default (VkAttachmentDescription& desc, VkFormat format, VkSampleCountFlagBits sample_count_bits) 
  {
    return AttachmentDescription_default (desc, format, sample_count_bits, VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ); 
  
  }
