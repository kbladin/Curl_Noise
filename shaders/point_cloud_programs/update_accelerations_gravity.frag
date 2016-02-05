#version 330 core

// Output data
out vec4 acceleration_out;

// Uniform data
// All attribures are from previous state
uniform sampler2D position_sampler_2D;
uniform sampler2D velocity_sampler_2D;
uniform sampler2D acceleration_sampler_2D;
// Particle system behavior properties
uniform vec3 emitter_position;
uniform float dt;
uniform int size;

float smoothstep(float edge0, float edge1, float x)
{
  // Scale, bias and saturate x to 0..1 range
  x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0); 
  // Evaluate polynomial
  return x*x*(3 - 2*x);
}

void main(){
  // Get the motion attributes of the particle
  vec3 a = texelFetch( acceleration_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
  vec3 v = texelFetch( velocity_sampler_2D,     ivec2(gl_FragCoord.xy), 0).xyz;
  vec3 p = texelFetch( position_sampler_2D,     ivec2(gl_FragCoord.xy), 0).xyz;

  // Distance to the emitter (attractor)
  vec3 center_diff = emitter_position - p;
  float center_dist = length(center_diff);
  
  // Set the acceleration towards the attractor
  a = normalize(center_diff) / pow(center_dist,2);
  
  // Decrease acceleration
  float max_norm = 1;
  float acceleration_norm = max(max_norm, length(a));
  
  // Add resistance
  a -= v;
  a /= acceleration_norm;

  // Resistance from sphere
  a += (1 - smoothstep(0.5, 0.5 + 0.1, length(p))) * normalize(p);

  // Write output
  acceleration_out = vec4(a.xyz,1);
}
