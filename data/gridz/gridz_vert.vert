#version 450



// input 
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_col;



// descriptors: uniform, sampler, texel buff
layout(binding = 0, set = 0) uniform MVPTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;

//
//  
//
layout (push_constant) uniform GridPushConstants {

    vec4 x_color;  // 10 is reasonable
    vec4 z_color;  // 10 is reasonable
    vec4 origin_color; 

    float xstep; // size every 'column'
    float zstep; // size every 'row'
    float xoffset;
    float zoffset;

} pc;


// out to next stage
layout(location = 0) out vec4 o_frag; 


void main() {

  gl_Position = mat.proj * mat.view * mat.model * vec4(in_pos, 1.0);
  o_frag      = vec4 (in_col, 1.0);
}
