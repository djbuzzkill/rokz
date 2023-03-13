
#ifndef ROKZ_VERTEX_DEF_INCLUDE
#define ROKZ_VERTEX_DEF_INCLUDE


#include "common.h"
#include "shared_types.h"












// N  | normal
// P  | position
// C  | color
// Tx | tex coord
// Ta | tangnent
        


namespace rokz {
  // -------------------------------------------------------------------------------------------
  //                        
  // -------------------------------------------------------------------------------------------
  struct Vertex_pos_nrm_col_txc {
    glm::vec3 pos; 
    glm::vec3 nrm; 
    glm::vec3 col; 
    glm::vec2 txc; 
  };

  typedef Vertex_pos_nrm_col_txc PNCTx_Vert;
  extern const VkVertexInputBindingDescription        kPNCTx_InputBindingDesc;
  extern const Vec<VkVertexInputAttributeDescription> kPNCTx_InputAttributeDesc; 

  // -------------------------------------------------------------------------------------------
  //                        
  // -------------------------------------------------------------------------------------------
  struct Vertex_pos_nrm_txc {
    glm::vec3 pos; 
    glm::vec3 nrm; 
    glm::vec2 txc; 
  };
  
  typedef Vertex_pos_nrm_col_txc                      PNTx_Vert;
  extern const VkVertexInputBindingDescription        kPNTx_InputBindingDesc;
  extern const Vec<VkVertexInputAttributeDescription> kPNTx_InputAttributeDesc; 

  // -------------------------------------------------------------------------------------------
  //                        
  // -------------------------------------------------------------------------------------------
  struct Vertex_pos_col {
    glm::vec3 pos; 
    glm::vec3 col; 
  };

  typedef Vertex_pos_col                              PC_Vert; 
  extern const VkVertexInputBindingDescription        kPC_InputBindingDesc;
  extern const Vec<VkVertexInputAttributeDescription> kPC_InputAttributeDesc; 

  // -------------------------------------------------------------------------------------------
  //                        
  // -------------------------------------------------------------------------------------------
  struct Vertex_pos_txc {
    glm::vec3 pos; 
    glm::vec2 txc; 
  };

  typedef Vertex_pos_txc                              PTx_Vert; 
  extern const VkVertexInputBindingDescription        kPTx_InputBindingDesc;
  extern const Vec<VkVertexInputAttributeDescription> kPTx_InputAttributeDesc; 




} // rokz


#endif
