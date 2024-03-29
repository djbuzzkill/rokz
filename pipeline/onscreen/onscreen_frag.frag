#version 460

#include "onscreen.h"
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

layout(location = 0) in vec2 txc;
layout(location = 1) flat in uint asciicode;

layout(location = 0) out vec4 ofrag;

// -------------- DESCRIPTORS ---------------
layout(set = 0, binding = GLOBAL_FONT_FACE_BINDINGI) uniform sampler2DArray glyphsamp;

// -------------- MAIN  ---------------
void main() {

  vec3 atxc = vec3 (txc.xy, asciicode); 
  ofrag = texture (glyphsamp, atxc).r * pc.color;

}

