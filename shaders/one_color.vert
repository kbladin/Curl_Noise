#version 330 core

// In data
layout(location = 0) in vec3 vertex_position_modelspace;

// Out data
out vec3 vertex_position_viewspace;

// Uniform data
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(){
  gl_Position = P * V * M * vec4(vertex_position_modelspace,1);
}