#version 460
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

// LANDSCAPE FRAGMENT PROGRAM
                                                   
// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
const vec3 mars_color = vec3 (1.0, 0.8, 0.6);


// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
layout(location = 1) in vec2  in_txcd;                       
layout(location = 0) in vec3  in_color;

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
layout (set = 1, binding = 2) uniform sampler2D colorsamp[128];
// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
layout(location = 0) out vec4 out_color;                    

// -----------------------------------------------------------------------------------------------
// push constants
// -----------------------------------------------------------------------------------------------
layout (push_constant) uniform PatchPushConstants {

  uint heightID;  // indices
  uint normalID;  // indices
  uint colorID; // indices
  uint _unused03; // indices

} pc;



void main () {

  out_color = texture (colorsamp[pc.colorID], in_txcd);

}                                                  
