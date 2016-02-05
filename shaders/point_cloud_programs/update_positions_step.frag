#version 330 core

// Out data
out vec4 position_out;

// Uniform data
// From previous state
uniform sampler2D position_sampler_2D;
// From current state
uniform sampler2D velocity_sampler_2D;
uniform float dt; // Time step

void main(){
  // Write output to fragment shader
  vec3 v = texelFetch( velocity_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
  // The lifetime is stored in the fourth element of position
  vec4 p_tmp = texelFetch( position_sampler_2D, ivec2(gl_FragCoord.xy), 0);
  vec3 p = p_tmp.xyz;
  float t = p_tmp.a;
 
  // Euler integration 
  vec3 delta_p = v * dt;
  vec3 new_pos = p + delta_p;

  // Write output
  position_out = vec4(new_pos,t);
}
