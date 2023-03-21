
#include "rekz.h"

#include "grid_pipeline.h"
using namespace rokz;



// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
bool rekz::SetupGridData (GridData& gd, const Device& device) {

  void*  mem = nullptr;
  size_t szmem = 0;

  const uint16_t vertdim    = 11;
  const uint16_t totalverts = vertdim * vertdim;
  const float    dimsize    = 20.0f;
  const float    dimstep    = float (dimsize) / float (vertdim - 1);

  glm::vec3 zaxis_co  (1.0, 0.4, 0.8);
  glm::vec3 xaxis_co  (0.3, 0.5, 1.0);
  glm::vec3 origin_co (0.0, 0.4, 0.6);

  float dx = 1.0f / float (vertdim);
  
  const glm::vec3 voffs (-dimsize * 0.5f, 0.0f, -dimsize * 0.5f);

  Vec<grid::Vert> verts (vertdim * vertdim);
  Vec<uint16_t> inds (2 * totalverts);

  // -- vertices --
  for (uint16_t iz = 0; iz < vertdim; ++iz) {
    for (uint16_t ix = 0; ix < vertdim; ++ix) {
      verts[iz * vertdim + ix].pos = glm::vec3 (ix * dimstep, 0.0f, iz * dimstep) + voffs;
      verts[iz * vertdim + ix].col = glm::mix (origin_co, zaxis_co, ix * dx); 

    }
  } // move 2 vb
  Create_VB_device (gd.vb_device, &verts[0], verts.size () * sizeof(grid::Vert), device); 

  // -- indices --
  for (uint16_t iz = 0; iz < vertdim; ++iz) { // draw x lines
    for (uint16_t ix = 0; ix < vertdim; ++ix) { 
      inds[iz * vertdim + ix] = iz * vertdim + ix;   
    }}

  for (uint32_t ix = 0; ix < vertdim; ++ix) {  // draw z lines
    for (uint32_t iz = 0; iz < vertdim; ++iz) { 
      inds[totalverts + ix * vertdim + iz] = iz * vertdim + ix; 
    }
  } // move
  Create_IB_16_device (gd.ib_device, &inds[0], inds.size (), device); 
  
  return true; 
}


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
void rekz::CleanupGridData (GridData& gd, const Device& device) {

  rokz::cx::Destroy (gd.vb_device, device.allocator); 
  rokz::cx::Destroy (gd.ib_device, device.allocator); 
  
}

