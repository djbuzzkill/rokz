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
layout(location = 1) flat out int char_index;


// ----------------------------------------------------------------------------
// DESCRIPTOR                 
// ----------------------------------------------------------------------------
layout(binding = GLOBAL_MVP_OVERLAY_BINDINGI, set = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;

// -----------------------------------------------------------
void main() {

  vec4 hpos = vec4(in_pos, 1.0);

  hpos.x = hpos.x * pc.advance.x;
  hpos.y = hpos.y * pc.advance.x;
 
  hpos.xyz += pc.position.xyz; 
  
  hpos.x = hpos.x + pc.advance.x * gl_InstanceIndex;
  hpos.y = hpos.y + pc.advance.y ;
  // each gl_InstanceIndex is used as  string[gl_InstanceIndex]: char
  char_index = gl_InstanceIndex; 
  o_txc = in_txc;

  gl_Position = mat.proj * hpos;
}
