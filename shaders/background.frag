#version 330 core

// In data
in vec3 vertex_position;

// Out data
out vec4 color;

// Uniform data
uniform vec3 material_diffise_color;

void main(){
  // Color gradient
  float w = 0.8;
  float i = 0.3;
  color.rgb = ((vec3(vertex_position.y + 1, vertex_position.y + 1, 2) / 2) * w + vec3(1,1,1) * (1-w)) * i;
  color.a = 1;
}
