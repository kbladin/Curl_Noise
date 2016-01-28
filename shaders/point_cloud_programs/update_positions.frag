#version 330 core

// From previous state
uniform sampler2D position_sampler_2D;
// From current state
uniform sampler2D velocity_sampler_2D;
uniform sampler2D acceleration_sampler_2D;

uniform float dt; // Time step

// Properties of the particle system
uniform float inv_life_length_factor;
uniform float emitter_size;
uniform vec3 emitter_position;

out vec4 position_out;

// Random random function..
float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
  // Write output to fragment shader
  vec3 a = texelFetch( acceleration_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
  vec3 v = texelFetch( velocity_sampler_2D,     ivec2(gl_FragCoord.xy), 0).xyz;
  
  // The lifetime is stored in the fourth element of position
  vec4 p_tmp = texelFetch( position_sampler_2D, ivec2(gl_FragCoord.xy), 0);
  vec3 p = p_tmp.xyz;
  float t = p_tmp.a;

  // Euler integration
  vec3 delta_p = v * dt;
  vec3 new_pos = p + delta_p;

  // If particle is too old, reset position and give it a new life length
  if(t >= 1)
  {
    new_pos.x = emitter_position.x + (fract(new_pos.x - 0.5) - 0.5) * emitter_size;
    new_pos.y = emitter_position.y + (fract(new_pos.y - 0.5) - 0.5) * emitter_size;
    new_pos.z = emitter_position.z + (fract(new_pos.z - 0.5) - 0.5) * emitter_size;
    t = rand(new_pos.xy * 17);
  }

  // Add age to particle
  t += dt * inv_life_length_factor * 0.1;
  // Write to output
  position_out = vec4(new_pos,t);
}
