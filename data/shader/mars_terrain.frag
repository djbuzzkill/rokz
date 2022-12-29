#version 460



//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader

// MARS FRAGMENT PROGRAM
                                                   
const vec3 mars_color = vec3 (1.0, 0.8, 0.6);

uniform sampler2D colorMap;                        
in vec2           frag_txcd;                       
out vec4          frag_color;                    

void main ()                                       
{
	frag_color.rgb	= texture2D (colorMap, frag_txcd).r * mars_color;
	frag_color.a	= 1.0; 
}                                                  
