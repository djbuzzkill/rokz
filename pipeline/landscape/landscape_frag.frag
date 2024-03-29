#version 460

#include "lscape.h"
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

// ---------- LANDSCAPE FRAGMENT PROGRAM -----------

// -----------------------------------------------------------------------------------------------
// push constants
// -----------------------------------------------------------------------------------------------
layout (push_constant) uniform PatchPushConstant {

  vec4 position;
  vec4 scale;
  uint res_id;
  uint _unused3;
  uint _unused4;
  uint _unused5;
  
} pc;

// -----------------------------------------------------------------------------------------------
// descriptors
// -----------------------------------------------------------------------------------------------
layout (set = 0, binding = COLOR_IMAGE_BINDINGI) uniform sampler2D colorsamp[128];


// layout (set = 1, binding = 3) uniform PatchParams {
//     mat4 model;
//     vec4 unused0;
//     vec4 unused1;
// } params[128];                                             



// -----------------------------------------------------------------------------------------------
// input
// -----------------------------------------------------------------------------------------------
layout(location = 0) in vec2  in_txcd;                       


// -----------------------------------------------------------------------------------------------
// output
// -----------------------------------------------------------------------------------------------
layout(location = 0) out vec4 out_color;                    



void main () {

  vec4 colmul = vec4 (0.8, 0.2, 0.1, 1.0);

  out_color.rgb   = colmul.xyz * texture (colorsamp[pc.res_id], in_txcd).rrr;
  out_color.a = 1.0;
}                                                  
