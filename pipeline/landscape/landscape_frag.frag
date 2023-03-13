#version 460
//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

// LANDSCAPE FRAGMENT PROGRAM
                                                   
const vec3 mars_color = vec3 (1.0, 0.8, 0.6);


layout (binding = 1) uniform sampler2D color_map;

layout(location = 0) out vec4          out_color;                    

layout(location = 1) in vec2  in_txcd;                       
layout(location = 0) in vec3  in_color;

void main ()                                       
{
	out_color.rgb	= texture (color_map, in_txcd).r * mars_color;
	out_color.a	= 1.0; 

        gl_Position = 
}                                                  
