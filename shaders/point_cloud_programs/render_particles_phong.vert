#version 330 core

// Index to pick the current particle
layout(location = 0) in vec2 index;

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

// Output

// Particle
//out vec3 a; // Acceleration
//out vec3 v; // Velocity
//out vec3 p; // Position
//out float t; // Life time of particle
//// Rendering of particle
//out float radius;

// Light source
out vec3 light_position_viewspace;
out vec4 vertex_position_viewspace;

void main(){
  // Write output to fragment shader
  // a = texelFetch( acceleration_sampler_2D, ivec2(index), 0).xyz;
  // v = texelFetch( velocity_sampler_2D, ivec2(index), 0).xyz;
  // The lifetime is stored in the fourth element of position
  vec4 p_tmp =  texelFetch( position_sampler_2D, ivec2(index), 0);
  vec3 p =      p_tmp.xyz;
  //t = p_tmp.a;

  // Set camera position
  vertex_position_viewspace = V * M * vec4(p ,1);
  // Position and size of point
  // Set size dependent on distance to camera
  gl_Position =   P * vertex_position_viewspace;
  gl_PointSize =  particle_radius * 5 / (-vertex_position_viewspace.z);
  
  light_position_viewspace = ( V * vec4(light_position,1)).xyz;
}