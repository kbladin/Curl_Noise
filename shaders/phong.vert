#version 330 core

// In data
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_normal_modelspace;

// Out data
out vec3 normal_viewspace;
out vec3 vertex_position_viewspace;
out vec3 light_position_viewspace;

// Uniform data
// Light properties
uniform vec3 light_position;
// Transformation matrices
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(){
  gl_Position = P * V * M * vec4(vertex_position_modelspace,1);

  // Using inverse transpose for normal in case model is resized.
  normal_viewspace =          ( inverse(transpose(V * M)) * vec4(vertex_normal_modelspace,0)).xyz;
  vertex_position_viewspace = ( V * M * vec4(vertex_position_modelspace,1)).xyz;
  light_position_viewspace =  ( V * vec4(light_position,1)).xyz;
}