#version 460

//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

layout (push_constant) uniform PushConstants {
  ivec4 draw_ids; 

} pc;

layout(location = 0) in vec3 frag_color;
layout(location = 1) in vec3 frag_norm;
layout(location = 2) in vec2 txcrd2;


layout(location = 0) out vec4 out_color;

layout(binding = 2) uniform sampler2D tx_sampler;

void main() {
     
     out_color = texture (tx_sampler, txcrd2);

}





