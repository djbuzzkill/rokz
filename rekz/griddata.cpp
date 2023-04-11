
#include "rekz.h"

#include "grid_pipeline.h"
#include "rokz/buffer.hpp"
#include "rokz/rc_types.hpp"
using namespace rokz;

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
struct fillparams {

  uint32 totalverts   ;
  uint32 totalindices ;
  size_t reqsize      ;

  glm::uvec3 vertcount;
  glm::vec3  dimsize;

  glm::vec3 origin_co ; // (0.0, 0.4, 0.6);
  glm::vec3 xaxis_co  ; // (0.3, 0.5, 1.0);
  glm::vec3 zaxis_co  ; // (1.0, 0.4, 0.8);
};

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
struct fillgridbuffer : public cx::mappedbuffer_cb {

  size_t&           vertexoffset;
  size_t&           indexoffset;
  const fillparams& p;
  //
  fillgridbuffer (size_t& voffs, size_t& ioffs, const fillparams& params)
    : p(params) , vertexoffset (voffs), indexoffset(ioffs) {
  }
  //
  virtual int on_mapped  (void* mappedp, size_t maxsize) {

    byte* ucp = (byte*) mappedp;

    uint32 totalvertsize  = p.totalverts * sizeof(rekz::grid::Vert);
    uint32 totalindexsize = 2 * p.totalverts * sizeof(uint16);

    vertexoffset = 0;
    indexoffset  = totalvertsize;
    
    float xstep = float(p.dimsize.x) / float(p.vertcount.x); 
    float zstep = float(p.dimsize.z) / float(p.vertcount.z); 

    float dx = 1.0f / float (p.vertcount.x - 1);
    float dz = 1.0f / float (p.vertcount.z - 1);
  
    const glm::vec3 voffs (-p.dimsize.x * 0.5f, 0.0f, -p.dimsize.z * 0.5f);

    rekz::grid::Vert* verts = reinterpret_cast<rekz::grid::Vert*> (ucp); 
    uint16*           inds  = reinterpret_cast<uint16*> (ucp + totalvertsize);

    // -- vertices --
    for (uint16_t iz = 0; iz < p.vertcount.z; ++iz) {
      for (uint16_t ix = 0; ix < p.vertcount.x; ++ix) {
        verts[iz * p.vertcount.x + ix].pos = glm::vec3 (ix * xstep, 0.0f, iz * zstep) + voffs;
        verts[iz * p.vertcount.x + ix].col = glm::mix (p.origin_co, p.zaxis_co, ix * dx); 
      }
    } // move 2 vb

    // -- indices --
    for (uint16_t iz = 0; iz < p.vertcount.z; ++iz) { // draw x lines
      for (uint16_t ix = 0; ix < p.vertcount.x; ++ix) { 
        inds[iz * p.vertcount.x + ix] = iz * p.vertcount.x + ix;   
      }}

    for (uint32_t ix = 0; ix < p.vertcount.x; ++ix) {  // draw z lines
      for (uint32_t iz = 0; iz < p.vertcount.z; ++iz) { 
        inds[p.totalverts + ix * p.vertcount.z + iz] = iz * p.vertcount.x + ix; 
      }
    } // move

    return 0;

  }

};

// ----------------------------------------------------------------------------------------------
//                                                
// ----------------------------------------------------------------------------------------------
rc::Buffer::Ref rekz::SetupGridData (size_t& vertoffset, size_t& indoffset, 
                                     uint32 xvertcount, uint32 zvertcount,
                                     float xsize, float zsize,
                                     const Device& device) {
  // v.y is ignored mostly
  const uint32 totalverts   = xvertcount *  zvertcount;
  const uint32 totalindices = 2 * totalverts;
  const uint32 reqsize      = totalverts*sizeof(rekz::grid::Vert) + totalindices*sizeof(uint16); 

  fillparams params = {
    totalverts, 
    totalindices,
    reqsize,

    glm::uvec3 (xvertcount, 0, zvertcount),  // num verts in a direction
    glm::fvec3 (xsize, 0.0f, zsize),         //in world units

    glm::vec3 (0.0, 0.4, 0.6), // origin color
    glm::vec3 (0.3, 0.5, 1.0), // x color
    glm::vec3 (1.0, 0.4, 0.8), // zcolor
  }; 

  rc::Buffer::Ref buf = rc::CreateDeviceBuffer (reqsize, cx::kDeviceGeometryUsage, device);

  int res = cx::TransferToDeviceBuffer (buf->handle, reqsize,
                                        std::make_shared <fillgridbuffer> (vertoffset, indoffset, params),
                                        device); 

  if (res != 0) {
    return rc::Buffer::Ref(nullptr); 
  }

  //const uint16_t vertdim    = 11;

  return buf; 
}

