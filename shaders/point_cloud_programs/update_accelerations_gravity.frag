#version 330 core

// All attribures are from previous state
uniform sampler2D position_sampler_2D;
uniform sampler2D velocity_sampler_2D;
uniform sampler2D acceleration_sampler_2D;

uniform vec3 emitter_position;
uniform float dt;
uniform int size;
out vec4 acceleration_out;

void main(){
	vec3 a = texelFetch( acceleration_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
	vec3 v = texelFetch( velocity_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
	vec3 p = texelFetch( position_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;

	vec3 center_diff = emitter_position - p;
	float center_dist = length(center_diff);
	float G = 1;
	float m = 1;
	vec3 F = G * normalize(center_diff) / pow(center_dist,2);
	
	a = F / m;
	float max_norm = 1;
	float acceleration_norm = max(max_norm, length(a));
	a -= v;
	a /= acceleration_norm;

    acceleration_out = vec4(a.xyz,1);
}
