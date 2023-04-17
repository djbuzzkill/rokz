
#include "rekz.h"


using namespace rekz;

// -- each face will not share normals
const  size_t kCubeVertexCount = 24;

const geomz::Vert kCubeVerts[kCubeVertexCount] = {
  // top 0
  {{-1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 }, {0.0, 1.0}}, 
  {{-1.0, 1.0,  1.0 }, { 0.0, 1.0, 0.0 }, {0.0, 0.0}}, 
  {{ 1.0, 1.0,  1.0 }, { 0.0, 1.0, 0.0 }, {1.0, 0.0}}, 
  {{ 1.0, 1.0, -1.0 }, { 0.0, 1.0, 0.0 }, {1.0, 1.0}}, 

  // bottom 4
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 

  // font 8
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 

  // back 12
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 

  // right 16
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 

  // left 20
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 
    
}; 

const uint16 kCubeIndices_CCW[] = {
  0, 1, 2,  2, 3, 0, // top
  0, 1, 2,  2, 3, 0, // bott

  0, 1, 2,  2, 3, 0, // left
  0, 1, 2,  2, 3, 0, // right

  0, 1, 2,  2, 3, 0, // front 
  0, 1, 2,  2, 3, 0, // back 

}; 

const uint16 kCubeIndicesCount = sizeof(kCubeIndices_CCW) / sizeof(uint16); 
// -------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------
const size_t kOctohedronVertexCount = 6;
const geomz::Vert OctohedronVerts[kOctohedronVertexCount] = {
  // top 
  {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.5, 0.5}}, 

  {{ 0.0, 0.0,  1.0 }, {-0.707106, 0.0,  0.707106}, {0.0, 1.0}}, 
  {{ 1.0, 0.0,  0.0 }, { 0.707106, 0.0,  0.707106}, {1.0, 1.0}}, 
  {{ 0.0, 0.0, -1.0 }, { 0.707106, 0.0, -0.707106}, {1.0, 0.0}}, 
  {{-1.0, 0.0,  0.0 }, {-0.707106, 0.0, -0.707106}, {0.0, 0.0}}, 

  // bottom
  {{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, {0.5, 0.5 }}, 
    
}; 

// -------------------------------------------------------------------------------------
static_assert (kOctohedronVertexCount == 6, "");

// CW
const uint16_t OctohedronIndices[] = {
  0, 2, 1,  0, 3, 2,  0, 4, 3,  0, 1, 4,
  5, 1, 2,  5, 2, 3,  5, 3, 4,  5, 4, 1
}; 

const std::array<uint16_t, 24> OctohedronIndices16[] = {
  0, 2, 1,  0, 3, 2,  0, 4, 3,  0, 1, 4,
  5, 1, 2,  5, 2, 3,  5, 3, 4,  5, 4, 1
}; 

// CCW 
// const uint16_t OctohedronIndices[] = {
//   0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,
//   5, 2, 1,  5, 3, 2,  5, 4, 3,  5, 1, 4
// }; 

const size_t kOctohedronIndicesCount = sizeof(OctohedronIndices) / sizeof (uint16_t); 
static_assert (kOctohedronIndicesCount == 24, "");


struct CubeMesh : public geomz::Mesh {

    CubeMesh () : geomz::Mesh (kCubeVerts, kCubeVertexCount, kCubeIndices_CCW, kCubeIndicesCount) {
    }
  }; 


struct OctoMesh : public geomz::Mesh {

  OctoMesh () : geomz::Mesh (OctohedronVerts, kOctohedronVertexCount,
                             OctohedronIndices,  kOctohedronIndicesCount) {
  }
}; 

const CubeMesh kCubeMesh; 
const OctoMesh kOctoMesh;

const std::array<geomz::Mesh, rekz::geomz::PT::NUM_GEOM_TYPES> rekz::geomz::Geomz = {
  geomz::Mesh (kCubeVerts, kCubeVertexCount, kCubeIndices_CCW, kCubeIndicesCount),
  geomz::Mesh (OctohedronVerts, kOctohedronVertexCount, OctohedronIndices, kOctohedronIndicesCount)
}; 
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------

