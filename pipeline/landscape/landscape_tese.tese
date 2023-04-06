#version 460


#include "lscape.h"
//  .vert - a vertex shader
//  .tesc - a tessellation control shader
//  .tese - a tessellation evaluation shader
//  .geom - a geometry shader
//  .frag - a fragment shader
//  .comp - a compute shader

// ------ LANDSCAPE TESSELLATION EVALUATION PROGRAM --------
//#extension GL_EXT_nonuniform_qualifier : enable


layout(quads, equal_spacing, cw) in;
// -----------------------------------------------------------------------------------------------
// incoming
// -----------------------------------------------------------------------------------------------
//layout (location = 0) in vec4 in_position[];
layout (location = 0) in vec2 in_txcrd[]; 

in gl_PerVertex {
  vec4 gl_Position; 
} gl_in[]; 



// -------------------------------------------------------------------------
//  outgoing
// -------------------------------------------------------------------------
out gl_PerVertex {
  vec4 gl_Position; 
}; 

layout (location = 0) out vec2 out_txcd; 



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
// uniform constants 
layout (set = 0, binding = GLOBAL_MVP_SCENE_BINDINGI) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;                                             


// set 1 
layout (set = 0, binding = HEIGHT_IMAGE_BINDINGI) uniform sampler2D heightmap[128];                                                  
// not used yet
//layout (set = 0, binding = NORMAL_IMAGE_BINDINGI) uniform sampler2D normalmap[128];                                                  

// layout (set = 1, binding = PATCH_PARAMS_BINDINGI) uniform PatchParams {
//     mat4 model;
//     vec4 unused0;
//     vec4 unused1;
// } params[128];                                             
// // 

//
// -----------------------------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------------------------
vec2 interpolate2 (in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3) {
	vec2 a = mix(v0, v1, gl_TessCoord.x);
	vec2 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

//quad interpol
vec4 interpolate4(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3) {
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

// -----------------------------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------------------------
void main() {
  
    out_txcd = interpolate2 (in_txcrd[0], in_txcrd[1], in_txcrd[2], in_txcrd[3] ); 		
   //vec4 pos = interpolate4 (in_position[0], in_position[1], in_position[2], in_position[3]);
    vec4 pos = interpolate4 (gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);

    pos.x = pc.scale.x * pos.x; 
    pos.y = pc.scale.y * texture (heightmap[pc.res_id], out_txcd).r;
    pos.z = pc.scale.z * pos.z; 

    pos.xyz = pos.xyz + pc.position.xyz;
    
    gl_Position = mat.proj * mat.view * pos; 
    //gl_Position = mat.proj * mat.view * params[pc.res_id].model * pos; 
    //gl_Position = mat.proj * mat.view * mat.model * pos;  
}				
