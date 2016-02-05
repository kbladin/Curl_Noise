#version 330 core

// In data
// Index to pick the current particle
layout(location = 0) in vec2 index;

// Out data
// Light source
out vec3 light_position_viewspace;
out vec4 vertex_position_viewspace;

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
// Light properties
uniform vec3 light_position;

void main(){
  // Position of particle
  vec3 p = texelFetch( position_sampler_2D, ivec2(index), 0).xyz;
  
  // Set camera position
  vertex_position_viewspace = V * M * vec4(p ,1);
  // Position and size of point
  // Set size dependent on distance to camera
  gl_Position =   P * vertex_position_viewspace;
  gl_PointSize =  particle_radius * 5 / (-vertex_position_viewspace.z);
  
  light_position_viewspace = ( V * vec4(light_position,1)).xyz;
}