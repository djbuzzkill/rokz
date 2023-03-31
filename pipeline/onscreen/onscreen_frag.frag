#version 460

#include "onscreen.h"
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

layout(location = 0) in vec2 txc;
layout(location = 1) flat in int chindex;


struct PushConstant {

  uint resource_id;
  uint _unused_1;
  uint _unused_2;
  uint _unused_3;

  vec4 color;
  vec2 advance;
  vec2 position; 

} pc;


layout (binding = 0, set =1) uniform UBText {
  uint text[64];
} str_elem[max_count]; 


layout(binding = 1, set = 1) uniform sampler2DArray glyphsamp;


layout(location = 0) out vec4 ofrag;


void main() {

  vec3  atxc = vec3 ( txc.xy,  str_elem[pc.resource_id].text[chindex] ); 
  
  ofrag.rgb = pc.color.rgb;
  ofrag.a   = texture (glyphsamp, atxc).r;

}

