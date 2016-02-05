#version 330 core

// In data
layout(location = 0) in vec3 vertex_position_modelspace;

void main(){
  // Simply set the position
  gl_Position = vec4(vertex_position_modelspace,1);
}