#type vertex
#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;

out vec2 a_uv;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
	a_uv = a_Texcoord;
}

#type fragment
#version 460

in vec2 a_uv;
layout(location = 0) out vec4 finalColor;
layout(binding = 0) uniform samplerCube tex; 

uniform int a_int;
uniform float a_float;
uniform vec2 a_float2;
uniform vec3 a_float3;
uniform vec4 a_float4;
uniform mat3 a_mat3;
uniform mat4 a_mat4;


void main()
{
	//vec2 a_uv_n = a_uv * 2.0f - vec2(1.0f,1.0f);
	//vec3 normvec = normalize(vec3(a_uv_n.g,1.0f,a_uv_n.r));
	//finalColor = vec4(texture(tex,normvec).xyz,1.0f);
	//finalColor = vec4(a_uv.r,a_uv.g,0.6,1.0);
	vec3 color = vec3(1.0f,1.0f,1.0f);
	if (a_int == 1){
		color = color * a_float + a_float2.xyx + a_float3.xxx + a_float4.xxx + a_mat3.xxx + a_mat4.xxx;
	}
	finalColor = vec4(color/255.0f,1.0f);
}