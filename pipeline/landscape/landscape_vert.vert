#version 460
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

// LANDSCAPE VERTEX PROGRAM


// input vertex 
layout(location=0) in vec3 in_pos;                                                        
layout(location=1) in vec3 in_norm;                                                        
layout(location=2) in vec2 in_txcd;                                                        

// output vertex 
//layout (location = 0) out vec4 gl_Position; 
layout (location = 0) out vec2 out_col0;                                                   
layout (location = 1) out vec2 out_txco;                                                   


////
// uniform constants 
layout (binding = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;                                             

layout (binding = 1) uniform PatchParams {
    mat4 model;
    vec4 unused0;
    vec4 unused1;

} params[100];                                             

////

// 
void main () {
  //vec4 pos	= vec4 (attrib_position, 1); 
  //pos			= mat_Model * pos;                                                   
  //pos			= mat_View * pos; 
  //gl_Position = mat_Proj * pos; //gl_Position = vec4 (attrib_position, 1);
  //txco		= attrib_texcoord;                                                   

  gl_Position = vec4 (in_pos, 1);
  out_txco    = in_txcd;
  
}


