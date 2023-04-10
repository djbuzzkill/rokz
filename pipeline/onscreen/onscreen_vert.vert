#version 460
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

// +vertex+shader+
//
// #extension GL_ARB_shading_language_include : require
// #extension GL_EXT_nonuniform_qualifier : enable

#include  "onscreen.h"

// ----------------------------------------------------------------------------
// IN PER VERTEX
// ----------------------------------------------------------------------------
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_txc; 

// ----------------------------------------------------------------------------
// out PER VERTEX
// ----------------------------------------------------------------------------
layout(location = 0) out vec2 o_txc; 
layout(location = 1) flat out uint asciicode;

// ----------------------------------------------------------------------------
// DESCRIPTOR                 
// ----------------------------------------------------------------------------
layout(binding = GLOBAL_MVP_OVERLAY_BINDINGI, set = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;


// layout(binding = 69, set = 0) uniform  textelement[64];

// -------------------------------------------------------------------------
//  outgoing
// -------------------------------------------------------------------------
out gl_PerVertex {
  vec4 gl_Position; 
}; 

// -----------------------------------------------------------
void main() {
  //const float multf = ;
  vec4 hpos = vec4(in_pos, 1.0);
  hpos.xy = 16.0 * hpos.xy;
  //hpos.xy = 64.0 * hpos.xy;
  hpos.xyz += pc.position.xyz;   
  //hpos.xyz += vec3(0.0, -64.0, -1.0); ;   

  hpos.x = hpos.x + pc.advance.x * pc.resource_id;
  hpos.y = hpos.y + pc.advance.y ; 

  // out
  asciicode   = pc.asciicode; // string[charindex] : ascii code
  o_txc       = in_txc;           // o_txc = txcs[gl_VertexIndex];

  gl_Position = mat.proj * hpos;
}
