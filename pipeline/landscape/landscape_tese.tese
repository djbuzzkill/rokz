#version 460

//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader




// -- LANDSCAPE TESSELLATION EVALUATION PROGRAM -- 

//#extension GL_EXT_nonuniform_qualifier : enable



layout(quads, equal_spacing, cw) in;
// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
layout (location = 0) in vec4 in_position[];
layout (location = 1) in vec2 in_txcrd[]; 


// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
layout (location = 0) out vec2 out_txcd; 


// -----------------------------------------------------------------------------------------------
// uniforms
// -----------------------------------------------------------------------------------------------
layout (set = 0, binding = 0) uniform MVPTransform {
    mat4 model;   // !! model = rotm * scalem + translm 
    mat4 view;    // 
    mat4 proj;    // 
} mat;                                             


// -----------------------------------------------------------------------------------------------
// samplers  
// -----------------------------------------------------------------------------------------------
layout (set = 1, binding = 0) uniform sampler2D heightmap[128];                                                  
layout (set = 1, binding = 1) uniform sampler2D normalmap[128];                                                  

// 
// -----------------------------------------------------------------------------------------------
// push constants
// -----------------------------------------------------------------------------------------------
layout (push_constant) uniform PatchPushConstants {

  vec3 position;
  vec4 scale;

  uint res_id;
  
} pc;


//
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

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
void main() {
  
    out_txcd = interpolate2 (in_txcrd[0], in_txcrd[1], in_txcrd[2], in_txcrd[3] ); 		
    vec4 pos = interpolate4 (in_position[0], in_position[1], in_position[2], in_position[3]);

    pos.x = pc.scale.x * pos.x; 
    pos.y = pc.scale.y * texture (heightmap[pc.res_id], out_txcd).r;
    pos.z = pc.scale.y * pos.z; 

    pos.xyz = pos.xyz + pc.position;
    
    gl_Position = mat.proj * mat.view * mat.model * pos;                                                    
}				

 
