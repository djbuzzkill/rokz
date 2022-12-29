#version 460

//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader


layout(binding = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;




layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_nrm; 
layout(location = 2) in vec3 in_co0; 
layout(location = 3) in vec2 in_txc; 



layout(push_constant) uniform push_constants {

   mat4 lights;
};




layout(location = 0) out vec3 o_frag; 
layout(location = 1) out vec3 o_norm; 
layout(location = 2) out vec2 o_txcd; 

void main() {
     
     gl_Position = mat.proj * mat.view * mat.model * vec4(in_pos, 1.0);
     o_frag = in_co0;
     o_norm = mat.model * in_nrm; 
     o_txcd = in_txc;

}
