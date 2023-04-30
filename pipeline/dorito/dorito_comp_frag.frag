#version 460

#include "dusty.h"
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

layout(location = 0) in vec3 frag_norm;
layout(location = 1) in vec2 txcrd2;



layout(set = 0, binding = OB_TEXTURES_BINDINGI) uniform
                                                sampler2D tex_sampler[max_count];

layout (location = 0) out vec4 out_color;

void main() {
     
  out_color = texture (tex_sampler[pc.resource_id.x], txcrd2);

}

