#version 330 core

layout(location = 0) in vec2 index;

uniform sampler2D accelerationSampler2D;
uniform sampler2D velocitySampler2D;
uniform sampler2D positionSampler2D;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 a;
out vec3 v;
out vec3 p;
out float t; // Life time of particle

out float radius;

void main(){
	a = texelFetch( accelerationSampler2D, ivec2(index), 0).xyz;
	v = texelFetch( velocitySampler2D, ivec2(index), 0).xyz;
	vec4 p_tmp = texelFetch( positionSampler2D, ivec2(index), 0);
	p = p_tmp.xyz;
	t = p_tmp.a;
	vec4 pos_cam_space = V * M * vec4(p ,1);
	gl_Position = P * pos_cam_space;
	radius = 1;//  5 / (-pos_cam_space.z);
	gl_PointSize = radius;
}