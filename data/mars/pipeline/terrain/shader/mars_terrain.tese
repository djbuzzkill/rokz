#version 460

//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader
// MARS TESSELLATION EVALUATION PROGRAM

layout(quads, equal_spacing, cw) in;


#extension GL_EXT_nonuniform_qualifier : enable




layout (location = 0) in vec4 in_position[];
layout (location = 1) in vec2 in_txcrd[]; 


//layout (location = 0) out vec4 gl_Position; 
layout (location = 0) out vec2 out_txcrd; 


////
// uniform constants 
layout (binding = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;                                             


layout (binding = 1) uniform PatchParams {
    mat4 model;
    vec4 height_scale;                                               

vec4 unused1;

} params[];                                             


layout (binding = 2) uniform sampler2D height_map[];                                                  


//
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
// ?? gl_PrimitiveID == gl_InstanceIndex ??
// 
// -------------------------------------------------------------------------
void main() {

    vec2 tex_coord = interpolate2 (in_txcrd[0], in_txcrd[1], in_txcrd[2], in_txcrd[3] ); 		
    vec4 pos = interpolate4 (in_position[0], in_position[1], in_position[2], in_position[3]);

    pos.z = params[gl_PrimitiveID].height_scale.x * texture (height_map[gl_PrimitiveID], tex_coord).r;
    pos = params[gl_PrimitiveID].model * pos;                                                   
    pos = mat.view * pos;                                                    
    gl_Position = mat.proj * pos;
}				

