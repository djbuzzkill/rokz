#version 460


//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader
//

// ------- LANDSCAPE TESSELLATION CONTROL PROGRAM --------


layout(vertices = 4) out;
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
layout (set = 0, binding = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;                                             


// layout (set = 1, binding = 3) uniform PatchParams {
//     mat4 model;
//     vec4 unused0;
//     vec4 unused1;
// } params[128];                                             



// -----------------------------------------------------------------------------------------------
//  BUILTIN
// -----------------------------------------------------------------------------------------------
in gl_PerVertex {
  vec4 gl_Position; 
} gl_in[]; 

// -----------------------------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------------------------
out gl_PerVertex {
  vec4 gl_Position; 
} gl_out[]; 


// -----------------------------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------------------------
layout (location = 0) in vec2 in_txc[];

// -----------------------------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------------------------
layout (location = 0) out vec2 out_txc[];

//
void main ()
{
  float tess_val = 25.0;
  
  if (gl_InvocationID == 0) {

    gl_TessLevelInner[0] = tess_val;
    gl_TessLevelInner[1] = tess_val;

    gl_TessLevelOuter[0] = tess_val;
    gl_TessLevelOuter[1] = tess_val;
    gl_TessLevelOuter[2] = tess_val;
    gl_TessLevelOuter[3] = tess_val;
  }

  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; 
  out_txc[gl_InvocationID]           = in_txc[gl_InvocationID]; 

}
