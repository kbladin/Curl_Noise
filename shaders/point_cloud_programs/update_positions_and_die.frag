#version 330 core

// Out data
out vec4 position_out;

// Uniform data
// From previous state
uniform sampler2D position_sampler_2D;
// From current state
uniform sampler2D velocity_sampler_2D;
uniform sampler2D acceleration_sampler_2D;
// Time step
uniform float dt;
// Properties of the particle system
uniform float inv_life_length_factor;
uniform float emitter_size;
uniform vec3  emitter_position;

// Random random function
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
  float age = p_tmp.w;

  // Euler integration
  vec3 delta_p = v * dt;
  vec3 new_pos = p + delta_p;

  // If particle is too old, reset position and decrease age
  new_pos = age >= 1 ?
    vec3(
      emitter_position.x + (rand(new_pos.xy) - 0.5) *             emitter_size,
      emitter_position.y + (rand(new_pos.yz) - 0.5) *             emitter_size,
      emitter_position.z + (rand(vec2(new_pos.z, age--)) - 0.5) * emitter_size)
    : new_pos;

  // Add age to particle
  age += dt * inv_life_length_factor * 0.1;
  // Write to output
  position_out = vec4(new_pos,age);
}
