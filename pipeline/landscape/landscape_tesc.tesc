#version 460


//        .vert - a vertex shader
//        .tesc - a tessellation control shader
//        .tese - a tessellation evaluation shader
//        .geom - a geometry shader
//        .frag - a fragment shader
//        .comp - a compute shader
//
// LANDSCAPE TESSELLATION CONTROL PROGRAM

layout(vertices = 4) out;

in gl_PerVertex 
{
	vec4 gl_Position; 

} gl_in[]; 

// these pass thru
out gl_PerVertex
{
	vec4 gl_Position; 

} gl_out[]; 


layout (location = 0) in vec2 in_txco[];



layout (location = 0) out vec2 out_txco[];

//
void main ()
{
	if (gl_InvocationID == 0) 
	{

	gl_TessLevelInner[0] = 2.2;
	gl_TessLevelInner[1] = 2.2;

	gl_TessLevelOuter[0] = 2.2;
	gl_TessLevelOuter[1] = 2.2;
	gl_TessLevelOuter[2] = 2.2;
	gl_TessLevelOuter[3] = 2.2;

	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; 
	out_txco[gl_InvocationID]           = in_txco[gl_InvocationID]; 

}
