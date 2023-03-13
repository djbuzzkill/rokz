

#include "vert_defs.h"

using namespace rokz;

// ------------------------------------------------------------------------------------------
// vert input binding, diffrnt from input attriubutes                       
// ------------------------------------------------------------------------------------------
const VkVertexInputBindingDescription rokz::kPNCTx_InputBindingDesc = {
    0,                          // binding    
    sizeof (PNCTx_Vert),         // stride      
    VK_VERTEX_INPUT_RATE_VERTEX // inputRate   
  }; 

// ------------------------------------------------------------------------------------------
const Vec<VkVertexInputAttributeDescription> rokz::kPNCTx_InputAttributeDesc = {
  
  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(PNCTx_Vert, pos),  // .offset   
  },

  VkVertexInputAttributeDescription { // normal
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(PNCTx_Vert, nrm), 
  },

  VkVertexInputAttributeDescription { // color
    2,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(PNCTx_Vert, col), 
  },
    
  VkVertexInputAttributeDescription { // tex coord
    3,                             
    0, 
    VK_FORMAT_R32G32_SFLOAT,
    offsetof(PNCTx_Vert, txc), 
  }

};

// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
const VkVertexInputBindingDescription rokz::kPNTx_InputBindingDesc = {
    0,                          // binding    
    sizeof (PNTx_Vert),          // stride      
    VK_VERTEX_INPUT_RATE_VERTEX // inputRate   
  }; 

// ------------------------------------------------------------------------------------------
const Vec<VkVertexInputAttributeDescription> rokz::kPNTx_InputAttributeDesc = {
  
  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(PNTx_Vert, pos),  // .offset   
  },

  VkVertexInputAttributeDescription { // normal
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(PNTx_Vert, nrm), 
  },

  VkVertexInputAttributeDescription { // tex coord
    2,                             
    0, 
    VK_FORMAT_R32G32_SFLOAT,
    offsetof(PNTx_Vert, txc), 
  }

};

// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
const VkVertexInputBindingDescription rokz::kPC_InputBindingDesc = {
    0,                          // binding    
    sizeof (PC_Vert),          // stride      
    VK_VERTEX_INPUT_RATE_VERTEX // inputRate   
  }; 

// ------------------------------------------------------------------------------------------
const Vec<VkVertexInputAttributeDescription> rokz::kPC_InputAttributeDesc = {
  
  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(PC_Vert, pos),  // .offset   
  },

  VkVertexInputAttributeDescription { // color
    1,                              
    0, 
    VK_FORMAT_R32G32B32_SFLOAT,
    offsetof(PC_Vert, col), 
  },

};

// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
const VkVertexInputBindingDescription rokz::kPTx_InputBindingDesc = {
    0,                          // binding    
    sizeof (PTx_Vert),          // stride      
    VK_VERTEX_INPUT_RATE_VERTEX // inputRate   
  }; 

// ------------------------------------------------------------------------------------------
const Vec<VkVertexInputAttributeDescription> rokz::kPTx_InputAttributeDesc = {
  
  VkVertexInputAttributeDescription { // pos
    0,                             // .location 
    0,                             // .binding  
    VK_FORMAT_R32G32B32_SFLOAT,    // .format   
    offsetof(PTx_Vert, pos),  // .offset   
  },


  VkVertexInputAttributeDescription { // tex coord
    1,                             
    0, 
    VK_FORMAT_R32G32_SFLOAT,
    offsetof(PTx_Vert, txc), 
  }

};

