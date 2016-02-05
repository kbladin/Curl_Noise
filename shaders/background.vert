#version 330 core

// In data
layout(location = 0) in vec3 vertex_position_modelspace;

// Out data
out vec3 vertex_position;

// Uniform data
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(){
  vertex_position = vertex_position_modelspace;
  // Transform to screen space
  gl_Position = P * V * M * vec4(vertex_position_modelspace,1);
}