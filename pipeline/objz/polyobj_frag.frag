#version 460

//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

layout(location = 0) in vec3 frag_color;
layout(location = 1) in vec3 frag_norm;
layout(location = 2) in vec2 txcrd2;


layout (push_constant) uniform PushConstants {

  ivec4 resourceIDs; 

} pc;


layout(binding = 1, set = 1) uniform sampler2D tex_sampler;


layout(location = 0) out vec4 out_color;

void main() {
     
     out_color = texture (tex_sampler, txcrd2);

}





