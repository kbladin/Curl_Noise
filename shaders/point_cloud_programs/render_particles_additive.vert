#version 330 core

// In data
// Index to pick the current particle
layout(location = 0) in vec2 index;

// Out data
out float age;

// Uniform data
// Acceleration, velocity and position of previous time step
uniform sampler2D acceleration_sampler_2D;
uniform sampler2D velocity_sampler_2D;
uniform sampler2D position_sampler_2D;
// Transform matrices
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
// Particle rendering properties
uniform float particle_radius;

void main(){
  vec4 p_tmp = texelFetch( position_sampler_2D, ivec2(index), 0);
  vec3 p = p_tmp.xyz;
  age = p_tmp.w; // Age is stored in the fourth element

  // Set camera position
  vec4 vertex_position_viewspace = V * M * vec4(p ,1);
  
  // Position and size of point
  gl_Position = P * vertex_position_viewspace;
  gl_PointSize = particle_radius;
}