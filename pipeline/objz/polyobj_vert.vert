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

#include  "polyobj.h"
//
// IN PER VERTEX
//
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_nrm; 
layout(location = 2) in vec2 in_txc; 
//
// out PER VERTEX
//
layout(location = 0) out vec3 o_norm; 
layout(location = 1) out vec2 o_txcd; 

//
//  
//

//
// DESCRIPTOR
//
layout(set = 0, binding = GLOBAL_MVP_SCENE_BINDINGI) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;

layout(set = 0, binding = OB_PARAMS_BINDINGI) uniform ObjectParams {

  mat4 model;
  
} params[max_count];


void main() {
  //  gl_Position = mat.proj * mat.view * mat.model * vec4(in_pos, 1.0);
  //  o_norm = (mat.model * vec4(in_nrm, 1.0)).xyz;
  gl_Position = mat.proj * mat.view * params[pc.resource_id.x].model * vec4(in_pos, 1.0);
  o_norm = (params[pc.resource_id.x].model * vec4(in_nrm, 1.0)).xyz; 
  o_txcd = in_txc;
}
