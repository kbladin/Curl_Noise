#version 330 core

// From previous state
uniform sampler2D positionSampler2D;
// From current state
uniform sampler2D velocitySampler2D;
uniform sampler2D accelerationSampler2D;

uniform float dt;

// Properties of the particle system
uniform float inv_life_length_factor;
uniform float emission_area_factor;

out vec4 position_out;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	vec3 a = texelFetch( accelerationSampler2D, ivec2(gl_FragCoord.xy), 0).xyz;
	vec3 v = texelFetch( velocitySampler2D, ivec2(gl_FragCoord.xy), 0).xyz;
	vec4 tmp_p = texelFetch( positionSampler2D, ivec2(gl_FragCoord.xy), 0);
	vec3 p = tmp_p.xyz;
	float t = tmp_p.a;
	// Euler integration
	vec3 delta_p = v * dt;
	vec3 new_pos = p + delta_p;

	//new_pos.x = new_pos.x < -1 ? 1 : (new_pos.x > 1 ? -1 : new_pos.x); 
	//new_pos.y = (t >= 1 ? -2 : new_pos.y); 
	//new_pos.z = new_pos.z < -1 ? 1 : (new_pos.z > 1 ? -1 : new_pos.z); 

	if(t >= 1){
		new_pos.x = (fract(new_pos.x - 0.5) - 0.5) * emission_area_factor;
		new_pos.y = -2 + (fract(new_pos.y - 0.5) - 0.5) * emission_area_factor;
		new_pos.z = (fract(new_pos.z - 0.5) - 0.5) * emission_area_factor;
		t = rand(new_pos.xy * 17);
	}

	t += dt * inv_life_length_factor * 0.01;
    position_out = vec4(new_pos,t);
}
