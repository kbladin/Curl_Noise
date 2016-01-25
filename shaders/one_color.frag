#version 330 core

out vec4 color;

uniform vec3 material_diffise_color;

void main(){
  color.rgb = material_diffise_color;
  color.a = 1;
}
