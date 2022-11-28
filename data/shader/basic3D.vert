#version 450


layout(binding = 0) uniform BasicTransform {
    mat4 model;
    mat4 view;
    mat4 proj;
} mat;


layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec3 in_co0; 

layout(location = 0) out vec3 o_frag; 

void main() {

  gl_Position = mat.proj * mat.view * mat.model * vec4(in_pos, 0.0, 1.0);
  o_frag = in_co0;
}
