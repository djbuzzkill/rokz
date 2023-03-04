1#version 460
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


//
// IN PER VERTEX
//
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_nrm; 
layout(location = 2) in vec3 in_co0; 
layout(location = 3) in vec2 in_txc; 

//
// out PER VERTEX
//
layout(location = 0) out vec3 o_frag; 
layout(location = 1) out vec3 o_norm; 
layout(location = 2) out vec2 o_txcd; 

//
//  
//
layout (push_constant) uniform PushConstants {

  ivec4 draw_ids; 

} pc;

//
// DESCRIPTOR
//
layout(binding = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;


layout(binding = 1) uniform ObjectParams {

  // 
  mat4 model;
  //
  
} params[100];


void main() {

  //  gl_Position = mat.proj * mat.view * mat.model * vec4(in_pos, 1.0);
  //  o_norm = (mat.model * vec4(in_nrm, 1.0)).xyz; 

  gl_Position = mat.proj * mat.view * params[pc.draw_ids.x].model * vec4(in_pos, 1.0);
  o_norm = (params[pc.draw_ids.x].model * vec4(in_nrm, 1.0)).xyz; 
  o_frag = in_co0;
  o_txcd = in_txc;
}
