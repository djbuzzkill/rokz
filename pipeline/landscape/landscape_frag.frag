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
layout(location = 0) in vec2  in_txcd;                       

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
layout (set = 1, binding = 2) uniform sampler2D colorsamp[128];


layout (set = 1, binding = 3) uniform PatchParams {
    mat4 model;
    vec4 unused0;
    vec4 unused1;
} params[128];                                             

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
layout(location = 0) out vec4 out_color;                    

// -----------------------------------------------------------------------------------------------
// push constants
// -----------------------------------------------------------------------------------------------
layout (push_constant) uniform PatchPushConstant {

  vec4 position;
  vec4 scale;
  uint res_id;
  
} pc;



void main () {

  out_color = texture (colorsamp[pc.res_id], in_txcd);

}                                                  
