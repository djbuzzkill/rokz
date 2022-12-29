#version 460


//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader
layout(location = 0) out vec4 out_color;

layout(location = 0) in  vec3 frag_colo;r
layout(location = 1) in  vec3 frag_norm;
layout(location = 2) in  vec2 frag_txcrd;


layout(push_constant) uniform push_constants {

mat4 lights;
 
};



void main() {

     out_color = vec4(frag_color, 1.0);

}

