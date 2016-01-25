#version 330 core

in vec3 vertex_position;

out vec4 color;

uniform vec3 material_diffise_color;

void main(){
  float w = 0.8;
  float i = 0.3;
  color.rgb = ((vec3(vertex_position.y + 1, vertex_position.y + 1, 2) / 2) * w + vec3(1,1,1) * (1-w)) * i;
  color.a = 1;
}
