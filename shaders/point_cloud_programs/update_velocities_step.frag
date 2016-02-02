#version 330 core

// From current state
uniform sampler2D velocity_sampler_2D;
// From previous state
uniform sampler2D acceleration_sampler_2D;

uniform float dt; // Time step

out vec4 velocity_out;

void main(){
  // Write output to fragment shader
  vec3 a = texelFetch( acceleration_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
	vec3 v = texelFetch( velocity_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;

	vec3 delta_v = a * dt;
	vec3 new_vel = v + delta_v;

	velocity_out = vec4(new_vel,1);
}
